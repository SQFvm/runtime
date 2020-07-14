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
		using cref = const sqfop_binary&;
		typedef value(*callback)(sqf::runtime::runtime&, sqf::runtime::value::cref, value::cref);
		struct key
		{
			std::string name;
			sqf::runtime::type left_type;
			sqf::runtime::type right_type;
			short precedence;

			bool operator==(const key& other) const
			{
				return name == other.name && left_type == other.left_type && right_type == other.right_type;
			}
		};
	private:
		key m_key;
		std::string m_description;
		callback m_callback;
	public:
		std::string_view name() const { return m_key.name; }
		std::string_view description() const { return m_description; }
		short precedence() const { return m_key.precedence; }
		sqf::runtime::type left_type() const { return m_key.left_type; }
		sqf::runtime::type right_type() const { return m_key.right_type; }
		value execute(sqf::runtime::runtime& vm, sqf::runtime::value::cref left, value::cref right) const { return m_callback(vm, left, right); }
		key get_key() const { return m_key; }
	};
	class sqfop_unary
	{
	public:
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
		std::string_view name() const { return m_key.name; }
		std::string_view description() const { return m_description; }
		sqf::runtime::type right_type() const { return m_key.right_type; }
		value execute(sqf::runtime::runtime& vm, value::cref right) const { return m_callback(vm, right); }
		key get_key() const { return m_key; }
	};
	class sqfop_nular
	{
	public:
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
		std::string_view name() const { return m_key.name; }
		std::string_view description() const { return m_description; }
		value execute(sqf::runtime::runtime& vm) const { return m_callback(vm); }
		key get_key() const { return m_key; }
	};
}


template<> struct std::hash<sqf::runtime::sqfop_binary::key> {
	std::size_t operator()(sqf::runtime::sqfop_binary::key const& s) const noexcept {
		std::size_t h1 = std::hash<std::string>{}(s.name);
		std::size_t h2 = std::hash<short>{}(s.left_type);
		std::size_t h3 = std::hash<short>{}(s.right_type);
		std::size_t h4 = std::hash<short>{}(s.precedence);

		h1 ^= (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
		h1 ^= (h3 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
		h1 ^= (h4 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
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