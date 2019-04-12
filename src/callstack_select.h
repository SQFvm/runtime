#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_select : public callstack
	{
	private:
		size_t m_current_index;
		bool m_is_done;
		std::vector<std::shared_ptr<value>> m_input_vector;
		std::vector<std::shared_ptr<value>> m_output_vector;
		std::shared_ptr<codedata> m_codedata;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_select(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::vector<std::shared_ptr<value>> arr,
			std::shared_ptr<codedata> cond
		) : callstack(ns),
			m_current_index(0),
			m_is_done(arr.empty()),
			m_input_vector(std::move(arr)),
			m_output_vector(),
			m_codedata(std::move(cond))
		{
		}

		std::string get_name() override { return "select"; }
	};
}