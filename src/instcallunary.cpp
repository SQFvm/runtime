#include "instcallunary.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "value.h"

void sqf::inst::callunary::execute(virtualmachine* vm) const
{
	bool flag;
	auto right = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf("ASS") << "callUnary could not receive a value for right arg." << std::endl;
		return;
	}
	auto cmd = sqf::commandmap::find(mcmds, right->dtype()); 
	if (cmd.get())
	{
		auto val = cmd->execute(vm, std::shared_ptr<value>(), right);
		if (val.get())
			vm->stack()->pushval(val);
	}
	else
	{
		vm->err() << dbginf("ERR") << "Unknown input type combination. RType:" << sqf::type_str(right->dtype()) << '.' << std::endl;
	}
}
