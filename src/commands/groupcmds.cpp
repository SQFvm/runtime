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
			vm->wrn() << "Attempt to delete a non-empty group was made. GroupID: " << grp->groupid() << '.' << std::endl;
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
}
