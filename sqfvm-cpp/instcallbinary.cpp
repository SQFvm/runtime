#include "full.h"

void sqf::inst::callbinary::execute(const virtualmachine* vm, std::shared_ptr<vmstack> stack) const
{
	bool flag;
	auto left = stack->popval(flag);
	if (!flag)
	{
		vm->err() << L"[ASS]" << vm->dbginf() << "callBinary could not receive a value for left arg." << std::endl;
		return;
	}
	auto right = stack->popval(flag);
	if (!flag)
	{
		vm->err() << L"[ASS]" << vm->dbginf() << "callBinary could not receive a value for right arg." << std::endl;
		return;
	}
	//ToDo: finish
}
