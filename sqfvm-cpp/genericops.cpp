#include "full.h"

using namespace sqf;
void sqf::commandmap::initgenericops(void)
{
	add(nular(L"missionNamespace", L"Returns the global namespace attached to mission.", [](const virtualmachine* vm) -> value_s { return std::make_shared<value>(sqf::commands::namespaces::missionNamespace()); }));
}
