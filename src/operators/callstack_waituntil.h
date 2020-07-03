#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_waituntil : public callstack
	{
	private:
		std::shared_ptr<codedata> m_codedata_condition;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_waituntil(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<codedata> condition
		) : callstack(ns),
			m_codedata_condition(std::move(condition))
		{
		}

		std::string get_name() override { return "waituntil"; }
	};
}