#pragma once
#include "instruction.h"

namespace sqf
{
	namespace inst
	{
		class makearray : public instruction
		{
		private:
			size_t msize;
		public:
			makearray(size_t size) { msize = size; }
			virtual void execute(virtualmachine* vm) const;
			virtual insttype thistype() const { return insttype::makearray; }
			inline size_t size() { return msize; }
		};
	}
}
