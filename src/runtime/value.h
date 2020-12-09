#pragma once
#include "data.h"

#include <string>
#include <optional>
#include <memory>
#include <functional>
#include <type_traits>



namespace sqf
{
    namespace types
    {
        template<typename T>
        inline std::shared_ptr<sqf::runtime::data> to_data(T t);
    }
    namespace runtime
    {
        class value
        {
        public:
            using cwref = std::reference_wrapper<const value>;
            using cref = const value&;
        private:
            std::shared_ptr<sqf::runtime::data> m_data;
        public:
            constexpr value() noexcept {}

            template<typename T>
            value(T t) : m_data(::sqf::types::to_data<T>(t)) { }
            template<typename T>
            value(std::shared_ptr<T> d) : m_data(std::move(d)) { }

            template<class T>
            bool is() const
            {
                static_assert(std::is_base_of<sqf::runtime::type, T>::value, "value::is<T>() can only be used with sqf::runtime::type types.");
                return type() == T();
            }
            bool is(sqf::runtime::type t) const
            {
                return this->operator sqf::runtime::type() == t;
            }
            bool empty() const { return !m_data.get(); }

            bool operator==(cref other) const { return (((bool)m_data.get()) && ((bool)other.m_data.get()) && m_data->equals(other.m_data)) || (!((bool)m_data.get()) && !((bool)other.m_data.get())); }
            bool operator!=(cref other) const { return !(*this == other); }


            /// <summary>
            /// Returns the m_data member.
            /// </summary>
            /// <returns></returns>
            std::shared_ptr<sqf::runtime::data> data() const { return m_data; }

            std::string to_string_sqf() const { return m_data ? m_data->to_string_sqf() : std::string("nil"); }
            std::string to_string() const { return m_data ? m_data->to_string() : std::string(); }

            explicit operator sqf::runtime::type() const { return m_data ? m_data->type() : sqf::types::t_nothing(); };
            sqf::runtime::type type() const { return m_data ? m_data->type() : sqf::types::t_nothing(); }

            /// <summary>
            /// Attempts to convert the data-member to the provided data type.
            /// Will use std::dynamic_pointer_cast.
            /// </summary>
            /// <remarks>
            /// If it can be ensured that this will be the correct type, use sqf::runtime::value::data&lt;T&gt;.
            /// </remarks>
            /// <typeparam name="T"></typeparam>
            /// <returns></returns>
            template <class T>
            std::shared_ptr<T> data_try() const {
                static_assert(std::is_base_of<sqf::runtime::data, T>::value, "value::data_try_as<T>() can only convert to sqf::runtime::data types");
                return std::dynamic_pointer_cast<T>(m_data);
            }
            template <class T, typename TValue>
            std::optional<TValue> data_try() const {
                auto conv = data_try<T>();
                if (conv)
                {
                    return (TValue)*conv;
                }
                else
                {
                    return {};
                }
            }
            template <class T, typename TValue>
            TValue data_try(TValue def) const {
                auto conv = data_try<T>();
                if (conv)
                {
                    return (TValue)*conv;
                }
                else
                {
                    return def;
                }
            }
            /// <summary>
            /// Converts the data-member to the provided data type.
            /// Will use std::static_pointer_cast.
            /// </summary>
            /// <remark>
            /// If in doubt of the correct type, use sqf::runtime::value::data_try<T>.
            /// </remarks>
            /// <typeparam name="T"></typeparam>
            /// <returns></returns>
            template<class T>
            std::shared_ptr<T> data() const {
                static_assert(std::is_base_of<sqf::runtime::data, T>::value, "value::data<T>() can only convert to sqf::runtime::data types");
                return std::static_pointer_cast<T>(m_data);
            }

            template<class TData, typename TValue>
            TValue data() const { return (TValue)(*data<TData>()); }

            std::size_t hash() const
            {
                return empty() ? 0 : m_data->hash();
            }
        };
    }
}

namespace std
{
    template<> struct hash<sqf::runtime::value>
    {
        std::size_t operator()(sqf::runtime::value const& val) const noexcept
        {
            return val.hash();
        }
    };
}
