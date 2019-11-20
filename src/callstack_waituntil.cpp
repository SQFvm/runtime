#include "callstack_waituntil.h"
#include "codedata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"
::sqf::callstack::nextinstres sqf::callstack_waituntil::do_next(sqf::virtualmachine* vm)
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

	// Receive the last result from the value stack
	bool success;
	auto val = vm->active_vmstack()->pop_back_value(success);
	if (!success)
	{
		vm->err() << "waituntil callstack found no value." << std::endl;
		return done;
	}
	else if (val.dtype() == type::BOOL)
	{
		if (!val.as_bool())
		{
			drop_values();
			auto sptr = std::shared_ptr<callstack_waituntil>(this, [](callstack_waituntil*) {});
			m_codedata_condition->loadinto(vm->active_vmstack(), sptr);
			if (vm->allow_suspension())
			{
				if (!vm->active_vmstack()->scheduled())
				{
					vm->wrn() << "waitUntil in non-scheduled code." << std::endl;
				}
				return suspend;
			}
			else
			{
				vm->err() << "Suspension not allowed." << std::endl;
				return done;
			}
		}
		else
		{
			push_back(value());
			return done;
		}
	}
	else if (val.dtype() == type::NOTHING)
	{
		vm->wrn() << "waituntil value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
		push_back(value());
		return done;
	}
	else
	{
		vm->err() << "waituntil value was expected to be of type BOOL, got " << sqf::type_str(val.dtype()) << "." << std::endl;
		return done;
	}
	return do_next(vm);
}