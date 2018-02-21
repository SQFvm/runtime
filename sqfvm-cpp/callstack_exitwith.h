#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class callstack_exitwith : public callstack
	{
	public:
		callstack_exitwith() {}
		virtual std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm);
	};
}