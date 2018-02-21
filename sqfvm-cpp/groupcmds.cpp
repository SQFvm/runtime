#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "sidedata.h"
#include "groupdata.h"
#include <sstream>

using namespace sqf;
namespace
{
	std::shared_ptr<value> grpnull_(virtualmachine* vm)
	{
		return std::make_shared<value>(groupdata::create(), type::GROUP);
	}
	std::shared_ptr<value> creategroup_side(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto side = right->data<sidedata>();
		return std::make_shared<value>(groupdata::create(vm, side), type::GROUP);
	}
	std::shared_ptr<value> groupid_group(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto grp = right->data<groupdata>();
		return std::make_shared<value>(grp->groupid());
	}
	std::shared_ptr<value> allvariables_group(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->data<groupdata>();
		std::vector<std::shared_ptr<value>> arr(r->varmap().size());
		transform(r->varmap().begin(), r->varmap().end(), arr.begin(), [](auto pair) { return std::make_shared<value>(pair.first); });
		return std::make_shared<value>(arr);
	}
}
void sqf::commandmap::initgroupcmds(void)
{
	//GetVariable & SetVariable are in namespacecmds as simple alias.
	add(nular(L"blufor", L"Western side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::West), type::SIDE); }));
	add(nular(L"west", L"Western side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::West), type::SIDE); }));
	add(nular(L"opfor", L"Eastern side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::East), type::SIDE); }));
	add(nular(L"east", L"Eastern side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::East), type::SIDE); }));
	add(nular(L"resistance", L"Guerilla side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Guerilla), type::SIDE); }));
	add(nular(L"independent", L"Guerilla side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Guerilla), type::SIDE); }));
	add(nular(L"civilian", L"Civilian side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Civilian), type::SIDE); }));
	add(nular(L"sideEmpty", L"Empty side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Empty), type::SIDE); }));
	add(nular(L"sideEnemy", L"Enemy side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Enemy), type::SIDE); }));
	add(nular(L"sideFriendly", L"Friendly side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Friendly), type::SIDE); }));
	add(nular(L"sideLogic", L"Logic side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Logic), type::SIDE); }));
	add(nular(L"sideUnknown", L"Unknown side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Unknown), type::SIDE); }));

	add(nular(L"grpNull", L"A non-existing Group. To compare non-existent groups use isNull or isEqualTo.", grpnull_));
	add(unary(L"createGroup", type::SIDE, L"Creates a new Group for the given Side.", creategroup_side));
	add(unary(L"groupId", type::GROUP, L"Returns group name.", groupid_group));
	add(unary(L"allVariables", type::GROUP, L"Returns a list of all variables from desired group.", allvariables_group));
}
