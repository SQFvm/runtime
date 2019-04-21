#include "instcallunary.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "value.h"

void sqf::inst::callunary::execute(virtualmachine* vm) const
{
	bool flag;
	auto right = vm->active_vmstack()->popval(flag);
	if (!flag || right == nullptr || right->dtype() == sqf::type::NOTHING)
	{
		vm->err() << "callUnary could not receive a value for right arg." << std::endl;
		return;
	}
	auto cmd = sqf::commandmap::find(mcmds, right->dtype()); 
	if (cmd)
	{
        value dummy;
		auto val = cmd->execute(vm, dummy, *right);
		if (val)
			vm->active_vmstack()->pushval(val);
	}
	else
	{
		vm->err() << "Unknown input type combination. RType:" << sqf::type_str(right->dtype()) << '.' << std::endl;
	}
}

std::string sqf::inst::callunary::to_string() const
{
	return "CALLUNARY " + (*mcmds->begin())->name();
}
