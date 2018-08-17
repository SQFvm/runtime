#include "callstack_foreach.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "value.h"

sqf::callstack_foreach::callstack_foreach(std::shared_ptr<codedata> exec, std::shared_ptr<arraydata> arr) : mexec(exec), marr(arr), mCurIndex(0)
{
}

std::shared_ptr<sqf::instruction> sqf::callstack_foreach::popinst(sqf::virtualmachine * vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (ret.get())
		return ret;
	if (mCurIndex >= marr->size())
	{
		return ret;
	}
	else
	{
		bool success;
		vm->stack()->popval(success);
		auto sptr = std::shared_ptr<callstack_foreach>(this, [](callstack_foreach*) {});
		mexec->loadinto(vm->stack(), sptr);
		sptr->setvar("_x", marr->at(mCurIndex));
		sptr->setvar("_forEachIndex", std::make_shared<value>(mCurIndex));
		mCurIndex++;
		return sqf::callstack::popinst(vm);
	}
	return ret;
}
