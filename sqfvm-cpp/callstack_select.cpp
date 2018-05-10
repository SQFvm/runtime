#include "callstack_select.h"
#include "codedata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"

std::shared_ptr<sqf::instruction> sqf::callstack_select::popinst(sqf::virtualmachine * vm)
{
	if (mcurindex == 0 && marr.size() > 0)
	{
		setvar("_x", marr[mcurindex]);
		mcurindex++;
		auto sptr = std::shared_ptr<callstack_select>(this, [](callstack_select*) {});
		mcond->loadinto(vm->stack(), sptr);
		return sqf::callstack::popinst(vm);
	}
	else
	{
		auto ret = sqf::callstack::popinst(vm);
		if (ret.get() || mend)
			return ret;
		bool success;
		auto val = vm->stack()->popval(success);
		if (success && val->dtype() == type::BOOL && val->as_bool())
		{
			moutarr.push_back(marr[mcurindex - 1]);
		}

		if (marr.size() <= mcurindex)
		{
			pushinst(std::make_shared<sqf::inst::endstatement>());
			pushinst(std::make_shared<sqf::inst::push>(std::make_shared<value>(moutarr)));
			mend = true;
			return sqf::callstack::popinst(vm);
		}
		setvar("_x", marr[mcurindex]);
		mcurindex++;

		auto sptr = std::shared_ptr<callstack_select>(this, [](callstack_select*) {});
		mcond->loadinto(vm->stack(), sptr);
		return sqf::callstack::popinst(vm);
	}
}