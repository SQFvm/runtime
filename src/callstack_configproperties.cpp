#include "callstack_configproperties.h"
#include "codedata.h"
#include "configdata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"

::sqf::callstack::nextinstres sqf::callstack_configproperties::do_next(sqf::virtualmachine* vm)
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

	// Check wether or not we hit a dead-end
	if (m_configdata->size() == m_current_index && !m_is_done)
	{
		// Check if we still can go one level higher
		if (!m_configdata->inherited_parent_name().empty())
		{
			m_configdata = m_configdata->inherited_parent().data<configdata>();
			m_current_index = 0;
			return do_next(vm);
		}

		// Receive the last result from the value stack
		bool success;
		auto val = vm->active_vmstack()->pop_back_value(success);
		if (!success)
		{
			vm->err() << "configProperties callstack found no value." << std::endl;
		}
		else if (val.dtype() == type::BOOL)
		{
			if (val.as_bool())
			{
				m_output_vector.push_back(m_configdata->at(m_current_index - 1));
			}
		}
		else
		{
			vm->err() << "configProperties value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
		}

		// set the "is done" flag to true
		m_is_done = true;
		// and update the value stack
		drop_values();
		push_back(value(m_output_vector));
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
				vm->err() << "configProperties callstack found no value." << std::endl;
			}
			else if (val.dtype() == type::BOOL)
			{
				if (val.as_bool())
				{
					m_output_vector.push_back(m_configdata->at(m_current_index - 1));
				}
			}
			else
			{
				vm->err() << "configProperties value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
			}
		}

		auto field = m_configdata->at(m_current_index++);
		auto fieldname = field.data<configdata>()->name();
		if (std::find(m_visited.begin(), m_visited.end(), fieldname) == m_visited.end())
		{
			m_visited.push_back(field.data<configdata>()->name());
			set_variable("_x", field);
		}
		else
		{
			return callstack::do_next(vm);
		}

		auto sptr = std::shared_ptr<callstack_configproperties>(this, [](callstack_configproperties*) {});
		m_code_condition->loadinto(vm->active_vmstack(), sptr);
	}

	// Proceed normal
	return do_next(vm);
}