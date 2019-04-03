#pragma once
#include <string>
#include "instruction.h"

namespace sqf
{
	namespace inst
	{
		class assignto : public instruction
		{
		private:
			std::string mvarname;
		public:
			assignto(std::string varname) { mvarname = varname; }
			void execute(virtualmachine*) const override;
			insttype thistype() const override { return insttype::assignto; }
			std::string to_string() const override { return "ASSIGNTO " + mvarname; }
		};
	}
}