#pragma once
#include <string>
#include <string_view>
#include <memory>
#include <vector>

#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"
#include "../runtime/confighost.h"


namespace sqf
{
    namespace runtime
    {
        struct t_config : public type::extend<t_config> { t_config() : extend() {} static const std::string name() { return "CONFIG"; } };
    }
    namespace types
    {
        class d_config : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_config;
        private:
            sqf::runtime::config m_value;
        protected:
            bool do_equals(std::shared_ptr<data> other, bool invariant) const override
            {
                auto other_config = std::static_pointer_cast<d_config>(other)->m_value;
                return other_config == m_value;
            }
        public:
            d_config() = default;
            d_config(sqf::runtime::config conf) : m_value(conf) {}

            std::string to_string_sqf() const override
            {
                return std::string(m_value.name());
            }
            std::string to_string() const override
            {
                return std::string(m_value.name());
            }
            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return std::hash<size_t>()(m_value.container_id()); }
            sqf::runtime::config value() const { return m_value; }
            void value(sqf::runtime::config conf) { m_value = conf; }

            operator sqf::runtime::config() { return m_value; }
        };

        template<>
        inline std::shared_ptr<sqf::runtime::data> to_data<sqf::runtime::config>(sqf::runtime::config conf)
        {
            return std::make_shared<d_config>(conf);
        }
        template<>
        inline std::shared_ptr<sqf::runtime::data> to_data<sqf::runtime::confignav>(sqf::runtime::confignav confignav)
        {
            return std::make_shared<d_config>(*confignav);
        }
    }
}