#include "instpush.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "value.h"

void sqf::inst::push::execute(virtualmachine* vm) const { vm->active_vmstack()->push_back(mvalue); }

std::string sqf::inst::push::to_string() const
{
	return std::string("PUSH ") + mvalue.tosqf();
}
