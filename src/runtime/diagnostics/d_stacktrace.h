#pragma once
#include "../data.h"
#include "../type.h"
#include "../value.h"
#include "stacktrace.h"

#include <string>
#include <memory>

namespace sqf
{
	namespace runtime
	{
		struct t_stacktrace : public type::extend<t_stacktrace> { t_stacktrace() : extend() {} static const std::string name() { return "VM-STACKTRACE"; } };
	}
	namespace types
	{
		class d_stacktrace : public sqf::runtime::data
		{
		public:
			static sqf::runtime::type cexp_type() { return sqf::runtime::t_stacktrace(); }
		private:
			sqf::runtime::diagnostics::stacktrace m_value;
		protected:
			bool do_equals(std::shared_ptr<sqf::runtime::data> other, bool invariant) const override
			{
				// A stacktrace is never equal to another stacktrace
				return false;
			}
		public:
			d_stacktrace() = default;
			d_stacktrace(sqf::runtime::diagnostics::stacktrace stacktrace) : m_value(stacktrace) {}

			std::string to_string_sqf() const override
			{
				return "\"STACKTRACE\"";
			}
			std::string to_string() const override
			{
				return "STACKTRACE";
			}

			sqf::runtime::type type() const override { return cexp_type(); }

			sqf::runtime::diagnostics::stacktrace value() const { return m_value; }
			void value(sqf::runtime::diagnostics::stacktrace stacktrace) { m_value = stacktrace; }

			operator sqf::runtime::diagnostics::stacktrace()
			{
				return m_value;
			}
		};

		std::shared_ptr<sqf::runtime::data>& operator<<(std::shared_ptr<sqf::runtime::data>& input, sqf::runtime::diagnostics::stacktrace str)
		{
			input = std::make_shared<d_stacktrace>(str);
			return input;
		}
	}
}