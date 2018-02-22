#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_apply : public callstack
	{
	private:
		size_t mcurindex;
		bool mend;
		std::vector<std::shared_ptr<value>> marr;
		std::vector<std::shared_ptr<value>> moutarr;
		std::shared_ptr<codedata> mapply;
	public:
		callstack_apply(std::vector<std::shared_ptr<value>> arr, std::shared_ptr<codedata> apply) : marr(arr), mapply(apply), mcurindex(0), mend(arr.size() == 0), moutarr(arr.size()) {}
		virtual std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm);
	};
}