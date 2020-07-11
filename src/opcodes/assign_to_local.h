#pragma once
#include "../runtime/instruction.h"

namespace sqf::opcodes
{
	class assign_to_local : public sqf::runtime::instruction
	{
	private:
		std::string mvarname;
	public:
		assign_to_local(std::string varname) { mvarname = varname; }
		void execute(virtualmachine*) const override;
		insttype thistype() const override { return insttype::assigntolocal; }
		std::string to_string() const override { return "ASSIGNTOLOCAL " + mvarname; }
		std::string variable_name() const { return mvarname; }
	};
}
