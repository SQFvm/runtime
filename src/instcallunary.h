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
			void execute(virtualmachine*) const override;
			insttype thistype() const override { return insttype::callunary; }
			std::string to_string() const override;
		};
	}
}