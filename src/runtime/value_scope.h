#pragma once
#include "value.h"

#include <unordered_map>
#include <string>
#include <string_view>
#include <optional>

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

		bool contains(const std::string& view) const { return m_map.find(view) != m_map.end(); }
		sqf::runtime::value at(const std::string& view) const
		{
			auto res = m_map.find(view);
			return res == m_map.end() ? value() : res->second;
		}
		sqf::runtime::value& at(const std::string& view) { return m_map.at(view); }
		std::string_view scope_name() const { return m_scope_name; }
		void scope_name(std::string value) { m_scope_name = value; }
		void clear() { m_map.clear(); }

		std::unordered_map<std::string, sqf::runtime::value>::iterator begin() { return m_map.begin(); }
		std::unordered_map<std::string, sqf::runtime::value>::iterator end() { return m_map.end(); }
	};
}