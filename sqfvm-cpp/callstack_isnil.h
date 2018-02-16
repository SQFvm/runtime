#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_isnil : public callstack
	{
	public:
		callstack_isnil(sqf::virtualmachine * vm, std::shared_ptr<codedata> exec);
		virtual std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm);
	};
}
