#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class fordata;
	class codedata;
	class callstack_for_step : public callstack
	{
	private:
		std::shared_ptr<fordata> m_fordata;
		std::shared_ptr<codedata> m_codedata;
		bool m_initialized;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_for_step(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<fordata> fordata,
			std::shared_ptr<codedata> exec
		) : callstack(std::move(ns)),
			m_fordata(std::move(fordata)),
			m_codedata(std::move(exec)),
			m_initialized(false)
		{
		}

		std::string get_name() override { return "for_step"; }
	};
}