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
			std::wstring mvarname;
		public:
			getvariable(std::wstring varname) { mvarname = varname; }
			virtual void execute(virtualmachine*) const;
			virtual insttype thistype(void) const { return insttype::getvariable; }
		};
	}
}