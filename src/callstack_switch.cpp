#include "callstack_switch.h"
#include "switchdata.h"
#include "codedata.h"
#include "virtualmachine.h"

std::shared_ptr<sqf::instruction> sqf::callstack_switch::popinst(sqf::virtualmachine * vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (ret)
		return ret;
	if (!mswtch->executed() && mswtch->defaultexec().get())
	{
		auto sptr = std::shared_ptr<callstack_switch>(this, [](callstack_switch*) {});
		mswtch->defaultexec()->loadinto(vm->stack(), sptr);
		mswtch->defaultexec(std::shared_ptr<codedata>());
		return sqf::callstack::popinst(vm);
	}
	return ret;
}
