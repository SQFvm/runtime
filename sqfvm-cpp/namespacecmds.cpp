#include "full.h"
#include <algorithm>

sqf::namespace_s sqf::commands::namespaces::MissionNamespace = std::make_shared<sqf::sqfnamespace>(L"missionNamespace");
sqf::namespace_s sqf::commands::namespaces::UiNamespace = std::make_shared< sqf::sqfnamespace>(L"uiNamespace");
sqf::namespace_s sqf::commands::namespaces::ParsingNamespace = std::make_shared<sqf::sqfnamespace>(L"parsingNamespace");
sqf::namespace_s sqf::commands::namespaces::ProfileNamespace = std::make_shared<sqf::sqfnamespace>(L"profileNamespace");

using namespace sqf;
namespace
{
	value_s allvariables_namespace(const virtualmachine* vm, value_s right)
	{
		auto r = std::static_pointer_cast<sqfnamespace>(right->as_data());
		std::vector<value_s> arr(r->varmap().size());
		transform(r->varmap().begin(), r->varmap().end(), arr.begin(), [](auto pair) { return std::make_shared<value>(pair.first); });
		return std::make_shared<value>(arr);
	}
}
void sqf::commandmap::initnamespacecmds(void)
{
	add(nular(L"missionNamespace", L"Returns the global namespace attached to mission.",
		[](const virtualmachine* vm) -> value_s { return std::make_shared<value>(sqf::commands::namespaces::missionNamespace(), sqf::type::NAMESPACE); }));
	add(nular(L"uiNamespace", L"The value of PI.",
		[](const virtualmachine* vm) -> value_s { return std::make_shared<value>(sqf::commands::namespaces::uiNamespace(), sqf::type::NAMESPACE); }));
	add(nular(L"parsingNamespace", L"Returns the global namespace attached to config parser.",
		[](const virtualmachine* vm) -> value_s { return std::make_shared<value>(sqf::commands::namespaces::parsingNamespace(), sqf::type::NAMESPACE); }));
	add(nular(L"profileNamespace", L"Returns the global namespace attached to the active user profile. Use setVariable and getVariable to save and load data to and from this Namespace. A variable can be deleted by setting its value to nil. By default the variables set in this namespace will exist while the game is running. In order to make variables save permanently, use saveProfileNamespace before the game is over.",
		[](const virtualmachine* vm) -> value_s { return std::make_shared<value>(sqf::commands::namespaces::profileNamespace(), sqf::type::NAMESPACE); }));
	add(unary(L"allVariables", type::NAMESPACE, L"Returns a list of all variables from desired namespace.", allvariables_namespace));
}
