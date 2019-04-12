#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class callstack_apply : public callstack
	{
	private:
		size_t m_current_index;
		std::vector<std::shared_ptr<value>> m_output_vector;
		std::vector<std::shared_ptr<value>> m_input_vector;
		std::shared_ptr<codedata> m_codedata;
		bool m_is_done;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_apply(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::vector<std::shared_ptr<value>> arr,
			std::shared_ptr<codedata> apply
		) : callstack(ns),
			m_current_index(0),
			m_output_vector(arr.size()),
			m_input_vector(std::move(arr)),
			m_codedata(std::move(apply)),
			m_is_done(false)
		{
		}


		std::string get_name() override { return "apply"; }
	};
}