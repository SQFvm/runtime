#include "instassignto.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "callstack.h"
#include "sqfnamespace.h"

void sqf::inst::assignto::execute(virtualmachine* vm) const
{
	bool flag;
	auto val = vm->active_vmstack()->pop_back_value(flag);
	if (!flag)
	{
		vm->err() << "assignTo could not receive a value." << std::endl;
		return;
	}
	if (mvarname[0] == '_')
	{
		for (auto it = vm->active_vmstack()->stacks_begin(); it != vm->active_vmstack()->stacks_end(); ++it)
		{
			if (it->get()->has_variable(mvarname))
			{
				it->get()->set_variable(mvarname, val);
				return;
			}
		}
		vm->active_vmstack()->stacks_top()->set_variable(mvarname, val);
	}
	else
	{
		vm->active_vmstack()->stacks_top()->get_namespace()->set_variable(mvarname, val);
	}
}
