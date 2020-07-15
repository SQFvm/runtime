#pragma once
#include "diagnostics/diag_info.h"

#include <string>
#include <memory>
#include <vector>
#include <optional>

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
		virtual std::optional<std::string> reconstruct(
			std::vector<sqf::runtime::instruction::sptr>::const_iterator& current,
			std::vector<sqf::runtime::instruction::sptr>::const_iterator end,
			short parent_precedence, bool left_from_binary) const = 0;
		virtual bool equals(const instruction* p_other) const = 0;

		bool operator==(const sptr other) const { return equals(other.get()); }

		sqf::runtime::diagnostics::diag_info diag_info() const { return m_diag_info; }
		void diag_info(sqf::runtime::diagnostics::diag_info dinf) { m_diag_info = dinf; }
	};
}