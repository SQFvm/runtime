#pragma once
#include "../runtime/instruction.h"

namespace sqf::opcodes
{
	class call_unary : public sqf::runtime::instruction
	{
	private:
		std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> mcmds;
	public:
		call_unary(std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> cmds) { mcmds = cmds; }
		void execute(virtualmachine*) const override;
		insttype thistype() const override { return insttype::callunary; }
		std::string to_string() const override;
		std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> commands() { return mcmds; }
	};
}
