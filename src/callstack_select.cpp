#include "callstack_select.h"
#include "codedata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"

::sqf::callstack::nextinstres sqf::callstack_select::do_next(sqf::virtualmachine* vm)
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

	// Check wether or not we hit a dead-end
	if (m_input_array->size() == m_current_index && !m_is_done)
	{
		// Receive the last result from the value stack
		bool success;
		auto val = vm->active_vmstack()->pop_back_value(success);
		if (!success)
		{
			vm->err() << "select callstack found no value." << std::endl;
		}
		else if (val.dtype() == type::BOOL)
		{
			if (val.as_bool())
			{
				m_output_vector.push_back((*m_input_array)[m_current_index - 1]);
			}
		}
		else if (val.dtype() == type::NOTHING)
		{
			vm->wrn() << "select value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
		}
		else
		{
			vm->err() << "select value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
		}
		// set the "is done" flag to true
		m_is_done = true;
		// and update the value stack
		drop_values();
		push_back(m_output_vector);
		return done;
	}
	// Normal mode
	else if (m_input_array->size() > m_current_index)
	{
		if (m_current_index > 0)
		{
			bool success;
			auto val = vm->active_vmstack()->pop_back_value(success);
			if (!success)
			{
				vm->err() << "select callstack found no value." << std::endl;
			}
			else if (val.dtype() == type::BOOL)
			{
				if (val.as_bool())
				{
					m_output_vector.push_back((*m_input_array)[m_current_index - 1]);
				}
			}
			else if (val.dtype() == type::NOTHING)
			{
				vm->wrn() << "select value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
			}
			else
			{
				vm->err() << "select value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
			}
		}
		set_variable("_x", (*m_input_array)[m_current_index++]);
		auto sptr = std::shared_ptr<callstack_select>(this, [](callstack_select*) {});
		m_codedata->loadinto(vm->active_vmstack(), sptr);
	}

	// Proceed normal
	return do_next(vm);
}