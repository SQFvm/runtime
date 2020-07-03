#pragma once
#include <memory>
#include "callstack.h"
#include "arraydata.h"

namespace sqf
{
	class codedata;
	class callstack_apply : public callstack
	{
	private:
		size_t m_current_index;
		std::vector<value> m_output_vector;
		std::shared_ptr<arraydata> m_input_array;
		std::shared_ptr<codedata> m_codedata;
		bool m_is_done;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_apply(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<arraydata> arr,
			std::shared_ptr<codedata> apply
		) : callstack(std::move(ns)),
			m_current_index(0),
			m_output_vector(arr->size()),
			m_input_array(std::move(arr)),
			m_codedata(std::move(apply)),
			m_is_done(false)
		{
		}


		std::string get_name() override { return "apply"; }
	};
}