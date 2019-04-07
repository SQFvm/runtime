#pragma once
#include <memory>
#include <queue>
#include <string>
#include "callstack.h"

namespace sqf
{
	class instruction;
	class virtualmachine;
	class sqfnamespace;
	class codedata;
	class arraydata;
	class callstack_sqftry : public callstack
	{
	private:
		std::shared_ptr<codedata> mcatchblock;
		std::string mmessage;
		std::shared_ptr<sqf::arraydata> mstackdump;
		bool mdoExcept;
	public:
		callstack_sqftry(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<codedata> catchblock)
			: callstack(ns), mcatchblock(catchblock), mdoExcept(false) {}

		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;
		void except(std::string message, std::shared_ptr<sqf::arraydata> stackdump) { mmessage = message; mstackdump = stackdump; mdoExcept = true; }
		bool recover() override { return true; }

		std::string get_name() override { return "callstack_sqftry"; }
	};
}
