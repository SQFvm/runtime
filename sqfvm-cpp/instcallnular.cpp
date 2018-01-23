#include "full.h"

void sqf::inst::callnular::execute(const virtualmachine* vm) const
{
	auto val = mcmd->execute(vm, value_s(), value_s());
	vm->stack()->pushval(val);
}
