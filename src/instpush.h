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
			void execute(virtualmachine* vm) const override;
			insttype thistype() const override { return insttype::push; }
			std::string to_string() const override;
		};
	}
}