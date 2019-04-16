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
		val = vm->active_vmstack()->getlocalvar(mvarname);
	}
	else
	{ // global variable
		val = vm->active_vmstack()->stacks_top()->get_namespace()->getvar(mvarname);
	}
	vm->active_vmstack()->pushval(val);
}
