#pragma once
#include "../runtime/instruction.h"
#include "../runtime/value.h"
#include "../runtime/data.h"
#include "../runtime/runtime.h"

namespace sqf::opcodes
{
    class assign_to_local : public sqf::runtime::instruction
    {
    private:
#if _DEBUG
        std::string ___TYPE = "assign_to_local";
#endif
        std::string m_variable_name;

    public:
        assign_to_local(std::string value) : m_variable_name(value) {}
        assign_to_local(std::string_view value) : m_variable_name(value.begin(), value.end()) {}
        virtual void execute(sqf::runtime::runtime& vm) const override
        {
            auto& context = vm.context_active();

            auto value = vm.context_active().pop_value();
            if (m_variable_name.empty()) { return; }
            if (!value.has_value())
            {
                if (context.weak_error_handling())
                {
                    vm.__logmsg(logmessage::runtime::FoundNoValueWeak(diag_info()));
                }
                else
                {
                    vm.__logmsg(logmessage::runtime::FoundNoValue(diag_info()));
                }
                return;
            }
            else if (value->is<sqf::types::t_nothing>())
            {
                vm.__logmsg(logmessage::runtime::AssigningNilValue(diag_info(), m_variable_name));
            }

            context.current_frame()[m_variable_name] = *value;
        }
        virtual std::string to_string() const override { return std::string("ASSIGNTOLOCAL ") + m_variable_name; }
        std::string_view variable_name() const { return m_variable_name; }

        virtual std::optional<std::string> reconstruct(
            std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator& current,
            std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator end,
            short parent_precedence, bool left_from_binary) const override
        {
            if (++current == end)
            {
                return {};
            }
            auto exp = (*current)->reconstruct(current, end, 10, false);
            if (!exp.has_value())
            {
                return {};
            }
#if defined(SQFVM_ARMA2_SYNTAX)
            return "local " + m_variable_name + " = " + *exp;
#else
            return "private " + m_variable_name + " = " + *exp;
#endif
        }

        virtual bool equals(const instruction* p_other) const override
        {
            auto casted = dynamic_cast<const assign_to_local*>(p_other);
            return casted != nullptr && casted->m_variable_name == m_variable_name;
        }
    };
}