#pragma once
#include <memory>
#include <string>
#include <vector>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class configdata;
	class callstack_configproperties : public callstack
	{
	private:
		size_t m_current_index;
		bool m_is_done;
		std::vector<value> m_output_vector;
		std::shared_ptr<codedata> m_code_condition;
		std::shared_ptr<configdata> m_configdata;
		std::vector<std::string> m_visited;
		bool m_include_inherited;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_configproperties(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<configdata> data,
			std::shared_ptr<codedata> cond,
			bool include_inherited
		)
		: callstack(std::move(ns)),
			m_current_index(0),
			m_is_done(false),
			m_code_condition(std::move(cond)),
			m_configdata(std::move(data)),
			m_include_inherited(include_inherited)
		{
		}

		std::string get_name() override { return "configProperties"; }
	};
}