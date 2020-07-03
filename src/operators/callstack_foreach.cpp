#include "callstack_foreach.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "value.h"


::sqf::callstack::nextinstres sqf::callstack_foreach::do_next(sqf::virtualmachine* vm)
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
	
	if (m_current_index >= m_array->size())
	{
		return done;
	}

	drop_values();
	drop_variables();
	auto sptr = std::shared_ptr<callstack_foreach>(this, [](callstack_foreach*) {});
	m_codedata->loadinto(vm->active_vmstack(), sptr);
	sptr->set_variable("_x", (*m_array)[m_current_index]);
	sptr->set_variable("_forEachIndex", m_current_index);
	m_current_index++;

	// Proceed normal
	return do_next(vm);
}