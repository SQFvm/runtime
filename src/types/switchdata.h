#pragma once
#include <string>
#include <memory>
#include <utility>
#include <vector>

#include "data.h"
#include "value.h"


namespace sqf
{
	class value;
	class codedata;
	class switchdata : public data
	{
	private:
		value mvalue;
		std::shared_ptr<codedata> m_exec_default;
		bool m_has_match;
		std::shared_ptr<codedata> m_exec;
		bool m_was_executed;
	public:
		switchdata(value val) : mvalue(std::move(val)), m_exec_default(), m_has_match(false), m_exec(), m_was_executed(false) {}
		std::string tosqf() const override;

		void has_match(bool f)
		{
			m_has_match = f;
		}
		bool has_match() const
		{
			return m_has_match;
		}

		void was_executed(bool f)
		{
			m_was_executed = f;
		}
		bool was_executed() const
		{
			return m_was_executed;
		}

		void exec_default(std::shared_ptr<codedata> exec)
		{
			m_exec_default = std::move(exec);
		}
		std::shared_ptr<codedata> exec_default() const
		{
			return m_exec_default;
		}

		void exec(std::shared_ptr<codedata> exec)
		{
			m_exec = std::move(exec);
		}
		std::shared_ptr<codedata> exec() const
		{
			return m_exec;
		}

		value val() const { return mvalue; }
		bool equals(std::shared_ptr<data> d) const override { return mvalue.equals(std::dynamic_pointer_cast<switchdata>(d)->mvalue); }
		sqf::type dtype() const override { return sqf::type::SWITCH; }
	};
}