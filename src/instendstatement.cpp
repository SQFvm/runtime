#include "instendstatement.h"
#include "virtualmachine.h"
#include "vmstack.h"

void sqf::inst::endstatement::execute(virtualmachine* vm) const { vm->stack()->dropvals(); }
