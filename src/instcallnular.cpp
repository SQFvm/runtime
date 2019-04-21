#include "instcallnular.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "value.h"

void sqf::inst::callnular::execute(virtualmachine* vm) const
{
    value dummy;
    auto val = mcmd->execute(vm, dummy, dummy);
	if (val)
		vm->active_vmstack()->pushval(val);
}

std::string sqf::inst::callnular::to_string() const
{
	return "CALLNULAR " + mcmd->name();
}
