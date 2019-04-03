#pragma once
#include <string>
#include "instruction.h"

namespace sqf
{
	namespace inst
	{
		class getvariable : public instruction
		{
		private:
			std::string mvarname;
		public:
			getvariable(std::string varname) { mvarname = varname; }
			void execute(virtualmachine*) const override;
			insttype thistype() const override { return insttype::getvariable; }
			std::string to_string() const override { return "GETVARIABLE " + mvarname; }
		};
	}
}