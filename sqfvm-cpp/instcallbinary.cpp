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
	auto cmd = sqf::commandmap::find(mcmds, left->get_valuetype(), right->get_valuetype());
	if (cmd.get())
	{
		cmd->execute(vm, value_s(), right);
	}
	else
	{
		vm->err() << dbginf(L"ERR") << L"Unknown input type combination. LType:" << right->get_valuetype() << L", RType: " << left->get_valuetype() << L'.' << std::endl;
	}
}
