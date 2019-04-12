#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_isnil : public callstack
	{
	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_isnil(std::shared_ptr<sqf::sqfnamespace> ns, sqf::virtualmachine * vm, std::shared_ptr<codedata> exec);

		std::string get_name() override { return "isNil"; }
	};
}
