#pragma once
#include "../runtime/instruction.h"
#include "../runtime/value.h"
#include "../runtime/data.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"



namespace sqf::opcodes
{
    class call_binary : public sqf::runtime::instruction
    {
#if _DEBUG
        std::string ___TYPE = "call_binary";
#endif
    private:
        std::string m_operator_name;
        short m_precedence;

    public:
        call_binary(std::string key, short precedence) : m_operator_name(key), m_precedence(precedence) {}
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



            auto left_value = vm.context_active().pop_value();
            if (!left_value.has_value())
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
            else if (left_value->is<sqf::types::t_nothing>())
            {
                vm.__logmsg(logmessage::runtime::NilValueFoundForRightArgumentWeak(diag_info()));
                return;
            }

            auto tleft = left_value->type();
            auto tright = right_value->type();
            sqf::runtime::sqfop_binary::key key = { m_operator_name, tleft, tright };

            if (!vm.sqfop_exists(key))
            {
                key = { m_operator_name, sqf::types::t_any(), tright };
                if (!vm.sqfop_exists(key))
                {
                    key = { m_operator_name, tleft, sqf::types::t_any() };
                    if (!vm.sqfop_exists(key))
                    {
                        key = { m_operator_name, sqf::types::t_any(), sqf::types::t_any() };
                        if (!vm.sqfop_exists(key))
                        {
                            vm.__logmsg(logmessage::runtime::UnknownInputTypeCombinationBinary(diag_info(), tleft, key.name, tright));
                            return;
                        }
                    }
                }
            }
            auto op = vm.sqfop_at(key);
            auto return_value = op.execute(vm, *left_value, *right_value);

            context.push_value(return_value);
        }
        virtual std::string to_string() const override { return std::string("CALLBINARY ") + m_operator_name; }
        std::string_view operator_name() const { return m_operator_name; }
        short precedence() const { return m_precedence; }




        virtual std::optional<std::string> reconstruct(
            std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator& current,
            std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator end,
            short parent_precedence, bool left_from_binary) const override
        {
            auto prec = m_precedence;
            if (++current == end)
            {
                return {};
            }
            auto rexpression = (*current)->reconstruct(current, end, prec, false);
            if (!rexpression.has_value() || ++current == end)
            {
                return {};
            }
            auto lexpression = (*current)->reconstruct(current, end, prec, true);
            if (!lexpression.has_value())
            {
                return {};
            }
            if (left_from_binary ? parent_precedence > prec : parent_precedence >= prec)
            {
                return "(" + *lexpression + " " + m_operator_name + " " + *rexpression + ")";
            }
            else
            {
                return *lexpression + " " + m_operator_name + " " + *rexpression;
            }
        }

        virtual bool equals(const instruction* p_other) const override
        {
            auto casted = dynamic_cast<const call_binary*>(p_other);
            return casted != nullptr && casted->m_operator_name == m_operator_name;
        }
    };
}