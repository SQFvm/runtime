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
	class callstack_sqftry : public callstack
	{
	private:
		std::shared_ptr<codedata> mcatchblock;
		std::string mmessage;
		bool mdoExcept;
	public:
		callstack_sqftry(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<codedata> catchblock)
			: callstack(ns), mcatchblock(catchblock), mdoExcept(false) {}

		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;
		void except(std::string message) { mmessage = message; mdoExcept = true; }
		bool recover() override { return true; }
	};
}
