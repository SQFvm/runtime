#include "callstack_configproperties.h"
#include "codedata.h"
#include "configdata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"

std::shared_ptr<sqf::instruction> sqf::callstack_configproperties::popinst(sqf::virtualmachine * vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (ret.get() || mend)
		return ret;
	if (mcurindex == 0 && mdata->size() > 0)
	{
		auto field = mdata->at(mcurindex);
		auto fieldname = field->data<configdata>()->name();
		mcurindex++;
		if (std::find(mvisited.begin(), mvisited.end(), fieldname) == mvisited.end())
		{
			mvisited.push_back(field->data<configdata>()->name());
			setvar("_x", field);
		}
		else
		{
			return popinst(vm);
		}

		auto sptr = std::shared_ptr<callstack_configproperties>(this, [](callstack_configproperties*) {});
		mcond->loadinto(vm->stack(), sptr);
		return sqf::callstack::popinst(vm);
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
			if (!mdata->actual_parent().empty())
			{
				mdata = mdata->parent()->data<configdata>();
				mcurindex = 0;
				return popinst(vm);
			}
			pushinst(std::make_shared<sqf::inst::endstatement>());
			pushinst(std::make_shared<sqf::inst::push>(std::make_shared<value>(moutarr)));
			mend = true;
			return sqf::callstack::popinst(vm);
		}

		auto field = mdata->at(mcurindex);
		auto fieldname = field->data<configdata>()->name();
		mcurindex++;
		if (std::find(mvisited.begin(), mvisited.end(), fieldname) == mvisited.end())
		{
			mvisited.push_back(field->data<configdata>()->name());
			setvar("_x", field);
		}
		else
		{
			return popinst(vm);
		}

		auto sptr = std::shared_ptr<callstack_configproperties>(this, [](callstack_configproperties*) {});
		mcond->loadinto(vm->stack(), sptr);
		return sqf::callstack::popinst(vm);
	}
}