#pragma once
#include <memory>
#include <vector>
#include "instruction.h"

namespace sqf
{
	class unarycmd;
	namespace inst
	{
		class callunary : public instruction
		{
		private:
			std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> mcmds;
		public:
			callunary(std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> cmds) { mcmds = cmds; }
			virtual void execute(virtualmachine*) const;
			virtual insttype thistype() const { return insttype::callunary; }
		};
	}
}