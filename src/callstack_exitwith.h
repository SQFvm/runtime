#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class callstack_exitwith : public callstack
	{
	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_exitwith(std::shared_ptr<sqf::sqfnamespace> ns) : callstack(std::move(ns)) {}

		std::string get_name() override { return "exitWith"; }
	};
}