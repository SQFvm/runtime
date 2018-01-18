#include "full.h"

void sqf::inst::callunary::execute(const virtualmachine* vm) const
{
	bool flag;
	auto right = vm->stack()->popval(flag);
	if (!flag)
	{
		vm->err() << dbginf(L"ASS") << "callUnary could not receive a value for right arg." << std::endl;
		return;
	}
	mcmd->execute(vm, value_s(), right);
}
