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
	auto cmd = sqf::commandmap::find(mcmds, left->as_type(), right->as_type());
	if (cmd.get())
	{
		auto val = cmd->execute(vm, left, right);
		vm->stack()->pushval(val);
	}
	else
	{
		vm->err() << dbginf(L"ERR") << L"Unknown input type combination. LType:" << right->as_type() << L", RType: " << left->as_type() << L'.' << std::endl;
	}
}
