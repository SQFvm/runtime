#include "instcallbinary.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "value.h"

void sqf::inst::callbinary::execute(virtualmachine* vm) const
{
	bool flag;
	auto right = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf("ASS") << "callBinary could not receive a value for right arg." << std::endl;
		return;
	}
	auto left = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf("ASS") << "callBinary could not receive a value for left arg." << std::endl;
		return;
	}
	auto cmd = sqf::commandmap::find(mcmds, left->dtype(), right->dtype());
	if (cmd.get())
	{
		if (left->dtype() == sqf::type::NOTHING)
		{
			vm->wrn() << dbginf("ASS") << "callBinary could not receive a value for left arg." << std::endl;
		}
		auto val = cmd->execute(vm, left, right);
		if (val.get())
			vm->stack()->pushval(val);
	}
	else
	{
		vm->err() << dbginf("ERR") << "Unknown input type combination. LType:" << sqf::type_str(left->dtype()) << ", RType: " << sqf::type_str(right->dtype()) << '.' << std::endl;
	}
}
