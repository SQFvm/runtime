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
		m_codedata_catch->loadinto(vm->active_vmstack(), sptr);
		set_variable("_exception", m_message);
		set_variable("_callstack", value(m_stackdump));
	}
	// Proceed normal
	return callstack::do_next(vm);
}