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
		auto val = vm->stack()->popval(success);
		if (!success)
		{
			vm->err() << "while callstack found no value." << std::endl;
		}
		else if (val->dtype() == type::BOOL)
		{
			if (val->as_bool())
			{
				auto sptr = std::shared_ptr<callstack_while>(this, [](callstack_while*) {});
				m_codedata_body->loadinto(vm->stack(), sptr);
			}
		}
		else
		{
			vm->err() << "while callstack expected condition value to be of type BOOL, got " << sqf::type_str(val->dtype()) << "." << std::endl;
		}
	}
	else
	{
		drop_values();
		m_was_condition = true;
		auto sptr = std::shared_ptr<callstack_while>(this, [](callstack_while*) {});
		m_codedata_condition->loadinto(vm->stack(), sptr);
	}
	return callstack::do_next(vm);
}