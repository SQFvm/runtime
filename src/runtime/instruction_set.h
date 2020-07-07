#pragma once
#include <vector>
#include <initializer_list>

#include "instruction.h"

namespace sqf::runtime
{
	/// <summary>
	/// A way to represent a "mutable" instruction set.
	/// </summary>
	class instruction_set
	{
	public:
		using iterator = std::vector<sqf::runtime::instruction::sptr>::const_iterator;
		using reverse_iterator = std::vector<sqf::runtime::instruction::sptr>::const_reverse_iterator;
	private:
		std::vector<sqf::runtime::instruction::sptr> m_instructions;
	public:
		instruction_set() {}
		instruction_set(std::initializer_list<sqf::runtime::instruction::sptr> initializer) : m_instructions(initializer.begin(), initializer.end()) {}
		instruction_set(std::vector<sqf::runtime::instruction::sptr> instructions) : m_instructions(std::move(instructions)) {}

		iterator begin() const { return m_instructions.begin(); }
		iterator end() const { return m_instructions.end(); }
		reverse_iterator rbegin() const { return m_instructions.rbegin(); }
		reverse_iterator rend() const { return m_instructions.rend(); }
	};
}