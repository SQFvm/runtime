#include "instassignto.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "callstack.h"
#include "sqfnamespace.h"

void sqf::inst::assignto::execute(virtualmachine* vm) const
{
	bool flag;
	auto val = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf("ASS") << "assignTo could not receive a value." << std::endl;
		return;
	}
	if (mvarname[0] == '_')
	{
		for (auto it = vm->stack()->stacks_begin(); it != vm->stack()->stacks_end(); it++)
		{
			if (it->get()->containsvar(mvarname))
			{
				it->get()->setvar(mvarname, val);
				return;
			}
		}
		vm->stack()->stacks_top()->setvar(mvarname, val);
	}
	else
	{
		vm->stack()->stacks_top()->getnamespace()->setvar(mvarname, val);
	}
}
