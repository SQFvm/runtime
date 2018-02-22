#include "callstack_apply.h"
#include "codedata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"

std::shared_ptr<sqf::instruction> sqf::callstack_apply::popinst(sqf::virtualmachine * vm)
{
	if (mcurindex == 0 && marr.size() > 0)
	{
		setvar(L"_x", marr[mcurindex]);
		mcurindex++;
		auto sptr = std::shared_ptr<callstack_apply>(this, [](callstack_apply*) {});
		mapply->loadinto(vm->stack(), sptr);
		return sqf::callstack::popinst(vm);
	}
	else
	{
		auto ret = sqf::callstack::popinst(vm);
		if (ret.get() || mend)
			return ret;
		bool success;
		auto val = vm->stack()->popval(success);
		moutarr[mcurindex - 1] = val;

		if (marr.size() <= mcurindex)
		{
			pushinst(std::make_shared<sqf::inst::endstatement>());
			pushinst(std::make_shared<sqf::inst::push>(std::make_shared<value>(moutarr)));
			mend = true;
			return sqf::callstack::popinst(vm);
		}
		setvar(L"_x", marr[mcurindex]);
		mcurindex++;

		auto sptr = std::shared_ptr<callstack_apply>(this, [](callstack_apply*) {});
		mapply->loadinto(vm->stack(), sptr);
		return sqf::callstack::popinst(vm);
	}
}
