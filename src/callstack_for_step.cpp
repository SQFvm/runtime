#include "callstack_for_step.h"
#include <utility>
#include "fordata.h"
#include "codedata.h"
#include "value.h"
#include "scalardata.h"
#include "virtualmachine.h"
#include "instpush.h"
#include "instassigntolocal.h"
#include "instendstatement.h"


::sqf::callstack::nextinstres sqf::callstack_for_step::do_next(sqf::virtualmachine* vm)
{
	if (m_initialized)
	{
		// If callstack_apply is done, always return done
		if (previous_nextresult() == done)
		{
			return done;
		}
		// Receive the next "normal" result
		// and unless it is done, return it
		auto next = callstack::do_next(vm);
		if (next != done)
		{
			return next;
		}

		auto variable = get_variable(m_fordata->variable());
		if (variable.dtype() != SCALAR)
		{
			vm->logmsg(logmessage::runtime::ForStepVariableTypeMissmatch(*vm->current_instruction(), m_fordata->variable(), ::sqf::type::SCALAR, variable.dtype()));
			return done;
		}
		// Check if exit condition is met
		auto current_value = variable.as_double() + m_fordata->step();
		if (m_fordata->step() > 0 ?
			current_value > m_fordata->to() :
			current_value < m_fordata->to())
		{
			return done;
		}

		// Update the variable 
		set_variable(m_fordata->variable(), current_value);

		// Load the code
		auto sptr = std::shared_ptr<callstack_for_step>(this, [](callstack_for_step*) {});
		m_codedata->loadinto(vm->active_vmstack(), sptr);
	}
	else
	{
		m_initialized = true;
		auto current_value = m_fordata->from();
		if (m_fordata->step() > 0 ? current_value > m_fordata->to() : current_value < m_fordata->to())
		{
			vm->logmsg(logmessage::runtime::ForStepNoWorkShouldBeDone(*vm->current_instruction(), m_fordata->step(), current_value, m_fordata->to()));
			return done;
		}
		// Set the for variable to its initial value
		set_variable(m_fordata->variable(), current_value);

		// Load the code
		auto sptr = std::shared_ptr<callstack_for_step>(this, [](callstack_for_step*) {});
		m_codedata->loadinto(vm->active_vmstack(), sptr);
	}

	// Proceed normal
	return do_next(vm);
}