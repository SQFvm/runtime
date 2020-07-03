#include "callstack_while.h"
#include "codedata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"
::sqf::callstack::nextinstres sqf::callstack_while::do_next(sqf::virtualmachine* vm)
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

	if (m_was_condition)
	{
		m_was_condition = false;

		// Receive the last result from the value stack
		bool success;
		auto val = vm->active_vmstack()->pop_back_value(success);
		if (!success)
		{
			vm->logmsg(logmessage::runtime::CallstackFoundNoValue(*vm->current_instruction(), "while"sv));
			return done;
		}
		else if (val.dtype() == type::BOOL)
		{
			if (val.as_bool())
			{
				auto sptr = std::shared_ptr<callstack_while>(this, [](callstack_while*) {});
				m_codedata_body->loadinto(vm->active_vmstack(), sptr);
			}
			else
			{
				push_back(value());
				return done;
			}
		}
		else if (val.dtype() == type::NOTHING)
		{
			vm->logmsg(logmessage::runtime::TypeMissmatchWeak(*vm->current_instruction(), sqf::type::BOOL, val.dtype()));
			push_back(value());
			return done;
		}
		else
		{
			vm->logmsg(logmessage::runtime::TypeMissmatch(*vm->current_instruction(), sqf::type::BOOL, val.dtype()));
			return done;
		}
	}
	else
	{
		drop_values();
		m_was_condition = true;
		auto sptr = std::shared_ptr<callstack_while>(this, [](callstack_while*) {});
		m_codedata_condition->loadinto(vm->active_vmstack(), sptr);
	}
	return do_next(vm);
}