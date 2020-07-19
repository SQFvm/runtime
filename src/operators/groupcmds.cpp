#ifndef NO_COMMANDS
#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include "../sidedata.h"
#include "../groupdata.h"
#include "../objectdata.h"
#include "../innerobj.h"
#include "../arraydata.h"
#include <sstream>
#include <algorithm>

using namespace sqf;
namespace
{
	value grpnull_(virtualmachine* vm)
	{
		return value(groupdata::create());
	}
	value creategroup_side(virtualmachine* vm, value::cref right)
	{
		auto side = right.data<sidedata>();
		return value(groupdata::create(vm, side));
	}
	value groupid_group(virtualmachine* vm, value::cref right)
	{
		auto grp = right.data<groupdata>();
		return value(grp->groupid());
	}
	value units_group(virtualmachine* vm, value::cref right)
	{
		auto grp = right.data<groupdata>();
		auto arr = std::make_shared<arraydata>();
		for (auto& unit : grp->get_units())
		{
			arr->push_back(value(std::make_shared<objectdata>(unit)));
		}
		return value(arr);
	}
	value units_object(virtualmachine* vm, value::cref right)
	{
		auto grp = right.data<objectdata>()->obj()->group();
		auto arr = std::make_shared<arraydata>();
		if (grp)
			for (auto& unit : grp->get_units())
			{
				arr->push_back(value(std::make_shared<objectdata>(unit)));
			}
		return value(arr);
	}
	value deletegroup_group(virtualmachine* vm, value::cref right)
	{
		auto grp = right.data<groupdata>();
		if (grp->is_empty())
		{
			vm->drop_group(grp);
		}
		else
		{
			vm->logmsg(logmessage::runtime::GroupNotEmpty(*vm->current_instruction(), grp->groupid()));
		}
		return {};
	}
	value isnull_group(virtualmachine* vm, value::cref right)
	{
		auto grp = right.data<groupdata>();
		return grp->is_null();
	}
	value side_group(virtualmachine* vm, value::cref right)
	{
		auto grp = right.data<groupdata>();
		return value(grp->side());
	}
	value selectleader_group_object(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto grp = right.data<groupdata>();
		auto leader = right.data<objectdata>();
		if (grp->is_null())
		{
			vm->wrn() << "Provided group is null." << std::endl;
			return {};
		}
		if (leader->is_null())
		{
			vm->wrn() << "Provided object is null." << std::endl;
			return {};
		}
		auto res = std::find(grp->get_units().begin(), grp->get_units().end(), leader->obj());
		if (res == grp->get_units().end())
		{
			vm->wrn() << "Provided object '" << leader->tosqf() << "' is not part of provided group '" << grp->groupid() << "'." << std::endl;
		}
		grp->leader(leader->obj());
		return {};
	}
	value allvariables_namespace(runtime& runtime, value::cref right)
	{
		std::shared_ptr<varscope> r;
		if (right.dtype() == OBJECT)
		{
			auto obj = right.data<objectdata>();
			if (obj->is_null())
			{
				vm->logmsg(err::ExpectedNonNullValueWeak(*vm->current_instruction()));
				vm->logmsg(err::ReturningEmptyArray(*vm->current_instruction()));
				return value(std::make_shared<arraydata>());
			}
			r = std::dynamic_pointer_cast<varscope>(obj->obj());
		}
		else
		{
			r = std::dynamic_pointer_cast<varscope>(right.data());
		}
		std::vector<value> arr(r->get_variable_map().size());
		transform(r->get_variable_map().begin(), r->get_variable_map().end(), arr.begin(), [](auto pair) { return value(pair.first); });
		return value(arr);
	}
	value getVariable_namespace_string(runtime& runtime, value::cref left, value::cref right)
	{
		std::shared_ptr<varscope> l;
		if (left.dtype() == OBJECT)
		{
			auto obj = left.data<objectdata>();
			if (obj->is_null())
			{
				vm->logmsg(err::ExpectedNonNullValueWeak(*vm->current_instruction()));
				vm->logmsg(err::ReturningNil(*vm->current_instruction()));
				return {};
			}
			l = std::dynamic_pointer_cast<varscope>(obj->obj());
		}
		else
		{
			l = std::dynamic_pointer_cast<varscope>(left.data());
		}
		auto r = right.as_string();
		return l->get_variable(r);
	}
	value getVariable_namespace_array(runtime& runtime, value::cref left, value::cref right)
	{
		std::shared_ptr<varscope> l;
		if (left.dtype() == OBJECT)
		{
			auto obj = left.data<objectdata>();
			if (obj->is_null())
			{
				vm->logmsg(err::ExpectedNonNullValueWeak(*vm->current_instruction()));
				vm->logmsg(err::ReturningNil(*vm->current_instruction()));
				return {};
			}
			l = std::dynamic_pointer_cast<varscope>(obj->obj());
		}
		else
		{
			l = std::dynamic_pointer_cast<varscope>(left.data());
		}
		auto r = right.as_vector();
		if (r.size() != 2)
		{
			vm->logmsg(err::ExpectedArraySizeMissmatch(*vm->current_instruction(), 2, r.size()));
			return {};
		}
		if (r[0].dtype() != sqf::type::STRING)
		{
			vm->logmsg(err::ExpectedArrayTypeMissmatch(*vm->current_instruction(), 2, sqf::type::STRING, r[0].dtype()));
			return {};
		}
		bool success = false;
		auto var = l->get_variable(r[0].as_string(), success);
		if (success)
		{
			return var;
		}
		else
		{
			return r[1];
		}
	}
	value setVariable_namespace_array(runtime& runtime, value::cref left, value::cref right)
	{
		std::shared_ptr<varscope> l;
		if (left.dtype() == OBJECT)
		{
			auto obj = left.data<objectdata>();
			if (obj->is_null())
			{
				vm->logmsg(err::ExpectedNonNullValueWeak(*vm->current_instruction()));
				vm->logmsg(err::ReturningNil(*vm->current_instruction()));
				return {};
			}
			l = std::dynamic_pointer_cast<varscope>(obj->obj());
		}
		else
		{
			l = std::dynamic_pointer_cast<varscope>(left.data());
		}
		auto r = right.as_vector();
		if (r.size() != 2 && r.size() != 3)
		{
			vm->logmsg(err::ExpectedArraySizeMissmatch(*vm->current_instruction(), 2, r.size()));
			return {};
		}
		//Third element is ignored due to no networking in sqf-vm
		if (r[0].dtype() != sqf::type::STRING)
		{
			vm->logmsg(err::ExpectedArrayTypeMissmatch(*vm->current_instruction(), 2, sqf::type::STRING, r[0].dtype()));
			return {};
		}
		auto val = r[1];
		l->set_variable(r[0].as_string(), val);
		return {};
	}
}
void sqf::commandmap::initgroupcmds()
{
	//GetVariable & SetVariable & AllVariables are in namespacecmds as simple alias.
	add(nular("blufor", "Western side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::West)); }));
	add(nular("west", "Western side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::West)); }));
	add(nular("opfor", "Eastern side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::East)); }));
	add(nular("east", "Eastern side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::East)); }));
	add(nular("resistance", "Guerilla side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::Guerilla)); }));
	add(nular("independent", "Guerilla side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::Guerilla)); }));
	add(nular("civilian", "Civilian side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::Civilian)); }));
	add(nular("sideEmpty", "Empty side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::Empty)); }));
	add(nular("sideEnemy", "Enemy side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::Enemy)); }));
	add(nular("sideFriendly", "Friendly side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::Friendly)); }));
	add(nular("sideLogic", "Logic side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::Logic)); }));
	add(nular("sideUnknown", "Unknown side.", [](virtualmachine* vm) -> value { return value(std::make_shared<sidedata>(sidedata::Unknown)); }));

	add(nular("grpNull", "A non-existing Group. To compare non-existent groups use isNull or isEqualTo.", grpnull_));
	add(unary("createGroup", type::SIDE, "Creates a new Group for the given Side.", creategroup_side));
	add(unary("groupId", type::GROUP, "Returns group name.", groupid_group));
	add(unary("units", type::GROUP, "Returns an array with all the units in the group.", units_group));
	add(unary("units", type::OBJECT, "Returns an array with all the units in the group of the unit. For a destroyed object an empty array is returned.", units_object));
	add(unary("deleteGroup", type::GROUP, "Destroys the given group. Group must be empty.", deletegroup_group));
	add(unary("isNull", type::GROUP, "Checks whether the tested item is Null.", isnull_group));
	add(unary("side", type::GROUP, "Returns the side of a group.", side_group));
	add(binary(4, "selectLeader", type::GROUP, type::OBJECT, "Select the group's leader.", selectleader_group_object));



	runtime.register_sqfop(unary("allVariables", type::GROUP, "Returns a list of all variables from desired namespace.", allvariables_namespace));
	runtime.register_sqfop(binary(4, "getVariable", type::GROUP, type::STRING, "Return the value of variable in the variable space assigned to various data types. Returns nil if variable is undefined.", getVariable_namespace_string));
	runtime.register_sqfop(binary(4, "getVariable", type::GROUP, type::ARRAY, "Return the value of variable in the provided variable space. First element is expected to be the variable name as string. Returns second array item if variable is undefined.", getVariable_namespace_array));
	runtime.register_sqfop(binary(4, "setVariable", type::GROUP, type::ARRAY, "Sets a variable to given value in the provided variable space. First element is expected to be the variable name as string. Second element is expected to be anything.", setVariable_namespace_array));
}

#endif