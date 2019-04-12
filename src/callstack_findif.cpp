#include "callstack_findif.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "value.h"
#include "instpush.h"

::sqf::callstack::nextinstres sqf::callstack_findif::do_next(sqf::virtualmachine* vm)
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
	if (m_input_vector.size() == m_current_index)
	{
		// Receive the last result from the value stack
		bool success;
		auto val = vm->stack()->popval(success);
		if (!success)
		{
			vm->err() << "findIf callstack found no value." << std::endl;
		}
		else if (val->dtype() == type::BOOL)
		{
			if (val->as_bool())
			{
				// Update the value stack
				drop_values();
				push_back(std::make_shared<value>(m_current_index));
				return done;
			}
		}
		else
		{
			vm->err() << "findIf value was expected to be of type BOOL, got " << sqf::type_str(val->dtype()) << "." << std::endl;
		}
		// Update the value stack
		drop_values();
		push_back(std::make_shared<value>(-1));
		return done;
	}
	// Normal mode
	else if (m_input_vector.size() > m_current_index)
	{
		if (m_current_index > 0)
		{
			bool success;
			auto val = vm->stack()->popval(success);
			if (!success)
			{
				vm->err() << "findIf callstack found no value." << std::endl;
			}
			else if (val->dtype() == type::BOOL)
			{
				if (val->as_bool())
				{
					// Update the value stack
					drop_values();
					push_back(std::make_shared<value>(m_current_index));
					return done;
				}
			}
			else
			{
				vm->err() << "findIf value was expected to be of type BOOL, got " << sqf::type_str(val->dtype()) << "." << std::endl;
			}
		}
		setvar("_x", m_input_vector[m_current_index++]);
		auto sptr = std::shared_ptr<callstack_findif>(this, [](callstack_findif*) {});
		m_codedata->loadinto(vm->stack(), sptr);
	}

	// Proceed normal
	return callstack::do_next(vm);
}