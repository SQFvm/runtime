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

	if (!m_switchdata->executed() && m_switchdata->defaultexec().get())
	{
		auto sptr = std::shared_ptr<callstack_switch>(this, [](callstack_switch*) {});
		m_switchdata->defaultexec()->loadinto(vm->stack(), sptr);
		m_switchdata->defaultexec(std::shared_ptr<codedata>());
		return callstack::do_next(vm);
	}

	return done;
}