#pragma once
#include <string>
#include "instruction.h"

namespace sqf
{
	namespace inst
	{
		class assigntolocal : public instruction
		{
		private:
			std::string mvarname;
		public:
			assigntolocal(std::string varname) { mvarname = varname; }
			void execute(virtualmachine*) const override;
			insttype thistype() const override { return insttype::assigntolocal; }
			std::string to_string() const override { return "ASSIGNTOLOCAL " + mvarname; }
		};
	}
}