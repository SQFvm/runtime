#pragma once
#include <map>
#include <string>
#include <memory>
#include <algorithm>

namespace sqf
{
	class value;
	class varscope
	{
	private:
		std::map<std::string, std::shared_ptr<value>> m_variable_map;
		std::string m_scopename;
		std::string to_lower_string(std::string s)
		{
			std::transform(s.begin(), s.end(), s.begin(), ::tolower);
			return s;
		}
	public:
		varscope() { m_scopename = ""; }
		varscope(std::string name) { m_scopename = name; }
		void set_variable(std::string key, std::shared_ptr<value> value) { m_variable_map[to_lower_string(key)] = value; }
		std::shared_ptr<value> get_variable_empty(std::string key)
		{
			auto it = m_variable_map.find(to_lower_string(key));
			return it == m_variable_map.end() ? std::shared_ptr<value>() : it->second;
		}
		std::shared_ptr<value> get_variable(std::string key);

		// Alias for has_variable(std::string key)
		bool contains_variable(std::string key) { return has_variable(key); }

		bool has_variable(std::string key) { auto it = m_variable_map.find(to_lower_string(key)); return it != m_variable_map.end(); }

		void set_scopename(std::string newname) { m_scopename = newname; }

		std::string get_scopename() { return m_scopename; }

		const std::map<std::string, std::shared_ptr<value>>& get_variable_map() const { return m_variable_map; }

		void drop_variables() { m_variable_map.clear(); }
	};
}