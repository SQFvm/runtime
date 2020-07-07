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
		virtual void execute(runtime* runtime) const { }
		virtual std::string to_string() const { return {}; }

		sqf::runtime::diagnostics::diag_info diag_info() const { return m_diag_info; }
	};
}