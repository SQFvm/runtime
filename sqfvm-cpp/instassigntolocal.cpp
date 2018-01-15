#include "full.h"

void sqf::inst::assigntolocal::execute(const virtualmachine* vm, std::shared_ptr<vmstack> stack) const
{
	bool flag;
	auto val = stack->popval(flag);
	if (!flag)
	{
		vm->err() << L"[ASS]" << vm->dbginf() << "assignToLocal could not receive a value." << std::endl;
		return;
	}
	stack->stacks_top()->setvar(mvarname, val);
}
