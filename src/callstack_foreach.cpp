#include "callstack_foreach.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "value.h"



std::shared_ptr<sqf::instruction> sqf::callstack_foreach::popinst(sqf::virtualmachine * vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (ret)
		return ret;
	if (mCurIndex >= marr->size())
		return ret;

	bool success;
	vm->stack()->popval(success);
	auto sptr = std::shared_ptr<callstack_foreach>(this, [](callstack_foreach*) {});
	mexec->loadinto(vm->stack(), sptr);
	sptr->setvar("_x", marr->at(mCurIndex));
	sptr->setvar("_forEachIndex", std::make_shared<value>(mCurIndex));
	mCurIndex++;
	return sqf::callstack::popinst(vm);
}
