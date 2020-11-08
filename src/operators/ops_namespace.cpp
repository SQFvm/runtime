#include "ops_namespace.h"
#include "../runtime/value.h"
#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../runtime/version.h"
#include "../runtime/value_scope.h"
#include "../runtime/sqfop.h"
#include "../runtime/d_array.h"
#include "../runtime/d_code.h"
#include "../runtime/d_string.h"


#include <algorithm>


namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;
using namespace std::string_literals;

namespace
{
    value allvariables_namespace(runtime& runtime, value::cref right)
    {
        auto scope = right.data<d_namespace, std::shared_ptr<value_scope>>();
        std::vector<value> variable_names;

        for (auto& kvp : *scope)
        {
            variable_names.push_back(kvp.first);
        }
        return variable_names;
    }
    value with_namespace(runtime& runtime, value::cref right)
    {
        auto scope = right.data<d_namespace, std::shared_ptr<value_scope>>();
        return std::make_shared<d_with>(scope);
    }
    value do_with_code(runtime& runtime, value::cref left, value::cref right)
    {
        auto scope = left.data<d_with, std::shared_ptr<value_scope>>();
        auto set = right.data<d_code, instruction_set>();

        frame f(scope, set);
        runtime.context_active().push_frame(f);
        return {};
    }
    value getVariable_namespace_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto scope = left.data<d_with, std::shared_ptr<value_scope>>();
        auto variable = right.data<d_string, std::string>();

        auto res = scope->try_get(variable);
        if (res.has_value())
        {
            return *res;
        }
        return {};
    }
    value getVariable_namespace_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto scope = left.data<d_with, std::shared_ptr<value_scope>>();
        auto r = right.data<d_array>();
        if (r->size() != 2)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, r->size()));
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (!r->at(0).is<t_string>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, t_string(), r->at(0).type()));
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }

        auto res = scope->try_get(r->at(0).data<d_string, std::string>());
        if (res.has_value())
        {
            return *res;
        }
        return r->at(1);

    }
    value setVariable_namespace_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto scope = left.data<d_with, std::shared_ptr<value_scope>>();
        auto r = right.data<d_array>();
        if (r->size() != 2)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, r->size()));
            return {};
        }
        if (!r->at(0).is<t_string>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, t_string(), r->at(0).type()));
            return {};
        }

        scope->at(r->at(0).data<d_string, std::string>()) = r->at(1);
        return {};
    }
    value currentnamespace_(runtime& runtime)
    {
        return { std::make_shared<d_namespace>(runtime.context_active().current_frame().globals_value_scope()) };
    }
}
void sqf::operators::ops_namespace(::sqf::runtime::runtime& runtime)
{
    using namespace sqf::runtime::sqfop;
    runtime.default_value_scope(missionNamespace);

    runtime.register_sqfop(nular("missionNamespace", "Returns the global namespace attached to mission.",
        [](::sqf::runtime::runtime& runtime) -> value { return std::make_shared<d_namespace>(runtime.get_value_scope(missionNamespace)); }));
    runtime.register_sqfop(nular("uiNamespace", "Returns the global namespace attached to user interface.",
        [](::sqf::runtime::runtime& runtime) -> value { return std::make_shared<d_namespace>(runtime.get_value_scope(uiNamespace)); }));
    runtime.register_sqfop(nular("parsingNamespace", "Returns the global namespace attached to config parser.",
        [](::sqf::runtime::runtime& runtime) -> value { return std::make_shared<d_namespace>(runtime.get_value_scope(parsingNamespace)); }));
    runtime.register_sqfop(nular("profileNamespace", "Returns the global namespace attached to the active user profile.",
        [](::sqf::runtime::runtime& runtime) -> value { return std::make_shared<d_namespace>(runtime.get_value_scope(profileNamespace)); }));

    runtime.register_sqfop(unary("allVariables", t_namespace(), "Returns a list of all variables from desired namespace.", allvariables_namespace));
    runtime.register_sqfop(unary("with", t_namespace(), "Creates a WITH type that is used inside a do construct in order to execute code inside a given namespace.", with_namespace));
    runtime.register_sqfop(binary(4, "do", t_with(), t_code(), "Executes code in the namespace provided via the WITH parameter.", do_with_code));
    runtime.register_sqfop(binary(4, "getVariable", t_namespace(), t_string(), "Return the value of variable in the variable space assigned to various data types. Returns nil if variable is undefined.", getVariable_namespace_string));
    runtime.register_sqfop(binary(4, "getVariable", t_namespace(), t_array(), "Return the value of variable in the provided variable space. First element is expected to be the variable name as string. Returns second array item if variable is undefined.", getVariable_namespace_array));
    runtime.register_sqfop(binary(4, "setVariable", t_namespace(), t_array(), "Sets a variable to given value in the provided variable space. First element is expected to be the variable name as string. Second element is expected to be anything.", setVariable_namespace_array));
    runtime.register_sqfop(nular("currentNamespace", "Returns the current scopes namespace.", currentnamespace_));
}