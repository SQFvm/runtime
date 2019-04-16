#include "callstack_apply.h"
#include "codedata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"

::sqf::callstack::nextinstres sqf::callstack_apply::do_next(sqf::virtualmachine* vm)
{
	// If callstack_apply is done, always return done
	if (previous_nextresult() == done ||
		m_input_vector.size() == 0)
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

	// Check wether or not we hit a dead-end
	if (m_input_vector.size() == m_current_index && !m_is_done)
	{
		// Receive the last result from the value stack
		bool success;
		auto val = vm->active_vmstack()->popval(success);
		if (!success)
		{
			vm->err() << "apply callstack found no value." << std::endl;
		}
		else
		{
			m_output_vector[m_current_index - 1] = val;
		}
		// set the "is done" flag to true
		m_is_done = true;
		// and update the value stack
		drop_values();
		push_back(std::make_shared<value>(m_output_vector));
		return done;
	}
	// Normal mode
	else if (m_input_vector.size() > m_current_index)
	{
		if (m_current_index > 0)
		{
			bool success;
			auto val = vm->active_vmstack()->popval(success);
			if (!success)
			{
				vm->err() << "apply callstack found no value." << std::endl;
			}
			else
			{
				m_output_vector[m_current_index - 1] = val;
			}
		}
		setvar("_x", m_input_vector[m_current_index++]);
		auto sptr = std::shared_ptr<callstack_apply>(this, [](callstack_apply*) {});
		m_codedata->loadinto(vm->active_vmstack(), sptr);
	}

	// Proceed normal
	return do_next(vm);
}