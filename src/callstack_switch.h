#pragma once
#include <memory>
#include <utility>
#include "callstack.h"

namespace sqf
{
	class switchdata;
	class callstack_switch : public callstack
	{
	private:
		std::shared_ptr<switchdata> mswtch;
	public:
		callstack_switch(std::shared_ptr<sqf::sqfnamespace> ns, std::shared_ptr<switchdata> swtch) : callstack(ns), mswtch(
			                                                                                             std::move(swtch)) {}
		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;
	};
}