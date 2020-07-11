#include "get_variable.h"
#include "../runtime/runtime.h"
#include "../runtime/value.h"
#include "../runtime/value_scope.h"

void sqf::opcodes::get_variable::execute(sqf::runtime::runtime& vm) const
{
	if (m_variable_name[0] == '_')
	{ // local variable
		auto opt = vm.active_context()->get_variable(m_variable_name);
		if (opt.has_value())
		{
			vm.active_context()->push_value(*opt);
		}
		else
		{
			vm.__logmsg(logmessage::runtime::VariableNotFound(diag_info(), m_variable_name));
		}
	}
	else
	{ // global variable
		auto global_scope = vm.active_context()->current_frame().globals_value_scope();
		if (global_scope->contains(m_variable_name))
		{
			vm.active_context()->push_value(global_scope->get(m_variable_name));
		}
		else
		{
			vm.__logmsg(logmessage::runtime::VariableNotFound(diag_info(), m_variable_name));
		}
	}
}
