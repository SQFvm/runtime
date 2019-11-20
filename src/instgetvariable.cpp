#include "instgetvariable.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "sqfnamespace.h"

void sqf::inst::getvariable::execute(virtualmachine* vm) const
{
	value val;
	if (mvarname[0] == '_')
	{ // local variable
		val = vm->active_vmstack()->get_local_variable(mvarname);
	}
	else
	{ // global variable
		val = vm->active_vmstack()->stacks_top()->get_namespace()->get_variable(mvarname);
	}
	vm->active_vmstack()->push_back(val);
}
