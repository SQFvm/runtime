#include "full.h"

void sqf::inst::callunary::execute(const virtualmachine* vm, std::shared_ptr<vmstack> stack) const
{
	bool flag;
	auto right = stack->popval(flag);
	if (!flag)
	{
		vm->err() << L"[ASS]" << vm->dbginf() << "callUnary could not receive a value for right arg." << std::endl;
		return;
	}
	//ToDo: finish
}
