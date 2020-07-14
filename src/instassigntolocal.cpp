#include "instassigntolocal.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "callstack.h"

void sqf::inst::assigntolocal::execute(virtualmachine* vm) const
{
	bool flag;
	auto val = vm->active_vmstack()->pop_back_value(flag);
	if (!flag)
	{
		vm->logmsg(logmessage::runtime::FoundNoValue(*vm->current_instruction()));
		return;
	}
	vm->active_vmstack()->stacks_top()->set_variable(mvarname, val);
}
