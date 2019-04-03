#include "instendstatement.h"
#include "virtualmachine.h"
#include "vmstack.h"

void sqf::inst::endstatement::execute(virtualmachine* vm) const
{
	if (!vm->stack()->isempty())
	{
		vm->stack()->stacks_top()->drop_values();
	}
}
