#include "instgetvariable.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "sqfnamespace.h"

void sqf::inst::getvariable::execute(virtualmachine* vm) const
{
	value val;
	bool success = false;
	if (mvarname[0] == '_')
	{ // local variable
		val = vm->active_vmstack()->get_variable(mvarname, success);
	}
	else
	{ // global variable
		val = vm->active_vmstack()->stacks_top()->get_namespace()->get_variable(mvarname, success);
	}
	if (!success)
	{
		vm->wrn() << "Could not find variable with the name '" << mvarname << "'" << std::endl;
	}
	vm->active_vmstack()->push_back(val);
}
