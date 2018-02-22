#include "instmakearray.h"
#include <vector>
#include <memory>
#include "value.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "sqfnamespace.h"

void sqf::inst::makearray::execute(virtualmachine* vm) const
{
	auto vec = std::vector<std::shared_ptr<value>>(msize);
	for (size_t i = msize - 1; i != (size_t)~0; i--)
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