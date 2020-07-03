#include "callstack_exitwith.h"
#include "virtualmachine.h"
#include "vmstack.h"
::sqf::callstack::nextinstres sqf::callstack_exitwith::do_next(sqf::virtualmachine* vm)
{
	// Receive the next "normal" result
	// and unless it is done, return it
	auto next = callstack::do_next(vm);
	if (next != done)
	{
		return next;
	}
	return exitwith;
}
