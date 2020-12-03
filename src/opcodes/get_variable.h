#pragma once
#include "../runtime/instruction.h"
#include "../runtime/runtime.h"
#include "../runtime/value.h"
#include "../runtime/value_scope.h"

#include <string>

namespace sqf::runtime
{
    class runtime;
}
namespace sqf::opcodes
{
    class get_variable : public sqf::runtime::instruction
    {
    private:
#if _DEBUG
        std::string ___TYPE = "get_variable";
#endif
        std::string m_variable_name;
    public:
        get_variable(std::string variable_name) : m_variable_name(variable_name) {}
        get_variable(std::string_view value) : m_variable_name(value.begin(), value.end()) {}
        virtual void execute(sqf::runtime::runtime& vm) const override
        {
            if (m_variable_name[0] == '_')
            { // local variable
                auto opt = vm.context_active().get_variable(m_variable_name);
                if (opt.has_value())
                {
                    vm.context_active().push_value(*opt);
                }
                else
                {
                    vm.__logmsg(logmessage::runtime::VariableNotFound(diag_info(), m_variable_name));
                    vm.context_active().push_value({});
                }
            }
            else
            { // global variable
                auto global_scope = vm.context_active().current_frame().globals_value_scope();
                if (global_scope->contains(m_variable_name))
                {
                    vm.context_active().push_value(global_scope->at(m_variable_name));
                }
                else
                {
                    vm.__logmsg(logmessage::runtime::VariableNotFound(diag_info(), m_variable_name));
                    vm.context_active().push_value({});
                }
            }
        }
        std::string to_string() const override { return "GETVARIABLE " + m_variable_name; }
        std::string variable_name() const { return m_variable_name; }

        virtual std::optional<std::string> reconstruct(
            std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator& current,
            std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator end,
            short parent_precedence, bool left_from_binary) const override
        {
            return m_variable_name;
        }

        virtual bool equals(const instruction* p_other) const override
        {
            auto casted = dynamic_cast<const get_variable*>(p_other);
            return casted != nullptr && casted->m_variable_name == m_variable_name;
        }
    };
}
