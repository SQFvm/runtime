#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_while : public callstack
	{
	private:
		std::shared_ptr<codedata> m_codedata_condition;
		std::shared_ptr<codedata> m_codedata_body;
		bool m_was_condition;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_while(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<codedata> whilecond,
			std::shared_ptr<codedata> exec
		) : callstack(ns),
			m_codedata_condition(std::move(whilecond)),
			m_codedata_body(std::move(exec)),
			m_was_condition(false)
		{
		}

		std::string get_name() override { return "while"; }
	};
}