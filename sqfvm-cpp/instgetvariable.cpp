#include "full.h"

void sqf::inst::getvariable::execute(const virtualmachine* vm) const
{
	value_s val;
	if (mvarname[0] == L'_')
	{//localvar
		for (auto it = vm->stack()->stacks_begin(); it != vm->stack()->stacks_end(); it++)
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
		val = vm->stack()->stacks_top()->getnamespace()->getvar(mvarname);
	}
	vm->stack()->pushval(val);
}
