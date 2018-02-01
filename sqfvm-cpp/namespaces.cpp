#include "full.h"
sqf::sqfnamespace sqf::commands::namespaces::MissionNamespace = sqf::sqfnamespace(L"missionNamespace");
sqf::sqfnamespace sqf::commands::namespaces::UiNamespace = sqf::sqfnamespace(L"uiNamespace");
sqf::sqfnamespace sqf::commands::namespaces::ParsingNamespace = sqf::sqfnamespace(L"parsingNamespace");
sqf::sqfnamespace sqf::commands::namespaces::ProfileNamespace = sqf::sqfnamespace(L"profileNamespace");

using namespace sqf;
void sqf::commandmap::initnamespaces(void)
{
	add(nular(L"missionNamespace", L"Returns the global namespace attached to mission.",
		[](const virtualmachine* vm) -> value_s { return std::make_shared<value>(sqf::commands::namespaces::missionNamespace()); }));
	add(nular(L"uiNamespace", L"The value of PI.",
		[](const virtualmachine* vm) -> value_s { return std::make_shared<value>(sqf::commands::namespaces::uiNamespace()); }));
	add(nular(L"parsingNamespace", L"Returns the global namespace attached to config parser.",
		[](const virtualmachine* vm) -> value_s { return std::make_shared<value>(sqf::commands::namespaces::parsingNamespace()); }));
	add(nular(L"profileNamespace", L"Returns the global namespace attached to the active user profile. Use setVariable and getVariable to save and load data to and from this Namespace. A variable can be deleted by setting its value to nil. By default the variables set in this namespace will exist while the game is running. In order to make variables save permanently, use saveProfileNamespace before the game is over.",
		[](const virtualmachine* vm) -> value_s { return std::make_shared<value>(sqf::commands::namespaces::profileNamespace()); }));
}
