#pragma once
#include <memory>
#include "callstack.h"
#include "arraydata.h"

namespace sqf
{
	class fordata;
	class codedata;
	class callstack_count : public callstack
	{
	private:
		std::shared_ptr<arraydata> marr;
		std::shared_ptr<codedata> mexec;
		size_t mCurIndex;
		int mCount;
	public:
		callstack_count(std::shared_ptr<sqf::sqfnamespace> ns, std::shared_ptr<codedata> exec, std::shared_ptr<arraydata> arr) : callstack(ns), marr(std::move(arr)), mexec(std::move(exec)), mCurIndex(0), mCount(0) { }
		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;

		std::string get_name() override { return "callstack_count"; }
	};
}