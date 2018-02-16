#include "instassigntolocal.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "callstack.h"

void sqf::inst::assigntolocal::execute(virtualmachine* vm) const
{
	bool flag;
	auto val = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf(L"ASS") << "assignToLocal could not receive a value." << std::endl;
		return;
	}
	vm->stack()->stacks_top()->setvar(mvarname, val);
}
