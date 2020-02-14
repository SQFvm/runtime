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
		vm->logmsg(logmessage::runtime::CallstackFoundNoValue(*vm->current_instruction(), "waituntil"sv));
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
					vm->logmsg(logmessage::runtime::SuspensionInUnscheduledEnvironment(*vm->current_instruction()));
				}
				return suspend;
			}
			else
			{
				vm->logmsg(logmessage::runtime::SuspensionDisabled(*vm->current_instruction()));
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
		vm->logmsg(logmessage::runtime::TypeMissmatch(*vm->current_instruction(), sqf::type::BOOL, val.dtype()));
		push_back(value());
		return done;
	}
	else
	{
		vm->logmsg(logmessage::runtime::TypeMissmatch(*vm->current_instruction(), sqf::type::BOOL, val.dtype()));
		return done;
	}
	return do_next(vm);
}