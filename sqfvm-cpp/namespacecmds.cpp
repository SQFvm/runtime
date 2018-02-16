#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "sqfnamespace.h"
#include "namespaces.h"
#include <algorithm>

std::shared_ptr<sqf::sqfnamespace> sqf::commands::namespaces::MissionNamespace = std::make_shared<sqf::sqfnamespace>(L"missionNamespace");
std::shared_ptr<sqf::sqfnamespace> sqf::commands::namespaces::UiNamespace = std::make_shared< sqf::sqfnamespace>(L"uiNamespace");
std::shared_ptr<sqf::sqfnamespace> sqf::commands::namespaces::ParsingNamespace = std::make_shared<sqf::sqfnamespace>(L"parsingNamespace");
std::shared_ptr<sqf::sqfnamespace> sqf::commands::namespaces::ProfileNamespace = std::make_shared<sqf::sqfnamespace>(L"profileNamespace");

using namespace sqf;
namespace
{
	std::shared_ptr<value> allvariables_namespace(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->data<sqfnamespace>();
		std::vector<std::shared_ptr<value>> arr(r->varmap().size());
		transform(r->varmap().begin(), r->varmap().end(), arr.begin(), [](auto pair) { return std::make_shared<value>(pair.first); });
		return std::make_shared<value>(arr);
	}
}
void sqf::commandmap::initnamespacecmds(void)
{
	add(nular(L"missionNamespace", L"Returns the global namespace attached to mission.",
		[](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(sqf::commands::namespaces::missionNamespace(), sqf::type::NAMESPACE); }));
	add(nular(L"uiNamespace", L"The value of PI.",
		[](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(sqf::commands::namespaces::uiNamespace(), sqf::type::NAMESPACE); }));
	add(nular(L"parsingNamespace", L"Returns the global namespace attached to config parser.",
		[](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(sqf::commands::namespaces::parsingNamespace(), sqf::type::NAMESPACE); }));
	add(nular(L"profileNamespace", L"Returns the global namespace attached to the active user profile. Use setVariable and getVariable to save and load data to and from this Namespace. A variable can be deleted by setting its value to nil. By default the variables set in this namespace will exist while the game is running. In order to make variables save permanently, use saveProfileNamespace before the game is over.",
		[](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(sqf::commands::namespaces::profileNamespace(), sqf::type::NAMESPACE); }));
	add(unary(L"allVariables", type::NAMESPACE, L"Returns a list of all variables from desired namespace.", allvariables_namespace));
}
