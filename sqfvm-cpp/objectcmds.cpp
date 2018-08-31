#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "objectdata.h"
#include "innerobj.h"
#include "varscope.h"
#include "arraydata.h"
#include "configdata.h"
#include "groupdata.h"
#include <algorithm>
#include <cstdlib>

using namespace sqf;
namespace
{
	std::shared_ptr<value> objnull_(virtualmachine* vm)
	{
		return std::make_shared<value>(std::make_shared<objectdata>(), sqf::type::OBJECT);
	}
	std::shared_ptr<value> typeof_object(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto obj = right->data<objectdata>();
		if (obj->is_null())
		{
			vm->wrn() << "Attempt to get typeOf a NULL OBJECT has been made." << std::endl;
			return std::make_shared<value>("");
		}
		else
		{
			return std::make_shared<value>(obj->obj()->classname());
		}
	}
	std::shared_ptr<value> createvehicle_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto arr = right->data<arraydata>();
		if (arr->size() != 5)
		{
			vm->err() << "Array was expected to have exactly 5 elements. Got " << arr->size() << '.' << std::endl;
			return std::make_shared<value>();
		}
		//Type
		if (arr->at(0)->dtype() != STRING)
		{
			vm->err() << "Element 0 in input array was expected to be of type STRING. Got " << type_str(arr->at(0)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		auto type = arr->at(0)->as_string();
		//Position
		if (arr->at(1)->dtype() != ARRAY)
		{
			vm->err() << "Element 1 in input array was expected to be of type ARRAY. Got " << type_str(arr->at(0)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		auto position = arr->at(1)->data<arraydata>();
		if (!position->check_type(vm, SCALAR, 3))
		{
			return std::make_shared<value>();
		}
		//Markers
		if (arr->at(2)->dtype() != ARRAY)
		{
			vm->err() << "Element 2 in input array was expected to be of type ARRAY. Got " << type_str(arr->at(2)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		//Placement Radius
		if (arr->at(3)->dtype() != SCALAR)
		{
			vm->err() << "Element 3 in input array was expected to be of type SCALAR. Got " << type_str(arr->at(2)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		auto radius = arr->at(3)->as_double();
		//SPECIAL
		if (arr->at(4)->dtype() != STRING)
		{
			vm->err() << "Element 4 in input array was expected to be of type STRING. Got " << type_str(arr->at(2)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile()->data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgVehicles");
			auto vehConfig = cfgVehicles->data<sqf::configdata>()->navigate(type);
			if (vehConfig->data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry for '" << type << "' could not be located in `ConfigBin >> CfgVehicles`." << std::endl;
				return std::make_shared<value>();
			}
		}
		auto veh = innerobj::create(vm, type, true);
		veh->posx(position->at(0)->as_double() + ((std::rand() % (int)(radius * 2)) - radius));
		veh->posy(position->at(1)->as_double() + ((std::rand() % (int)(radius * 2)) - radius));
		veh->posz(position->at(2)->as_double());
		return std::make_shared<value>(std::make_shared<objectdata>(veh), OBJECT);
	}
	std::shared_ptr<value> createvehicle_string_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto type = left->as_string();
		auto position = right->data<arraydata>();
		if (position->size() != 3)
		{
			vm->err() << "Input array was expected to have 3 elements of type SCALAR. Got " << position->size() << '.' << std::endl;
			return std::make_shared<value>();
		}
		for (size_t i = 0; i < 3; i++)
		{
			if (position->at(i)->dtype() != SCALAR)
			{
				vm->err() << "Element " << i << " of input array was expected to be of type SCALAR. Got " << type_str(position->at(i)->dtype()) << '.' << std::endl;
				return std::make_shared<value>();
			}
		}
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile()->data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgVehicles");
			auto vehConfig = cfgVehicles->data<sqf::configdata>()->navigate(type);
			if (vehConfig->data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry for '" << type << "' could not be located in `ConfigBin >> CfgVehicles`." << std::endl;
				return std::make_shared<value>();
			}
		}
		auto veh = innerobj::create(vm, type, true);
		veh->posx(position->at(0)->as_double());
		veh->posy(position->at(1)->as_double());
		veh->posz(position->at(2)->as_double());
		return std::make_shared<value>(std::make_shared<objectdata>(veh), OBJECT);
	}

	std::shared_ptr<value> deletevehicle_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto veh = right->data<objectdata>();
		if (veh->is_null())
		{
			vm->wrn() << "Attempt to delete NULL object." << std::endl;
			return std::make_shared<value>();
		}
		veh->obj()->destroy(vm);
		return std::make_shared<value>();
	}
	std::shared_ptr<value> position_object(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto veh = right->data<objectdata>();
		if (veh->is_null())
		{
			vm->err() << "Object is null." << std::endl;
			return std::make_shared<value>();
		}
		auto pos = veh->obj()->pos();
		auto arr = std::make_shared<arraydata>();
		arr->push_back(std::make_shared<value>(pos[0]));
		arr->push_back(std::make_shared<value>(pos[1]));
		arr->push_back(std::make_shared<value>(pos[2]));
		return std::make_shared<value>(arr, ARRAY);
	}
	std::shared_ptr<value> setpos_object_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto veh = left->data<objectdata>();
		if (veh->is_null())
		{
			vm->wrn() << "Object is null." << std::endl;
			return std::make_shared<value>();
		}
		auto position = right->data<arraydata>();
		if (position->size() != 3)
		{
			vm->err() << "Input array was expected to have 3 elements of type SCALAR. Got " << position->size() << '.' << std::endl;
			return std::make_shared<value>();
		}
		for (size_t i = 0; i < 3; i++)
		{
			if (position->at(i)->dtype() != SCALAR)
			{
				vm->err() << "Element " << i << " of input array was expected to be of type SCALAR. Got " << type_str(position->at(i)->dtype()) << '.' << std::endl;
				return std::make_shared<value>();
			}
		}
		auto inner = veh->obj();
		inner->posx(position->at(0)->as_double());
		inner->posy(position->at(1)->as_double());
		inner->posz(position->at(2)->as_double());
		return std::make_shared<value>();
	}
	std::shared_ptr<value> velocity_object(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto veh = right->data<objectdata>();
		if (veh->is_null())
		{
			vm->err() << "Object is null." << std::endl;
			return std::make_shared<value>();
		}
		auto vel = veh->obj()->vel();
		auto arr = std::make_shared<arraydata>();
		arr->push_back(std::make_shared<value>(vel[0]));
		arr->push_back(std::make_shared<value>(vel[1]));
		arr->push_back(std::make_shared<value>(vel[2]));
		return std::make_shared<value>(arr, ARRAY);
	}
	std::shared_ptr<value> setvelocity_object_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto veh = left->data<objectdata>();
		if (veh->is_null())
		{
			vm->wrn() << "Object is null." << std::endl;
			return std::make_shared<value>();
		}
		auto velocity = right->data<arraydata>();
		if (velocity->size() != 3)
		{
			vm->err() << "Input array was expected to have 3 elements of type SCALAR. Got " << velocity->size() << '.' << std::endl;
			return std::make_shared<value>();
		}
		for (size_t i = 0; i < 3; i++)
		{
			if (velocity->at(i)->dtype() != SCALAR)
			{
				vm->err() << "Element " << i << " of input array was expected to be of type SCALAR. Got " << type_str(velocity->at(i)->dtype()) << '.' << std::endl;
				return std::make_shared<value>();
			}
		}
		auto inner = veh->obj();
		inner->velx(velocity->at(0)->as_double());
		inner->vely(velocity->at(1)->as_double());
		inner->velz(velocity->at(2)->as_double());
		return std::make_shared<value>();
	}
	std::shared_ptr<value> domove_object_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto obj = left->data<objectdata>()->obj();
		if (obj->is_vehicle())
		{
			vm->err() << "Attempt to execute doMove on a vehicle. The operator doMove can only be executed on Units." << std::endl;
			return std::make_shared<value>();
		}
		setpos_object_array(vm, left, right);
		return std::make_shared<value>();
	}
	std::shared_ptr<value> domove_array_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->data<arraydata>();
		bool errflag = false;
		for (size_t i = 0; i < arr->size(); i++)
		{
			if (arr->at(i)->dtype() != OBJECT)
			{
				vm->err() << "Element " << i << " of left input array was expected to be of type OBJECT. Got " << type_str(arr->at(i)->dtype()) << '.' << std::endl;
				errflag = true;
			}
			else if (arr->at(i)->data<objectdata>()->obj()->is_vehicle())
			{
				vm->err() << "Attempt to execute doMove on a vehicle. The operator doMove can only be executed on Units." << std::endl;
				errflag = true;
			}
		}
		if (errflag)
		{
			return std::shared_ptr<value>();
		}
		for (size_t i = 0; i < arr->size(); i++)
		{
			setpos_object_array(vm, arr->at(i), right);
		}
		return std::make_shared<value>();
	}

	std::shared_ptr<value> createUnit_group_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto grp = left->data<groupdata>();
		auto arr = right->data<arraydata>();

		if (arr->size() != 5)
		{
			vm->err() << "Array was expected to have exactly 5 elements. Got " << arr->size() << '.' << std::endl;
			return std::make_shared<value>();
		}
		//Type
		if (arr->at(0)->dtype() != STRING)
		{
			vm->err() << "Element 0 in input array was expected to be of type STRING. Got " << type_str(arr->at(0)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		auto type = arr->at(0)->as_string();
		//Position
		if (arr->at(1)->dtype() != ARRAY)
		{
			vm->err() << "Element 1 in input array was expected to be of type ARRAY. Got " << type_str(arr->at(0)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		auto position = arr->at(1)->data<arraydata>();
		if (!position->check_type(vm, SCALAR, 3))
		{
			return std::make_shared<value>();
		}
		//Markers
		if (arr->at(2)->dtype() != ARRAY)
		{
			vm->err() << "Element 2 in input array was expected to be of type ARRAY. Got " << type_str(arr->at(2)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		//Placement Radius
		if (arr->at(3)->dtype() != SCALAR)
		{
			vm->err() << "Element 3 in input array was expected to be of type SCALAR. Got " << type_str(arr->at(2)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		auto radius = arr->at(3)->as_double();
		//SPECIAL
		if (arr->at(4)->dtype() != STRING)
		{
			vm->err() << "Element 4 in input array was expected to be of type STRING. Got " << type_str(arr->at(2)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile()->data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgVehicles");
			auto vehConfig = cfgVehicles->data<sqf::configdata>()->navigate(type);
			if (vehConfig->data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry for '" << type << "' could not be located in `ConfigBin >> CfgVehicles`." << std::endl;
				return std::make_shared<value>();
			}
		}
		auto veh = innerobj::create(vm, type, false);
		grp->add_unit(veh);
		veh->posx(position->at(0)->as_double() + ((std::rand() % (int)(radius * 2)) - radius));
		veh->posy(position->at(1)->as_double() + ((std::rand() % (int)(radius * 2)) - radius));
		veh->posz(position->at(2)->as_double());
		return std::make_shared<value>(std::make_shared<objectdata>(veh), OBJECT);
	}
	std::shared_ptr<value> createUnit_string_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto type = left->as_string();
		auto arr = right->data<arraydata>();
		std::string init = "";
		double skill = 0.5;
		std::string rank = "PRIVATE";

		if (arr->size() < 2)
		{
			vm->err() << "Array was expected to have at least 2 elements. Got " << arr->size() << '.' << std::endl;
			return std::make_shared<value>();
		}
		//Position
		if (arr->at(0)->dtype() != ARRAY)
		{
			vm->err() << "Element 0 in input array was expected to be of type ARRAY. Got " << type_str(arr->at(0)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		auto position = arr->at(0)->data<arraydata>();
		if (!position->check_type(vm, SCALAR, 3))
		{
			return std::make_shared<value>();
		}
		//Group
		if (arr->at(1)->dtype() != GROUP)
		{
			vm->err() << "Element 1 in input array was expected to be of type GROUP. Got " << type_str(arr->at(1)->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		auto grp = arr->at(1)->data<groupdata>();

		//Optionals
		//init
		if (arr->size() >= 3)
		{
			if (arr->at(2)->dtype() != STRING)
			{
				vm->err() << "Element 2 in input array was expected to be of type STRING. Got " << type_str(arr->at(2)->dtype()) << '.' << std::endl;
				return std::make_shared<value>();
			}
			else
			{
				init = arr->at(2)->as_string();
			}
		}
		//skill
		if (arr->size() >= 4)
		{
			if (arr->at(3)->dtype() != SCALAR)
			{
				vm->err() << "Element 3 in input array was expected to be of type SCALAR. Got " << type_str(arr->at(3)->dtype()) << '.' << std::endl;
				return std::make_shared<value>();
			}
			else
			{
				skill = arr->at(3)->as_double();
			}
		}
		//rank
		if (arr->size() >= 5)
		{
			if (arr->at(4)->dtype() != STRING)
			{
				vm->err() << "Element 4 in input array was expected to be of type STRING. Got " << type_str(arr->at(4)->dtype()) << '.' << std::endl;
				return std::make_shared<value>();
			}
			else
			{
				rank = arr->at(4)->as_double();
			}
		}
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile()->data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgVehicles");
			auto vehConfig = cfgVehicles->data<sqf::configdata>()->navigate(type);
			if (vehConfig->data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry for '" << type << "' could not be located in `ConfigBin >> CfgVehicles`." << std::endl;
				return std::make_shared<value>();
			}
		}
		auto veh = innerobj::create(vm, type, false);
		grp->add_unit(veh);
		veh->posx(position->at(0)->as_double());
		veh->posy(position->at(1)->as_double());
		veh->posz(position->at(2)->as_double());
		return std::make_shared<value>(std::make_shared<objectdata>(veh), OBJECT);
	}
	std::shared_ptr<value> distance_array_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->data<arraydata>();
		auto r = right->data<arraydata>();
		if (!l->check_type(vm, SCALAR, 3) || !r->check_type(vm, SCALAR, 3))
		{
			return std::make_shared<value>();
		}
		return std::make_shared<value>(arraydata::distance3d(l, r));
	}
	std::shared_ptr<value> distance_object_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->data<objectdata>();
		auto r = right->data<arraydata>();
		if (l->is_null())
		{
			vm->err() << "Left value provided is NULL object." << std::endl;
			return std::shared_ptr<value>();
		}
		if (!r->check_type(vm, SCALAR, 3))
		{
			return std::make_shared<value>();
		}
		return std::make_shared<value>(l->obj()->distance3d(r->as_vec3()));
	}
	std::shared_ptr<value> distance_array_object(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->data<arraydata>();
		auto r = right->data<objectdata>();
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return std::shared_ptr<value>();
		}
		if (!l->check_type(vm, SCALAR, 3))
		{
			return std::make_shared<value>();
		}
		return std::make_shared<value>(r->obj()->distance3d(l->as_vec3()));
	}
	std::shared_ptr<value> distance_object_object(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->data<objectdata>();
		auto r = right->data<objectdata>();
		if (l->is_null())
		{
			vm->err() << "Left value provided is NULL object." << std::endl;
			return std::shared_ptr<value>();
		}
		if (r->is_null())
		{
			vm->err() << "Right value provided is NULL object." << std::endl;
			return std::shared_ptr<value>();
		}
		return std::make_shared<value>(l->obj()->distance3d(r->obj()));
	}
	class nearestobjects_distancesort3d
	{
		std::array<double, 3> pos;
	public:
		nearestobjects_distancesort3d(std::array<double, 3> p) : pos(p) {}
		bool operator() (std::shared_ptr<value> l, std::shared_ptr<value> r) { return l->data<objectdata>()->obj()->distance3d(pos) < r->data<objectdata>()->obj()->distance3d(pos); }
	};
	class nearestobjects_distancesort2d
	{
		std::array<double, 2> pos;
	public:
		nearestobjects_distancesort2d(std::array<double, 2> p) : pos(p) {}
		bool operator() (std::shared_ptr<value> l, std::shared_ptr<value> r) { return l->data<objectdata>()->obj()->distance2d(pos) < r->data<objectdata>()->obj()->distance2d(pos); }
	};
	std::shared_ptr<value> nearestobjects_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto arr = right->data<arraydata>();
		if (arr->size() != 3 && arr->size() != 4)
		{
			vm->err() << "Input array was expected to contain either 3 or 4 elements. Got " << arr->size() << '.' << std::endl;
			return std::shared_ptr<value>();
		}
		std::array<double, 3> position;
		auto dtype = arr->at(0)->dtype();
		if (dtype == ARRAY)
		{
			if (!arr->at(0)->data<arraydata>()->check_type(vm, SCALAR, 3))
			{
				return std::make_shared<value>();
			}
			position = arr->at(0)->data<arraydata>()->as_vec3();
		}
		else if (dtype == OBJECT)
		{
			if (arr->at(0)->data<objectdata>()->is_null())
			{
				vm->err() << "Input array element 0 is NULL object." << std::endl;
				return std::shared_ptr<value>();
			}
			position = arr->at(0)->data<objectdata>()->obj()->pos();
		}
		else
		{
			vm->err() << "Input array element 0 was expected to be of type ARRAY or OBJECT. Got " << type_str(arr->at(0)->dtype()) << '.' << std::endl;
			return std::shared_ptr<value>();
		}
		if (arr->at(1)->dtype() != ARRAY)
		{
			vm->err() << "Input array element 1 was expected to be of type ARRAY. Got " << type_str(arr->at(1)->dtype()) << '.' << std::endl;
			return std::shared_ptr<value>();
		}
		auto filterarr = arr->at(1)->data<arraydata>();
		for (size_t i = 0; i < filterarr->size(); i++)
		{
			if (filterarr->at(i)->dtype() != STRING)
			{
				vm->err() << "Input array element 1 contains non-string element at array index " << i << ". Got " << type_str(filterarr->at(i)->dtype()) << '.' << std::endl;
				return std::shared_ptr<value>();
			}
		}
		if (arr->at(2)->dtype() != SCALAR)
		{
			vm->err() << "Input array element 2 was expected to be of type SCALAR. Got " << type_str(arr->at(2)->dtype()) << '.' << std::endl;
			return std::shared_ptr<value>();
		}
		auto radius = arr->at(2)->as_double();
		auto is2ddistance = false;
		if (arr->size() == 4)
		{
			if (arr->at(3)->dtype() != sqf::BOOL)
			{
				vm->err() << "Input array element 3 was expected to be of type BOOLEAN. Got " << type_str(arr->at(3)->dtype()) << '.' << std::endl;
				return std::shared_ptr<value>();
			}
			is2ddistance = arr->at(3)->as_bool();
		}
		auto outputarr = std::make_shared<arraydata>();
		if (is2ddistance)
		{
			auto position2d = std::array<double, 2>{ position[0], position[1] };
			for (auto& it = vm->get_objlist_iterator_begin(); it != vm->get_objlist_iterator_end(); it++)
			{
				if ((*it)->distance2d(position2d) <= radius)
				{
					bool match = filterarr->size() == 0 || !vm->perform_classname_checks() ? true : false;
					if (!match)
					{
						for (size_t i = 0; i < filterarr->size(); i++)
						{
							auto str = filterarr->at(i)->as_string();
							if ((*it)->iskindof(str))
							{
								match = true;
								break;
							}
						}
					}
					if (match)
					{
						outputarr->push_back(std::make_shared<value>(std::make_shared<objectdata>(*it), OBJECT));
					}
				}
			}
			std::sort(outputarr->begin(), outputarr->end(), nearestobjects_distancesort2d(position2d));
		}
		else
		{
			for (auto& it = vm->get_objlist_iterator_begin(); it != vm->get_objlist_iterator_end(); it++)
			{
				if ((*it)->distance3d(position) <= radius)
				{
					bool match = filterarr->size() == 0 || !vm->perform_classname_checks() ? true : false;
					if (!match)
					{
						for (size_t i = 0; i < filterarr->size(); i++)
						{
							auto str = filterarr->at(i)->as_string();
							if ((*it)->iskindof(str))
							{
								match = true;
								break;
							}
						}
					}
					if (match)
					{
						outputarr->push_back(std::make_shared<value>(std::make_shared<objectdata>(*it), OBJECT));
					}
				}
			}
			std::sort(outputarr->begin(), outputarr->end(), nearestobjects_distancesort3d(position));
		}
		return std::make_shared<value>(outputarr, ARRAY);
	}
}
void sqf::commandmap::initobjectcmds(void)
{
	//GetVariable & SetVariable & AllVariables are in namespacecmds as simple alias.
	add(nular("objNull", "A non-existent Object. To compare non-existent objects use isNull or isEqualTo.", objnull_));
	add(unary("typeOf", sqf::type::OBJECT, "Returns the config class name of given object.", typeof_object));
	add(unary("createVehicle", type::ARRAY, "Creates an empty object of given classname type.", createvehicle_array));
	add(binary(4, "createVehicle", type::STRING, type::ARRAY, "Creates an empty object of given classname type.", createvehicle_string_array));
	add(binary(4, "createVehicleLocal", type::ANY, type::ANY, "Creates an empty object of given classname type.", createvehicle_string_array));
	add(unary("deleteVehicle", type::OBJECT, "Deletes an object.", deletevehicle_array));
	add(unary("position", type::OBJECT, "Returns the object position in format PositionAGLS. Z value is height over the surface underneath.", position_object));
	add(unary("getPos", type::OBJECT, "Returns the object position in format PositionAGLS. Z value is height over the surface underneath.", position_object));
	add(binary(4, "setPos", type::OBJECT, type::ARRAY, "", setpos_object_array));
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
	add(unary("nearestObjects", type::ARRAY, "Returns a list of nearest objects of the given types to the given position or object, within the specified distance. If more than one object is found they will be ordered by proximity, the closest one will be first in the array.", nearestobjects_array));

}
