#include "instendstatement.h"
#include "virtualmachine.h"
#include "vmstack.h"

void sqf::inst::endstatement::execute(virtualmachine* vm) const
{
	if (!vm->active_vmstack()->empty())
	{
		vm->active_vmstack()->stacks_top()->drop_values();
	}
}
