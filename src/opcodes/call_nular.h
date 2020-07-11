#pragma once
#include "../runtime/instruction.h"

namespace sqf::opcodes
{
	class call_nular : public sqf::runtime::instruction
	{
	private:
		std::shared_ptr<sqf::nularcmd> mcmd;
	public:
		call_nular(std::shared_ptr<sqf::nularcmd> cmd) { mcmd = cmd; }
		void execute(virtualmachine*) const override;
		insttype thistype() const override { return insttype::callnular; }
		std::string to_string() const override;
		std::shared_ptr<sqf::nularcmd> command() { return mcmd; }
	};
}
