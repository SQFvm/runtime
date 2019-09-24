#pragma once
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <utility>
#include "utility.h"
#include "value.h"

namespace sqf
{
	class value;
	class varscope
	{
	private:
		std::map<std::string, value> m_variable_map;
		std::string m_scopename;
	public:
		varscope() { m_scopename = ""; }
		varscope(std::string name) { m_scopename = std::move(name); }

		// Assigns the given value to the given variable.
		void set_variable(std::string_view key, value value) { m_variable_map[string_tolower(key)] = std::move(value); }

		// Allows to receive a previously set (using set_variable) variable
		value get_variable_empty(std::string_view key)
		{
			auto it = m_variable_map.find(string_tolower(key));
			return it == m_variable_map.end() ? value() : it->second;
		}
        value get_variable(std::string_view key) { return get_variable_empty(key); };

		// Checks if a given variable was previously set using eg. set_variable.
		bool has_variable(std::string_view key) { auto it = m_variable_map.find(string_tolower(key)); return it != m_variable_map.end(); }

		// Alias for has_variable(std::string key)
		// Checks if a given variable was previously set using eg. set_variable.
		bool contains_variable(std::string_view key) { return has_variable(key); }


		void set_scopename(std::string newname) { m_scopename = std::move(newname); }

		std::string get_scopename() const { return m_scopename; }

		const std::map<std::string, value>& get_variable_map() const { return m_variable_map; }

		void drop_variables() { m_variable_map.clear(); }
	};
}
