#include "instassignto.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "callstack.h"
#include "sqfnamespace.h"

void sqf::inst::assignto::execute(virtualmachine* vm) const
{
	bool flag;
	auto val = vm->active_vmstack()->popval(flag);
	if (!flag)
	{
		vm->err() << "assignTo could not receive a value." << std::endl;
		return;
	}
	if (mvarname[0] == '_')
	{
		for (auto it = vm->active_vmstack()->stacks_begin(); it != vm->active_vmstack()->stacks_end(); ++it)
		{
			if (it->get()->containsvar(mvarname))
			{
				it->get()->setvar(mvarname, val);
				return;
			}
		}
		vm->active_vmstack()->stacks_top()->setvar(mvarname, val);
	}
	else
	{
		vm->active_vmstack()->stacks_top()->get_namespace()->setvar(mvarname, val);
	}
}
