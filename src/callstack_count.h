#pragma once
#include <memory>
#include <vector>
#include "callstack.h"
#include "arraydata.h"

namespace sqf
{
	class fordata;
	class codedata;
	class callstack_count : public callstack
	{
	private:
		std::shared_ptr<arraydata> m_input_vector;
		std::shared_ptr<codedata> m_codedata;
		size_t m_current_index;
		int m_count;

	protected:
		::sqf::callstack::nextinstres do_next(sqf::virtualmachine* vm) override;

	public:
		callstack_count(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<codedata> exec,
			std::shared_ptr<arraydata> arr
		)
			: callstack(std::move(ns)),
			m_input_vector(std::move(arr)),
			m_codedata(std::move(exec)),
			m_current_index(0),
			m_count(0)
		{
		}

		std::string get_name() override { return "count"; }
	};
}