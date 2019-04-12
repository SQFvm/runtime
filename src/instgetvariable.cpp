#include "instgetvariable.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "sqfnamespace.h"

void sqf::inst::getvariable::execute(virtualmachine* vm) const
{
	std::shared_ptr<value> val;
	if (mvarname[0] == '_')
	{ // local variable
		val = vm->stack()->getlocalvar(mvarname);
	}
	else
	{ // global variable
		val = vm->stack()->stacks_top()->get_namespace()->getvar(mvarname);
	}
	vm->stack()->pushval(val);
}
