#pragma once
#include "value.h"

#include <unordered_map>
#include <string>
#include <string_view>
#include <optional>
#include <cctype>
#include <algorithm>

namespace sqf::runtime
{
	class value_scope
	{
	private:
		std::string m_scope_name;
		std::unordered_map<std::string, sqf::runtime::value> m_map;
	public:
		value_scope() = default;

		sqf::runtime::value operator[](const std::string& index) const { return at(index); }
		sqf::runtime::value& operator[](const std::string& index) { return at(index); }

		bool contains(std::string variable_name) const
		{
			std::transform(variable_name.begin(), variable_name.end(), variable_name.begin(), [](char& c) { return std::tolower(c); });
			return m_map.find(variable_name) != m_map.end();
		}
		sqf::runtime::value at(std::string variable_name) const
		{
			std::transform(variable_name.begin(), variable_name.end(), variable_name.begin(), [](char& c) { return std::tolower(c); });
			auto res = m_map.find(variable_name);
			return res == m_map.end() ? value() : res->second;
		}
		sqf::runtime::value& at(std::string variable_name)
		{
			std::transform(variable_name.begin(), variable_name.end(), variable_name.begin(), [](char& c) { return std::tolower(c); });
			return m_map[variable_name];
		}
		std::string_view scope_name() const { return m_scope_name; }
		void scope_name(std::string value) { m_scope_name = value; }
		void clear() { m_map.clear(); }

		std::unordered_map<std::string, sqf::runtime::value>::iterator begin() { return m_map.begin(); }
		std::unordered_map<std::string, sqf::runtime::value>::iterator end() { return m_map.end(); }
	};
}