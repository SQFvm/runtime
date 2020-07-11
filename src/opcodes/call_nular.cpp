#include "instcallnular.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "value.h"

void sqf::inst::call_nular::execute(virtualmachine* vm) const
{
    value dummy;
    auto val = mcmd->execute(vm, dummy, dummy);
	vm->active_vmstack()->push_back(val);
}

std::string sqf::inst::call_nular::to_string() const
{
	return "CALLNULAR " + mcmd->name();
}
