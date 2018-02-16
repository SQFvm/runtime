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
			std::wstring mvarname;
		public:
			assigntolocal(std::wstring varname) { mvarname = varname; }
			virtual void execute(virtualmachine*) const;
			virtual insttype thistype(void) const { return insttype::assigntolocal; }
		};
	}
}