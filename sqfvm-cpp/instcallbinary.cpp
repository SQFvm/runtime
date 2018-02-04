#include "full.h"

void sqf::inst::callbinary::execute(const virtualmachine* vm) const
{
	bool flag;
	auto left = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf(L"ASS") << L"callBinary could not receive a value for left arg." << std::endl;
		return;
	}
	auto right = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf(L"ASS") << L"callBinary could not receive a value for right arg." << std::endl;
		return;
	}
	auto cmd = sqf::commandmap::find(mcmds, left->dtype(), right->dtype());
	if (cmd.get())
	{
		auto val = cmd->execute(vm, left, right);
		if (val.get())
			vm->stack()->pushval(val);
	}
	else
	{
		vm->err() << dbginf(L"ERR") << L"Unknown input type combination. LType:" << sqf::type_str(left->dtype()) << L", RType: " << sqf::type_str(right->dtype()) << L'.' << std::endl;
	}
}
