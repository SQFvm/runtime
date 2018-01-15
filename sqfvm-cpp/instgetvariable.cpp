#include "full.h"

void sqf::inst::getvariable::execute(const virtualmachine* vm, std::shared_ptr<vmstack> stack) const
{
	value_s val;
	if (mvarname[0] == L'_')
	{//localvar
		for (auto it = stack->stacks_begin(); it != stack->stacks_end(); it++)
		{
			if (it->get()->containsvar(mvarname))
			{
				val = it->get()->getvar(mvarname);
				break;
			}
		}
	}
	else
	{//globalvar
		//ToDo: get from current namespace
	}
	stack->pushval(val);
}
