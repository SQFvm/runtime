#include "callstack_catch.h"
#include "value.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "arraydata.h"

::sqf::callstack::nextinstres sqf::callstack_catch::do_next(sqf::virtualmachine* vm)
{
	if (m_do_catch && !m_done_catch)
	{
		drop_instructions();
		drop_values();
		m_done_catch = true;
		auto sptr = std::shared_ptr<callstack_catch>(this, [](callstack_catch*) {});
		m_codedata_catch->loadinto(vm->active_vmstack(), sptr);
		set_variable("_exception", m_message);
	}
	// Proceed normal
	return callstack::do_next(vm);
}