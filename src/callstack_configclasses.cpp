#include "callstack_configclasses.h"
#include "codedata.h"
#include "configdata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"

std::shared_ptr<sqf::instruction> sqf::callstack_configclasses::popinst(sqf::virtualmachine * vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (ret.get() || mend)
		return ret;

	if (mcurindex == 0 && mdata->size() > 0)
	{
		auto val = mdata->at(mcurindex);
		while (val->data<configdata>()->cfgvalue().get() != nullptr && mdata->size() > mcurindex)
		{
			mcurindex++;
			val = mdata->at(mcurindex);
		}
		if (mdata->size() > mcurindex)
		{
			setvar("_x", val);
			auto sptr = std::shared_ptr<callstack_configclasses>(this, [](callstack_configclasses*) {});
			mcond->loadinto(vm->stack(), sptr);
			return sqf::callstack::popinst(vm);
		}
		else
		{
			pushinst(std::make_shared<sqf::inst::endstatement>());
			pushinst(std::make_shared<sqf::inst::push>(std::make_shared<value>(moutarr)));
			mend = true;
			return sqf::callstack::popinst(vm);
		}
	}
	else
	{
		bool success;
		auto val = vm->stack()->popval(success);
		if (success && val->dtype() == type::BOOL && val->as_bool())
		{
			moutarr.push_back(mdata->at(mcurindex - 1));
		}

		if (mdata->size() <= mcurindex)
		{
			pushinst(std::make_shared<sqf::inst::endstatement>());
			pushinst(std::make_shared<sqf::inst::push>(std::make_shared<value>(moutarr)));
			mend = true;
			return sqf::callstack::popinst(vm);
		}
		while (val->data<configdata>()->cfgvalue().get() != nullptr && mdata->size() > mcurindex)
		{
			mcurindex++;
			val = mdata->at(mcurindex);
		}
		if (mdata->size() > mcurindex)
		{
			setvar("_x", val);
		}
		else
		{
			pushinst(std::make_shared<sqf::inst::endstatement>());
			pushinst(std::make_shared<sqf::inst::push>(std::make_shared<value>(moutarr)));
			mend = true;
			return sqf::callstack::popinst(vm);
		}

		auto sptr = std::shared_ptr<callstack_configclasses>(this, [](callstack_configclasses*) {});
		mcond->loadinto(vm->stack(), sptr);
		return sqf::callstack::popinst(vm);
	}
}