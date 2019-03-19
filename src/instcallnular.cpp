#include "instcallnular.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"

void sqf::inst::callnular::execute(virtualmachine* vm) const
{
	auto val = mcmd->execute(vm, std::shared_ptr<value>(), std::shared_ptr<value>());
	if (val)
		vm->stack()->pushval(val);
}

std::string sqf::inst::callnular::to_string() const
{
	return "CALLNULAR " + mcmd->name();
}
