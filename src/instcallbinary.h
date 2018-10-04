#pragma once
#include <memory>
#include <vector>
#include "instruction.h"

namespace sqf
{
	class binarycmd;
	namespace inst
	{
		class callbinary : public instruction
		{
		private:
			std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> mcmds;
		public:
			callbinary(std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> cmds) { mcmds = cmds; }
			virtual void execute(virtualmachine*) const;
			virtual insttype thistype() const { return insttype::callbinary; }
		};
	}
}