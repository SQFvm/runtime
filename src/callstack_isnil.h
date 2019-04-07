#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_isnil : public callstack
	{
	public:
		callstack_isnil(std::shared_ptr<sqf::sqfnamespace> ns, sqf::virtualmachine * vm, std::shared_ptr<codedata> exec);
		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;

		std::string get_name() override { return "callstack_isnil"; }
	};
}
