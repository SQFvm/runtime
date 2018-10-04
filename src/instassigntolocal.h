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
			virtual void execute(virtualmachine*) const;
			virtual insttype thistype() const { return insttype::assigntolocal; }
		};
	}
}