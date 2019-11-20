#include "instmakearray.h"
#include <vector>
#include <memory>
#include "value.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "sqfnamespace.h"

void sqf::inst::makearray::execute(virtualmachine* vm) const
{
	auto vec = std::vector<value>(msize);
	for (size_t i = msize - 1; i != (size_t)~0; i--)
	{
		bool flag;
		vec[i] = vm->active_vmstack()->pop_back_value(flag);
		if (!flag)
		{
			vm->err() << "Missing elements on value stack. Expected " << msize << ", got " << msize - i << std::endl;
			break;
		}
	}
	vm->active_vmstack()->push_back(value(vec));
}