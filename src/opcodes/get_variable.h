#pragma once
#include "../runtime/instruction.h"

#include <string>

namespace sqf::runtime
{
	class runtime;
}
namespace sqf::opcodes
{
	class get_variable : public sqf::runtime::instruction
	{
	private:
		std::string m_variable_name;
	public:
		get_variable(std::string variable_name) : m_variable_name(variable_name) {}
		virtual void execute(sqf::runtime::runtime& vm) const override;
		std::string to_string() const override { return "GETVARIABLE " + m_variable_name; }
		std::string variable_name() const { return m_variable_name; }
	};
}
