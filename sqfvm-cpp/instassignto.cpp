#include "full.h"

void sqf::inst::assignto::execute(const virtualmachine* vm, std::shared_ptr<vmstack> stack) const
{
	bool flag;
	auto val = stack->popval(flag);
	if (!flag)
	{
		vm->err() << L"[ASS]" << vm->dbginf() << "assignTo could not receive a value." << std::endl;
		return;
	}
	for (auto it = stack->stacks_begin(); it != stack->stacks_end(); it++)
	{
		if (it->get()->containsvar(mvarname))
		{
			it->get()->setvar(mvarname, val);
			return;
		}
	}
	stack->stacks_top()->setvar(mvarname, val);
}
