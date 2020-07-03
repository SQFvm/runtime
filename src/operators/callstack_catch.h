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
		sqf::value m_message;
		bool m_do_catch;
		bool m_done_catch;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_catch(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<codedata> catchblock)
			: callstack(std::move(ns)),
			m_codedata_catch(std::move(catchblock)),
			m_do_catch(false),
			m_done_catch(false)
		{
		}

		void except(sqf::value message) { m_message = std::move(message); m_do_catch = true; }

		std::string get_name() override { return name(); }
		static std::string name() { return "catch"; }
	};
}
