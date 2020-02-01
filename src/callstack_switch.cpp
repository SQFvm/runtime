#include "callstack_switch.h"
#include "switchdata.h"
#include "codedata.h"
#include "virtualmachine.h"

::sqf::callstack::nextinstres sqf::callstack_switch::do_next(sqf::virtualmachine* vm)
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

	if (!m_switchdata->was_executed())
	{
		m_switchdata->was_executed(true);
		auto sptr = std::shared_ptr<callstack_switch>(this, [](callstack_switch*) {});
		if (m_switchdata->has_match())
		{
			m_switchdata->exec()->loadinto(vm->active_vmstack(), sptr);
			m_switchdata->exec(std::shared_ptr<codedata>());
		}
		else if (m_switchdata->exec_default())
		{
			m_switchdata->exec_default()->loadinto(vm->active_vmstack(), sptr);
			m_switchdata->exec_default(std::shared_ptr<codedata>());
		}
		return callstack::do_next(vm);
	}

	return done;
}