#pragma once
#include "../runtime/instruction.h"
#include "../runtime/runtime.h"

namespace sqf::opcodes
{
    class end_statement : public sqf::runtime::instruction
    {
#if _DEBUG
        std::string ___TYPE = "end_statement";
#endif
    public:
        virtual void execute(sqf::runtime::runtime& vm) const override { vm.context_active().clear_values(); }
        virtual std::string to_string() const override { return "ENDSTATEMENT"; }
        virtual std::optional<std::string> reconstruct(
            std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator& current,
            std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator end,
            short parent_precedence, bool left_from_binary) const override
        {
            return "";
        }

        virtual bool equals(const instruction* p_other) const override
        {
            auto casted = dynamic_cast<const end_statement*>(p_other);
            return casted != nullptr;
        }
    };
}
