#include "callstack_isnil.h"
#include "codedata.h"
#include "virtualmachine.h"
#include "instpush.h"
#include "value.h"

sqf::callstack_isnil::callstack_isnil(std::shared_ptr<sqf::sqfnamespace> ns, sqf::virtualmachine * vm, std::shared_ptr<codedata> exec) : callstack(ns)
{
	auto sptr = std::shared_ptr<callstack_isnil>(this, [](callstack_isnil*) {});
	exec->loadinto(vm->stack(), sptr);
}

std::shared_ptr<sqf::instruction> sqf::callstack_isnil::popinst(sqf::virtualmachine * vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (ret)
		return ret;
	bool success;
	auto val = vm->stack()->popval(success);
	vm->stack()->dropcallstack();
	return std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(!success || val->dtype() == sqf::type::NOTHING));
}
