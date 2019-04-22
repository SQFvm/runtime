#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class configdata;
	class callstack_configclasses : public callstack
	{
	private:
		size_t m_current_index;
		bool m_is_done;
		std::vector<value> m_output_vector;
		std::shared_ptr<codedata> m_code_condition;
		std::shared_ptr<configdata> m_configdata;

	protected:
		nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_configclasses(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<configdata> data,
			std::shared_ptr<codedata> cond
		)
		: callstack(std::move(ns)),
			m_current_index(0),
			m_is_done(false),
			m_code_condition(std::move(cond)),
			m_configdata(std::move(data))
		{}

		std::string get_name() override { return "configClasses"; }
	};
}