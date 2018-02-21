#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class switchdata;
	class callstack_switch : public callstack
	{
	private:
		std::shared_ptr<switchdata> mswtch;
	public:
		callstack_switch(std::shared_ptr<switchdata> swtch) : mswtch(swtch) {}
		virtual std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm);
	};
}