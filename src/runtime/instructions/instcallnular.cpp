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
	vm->active_vmstack()->push_back(val);
}

std::string sqf::inst::callnular::to_string() const
{
	return "CALLNULAR " + mcmd->name();
}
