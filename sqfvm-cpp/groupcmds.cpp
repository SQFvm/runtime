#include "full.h"
#include <sstream>

using namespace sqf;
namespace
{
}
void sqf::commandmap::initgroupcmds(void)
{
	add(nular(L"blufor", L"Western side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::West); }));
	add(nular(L"west", L"Western side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::West); }));
	add(nular(L"opfor", L"Eastern side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::East); }));
	add(nular(L"east", L"Eastern side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::East); }));
	add(nular(L"resistance", L"Guerilla side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::Guerilla); }));
	add(nular(L"independent", L"Guerilla side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::Guerilla); }));
	add(nular(L"civilian", L"Civilian side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::Civilian); }));
	add(nular(L"sideEmpty", L"Empty side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::Empty); }));
	add(nular(L"sideEnemy", L"Enemy side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::Enemy); }));
	add(nular(L"sideFriendly", L"Friendly side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::Friendly); }));
	add(nular(L"sideLogic", L"Logic side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::Logic); }));
	add(nular(L"sideUnknown", L"Unknown side.", [](const virtualmachine* vm) -> sqf::value_s { return std::make_shared<value>(sidedata::eside::Unknown); }));
}
