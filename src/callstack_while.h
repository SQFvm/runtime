#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_while : public callstack
	{
	private:
		std::shared_ptr<codedata> mwhilecond;
		std::shared_ptr<codedata> mexec;
		bool mdoexec;
	public:
		callstack_while(std::shared_ptr<sqf::sqfnamespace> ns, std::shared_ptr<codedata> whilecond, std::shared_ptr<codedata> exec) : callstack(ns), mwhilecond(whilecond), mexec(exec), mdoexec(false) {}
		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;
	};
}