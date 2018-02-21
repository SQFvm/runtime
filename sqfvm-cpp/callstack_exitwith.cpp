#include "callstack_exitwith.h"
#include "virtualmachine.h"
#include "vmstack.h"

std::shared_ptr<sqf::instruction> sqf::callstack_exitwith::popinst(sqf::virtualmachine * vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (!ret.get())
	{
		vm->stack()->dropcallstack();
		return ret;
	}
}
