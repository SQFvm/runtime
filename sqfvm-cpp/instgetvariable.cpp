#include "full.h"

void sqf::inst::getvariable::execute(const virtualmachine* vm) const
{
	value_s val;
	if (mvarname[0] == L'_')
	{//localvar
		val = vm->stack()->getlocalvar(mvarname);
	}
	else
	{//globalvar
		val = vm->stack()->stacks_top()->getnamespace()->getvar(mvarname);
	}
	vm->stack()->pushval(val);
}
