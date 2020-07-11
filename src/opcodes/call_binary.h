#pragma once
#include "../runtime/instruction.h"

namespace sqf::opcodes
{
	class call_binary : public sqf::runtime::instruction
	{
	private:
		std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> mcmds;
	public:
		call_binary(std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> cmds) { mcmds = cmds; }
		void execute(virtualmachine*) const override;
		insttype thistype() const override { return insttype::callbinary; }
		std::string to_string() const override;
		std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> commands() { return mcmds; }
	};
}
