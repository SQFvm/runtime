#pragma once
#include "../data.h"
#include "../type.h"
#include "../value.h"
#include "stacktrace.h"

#include <string>
#include <memory>

namespace sqf
{
    namespace runtime
    {
        struct t_stacktrace : public type::extend<t_stacktrace> { t_stacktrace() : extend() {} static const std::string name() { return "VM-STACKTRACE"; } };
    }
    namespace types
    {
        class d_stacktrace : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_stacktrace;
        private:
            sqf::runtime::diagnostics::stacktrace m_value;
        protected:
            bool do_equals(std::shared_ptr<sqf::runtime::data> other, bool invariant) const override
            {
                // A stacktrace is never equal to another stacktrace
                return false;
            }
        public:
            d_stacktrace() = default;
            d_stacktrace(sqf::runtime::diagnostics::stacktrace stacktrace) : m_value(stacktrace) {}

            std::string to_string_sqf() const override
            {
                return "'" + m_value.to_string() + "'";
            }
            std::string to_string() const override
            {
                return m_value.to_string();
            }

            sqf::runtime::type type() const override { return data_type(); }

            sqf::runtime::diagnostics::stacktrace value() const { return m_value; }
            void value(sqf::runtime::diagnostics::stacktrace stacktrace) { m_value = stacktrace; }

            operator sqf::runtime::diagnostics::stacktrace()
            {
                return m_value;
            }
            virtual std::size_t hash() const override { return std::hash<std::string>()(to_string_sqf()); }
        };

        template<>
        inline std::shared_ptr<sqf::runtime::data> to_data<sqf::runtime::diagnostics::stacktrace>(sqf::runtime::diagnostics::stacktrace str)
        {
            return std::make_shared<d_stacktrace>(str);
        }


    }
}