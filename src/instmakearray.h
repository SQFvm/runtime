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
			void execute(virtualmachine* vm) const override;
			insttype thistype() const override { return insttype::makearray; }
			size_t size() const { return msize; }
			std::string to_string() const override { return std::string("MAKEARRAY ") + std::to_string(msize); }
		};
	}
}
