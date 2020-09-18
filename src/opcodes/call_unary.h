#pragma once
#include "../runtime/instruction.h"
#include "../runtime/value.h"
#include "../runtime/data.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"


namespace sqf::opcodes
{
    class call_unary : public sqf::runtime::instruction
    {
#if _DEBUG
        std::string ___TYPE = "call_unary";
#endif
    private:
        std::string m_operator_name;

    public:
        call_unary(std::string key) : m_operator_name(key) {}
        virtual void execute(sqf::runtime::runtime& vm) const override
        {
            auto& context = vm.context_active();

            auto right_value = vm.context_active().pop_value();
            if (!right_value.has_value())
            {
                if (context.weak_error_handling())
                {
                    vm.__logmsg(logmessage::runtime::NoValueFoundForRightArgumentWeak(diag_info()));
                }
                else
                {
                    vm.__logmsg(logmessage::runtime::NoValueFoundForRightArgument(diag_info()));
                }
                return;
            }
            else if (right_value->is<sqf::types::t_nothing>())
            {
                vm.__logmsg(logmessage::runtime::NilValueFoundForRightArgumentWeak(diag_info()));
                return;
            }
            
            auto tright = right_value->operator sqf::runtime::type();
            sqf::runtime::sqfop_unary::key key = { m_operator_name, tright };
            if (!vm.sqfop_exists(key))
            {
                key = { m_operator_name, sqf::types::t_any() };
                if (!vm.sqfop_exists(key))
                {
                    vm.__logmsg(logmessage::runtime::UnknownInputTypeCombinationUnary(diag_info(), key.name, tright));
                    return;
                }
            }
            auto op = vm.sqfop_at(key);
            auto return_value = op.execute(vm, *right_value);

            context.push_value(return_value);
        }
        virtual std::string to_string() const override { return std::string("CALLUNARY ") + m_operator_name; }
        std::string_view operator_name() const { return m_operator_name; }
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
            return m_operator_name + " " + *exp;
        }

        virtual bool equals(const instruction* p_other) const override
        {
            auto casted = dynamic_cast<const call_unary*>(p_other);
            return casted != nullptr && casted->m_operator_name == m_operator_name;
        }
    };
}
