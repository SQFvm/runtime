#include "callstack_sqftry.h"
#include "value.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "arraydata.h"

::sqf::callstack::nextinstres sqf::callstack_sqftry::do_next(sqf::virtualmachine* vm)
{
	if (m_do_catch && !m_done_catch)
	{
		drop_instructions();
		drop_values();
		m_done_catch = true;
		auto sptr = std::shared_ptr<callstack_sqftry>(this, [](callstack_sqftry*) {});
		m_codedata_catch->loadinto(vm->stack(), sptr);
		setvar("_exception", std::make_shared<value>(m_message));
		setvar("_callstack", std::make_shared<value>(m_stackdump, sqf::type::ARRAY));
	}
	// Proceed normal
	return callstack::do_next(vm);
}