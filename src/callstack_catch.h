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
	class callstack_catch : public callstack
	{
	private:
		std::shared_ptr<codedata> m_codedata_catch;
		std::shared_ptr<sqf::value> m_message;
		bool m_do_catch;
		bool m_done_catch;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_catch(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<codedata> catchblock)
			: callstack(ns),
			m_codedata_catch(catchblock),
			m_do_catch(false),
			m_done_catch(false)
		{
		}

		void except(std::shared_ptr<sqf::value> message) { m_message = message; m_do_catch = true; }
		bool can_recover() override { return true; }

		std::string get_name() override { return name(); }
		static std::string name() { return "catch"; }
	};
}
