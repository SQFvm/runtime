#pragma once
#include "diagnostics/diag_info.h"

#include <string>
#include <memory>

namespace sqf::runtime
{
	class runtime;
	class instruction
	{
	public:
		using sptr = std::shared_ptr<sqf::runtime::instruction>;
	private:
		sqf::runtime::diagnostics::diag_info m_diag_info;
	public:
		virtual ~instruction() = 0;
		virtual void execute(runtime& runtime) const = 0;
		virtual std::string to_string() const = 0;

		sqf::runtime::diagnostics::diag_info diag_info() const { return m_diag_info; }
	};
}