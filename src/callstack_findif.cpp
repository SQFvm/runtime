#include "callstack_findif.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "value.h"
#include "instpush.h"


std::shared_ptr<sqf::instruction> sqf::callstack_findif::popinst(sqf::virtualmachine * vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (ret.get())
		return ret;
	if (mCurIndex > 0)
	{
		bool success;
		auto val = vm->stack()->popval(success);
		if (success && val->dtype() == sqf::BOOL)
		{
			if (val->as_bool())
			{
				auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(mCurIndex - 1));
				vm->stack()->dropcallstack();
				return inst;
			}
		}
		else
		{
			vm->err() << "Missing result for count." << std::endl;
		}
	}
	if (mCurIndex >= marr->size())
	{
		auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(-1));
		vm->stack()->dropcallstack();
		return inst;
	}
	else
	{
		auto sptr = std::shared_ptr<callstack_findif>(this, [](callstack_findif*) {});
		mexec->loadinto(vm->stack(), sptr);
		sptr->setvar("_x", marr->at(mCurIndex));
		mCurIndex++;
		return sqf::callstack::popinst(vm);
	}
}
