#pragma once
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <utility>
#include "utility.h"

namespace sqf
{
	class value;
	class varscope
	{
	private:
		std::map<std::string, std::shared_ptr<value>> m_variable_map;
		std::string m_scopename;
	public:
		varscope() { m_scopename = ""; }
		varscope(std::string name) { m_scopename = std::move(name); }
		void set_variable(std::string_view key, std::shared_ptr<value> value) { m_variable_map[string_tolower(key)] = std::move(value); }
		std::shared_ptr<value> get_variable_empty(std::string_view key)
		{
			auto it = m_variable_map.find(string_tolower(key));
			return it == m_variable_map.end() ? std::shared_ptr<value>() : it->second;
		}
		std::shared_ptr<value> get_variable(std::string_view key);

		// Alias for has_variable(std::string key)
		bool contains_variable(std::string_view key) { return has_variable(key); }

		bool has_variable(std::string_view key) { auto it = m_variable_map.find(string_tolower(key)); return it != m_variable_map.end(); }

		void set_scopename(std::string newname) { m_scopename = std::move(newname); }

		std::string get_scopename() const { return m_scopename; }

		const std::map<std::string, std::shared_ptr<value>>& get_variable_map() const { return m_variable_map; }

		void drop_variables() { m_variable_map.clear(); }
	};
}
