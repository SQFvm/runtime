#pragma once
#include <string>
#include <string_view>
#include <memory>

#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"


namespace sqf
{
    namespace runtime
    {
        struct t_text : public type::extend<t_text> { t_text() : extend() {} static const std::string name() { return "TEXT"; } };
    }
    namespace types
    {
        class d_text : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_text;
        private:
            std::string m_value;
        protected:
            bool do_equals(std::shared_ptr<data> other, bool invariant) const override
            {
                auto other_text = std::static_pointer_cast<d_text>(other)->m_value;
                return other_text == m_value;
            }
        public:
            d_text() = default;
            d_text(std::string str) : m_value(str) {}

            std::string to_string_sqf() const override
            {
                return m_value;
            }
            std::string to_string() const override
            {
                return m_value;
            }
            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return std::hash<std::string>()(m_value); }
            std::string value() const { return m_value; }
            void value(std::string str) { m_value = str; }

            operator std::string() { return m_value; }
            operator std::string_view() { return m_value; }
        };
    }
}
