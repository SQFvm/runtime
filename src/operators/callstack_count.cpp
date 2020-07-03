#include "callstack_count.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "value.h"
#include "instpush.h"

::sqf::callstack::nextinstres sqf::callstack_count::do_next(sqf::virtualmachine* vm)
{
    if (m_input_vector->empty())
    {
        push_back(m_count);
        return done;
    }

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

    if (m_current_index > 0)
    {
        bool success;
        auto val = vm->active_vmstack()->pop_back_value(success);
        if (!success)
        {
			vm->logmsg(logmessage::runtime::CallstackFoundNoValue(*vm->current_instruction(), "count"sv));
        }
        else if (val.dtype() == type::BOOL)
        {
            if (val.as_bool())
            {
                m_count++;
            }
        }
        else if (val.dtype() == type::NOTHING)
        {
			vm->logmsg(logmessage::runtime::TypeMissmatchWeak(*vm->current_instruction(), sqf::type::BOOL, val.dtype()));
        }
        else
        {
			vm->logmsg(logmessage::runtime::TypeMissmatch(*vm->current_instruction(), sqf::type::BOOL, val.dtype()));
        }
    }

	// Check wether or not we hit a dead-end
	if (m_input_vector->size() == m_current_index)
	{
		// set the "is done" flag to true
		// and update the value stack
		drop_values();
		push_back(m_count);
		return done;
	}
	// Normal mode
	else if (m_input_vector->size() > m_current_index)
	{
		set_variable("_x", (*m_input_vector)[m_current_index++]);
		auto sptr = std::shared_ptr<callstack_count>(this, [](callstack_count*) {});
		m_codedata->loadinto(vm->active_vmstack(), sptr);
	}

	// Proceed normal
	return do_next(vm);
}
