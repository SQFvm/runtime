#pragma once
#include "../runtime/instruction.h"
#include "../runtime/value.h"
#include "../runtime/data.h"
#include "../runtime/runtime.h"
#include "../runtime/logging.h"
#include "../runtime/d_array.h"

#include <sstream>

namespace sqf::runtime
{
    class runtime;
}
namespace sqf::opcodes
{
    class make_array : public sqf::runtime::instruction
    {
#if _DEBUG
        std::string ___TYPE = "make_array";
#endif
    private:
        size_t m_array_size;

    public:
        make_array(size_t array_size) : m_array_size(array_size) {}
        virtual void execute(sqf::runtime::runtime& vm) const override
        {
            auto vec = std::vector<sqf::runtime::value>(m_array_size);
            for (size_t i = m_array_size - 1; i != (size_t)~0; i--)
            {
                auto opt = vm.context_active().pop_value();
                if (!opt.has_value())
                {
                    vm.__logmsg(logmessage::runtime::StackCorruptionMissingValues(diag_info(), m_array_size, i));
                    break;
                }
                else
                {
                    vec[i] = *opt;
                }
            }
            vm.context_active().push_value(std::make_shared<sqf::types::d_array>(vec));
        }
        virtual std::string to_string() const override { return std::string("MAKEARRAY ") + std::to_string(m_array_size); }
        size_t array_size() const { return m_array_size; }

        virtual std::optional<std::string> reconstruct(
            std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator& current,
            std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator end,
            short parent_precedence, bool left_from_binary) const override
        {
            std::stringstream sstream;
            auto strvec = std::vector<std::string>(array_size());
            sstream << "[";
            for (size_t i = 0; i < array_size(); i++)
            {
                if (++current == end)
                {
                    return {};
                }
                auto exp = (*current)->reconstruct(current, end, 0, false);
                if (!exp.has_value())
                { 
                    return {};
                }
                strvec[i] = *exp;
            }
            for (auto it = strvec.rbegin(); it != strvec.rend(); it++)
            {
                if (it != strvec.rbegin())
                {
                    sstream << ", ";
                }
                sstream << *it;
            }
            sstream << "]";
            return sstream.str();
        }

        virtual bool equals(const instruction* p_other) const override
        {
            auto casted = dynamic_cast<const make_array*>(p_other);
            return casted != nullptr && casted->m_array_size == m_array_size;
        }
    };
}
