#pragma once
#include <memory>
#include "callstack.h"
#include "arraydata.h"

namespace sqf
{
	class fordata;
	class codedata;
	class callstack_findif : public callstack
	{
	private:
		std::shared_ptr<arraydata> marr;
		std::shared_ptr<codedata> mexec;
		size_t mCurIndex;
	public:
		callstack_findif(std::shared_ptr<sqf::sqfnamespace> ns, std::shared_ptr<codedata> exec, std::shared_ptr<arraydata> arr) : callstack(ns), mexec(exec), marr(arr), mCurIndex(0) { }
		virtual std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm);

		std::string get_name() override { return "callstack_findif"; }
	};
}