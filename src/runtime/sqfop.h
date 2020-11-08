#pragma once
#include "type.h"
#include "value.h"

#include <string>
#include <string_view>
#include <functional>

namespace sqf::runtime
{
    class runtime;
    class sqfop_binary
    {
    public:
        using cwref = std::reference_wrapper<const sqfop_binary>;
        using cref = const sqfop_binary&;
        typedef value(*callback)(sqf::runtime::runtime&, sqf::runtime::value::cref, value::cref);
        struct key
        {
            std::string name;
            sqf::runtime::type left_type;
            sqf::runtime::type right_type;

            bool operator==(const key& other) const
            {
                return name == other.name && left_type == other.left_type && right_type == other.right_type;
            }
        };
    private:
        key m_key;
        std::string m_description;
        callback m_callback;
        short m_precedence;
    public:
        sqfop_binary() = default;
        sqfop_binary(short precedence, key key, std::string description, callback callback) : m_key(key), m_description(description), m_callback(callback), m_precedence(precedence) {}
        std::string_view name() const { return m_key.name; }
        std::string_view description() const { return m_description; }
        short precedence() const { return m_precedence; }
        sqf::runtime::type left_type() const { return m_key.left_type; }
        sqf::runtime::type right_type() const { return m_key.right_type; }
        value execute(sqf::runtime::runtime& vm, sqf::runtime::value::cref left, value::cref right) const { return m_callback(vm, left, right); }
        key get_key() const { return m_key; }
    };
    class sqfop_unary
    {
    public:
        using cwref = std::reference_wrapper<const sqfop_unary>;
        using cref = const sqfop_unary&;
        typedef value(*callback)(sqf::runtime::runtime&, sqf::runtime::value::cref);
        struct key
        {
            std::string name;
            sqf::runtime::type right_type;

            bool operator==(const key& other) const
            {
                return name == other.name && right_type == other.right_type;
            }
        };
    private:
        key m_key;
        std::string m_description;

        callback m_callback;
    public:
        sqfop_unary() = default;
        sqfop_unary(key key, std::string description, callback callback) : m_key(key), m_description(description), m_callback(callback) {}
        std::string_view name() const { return m_key.name; }
        std::string_view description() const { return m_description; }
        sqf::runtime::type right_type() const { return m_key.right_type; }
        value execute(sqf::runtime::runtime& vm, value::cref right) const { return m_callback(vm, right); }
        key get_key() const { return m_key; }
    };
    class sqfop_nular
    {
    public:
        using cwref = std::reference_wrapper<const sqfop_nular>;
        using cref = const sqfop_nular&;
        typedef value(*callback)(sqf::runtime::runtime&);
        struct key
        {
            std::string name;

            bool operator==(const key& other) const
            {
                return name == other.name;
            }
        };
    private:
        key m_key;
        std::string m_description;

        callback m_callback;
    public:
        sqfop_nular() = default;
        sqfop_nular(key key, std::string description, callback callback) : m_key(key), m_description(description), m_callback(callback) {}
        std::string_view name() const { return m_key.name; }
        std::string_view description() const { return m_description; }
        value execute(sqf::runtime::runtime& vm) const { return m_callback(vm); }
        key get_key() const { return m_key; }
    };
    namespace sqfop
    {
        /// <summary>
        /// Utility method to create a nular operator.
        /// </summary>
        /// <param name="name">The name of the operator to create.</param>
        /// <param name="description">The description of the operator.</param>
        /// <param name="fnc">The method to execute when the operator gets invoked.</param>
        /// <returns>A valid sqfop_nular.</returns>
        static inline sqfop_nular nular(std::string name, std::string description, sqfop_nular::callback fnc)
        {
            std::transform(name.begin(), name.end(), name.begin(), [](char& c) { return (char)std::tolower((int)c); });
            return { { name }, description, fnc };
        }

        /// <summary>
        /// Utility method to create a unary operator.
        /// </summary>
        /// <param name="name">The name of the operator to create.</param>
        /// <param name="rtype">The accepted type on the right side of the operator.</param>
        /// <param name="description">The description of the operator.</param>
        /// <param name="fnc">The method to execute when the operator gets invoked.</param>
        /// <returns>A valid sqfop_unary.</returns>
        static inline sqfop_unary unary(std::string name, type rtype, std::string description, sqfop_unary::callback fnc)
        {
            std::transform(name.begin(), name.end(), name.begin(), [](char& c) { return (char)std::tolower((int)c); });
            return { { name, rtype }, description, fnc }; 
        }

        /// <summary>
        /// Utility method to create a unary operator.
        /// </summary>
        /// <param name="name">The name of the operator to create.</param>
        /// <param name="rtype">The accepted type on the right side of the operator.</param>
        /// <param name="description">The description of the operator.</param>
        /// <param name="fnc">The method to execute when the operator gets invoked.</param>
        /// <returns>A valid sqfop_unary.</returns>
        static inline sqfop_binary binary(short precedence, std::string name, type ltype, type rtype, std::string description, sqfop_binary::callback fnc)
        {
            std::transform(name.begin(), name.end(), name.begin(), [](char& c) { return (char)std::tolower((int)c); });
            return { precedence, { name, ltype, rtype }, description, fnc };
        }
    }
}


template<> struct std::hash<sqf::runtime::sqfop_binary::key> {
    std::size_t operator()(sqf::runtime::sqfop_binary::key const& s) const noexcept {
        std::size_t h1 = std::hash<std::string>{}(s.name);
        std::size_t h2 = std::hash<short>{}(s.left_type);
        std::size_t h3 = std::hash<short>{}(s.right_type);

        h1 ^= (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        h1 ^= (h3 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        return h1;
    }
};
template<> struct std::hash<sqf::runtime::sqfop_unary::key> {
    std::size_t operator()(sqf::runtime::sqfop_unary::key const& s) const noexcept {
        std::size_t h1 = std::hash<std::string>{}(s.name);
        std::size_t h2 = std::hash<short>{}(s.right_type);
        h1 ^= (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        return h1;
    }
};
template<> struct std::hash<sqf::runtime::sqfop_nular::key> {
    std::size_t operator()(sqf::runtime::sqfop_nular::key const& s) const noexcept {
        std::size_t h1 = std::hash<std::string>{}(s.name);
        return h1;
    }
};