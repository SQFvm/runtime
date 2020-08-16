#pragma once
#include <vector>
#include <initializer_list>

#include ".h"

namespace sqf::runtime
{
	class context
	{
	private:
		sqf::runtime::instruction_set m_instruction_set;
		sqf::runtime::instruction_set::iterator m_iterator;
	public:
		frame(sqf::runtime::instruction_set instruction_set) : m_instruction_set(instruction_set), m_iterator(instruction_set.begin()) {}
	};
}