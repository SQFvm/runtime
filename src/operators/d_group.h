#pragma once
#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"
#include "group.h"

#include <string>
#include <sstream>
#include <memory>


namespace sqf
{
    namespace runtime
    {
        struct t_group : public type::extend<t_group> { t_group() : extend() {} static const std::string name() { return "GROUP"; } };
    }
    namespace types
    {
        class d_group : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_group;
        private:
            std::weak_ptr<group> m_value;
        protected:
            bool do_equals(std::shared_ptr<sqf::runtime::data> other, bool invariant) const override
            {
                return value().get() == std::static_pointer_cast<d_group>(other)->value().get();
            }
        public:
            d_group() = default;
            d_group(std::weak_ptr<group> value) : m_value(value) {}
            d_group(std::shared_ptr<group> value) : m_value(value) {}


            std::string to_string_sqf() const override
            {
                auto obj = value();
                if (!obj)
                {
                    return "NULL";
                }
                if (obj->group_id().empty())
                {
                    std::stringstream sstream;
                    sstream << static_cast<const void*>(obj.get()) << "# " << obj->netid() << ": " << d_side::to_string(obj->side());
                    return sstream.str();
                }
                else
                {
                    return std::string(obj->group_id());
                }
            }
            std::string to_string() const override { return to_string_sqf(); }

            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return 0; }

            bool is_null() const { return m_value.expired(); }

            std::shared_ptr<::sqf::types::group> value() const
            {
                if (m_value.expired())
                {
                    return {};
                }
                return m_value.lock();
            }
            void value(std::shared_ptr<::sqf::types::group> val) { m_value = val; }

            operator std::shared_ptr<::sqf::types::group>() { return value(); }
        };

        inline bool operator==(const std::shared_ptr<d_group> left, const std::shared_ptr<group> right)
        {
            if (left->is_null()) { return false; }
            return left->value() == right;
        }
    }
}