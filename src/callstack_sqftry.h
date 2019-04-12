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
		std::shared_ptr<codedata> m_codedata_catch;
		std::string m_message;
		std::shared_ptr<sqf::arraydata> m_stackdump;
		bool m_do_catch;
		bool m_done_catch;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_sqftry(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<codedata> catchblock)
			: callstack(ns),
			m_codedata_catch(catchblock),
			m_do_catch(false),
			m_done_catch(false)
		{
		}

		void except(std::string message, std::shared_ptr<sqf::arraydata> stackdump) { m_message = message; m_stackdump = stackdump; m_do_catch = true; }
		bool can_recover() override { return true; }

		std::string get_name() override { return "except__"; }
	};
}
