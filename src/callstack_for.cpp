#include "callstack_for.h"
#include <utility>
#include "fordata.h"
#include "codedata.h"
#include "value.h"
#include "virtualmachine.h"
#include "instpush.h"
#include "instassigntolocal.h"
#include "instendstatement.h"

sqf::callstack_for::callstack_for(std::shared_ptr<sqf::sqfnamespace> ns, std::shared_ptr<fordata> fordata, std::shared_ptr<codedata> exec) 
	: callstack(ns), mfordata(std::move(fordata)), mexec(std::move(exec))
{
	setvar(mfordata->variable(), std::make_shared<value>(mfordata->from()));
}

std::shared_ptr<sqf::instruction> sqf::callstack_for::popinst(sqf::virtualmachine * vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (ret)
		return ret;
	auto val = getvar(mfordata->variable());
	if (   val->dtype() == type::SCALAR
		&& mfordata->step() > 0 
		   ? val->as_double() < mfordata->to()
		   : val->as_double() > mfordata->to()
		|| (mfordata->to() == val->as_double() && mfordata->from() == mfordata->to())
		)
	{
		bool success;
		vm->stack()->popval(success);
		auto sptr = std::shared_ptr<callstack_for>(this, [](callstack_for*) {});
		mexec->loadinto(vm->stack(), sptr);
		pushinst(std::make_shared<sqf::inst::push>(std::make_shared<value>(val->as_double() + mfordata->step())));
		pushinst(std::make_shared<sqf::inst::assigntolocal>(mfordata->variable()));
		pushinst(std::make_shared<sqf::inst::endstatement>());
		return sqf::callstack::popinst(vm);
	}
	return ret;
}
