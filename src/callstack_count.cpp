#include "callstack_count.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "value.h"
#include "instpush.h"

::sqf::callstack::nextinstres sqf::callstack_count::do_next(sqf::virtualmachine* vm)
{
    if (m_input_vector.size() == 0)
    {
        push_back(std::make_shared<value>(m_count));
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
        auto val = vm->stack()->popval(success);
        if (!success)
        {
            vm->err() << "count callstack found no value." << std::endl;
        }
        else if (val->dtype() == type::BOOL)
        {
            if (val->as_bool())
            {
                m_count++;
            }
        }
        else
        {
            vm->err() << "count value was expected to be of type BOOL, got " << sqf::type_str(val->dtype()) << "." << std::endl;
        }
    }

	// Check wether or not we hit a dead-end
	if (m_input_vector.size() == m_current_index)
	{
		// set the "is done" flag to true
		// and update the value stack
		drop_values();
		push_back(std::make_shared<value>(m_count));
		return done;
	}
	// Normal mode
	else if (m_input_vector.size() > m_current_index)
	{
		setvar("_x", m_input_vector[m_current_index++]);
		auto sptr = std::shared_ptr<callstack_count>(this, [](callstack_count*) {});
		m_codedata->loadinto(vm->stack(), sptr);
	}

	// Proceed normal
	return callstack::do_next(vm);
}
