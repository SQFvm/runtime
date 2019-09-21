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
			vm->wrn() << "for_step callstack variable '" << m_fordata->variable() << "' was expected to be of type SCALAR, got " << type_str(variable.dtype()) << "." << std::endl;
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
			vm->wrn() << "for_step callstack 'from " << current_value << " to " << m_fordata->to() << " step " << m_fordata->step() << "'." << std::endl;
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