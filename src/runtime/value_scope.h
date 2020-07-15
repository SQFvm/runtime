#pragma once
#include <unordered_map>
#include <string>
#include <string_view>
#include <optional>

#include "value.h"

namespace sqf::runtime
{
	class value_scope
	{
	private:
		std::unordered_map<std::string, sqf::runtime::value> m_map;
	public:
		value_scope() = default;

		sqf::runtime::value::cref operator[](const std::string& index) const { return get(index); }
		sqf::runtime::value& operator[](const std::string& index) { return get(index); }

		bool contains(const std::string& view) const { return m_map.find(view) != m_map.end(); }
		sqf::runtime::value get(const std::string& view) const
		{
			auto res = m_map.find(view);
			return res == m_map.end() ? value() : res->second;
		}
		sqf::runtime::value& get(const std::string& view) { return m_map.at(view); }
		
		void clear() { m_map.clear(); }
	};
}