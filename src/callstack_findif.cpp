#include "callstack_findif.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "value.h"
#include "instpush.h"

::sqf::callstack::nextinstres sqf::callstack_findif::do_next(sqf::virtualmachine* vm)
{
	// If callstack_apply is done, always return done
	if (previous_nextresult() == done ||
        m_input_array->empty())
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

    if (m_current_index > 0)
    {
        bool success;
        auto val = vm->active_vmstack()->pop_back_value(success);
        if (!success)
        {
            vm->err() << "findIf callstack found no value." << std::endl;
        }
        else if (val.dtype() == type::BOOL)
        {
            if (val.as_bool())
            {
                // Update the value stack
                drop_values();
                push_back(m_current_index-1);
                return done;
            }
        }
        else
        {
            vm->err() << "findIf value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
        }
    }

	// Check whether or not we hit a dead-end
	if (m_input_array->size() == m_current_index)
	{
		// Update the value stack
		drop_values();
		push_back(-1);
		return done;
	}
	// Normal mode
	else if (m_input_array->size() > m_current_index)
	{
		set_variable("_x", (*m_input_array)[m_current_index++]);
		auto sptr = std::shared_ptr<callstack_findif>(this, [](callstack_findif*) {});
		m_codedata->loadinto(vm->active_vmstack(), sptr);
	}

	// Proceed normal
	return do_next(vm);
}