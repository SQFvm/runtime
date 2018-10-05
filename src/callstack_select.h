#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_select : public callstack
	{
	private:
		size_t mcurindex;
		bool mend;
		std::vector<std::shared_ptr<value>> marr;
		std::vector<std::shared_ptr<value>> moutarr;
		std::shared_ptr<codedata> mcond;
	public:
		callstack_select(std::shared_ptr<sqf::sqfnamespace> ns, std::vector<std::shared_ptr<value>> arr, std::shared_ptr<codedata> cond)
		: callstack(ns), mcurindex(0), mend(arr.empty()), marr(std::move(arr)), mcond(std::move(cond)) {}
		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;
	};
}