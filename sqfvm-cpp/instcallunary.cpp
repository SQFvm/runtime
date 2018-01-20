#include "full.h"

void sqf::inst::callunary::execute(const virtualmachine* vm) const
{
	bool flag;
	auto right = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf(L"ASS") << L"callUnary could not receive a value for right arg." << std::endl;
		return;
	}
	auto cmd = sqf::commandmap::find(mcmds, right->get_valuetype());
	if (cmd.get())
	{
		cmd->execute(vm, value_s(), right);
	}
	else
	{
		vm->err() << dbginf(L"ERR") << L"Unknown input type combination. RType:" << right->get_valuetype() << L'.' << std::endl;
	}
}
