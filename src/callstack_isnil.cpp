#include "callstack_isnil.h"
#include "codedata.h"
#include "virtualmachine.h"
#include "instpush.h"
#include "value.h"

sqf::callstack_isnil::callstack_isnil(std::shared_ptr<sqf::sqfnamespace> ns, sqf::virtualmachine * vm, std::shared_ptr<codedata> exec) : callstack(std::move(ns))
{
	auto sptr = std::shared_ptr<callstack_isnil>(this, [](callstack_isnil*) {});
	exec->loadinto(vm->active_vmstack(), sptr);
}
::sqf::callstack::nextinstres sqf::callstack_isnil::do_next(sqf::virtualmachine* vm)
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

	bool success;
	auto val = vm->active_vmstack()->pop_back_value(success);
	if (success)
	{
		// Update the value stack
		drop_values();
		push_back(val.dtype() == sqf::type::NOTHING);
	}
	else
	{
		drop_values();
		vm->wrn() << "isNil callstack found no value." << std::endl;
	}
	return done;
}