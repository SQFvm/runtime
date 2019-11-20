#include "callstack_configclasses.h"
#include "codedata.h"
#include "configdata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"
::sqf::callstack::nextinstres sqf::callstack_configclasses::do_next(sqf::virtualmachine* vm)
{
	// If callstack_configclasses is done, always return done
	if (previous_nextresult() == done ||
        m_configdata->empty())
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

	// Search for the next configdata with an actual value
	value val;
	auto previous_current_index = m_current_index;
	while (m_configdata->size() > m_current_index)
	{
		val = m_configdata->at(static_cast<int>(m_current_index));
		m_current_index++;
		if (val.data<configdata>()->cfgvalue().dtype() != type::NOTHING)
		{
			break;
		}
	}

	// Check wether or not we hit a dead-end
	if (m_configdata->size() == m_current_index && !m_is_done)
	{
		// Receive the last result from the value stack
		bool success;
		auto val = vm->active_vmstack()->pop_back_value(success);
		if (!success)
		{
			if (previous_current_index)
			{
				vm->err() << "configClasses callstack found no value." << std::endl;
			}
		}
		else if (val.dtype() == type::BOOL)
		{
			if (val.as_bool())
			{
				m_output_vector.push_back(m_configdata->at(static_cast<int>(m_current_index - 1)));
			}
		}
		else
		{
			vm->err() << "configClasses value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
		}

		// set the "is done" flag to true
		m_is_done = true;
		// and update the value stack
		drop_values();
		push_back(m_output_vector);
		return done;
	}
	// Normal mode
	else if (m_configdata->size() > m_current_index)
	{
		if (m_current_index > 0)
		{
			bool success;
			auto val = vm->active_vmstack()->pop_back_value(success);
			if (!success)
			{
				vm->err() << "configClasses callstack found no value." << std::endl;
			}
			else if (val.dtype() == type::BOOL)
			{
				if (val.as_bool())
				{
					m_output_vector.push_back(m_configdata->at(static_cast<int>(m_current_index - 1)));
				}
			}
			else
			{
				vm->err() << "configClasses value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
			}
		}
		set_variable("_x", val);

		auto sptr = std::shared_ptr<callstack_configclasses>(this, [](callstack_configclasses*) {});
		m_code_condition->loadinto(vm->active_vmstack(), sptr);
	}

	// Proceed normal
	return do_next(vm);
}