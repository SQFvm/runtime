#pragma once
#include "../runtime/instruction.h"
#include "../runtime/runtime.h"

namespace sqf::opcodes
{
	class end_statement : public sqf::runtime::instruction
	{
	public:
		virtual void execute(sqf::runtime::runtime& vm) const override { vm.active_context()->current_frame().clear(); }
		virtual std::string to_string() const override { return "ENDSTATEMENT"; }
	};
}
