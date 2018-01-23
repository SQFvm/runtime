#include "full.h"

void sqf::inst::makearray::execute(const virtualmachine * vm) const
{
	auto vec = std::vector<value_s>(msize);
	for (size_t i = msize - 1; i != ~0; i--)
	{
		bool flag;
		vec[i] = vm->stack()->popval(flag);
		if (!flag)
		{
			vm->err() << dbginf(L"ERR") << L"Missing elements on value stack. Expected " << msize << L", got " << msize - i << std::endl;
			break;
		}
	}
	vm->stack()->pushval(std::make_shared<value>(vec));
}