#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include "../objectdata.h"
#include "../innerobj.h"
#include "../varscope.h"
#include "../arraydata.h"
#include "../configdata.h"
#include "../groupdata.h"
#include "../sidedata.h"
#include <algorithm>
#include <cstdlib>

using namespace sqf;
namespace
{
	value objnull_(virtualmachine* vm)
	{
		return value(std::make_shared<objectdata>());
	}
	value typeof_object(virtualmachine* vm, value::cref right)
	{
		auto obj = right.data<objectdata>();
		if (obj->is_null())
		{
			vm->wrn() << "Attempt to get typeOf a NULL OBJECT has been made." << std::endl;
			return "";
		}
		else
		{
			return obj->obj()->classname();
		}
	}
	value createvehicle_array(virtualmachine* vm, value::cref right)
	{
		auto arr = right.data<arraydata>();
		if (arr->size() != 5)
		{
			vm->err() << "Array was expected to have exactly 5 elements. Got " << arr->size() << '.' << std::endl;
			return {};
		}
		//Type
		if (arr->at(0).dtype() != STRING)
		{
			vm->err() << "Element 0 in input array was expected to be of type STRING. Got " << type_str(arr->at(0).dtype()) << '.' << std::endl;
			return {};
		}
		auto type = arr->at(0).as_string();
		//Position
		if (arr->at(1).dtype() != ARRAY)
		{
			vm->err() << "Element 1 in input array was expected to be of type ARRAY. Got " << type_str(arr->at(0).dtype()) << '.' << std::endl;
			return {};
		}
		auto position = arr->at(1).data<arraydata>();
		if (!position->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		//Markers
		if (arr->at(2).dtype() != ARRAY)
		{
			vm->err() << "Element 2 in input array was expected to be of type ARRAY. Got " << type_str(arr->at(2).dtype()) << '.' << std::endl;
			return {};
		}
		//Placement Radius
		if (arr->at(3).dtype() != SCALAR)
		{
			vm->err() << "Element 3 in input array was expected to be of type SCALAR. Got " << type_str(arr->at(2).dtype()) << '.' << std::endl;
			return {};
		}
		auto radius = arr->at(3).as_double();
		//SPECIAL
		if (arr->at(4).dtype() != STRING)
		{
			vm->err() << "Element 4 in input array was expected to be of type STRING. Got " << type_str(arr->at(2).dtype()) << '.' << std::endl;
			return {};
		}
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile().data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgVehicles");
			auto vehConfig = cfgVehicles.data<sqf::configdata>()->navigate(type);
			if (vehConfig.data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry for '" << type << "' could not be located in `ConfigBin >> CfgVehicles`." << std::endl;
				return {};
			}
		}
		auto veh = innerobj::create(vm, type, true);
		veh->posx(position->at(0).as_double() + ((std::rand() % static_cast<int>(radius * 2)) - radius));
		veh->posy(position->at(1).as_double() + ((std::rand() % static_cast<int>(radius * 2)) - radius));
		veh->posz(position->at(2).as_double());
		return value(std::make_shared<objectdata>(veh));
	}
	value createvehicle_string_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto type = left.as_string();
		auto position = right.data<arraydata>();
		if (position->size() != 3)
		{
			vm->err() << "Input array was expected to have 3 elements of type SCALAR. Got " << position->size() << '.' << std::endl;
			return {};
		}
		for (size_t i = 0; i < 3; i++)
		{
			if (position->at(i).dtype() != SCALAR)
			{
				vm->err() << "Element " << i << " of input array was expected to be of type SCALAR. Got " << type_str(position->at(i).dtype()) << '.' << std::endl;
				return {};
			}
		}
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile().data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgVehicles");
			auto vehConfig = cfgVehicles.data<sqf::configdata>()->navigate(type);
			if (vehConfig.data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry for '" << type << "' could not be located in `ConfigBin >> CfgVehicles`." << std::endl;
				return {};
			}
		}
		auto veh = innerobj::create(vm, type, true);
		veh->posx(position->at(0).as_double());
		veh->posy(position->at(1).as_double());
		veh->posz(position->at(2).as_double());
		return value(std::make_shared<objectdata>(veh));
	}

	value deletevehicle_array(virtualmachine* vm, value::cref right)
	{
		auto veh = right.data<objectdata>();
		if (veh->is_null())
		{
			vm->wrn() << "Attempt to delete NULL object." << std::endl;
			return {};
		}
		veh->obj()->destroy(vm);
		return {};
	}
	value position_object(virtualmachine* vm, value::cref right)
	{
		auto veh = right.data<objectdata>();
		if (veh->is_null())
		{
			vm->err() << "Object is null." << std::endl;
			return {};
		}
		auto pos = veh->obj()->pos();
		auto arr = std::make_shared<arraydata>();
		arr->push_back(pos[0]);
		arr->push_back(pos[1]);
		arr->push_back(pos[2]);
		return value(arr);
	}
	value setpos_object_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto veh = left.data<objectdata>();
		if (veh->is_null())
		{
			vm->wrn() << "Object is null." << std::endl;
			return {};
		}
		auto position = right.data<arraydata>();
		if (position->size() != 3)
		{
			vm->err() << "Input array was expected to have 3 elements of type SCALAR. Got " << position->size() << '.' << std::endl;
			return {};
		}
		for (size_t i = 0; i < 3; i++)
		{
			if (position->at(i).dtype() != SCALAR)
			{
				vm->err() << "Element " << i << " of input array was expected to be of type SCALAR. Got " << type_str(position->at(i).dtype()) << '.' << std::endl;
				return {};
			}
		}
		auto inner = veh->obj();
		inner->posx(position->at(0).as_double());
		inner->posy(position->at(1).as_double());
		inner->posz(position->at(2).as_double());
		return {};
	}
	value velocity_object(virtualmachine* vm, value::cref right)
	{
		auto veh = right.data<objectdata>();
		if (veh->is_null())
		{
			vm->err() << "Object is null." << std::endl;
			return {};
		}
		auto vel = veh->obj()->vel();
		auto arr = std::make_shared<arraydata>();
		arr->push_back(vel[0]);
		arr->push_back(vel[1]);
		arr->push_back(vel[2]);
		return value(arr);
	}
	value setvelocity_object_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto veh = left.data<objectdata>();
		if (veh->is_null())
		{
			vm->wrn() << "Object is null." << std::endl;
			return {};
		}
		auto velocity = right.data<arraydata>();
		if (velocity->size() != 3)
		{
			vm->err() << "Input array was expected to have 3 elements of type SCALAR. Got " << velocity->size() << '.' << std::endl;
			return {};
		}
		for (size_t i = 0; i < 3; i++)
		{
			if (velocity->at(i).dtype() != SCALAR)
			{
				vm->err() << "Element " << i << " of input array was expected to be of type SCALAR. Got " << type_str(velocity->at(i).dtype()) << '.' << std::endl;
				return {};
			}
		}
		auto inner = veh->obj();
		inner->velx(velocity->at(0).as_double());
		inner->vely(velocity->at(1).as_double());
		inner->velz(velocity->at(2).as_double());
		return {};
	}
	value domove_object_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto obj = left.data<objectdata>()->obj();
		if (obj->is_vehicle())
		{
			vm->err() << "Attempt to execute doMove on a vehicle. The operator doMove can only be executed on Units." << std::endl;
			return {};
		}
		setpos_object_array(vm, left, right);
		return {};
	}
	value domove_array_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto arr = left.data<arraydata>();
		bool errflag = false;
		for (size_t i = 0; i < arr->size(); i++)
		{
			if (arr->at(i).dtype() != OBJECT)
			{
				vm->err() << "Element " << i << " of left input array was expected to be of type OBJECT. Got " << type_str(arr->at(i).dtype()) << '.' << std::endl;
				errflag = true;
			}
			else if (arr->at(i).data<objectdata>()->obj()->is_vehicle())
			{
				vm->err() << "Attempt to execute doMove on a vehicle. The operator doMove can only be executed on Units." << std::endl;
				errflag = true;
			}
		}
		if (errflag)
		{
			return {};
		}
		for (const auto& i : *arr)
		{
			setpos_object_array(vm, i, right);
		}
		return {};
	}

	value createUnit_group_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto grp = left.data<groupdata>();
		auto arr = right.data<arraydata>();

		if (arr->size() != 5)
		{
			vm->err() << "Array was expected to have exactly 5 elements. Got " << arr->size() << '.' << std::endl;
			return {};
		}
		//Type
		if (arr->at(0).dtype() != STRING)
		{
			vm->err() << "Element 0 in input array was expected to be of type STRING. Got " << type_str(arr->at(0).dtype()) << '.' << std::endl;
			return {};
		}
		auto type = arr->at(0).as_string();
		//Position
		if (arr->at(1).dtype() != ARRAY)
		{
			vm->err() << "Element 1 in input array was expected to be of type ARRAY. Got " << type_str(arr->at(0).dtype()) << '.' << std::endl;
			return {};
		}
		auto position = arr->at(1).data<arraydata>();
		if (!position->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		//Markers
		if (arr->at(2).dtype() != ARRAY)
		{
			vm->err() << "Element 2 in input array was expected to be of type ARRAY. Got " << type_str(arr->at(2).dtype()) << '.' << std::endl;
			return {};
		}
		//Placement Radius
		if (arr->at(3).dtype() != SCALAR)
		{
			vm->err() << "Element 3 in input array was expected to be of type SCALAR. Got " << type_str(arr->at(2).dtype()) << '.' << std::endl;
			return {};
		}
		auto radius = arr->at(3).as_double();
		//SPECIAL
		if (arr->at(4).dtype() != STRING)
		{
			vm->err() << "Element 4 in input array was expected to be of type STRING. Got " << type_str(arr->at(2).dtype()) << '.' << std::endl;
			return {};
		}
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile().data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgVehicles");
			auto vehConfig = cfgVehicles.data<sqf::configdata>()->navigate(type);
			if (vehConfig.data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry for '" << type << "' could not be located in `ConfigBin >> CfgVehicles`." << std::endl;
				return {};
			}
		}
		auto veh = innerobj::create(vm, type, false);
		grp->add_unit(veh);
		veh->posx(position->at(0).as_double() + ((std::rand() % static_cast<int>(radius * 2)) - radius));
		veh->posy(position->at(1).as_double() + ((std::rand() % static_cast<int>(radius * 2)) - radius));
		veh->posz(position->at(2).as_double());
		return value(std::make_shared<objectdata>(veh));
	}
	value createUnit_string_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto type = left.as_string();
		auto arr = right.data<arraydata>();
		double skill = 0.5;
		std::string rank = "PRIVATE";

		if (arr->size() < 2)
		{
			vm->err() << "Array was expected to have at least 2 elements. Got " << arr->size() << '.' << std::endl;
			return {};
		}
		//Position
		if (arr->at(0).dtype() != ARRAY)
		{
			vm->err() << "Element 0 in input array was expected to be of type ARRAY. Got " << type_str(arr->at(0).dtype()) << '.' << std::endl;
			return {};
		}
		auto position = arr->at(0).data<arraydata>();
		if (!position->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		//Group
		if (arr->at(1).dtype() != GROUP)
		{
			vm->err() << "Element 1 in input array was expected to be of type GROUP. Got " << type_str(arr->at(1).dtype()) << '.' << std::endl;
			return {};
		}
		auto grp = arr->at(1).data<groupdata>();

		//Optionals
		//init
		if (arr->size() >= 3)
		{
			if (arr->at(2).dtype() != STRING)
			{
				vm->err() << "Element 2 in input array was expected to be of type STRING. Got " << type_str(arr->at(2).dtype()) << '.' << std::endl;
				return {};
			}
			else
			{
				std::string init = arr->at(2).as_string();
			}
		}
		//skill
		if (arr->size() >= 4)
		{
			if (arr->at(3).dtype() != SCALAR)
			{
				vm->err() << "Element 3 in input array was expected to be of type SCALAR. Got " << type_str(arr->at(3).dtype()) << '.' << std::endl;
				return {};
			}
			else
			{
				skill = arr->at(3).as_double();
			}
		}
		//rank
		if (arr->size() >= 5)
		{
			if (arr->at(4).dtype() != STRING)
			{
				vm->err() << "Element 4 in input array was expected to be of type STRING. Got " << type_str(arr->at(4).dtype()) << '.' << std::endl;
				return {};
			}
			else
			{
				rank = arr->at(4).as_string();
			}
		}
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile().data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgVehicles");
			auto vehConfig = cfgVehicles.data<sqf::configdata>()->navigate(type);
			if (vehConfig.data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry for '" << type << "' could not be located in `ConfigBin >> CfgVehicles`." << std::endl;
				return {};
			}
		}
		auto veh = innerobj::create(vm, type, false);
		grp->add_unit(veh);
		veh->posx(position->at(0).as_double());
		veh->posy(position->at(1).as_double());
		veh->posz(position->at(2).as_double());
		return value(std::make_shared<objectdata>(veh));
	}
	value distance_array_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<arraydata>();
		if (!l->check_type(vm, SCALAR, 2, 3) || !r->check_type(vm, SCALAR, 2, 3))
		{
			return {};
		}
		return arraydata::distance3d(l, r);
	}
	value distance_object_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<objectdata>();
		auto r = right.data<arraydata>();
		if (l->is_null())
		{
			vm->err() << "Left value provided is NULL object." << std::endl;
			return {};
		}
		if (!r->check_type(vm, SCALAR, 2, 3))
		{
			return {};
		}
		return l->obj()->distance3d(r->as_vec3());
	}
	value distance_array_object(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		if (!l->check_type(vm, SCALAR, 2, 3))
		{
			return {};
		}
		return r->obj()->distance3d(l->as_vec3());
	}
	value distance_object_object(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<objectdata>();
		auto r = right.data<objectdata>();
		if (l->is_null())
		{
			vm->err() << "Left value provided is NULL object." << std::endl;
			return {};
		}
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		return l->obj()->distance3d(r->obj());
	}
	value distance2d_array_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<arraydata>();
		if (!l->check_type(vm, SCALAR, 2, 3) || !r->check_type(vm, SCALAR, 2, 3))
		{
			return {};
		}
		return arraydata::distance2d(l, r);
	}
	value distance2d_object_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<objectdata>();
		auto r = right.data<arraydata>();
		if (l->is_null())
		{
			vm->err() << "Left value provided is NULL object." << std::endl;
			return {};
		}
		if (!r->check_type(vm, SCALAR, 2, 3))
		{
			return {};
		}
		return l->obj()->distance2d(r->as_vec2());
	}
	value distance2d_array_object(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		if (!l->check_type(vm, SCALAR, 2, 3))
		{
			return {};
		}
		return r->obj()->distance2d(l->as_vec2());
	}
	value distance2d_object_object(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<objectdata>();
		auto r = right.data<objectdata>();
		if (l->is_null())
		{
			vm->err() << "Left value provided is NULL object." << std::endl;
			return {};
		}
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		return l->obj()->distance2d(r->obj());
	}
	class nearestobjects_distancesort3d
	{
		std::array<double, 3> pos;
	public:
		nearestobjects_distancesort3d(std::array<double, 3> p) : pos(p) {}
		bool operator() (value::cref l, value::cref r) const { return l.data<objectdata>()->obj()->distance3d(pos) < r.data<objectdata>()->obj()->distance3d(pos); }
	};
	class nearestobjects_distancesort2d
	{
		std::array<double, 2> pos;
	public:
		nearestobjects_distancesort2d(std::array<double, 2> p) : pos(p) {}
		bool operator() (value::cref l, value::cref r) const { return l.data<objectdata>()->obj()->distance2d(pos) < r.data<objectdata>()->obj()->distance2d(pos); }
	};
	value nearestobjects_array(virtualmachine* vm, value::cref right)
	{
		auto arr = right.data<arraydata>();
		if (arr->size() != 3 && arr->size() != 4)
		{
			vm->err() << "Input array was expected to contain either 3 or 4 elements. Got " << arr->size() << '.' << std::endl;
			return {};
		}
		std::array<double, 3> position {0, 0, 0};
		auto dtype = arr->at(0).dtype();
		if (dtype == ARRAY)
		{
			if (!arr->at(0).data<arraydata>()->check_type(vm, SCALAR, 3))
			{
				return {};
			}
			position = arr->at(0).data<arraydata>()->as_vec3();
		}
		else if (dtype == OBJECT)
		{
			if (arr->at(0).data<objectdata>()->is_null())
			{
				vm->err() << "Input array element 0 is NULL object." << std::endl;
				return {};
			}
			position = arr->at(0).data<objectdata>()->obj()->pos();
		}
		else
		{
			vm->err() << "Input array element 0 was expected to be of type ARRAY or OBJECT. Got " << type_str(arr->at(0).dtype()) << '.' << std::endl;
			return {};
		}
		if (arr->at(1).dtype() != ARRAY)
		{
			vm->err() << "Input array element 1 was expected to be of type ARRAY. Got " << type_str(arr->at(1).dtype()) << '.' << std::endl;
			return {};
		}
		auto filterarr = arr->at(1).data<arraydata>();
		for (size_t i = 0; i < filterarr->size(); i++)
		{
			if (filterarr->at(i).dtype() != STRING)
			{
				vm->err() << "Input array element 1 contains non-string element at array index " << i << ". Got " << type_str(filterarr->at(i).dtype()) << '.' << std::endl;
				return {};
			}
		}
		if (arr->at(2).dtype() != SCALAR)
		{
			vm->err() << "Input array element 2 was expected to be of type SCALAR. Got " << type_str(arr->at(2).dtype()) << '.' << std::endl;
			return {};
		}
		auto radius = arr->at(2).as_double();
		auto is2ddistance = false;
		if (arr->size() == 4)
		{
			if (arr->at(3).dtype() != sqf::BOOL)
			{
				vm->err() << "Input array element 3 was expected to be of type BOOLEAN. Got " << type_str(arr->at(3).dtype()) << '.' << std::endl;
				return {};
			}
			is2ddistance = arr->at(3).as_bool();
		}
		auto outputarr = std::make_shared<arraydata>();
		if (is2ddistance)
		{
			std::array<double, 2> position2d{ position[0], position[1] };
			for (auto& object : vm->get_objlist())
			{
				if (object->distance2d(position2d) > radius) continue;

				bool match = filterarr->empty() || !vm->perform_classname_checks();
				if (!match)
				{
					auto found = std::find_if(filterarr->begin(), filterarr->end(), [&object](value::cref value) {
						return object->iskindof(value.as_string());
					});
					match = found != filterarr->end();
				}
				if (match)
				{
					outputarr->push_back(value(std::make_shared<objectdata>(object)));
				}
				
			}
			std::sort(outputarr->begin(), outputarr->end(), nearestobjects_distancesort2d(position2d));
		}
		else
		{
			for (auto& object : vm->get_objlist())
			{
				if (object->distance3d(position) > radius) continue;
				
				bool match = filterarr->empty() || !vm->perform_classname_checks();
				if (!match)
				{
					auto found = std::find_if(filterarr->begin(), filterarr->end(), [&object](value::cref value) {
						return object->iskindof(value.as_string());
					});
					match = found != filterarr->end();
				}
				if (match)
				{
					outputarr->push_back(value(std::make_shared<objectdata>(object)));
				}
			}
			std::sort(outputarr->begin(), outputarr->end(), nearestobjects_distancesort3d(position));
		}
		return value(outputarr);
	}
	value isnull_object(virtualmachine* vm, value::cref right)
	{
		auto obj = right.data<objectdata>();
		return obj->is_null();
	}
	value side_object(virtualmachine* vm, value::cref right)
	{
		auto obj = right.data<objectdata>();
		auto grp = obj->obj()->group();
        return value((!grp.get() || grp->is_null()) ? std::make_shared<sidedata>(sidedata::Civilian) : grp->side());
	}
	value allunits_(virtualmachine* vm)
	{
		auto arr = std::make_shared<arraydata>();
		for (auto& object : vm->get_objlist())
		{
			if (object->is_vehicle())
				continue;
			arr->push_back(value(std::make_shared<objectdata>(object)));
		}
		return value(arr);
	}
	bool iskindof__helper(virtualmachine* vm, std::string left, std::string base, std::shared_ptr<sqf::configdata> config)
	{
		auto node = config->navigate(left).data<configdata>();
		while (!node->is_null())
		{
			if (str_cmpi(node->name().c_str(), -1, base.c_str(), -1) == 0)
			{
				return true;
			}
			node = node->inherited_parent().data<configdata>();
		}
		return false;
	}
	value iskindof_object_string(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto obj = right.data<objectdata>();
		if (obj->is_null())
		{
			vm->err() << "Left value provided is NULL object." << std::endl;
			return {};
		}
		auto str = left.as_string();
		return obj->obj()->iskindof(str);
	}
	value iskindof_string_string(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto confname = left.as_string();
		auto configbin = configdata::configFile().data<configdata>();
		auto cfgVehicles = configbin->navigate("CfgVehicles").data<configdata>();
		if (cfgVehicles->is_null())
		{
			return false;
		}
		auto node = cfgVehicles->navigate(confname).data<configdata>();
		if (!node->is_null())
		{
			return node->is_kind_of(right.as_string());
		}
		auto cfgAmmo = configbin->navigate("CfgAmmo").data<configdata>();
		node = cfgAmmo->navigate(confname).data<configdata>();
		if (!node->is_null())
		{
			return node->is_kind_of(right.as_string());
		}
		auto cfgNonAiVehicles = configbin->navigate("CfgNonAiVehicles").data<configdata>();
		node = cfgNonAiVehicles->navigate(confname).data<configdata>();
		if (!node->is_null())
		{
			return node->is_kind_of(right.as_string());
		}
		return false;
	}
	value iskindof_string_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto classname = left.as_string();
		auto arr = right.data<arraydata>();
		if (!arr->check_type(vm, std::array<sqf::type, 2>{ STRING, CONFIG }))
		{
			return {};
		}
		auto basename = arr->at(0).as_string();
		auto conf = arr->at(1).data<configdata>();
		auto node = conf->navigate(classname).data<configdata>();
		if (node->is_null())
		{
			//ToDo: Check if isKindOf really does not errors here
			vm->wrn() << "Class '" << classname << "' was not found in " << conf->name() << "." << std::endl;
			return false;
		}
		else
		{
			return node->is_kind_of(basename);
		}
	}
	value player_(virtualmachine* vm)
	{
		return value(std::make_shared<objectdata>(vm->player_obj()));
	}

	value setdamage_object_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<objectdata>();
		auto r = right.as_float();
		if (l->is_null())
		{
			vm->err() << "Left value provided is NULL object." << std::endl;
			return {};
		}
		l->obj()->damage(r);
		return {};
	}
	value getdamage_object(virtualmachine* vm, value::cref right)
	{
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		return r->obj()->damage();
	}
	value alive_object(virtualmachine* vm, value::cref right)
	{
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->wrn() << "Right value provided is NULL object." << std::endl;
			return false;
		}
		return r->obj()->alive();
	}
	value crew_object(virtualmachine* vm, value::cref right)
	{
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		auto arr = std::make_shared<arraydata>();
		auto obj = r->obj();
		if (!obj->is_vehicle())
		{
			vm->wrn() << "Right value provided is not a vehicle object." << std::endl;
			return value(arr);
		}
		if (!obj->driver()->is_null())
		{
			arr->push_back(value(obj->driver()));
		}
		if (!obj->gunner()->is_null())
		{
			arr->push_back(value(obj->gunner()));
		}
		if (!obj->commander()->is_null())
		{
			arr->push_back(value(obj->commander()));
		}
		for (auto& it : obj->soldiers())
		{
			arr->push_back(value(it));
		}
		return value(arr);
	}
	value vehicle_object(virtualmachine* vm, value::cref right)
	{
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		auto obj = r->obj();
		if (!obj->is_vehicle())
		{
			vm->wrn() << "Right value provided is not a vehicle object." << std::endl;
			return right;
		}
		auto parent = obj->parent_object();
		if (parent->is_null() || !parent->obj()->is_vehicle())
		{
			return right;
		}
		else
		{
			return value(parent);
		}
	}
	value objectparent_object(virtualmachine* vm, value::cref right)
	{
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		return value(r->obj()->parent_object());
	}
	value driver_object(virtualmachine* vm, value::cref right)
	{
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		auto obj = r->obj();
		if (!obj->is_vehicle())
		{
			vm->wrn() << "Right value provided is not a vehicle object." << std::endl;
			return right;
		}
		return value(obj->driver());
	}
	value commander_object(virtualmachine* vm, value::cref right)
	{
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		auto obj = r->obj();
		if (!obj->is_vehicle())
		{
			vm->wrn() << "Right value provided is not a vehicle object." << std::endl;
			return right;
		}
		return value(obj->commander());
	}
	value gunner_object(virtualmachine* vm, value::cref right)
	{
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		auto obj = r->obj();
		if (!obj->is_vehicle())
		{
			vm->wrn() << "Right value provided is not a vehicle object." << std::endl;
			return right;
		}
		return value(obj->gunner());
	}
	value in_object_object(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<objectdata>();
		if (l->is_null())
		{
			vm->err() << "Left value provided is NULL object." << std::endl;
			return {};
		}
		if (l->obj()->is_vehicle())
		{
			vm->wrn() << "Right value provided is a vehicle object." << std::endl;
			return false;
		}
		auto r = right.data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return {};
		}
		if (!r->obj()->is_vehicle())
		{
			vm->wrn() << "Right value provided is not a vehicle object." << std::endl;
			return false;
		}
		auto veh = r->obj();
		auto unit = l->obj();
		if (veh->driver()->obj().get() == unit.get() || veh->commander()->obj().get() == unit.get() || veh->gunner()->obj().get() == unit.get())
		{
			return true;
		}
		else
		{
			auto res = std::find_if(veh->soldiers_begin(), veh->soldiers_end(), [unit](std::shared_ptr<objectdata> data) -> bool {
				return data->obj().get() == unit.get();
			});
			return res != veh->soldiers_end();
		}
	}
}
void sqf::commandmap::initobjectcmds()
{
	//GetVariable & SetVariable & AllVariables are in namespacecmds as simple alias.
	add(nular("allUnits", "Return a list of all units (all persons except agents) outside and inside vehicles.", allunits_));
	add(nular("objNull", "A non-existent Object. To compare non-existent objects use isNull or isEqualTo.", objnull_));
	add(unary("typeOf", sqf::type::OBJECT, "Returns the config class name of given object.", typeof_object));
	add(unary("createVehicle", type::ARRAY, "Creates an empty object of given classname type.", createvehicle_array));
	add(binary(4, "createVehicle", type::STRING, type::ARRAY, "Creates an empty object of given classname type.", createvehicle_string_array));
	add(binary(4, "createVehicleLocal", type::ANY, type::ANY, "Creates an empty object of given classname type.", createvehicle_string_array));
	add(unary("deleteVehicle", type::OBJECT, "Deletes an object.", deletevehicle_array));
	add(unary("position", type::OBJECT, "Returns the object position in format PositionAGLS. Z value is height over the surface underneath.", position_object));
	add(unary("getPos", type::OBJECT, "Returns the object position in format PositionAGLS. Z value is height over the surface underneath.", position_object));
	add(binary(4, "setPos", type::OBJECT, type::ARRAY, "Sets object position.", setpos_object_array));
	add(unary("velocity", type::OBJECT, "Return velocity (speed vector) of Unit as an array with format [x, y, z].", velocity_object));
	add(binary(4, "setVelocity", type::OBJECT, type::ARRAY, "Set velocity (speed vector) of a vehicle. Units are in metres per second.", setvelocity_object_array));
	add(binary(4, "doMove", type::OBJECT, type::ARRAY, "Order the given unit(s) to move to the given position (without radio messages). In SQFVM this command acts like setPos.", domove_object_array));
	add(binary(4, "doMove", type::ARRAY, type::ARRAY, "Order the given unit(s) to move to the given position (without radio messages). In SQFVM this command acts like setPos.", domove_array_array));
	add(binary(4, "createUnit", type::GROUP, type::ARRAY, "Create unit of a class that's defined in CfgVehicles.", createUnit_group_array));
	add(binary(4, "createUnit", type::STRING, type::ARRAY, "Create unit of a class that's defined in CfgVehicles.", createUnit_string_array));
	add(binary(4, "distance", type::ARRAY, type::ARRAY, "Returns a distance in meters between two positions.", distance_array_array));
	add(binary(4, "distance", type::OBJECT, type::ARRAY, "Returns a distance in meters between two positions.", distance_object_array));
	add(binary(4, "distance", type::ARRAY, type::OBJECT, "Returns a distance in meters between two positions.", distance_array_object));
	add(binary(4, "distance", type::OBJECT, type::OBJECT, "Returns a distance in meters between two positions.", distance_object_object));
	add(binary(4, "distance2d", type::ARRAY, type::ARRAY, "Returns a 2d distance in meters between two positions.", distance2d_array_array));
	add(binary(4, "distance2d", type::OBJECT, type::ARRAY, "Returns a 2d distance in meters between two positions.", distance2d_object_array));
	add(binary(4, "distance2d", type::ARRAY, type::OBJECT, "Returns a 2d distance in meters between two positions.", distance2d_array_object));
	add(binary(4, "distance2d", type::OBJECT, type::OBJECT, "Returns a 2d distance in meters between two positions.", distance2d_object_object));
	add(unary("nearestObjects", type::ARRAY, "Returns a list of nearest objects of the given types to the given position or object, within the specified distance. If more than one object is found they will be ordered by proximity, the closest one will be first in the array.", nearestobjects_array));
	add(unary("isNull", type::OBJECT, "Checks whether the tested item is Null.", isnull_object));
	add(unary("side", type::OBJECT, "Returns the side of an object.", side_object));
	add(binary(4, "isKindOf", type::OBJECT, type::STRING, "Checks whether the object is (a subtype) of the given type.", iskindof_object_string));
	add(binary(4, "isKindOf", type::STRING, type::STRING, "Checks whether the object is (a subtype) of the given type. Checks CfgVehicles, CfgAmmo and CfgNonAiVehicles.", iskindof_string_string));
	add(binary(4, "isKindOf", type::STRING, type::ARRAY, "Checks whether the object is (a subtype) of the given type.", iskindof_string_array));
	add(nular("player", "Theoretical player object. Practically, just a normal object.", player_));
	add(binary(4, "setDamage", type::OBJECT, type::SCALAR, "Damage / repair object. Damage 0 means fully functional, damage 1 means completely destroyed / dead.", setdamage_object_scalar));
	add(unary("getDammage", type::OBJECT, "Return the damage value of an object.", getdamage_object));
	add(unary("damage", type::OBJECT, "Return the damage value of an object.", getdamage_object));
	add(unary("alive", type::OBJECT, "Check if given vehicle/person/building is alive (i.e. not dead or destroyed). alive objNull returns false.", alive_object));
	add(unary("crew", type::OBJECT, "Returns the crew (both dead and alive) of the given vehicle.", crew_object));
	add(unary("vehicle", type::OBJECT, "Vehicle in which given unit is mounted. If none, unit is returned.", vehicle_object));
	add(unary("objectParent", type::OBJECT, "Returns parent of an object if the object is proxy, otherwise objNull.", objectparent_object));
	add(unary("driver", type::OBJECT, "Returns the driver of a vehicle. If provided object is a unit, the unit is returned.", driver_object));
	add(unary("commander", type::OBJECT, "Returns the primary observer. If provided object is a unit, the unit is returned.", commander_object));
	add(unary("gunner", type::OBJECT, "Returns the gunner of a vehicle. If provided object is a unit, the unit is returned.", gunner_object));
	add(binary(4, "in", type::OBJECT, type::OBJECT, "Checks whether unit is in vehicle.", in_object_object));

}
