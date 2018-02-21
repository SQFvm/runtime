#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_exitwith : public callstack
	{
	private:
		std::shared_ptr<codedata> mexec;
	public:
		callstack_exitwith(std::shared_ptr<codedata> exec) : mexec(exec) {}
		virtual std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm);
	};
}