#include "full.h"

void sqf::inst::callbinary::execute(const virtualmachine* vm) const
{
	bool flag;
	auto left = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf(L"ASS") << "callBinary could not receive a value for left arg." << std::endl;
		return;
	}
	auto right = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf(L"ASS") << "callBinary could not receive a value for right arg." << std::endl;
		return;
	}
	mcmd->execute(vm, left, right);
}
