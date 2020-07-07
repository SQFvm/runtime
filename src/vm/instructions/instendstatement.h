#pragma once
#include "instruction.h"

namespace sqf
{
	namespace inst
	{
		class endstatement : public instruction
		{
		public:
			void execute(virtualmachine* vm) const override;
			insttype thistype() const override { return insttype::endstatement; }
			std::string to_string() const override { return "ENDSTATEMENT"; }
		};
	}
}