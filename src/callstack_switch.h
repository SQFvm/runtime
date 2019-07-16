#pragma once
#include <memory>
#include <utility>
#include "callstack.h"

namespace sqf
{
	class switchdata;
	class callstack_switch : public callstack
	{
	private:
		std::shared_ptr<switchdata> m_switchdata;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_switch(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<switchdata> swtch
		) : callstack(ns),
			m_switchdata(std::move(swtch))
		{
		}
		std::string get_name() override { return name(); }
		static std::string name() { return "switch"; }
	};
}