#pragma once
#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"
#include "object.h"

#include <string>
#include <sstream>
#include <memory>


namespace sqf
{
    namespace runtime
    {
        struct t_object : public type::extend<t_object> { t_object() : extend() {} static const std::string name() { return "OBJECT"; } };
    }
    namespace types
    {
        class d_object : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_object;
        private:
            std::weak_ptr<object> m_value;
        protected:
            bool do_equals(std::shared_ptr<sqf::runtime::data> other, bool invariant) const override
            {
                return value().get() == std::static_pointer_cast<d_object>(other)->value().get();
            }
        public:
            d_object() = default;
            d_object(std::weak_ptr<object> value) : m_value(value) {}


            std::string to_string_sqf() const override
            {
                auto obj = value();
                if (!obj)
                {
                    return "NULL";
                }
                if (obj->varname().empty())
                {
                    std::stringstream sstream;
                    sstream << static_cast<const void*>(obj.get()) << "# " << obj->netid() << ": " << obj->config().name();
                    return sstream.str();
                }
                else
                {
                    return std::string(obj->varname());
                }
            }
            std::string to_string() const override { return to_string_sqf(); }

            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return 0; }

            bool is_null() const { return m_value.expired(); }

            std::shared_ptr<::sqf::types::object> value() const
            {
                if (m_value.expired())
                {
                    return {};
                }
                return m_value.lock();
            }
            void value(std::shared_ptr<::sqf::types::object> val) { m_value = val; }

            operator std::shared_ptr<::sqf::types::object>() { return value(); }
        };
        template<>
        inline std::shared_ptr<sqf::runtime::data> to_data<std::shared_ptr<sqf::types::object>>(std::shared_ptr<sqf::types::object> value)
        {
            return std::make_shared<d_object>(value);
        }

        inline bool operator==(const std::shared_ptr<sqf::types::d_object> left, const std::shared_ptr<sqf::types::object> right)
        {
            if (left->is_null()) { return false; }
            return left->value() == right;
        }
    }
}