#pragma once
#include "../runtime/instruction.h"
#include "../runtime/value.h"
#include "../runtime/data.h"
#include "../runtime/runtime.h"

namespace sqf::opcodes
{
	class assign_to : public sqf::runtime::instruction
	{
	private:
		std::string m_variable_name;

	public:
		assign_to(std::string value) : m_variable_name(value) {}
		virtual void execute(sqf::runtime::runtime& vm) const override
		{
			auto& context = vm.active_context();

			auto value = vm.active_context().pop_value();
			if (!value.has_value() || value->is<sqf::types::t_nothing>())
			{
				if (context.weak_error_handling())
				{
					vm.__logmsg(logmessage::runtime::FoundNoValueWeak(diag_info()));
				}
				else
				{
					vm.__logmsg(logmessage::runtime::FoundNoValue(diag_info()));
				}
				return;
			}

			if (m_variable_name[0] == '_')
			{
				for (auto it = context.frames_rbegin(); it != context.frames_rend(); ++it)
				{
					if (it->contains(m_variable_name))
					{
						(*it)[m_variable_name] = *value;
						return;
					}
				}
				context.current_frame()[m_variable_name] = *value;
			}
			else
			{
				context.current_frame().globals_value_scope()->at(m_variable_name) = *value;
			}
		}
		virtual std::string to_string() const override { return std::string("ASSIGNTO ") + m_variable_name; }
		std::string_view variable_name() const { return m_variable_name; }

		virtual std::optional<std::string> reconstruct(
			std::vector<sqf::runtime::instruction::sptr>::const_iterator& current,
			std::vector<sqf::runtime::instruction::sptr>::const_iterator end,
			short parent_precedence, bool left_from_binary) const override
		{
			if (++current == end)
			{
				return {};
			}
			auto exp = (*current)->reconstruct(current, end, 10, false);
			if (!exp.has_value())
			{
				return {};
			}
			return m_variable_name + " = " + *exp;
		}

		virtual bool equals(const instruction* p_other) const override
		{
			auto casted = dynamic_cast<const assign_to*>(p_other);
			return casted != nullptr && casted->m_variable_name == m_variable_name;
		}
	};
}