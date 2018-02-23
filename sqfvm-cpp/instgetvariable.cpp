#include "instgetvariable.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "sqfnamespace.h"

void sqf::inst::getvariable::execute(virtualmachine* vm) const
{
	std::shared_ptr<value> val;
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
