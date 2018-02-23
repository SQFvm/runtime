#include "callstack_while.h"
#include "codedata.h"
#include "virtualmachine.h"
#include "value.h"
#include "instpush.h"
#include "instendstatement.h"

std::shared_ptr<sqf::instruction> sqf::callstack_while::popinst(sqf::virtualmachine* vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (ret.get())
		return ret;
	if (mdoexec)
	{
		mdoexec = false;
		bool success;
		auto val = vm->stack()->popval(success);

		if (success && val->dtype() == type::BOOL && val->as_bool())
		{
			auto sptr = std::shared_ptr<callstack_while>(this, [](callstack_while*){});
			mexec->loadinto(vm->stack(), sptr);
			return popinst(vm);
		}
		else
		{
			return ret;
		}
	}
	else
	{
		mdoexec = true;
		auto sptr = std::shared_ptr<callstack_while>(this, [](callstack_while*) {});
		mwhilecond->loadinto(vm->stack(), sptr);
		return std::make_shared<sqf::inst::endstatement>();
	}
}