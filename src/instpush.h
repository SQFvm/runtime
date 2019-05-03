#pragma once
#include <memory>
#include "instruction.h"
#include "value.h"

namespace sqf
{
	class value;
	namespace inst
	{
		class push : public instruction
		{
		private:
			value mvalue;
		public:
			push(value val) { mvalue = val; }
			void execute(virtualmachine* vm) const override;
			insttype thistype() const override { return insttype::push; }
			std::string to_string() const override;
			value get_value() { return mvalue; }
			const value get_value() const { return mvalue; }
		};
	}
}