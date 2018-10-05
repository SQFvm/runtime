#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class callstack_exitwith : public callstack
	{
	public:
		callstack_exitwith(std::shared_ptr<sqf::sqfnamespace> ns) : callstack(ns) {}
		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;
	};
}