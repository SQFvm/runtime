#pragma once
#include <unordered_map>
#include <string>
#include <string_view>

#include "value.h"

namespace sqf::runtime
{
	class value_scope
	{
	private:
		std::unordered_map<std::string, sqf::runtime::value::cref> m_map;
	public:
		value_scope() = default;

		sqf::runtime::value::cref operator[](const std::string index) const
		{
			auto res = m_map.find(index);
			return res == m_map.end() ? value() : res->second;
		}
		sqf::runtime::value::cref& operator[](const std::string index)
		{
			return m_map.at(index);
		}

		void clear() { m_map.clear(); }
	};
}