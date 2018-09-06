#include "instpush.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "value.h"

void sqf::inst::push::execute(virtualmachine* vm) const { vm->stack()->pushval(mvalue); }
