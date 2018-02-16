#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "sidedata.h"
#include <sstream>

using namespace sqf;
namespace
{
}
void sqf::commandmap::initgroupcmds(void)
{
	add(nular(L"blufor", L"Western side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::West); }));
	add(nular(L"west", L"Western side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::West); }));
	add(nular(L"opfor", L"Eastern side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::East); }));
	add(nular(L"east", L"Eastern side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::East); }));
	add(nular(L"resistance", L"Guerilla side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::Guerilla); }));
	add(nular(L"independent", L"Guerilla side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::Guerilla); }));
	add(nular(L"civilian", L"Civilian side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::Civilian); }));
	add(nular(L"sideEmpty", L"Empty side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::Empty); }));
	add(nular(L"sideEnemy", L"Enemy side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::Enemy); }));
	add(nular(L"sideFriendly", L"Friendly side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::Friendly); }));
	add(nular(L"sideLogic", L"Logic side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::Logic); }));
	add(nular(L"sideUnknown", L"Unknown side.", [](virtualmachine* vm) -> std::shared_ptr<sqf::value> { return std::make_shared<value>(sidedata::eside::Unknown); }));
}
