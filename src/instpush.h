#pragma once
#include <memory>
#include "instruction.h"

namespace sqf
{
	class value;
	namespace inst
	{
		class push : public instruction
		{
		private:
			std::shared_ptr<value> mvalue;
		public:
			push(std::shared_ptr<value> val) { mvalue = val; }
			virtual void execute(virtualmachine* vm) const;
			virtual insttype thistype() const { return insttype::push; }
		};
	}
}