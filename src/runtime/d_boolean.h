#pragma once
#include "data.h"
#include "type.h"
#include "value.h"


#include <string>
#include <memory>

namespace sqf
{
    namespace runtime
    {
        struct t_boolean : public type::extend<t_boolean> { t_boolean() : extend() {} static const std::string name() { return "BOOL"; } };
    }
    namespace types
    {
        class d_boolean : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_boolean;
        private:
            bool m_value;
        protected:
            bool do_equals(std::shared_ptr<sqf::runtime::data> other, bool invariant) const override
            {
                return m_value == std::static_pointer_cast<d_boolean>(other)->m_value;
            }
        public:
            d_boolean() = default;
            d_boolean(bool flag) : m_value(flag) {}

            std::string to_string_sqf() const override
            {
                return m_value ? "true" : "false";
            }
            std::string to_string() const override
            {
                return m_value ? "true" : "false";
            }

            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return std::hash<bool>()(m_value); }

            bool value() const { return m_value; }
            void value(bool flag) { m_value = flag; }

            operator bool()
            {
                return m_value;
            }
        };
        template<>
        inline std::shared_ptr<sqf::runtime::data> to_data<bool>(bool flag)
        {
            return std::make_shared<d_boolean>(flag);
        }
    }
}