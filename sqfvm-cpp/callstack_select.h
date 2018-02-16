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
		callstack_select(std::vector<std::shared_ptr<value>> arr, std::shared_ptr<codedata> cond) : marr(arr), mcond(cond), mcurindex(0), mend(arr.size() == 0) {}
		virtual std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm);
	};
}