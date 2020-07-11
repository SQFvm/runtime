#pragma once
#include "../runtime/instruction.h"

namespace sqf::opcodes
{
	class assign_to : public sqf::runtime::instruction
	{
	private:
		std::string mvarname;
	public:
		assign_to(std::string varname) { mvarname = varname; }
		void execute(virtualmachine*) const override;
		insttype thistype() const override { return insttype::assignto; }
		std::string to_string() const override { return "ASSIGNTO " + mvarname; }
		std::string variable_name() const { return mvarname; }
	};
}