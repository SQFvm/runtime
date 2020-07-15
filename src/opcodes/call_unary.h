#pragma once
#include "../runtime/instruction.h"
#include "../runtime/value.h"
#include "../runtime/data.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"

namespace sqf::opcodes
{
	class call_unary : public sqf::runtime::instruction
	{
	private:
		std::string m_operator_name;

	public:
		call_unary(std::string key) : m_operator_name(key) {}
		virtual void execute(sqf::runtime::runtime& vm) const override
		{
			auto context = vm.active_context();

			auto right_value = vm.active_context()->pop_value();
			if (!right_value.has_value() || right_value->is<sqf::types::t_nothing>())
			{
				if (context->weak_error_handling())
				{
					vm.__logmsg(logmessage::runtime::NoValueFoundForRightArgumentWeak(diag_info()));
				}
				else
				{
					vm.__logmsg(logmessage::runtime::NoValueFoundForRightArgument(diag_info()));
				}
				return;
			}

			sqf::runtime::sqfop_unary::key key = { m_operator_name, *right_value };
			if (!vm.sqfop_exists(key))
			{
				vm.__logmsg(logmessage::runtime::UnknownInputTypeCombinationUnary(diag_info(), key.name, key.right_type));
				return;
			}
			auto op = vm.sqfop_at(key);
			auto return_value = op.execute(vm, *right_value);

			context->push_value(return_value);
		}
		virtual std::string to_string() const override { return std::string("CALLUNARY ") + m_operator_name; }
		std::string_view operator_name() const { return m_operator_name; }
	};
}
