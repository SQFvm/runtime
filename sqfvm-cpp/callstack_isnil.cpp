#include "full.h"

sqf::callstack_isnil::callstack_isnil(const sqf::virtualmachine * vm, std::shared_ptr<codedata> exec)
{
	auto sptr = std::shared_ptr<callstack_isnil>(this, [](callstack_isnil*) {});
	exec->loadinto(vm->stack(), sptr);
}

sqf::instruction_s sqf::callstack_isnil::popinst(const sqf::virtualmachine * vm)
{
	auto ret = sqf::callstack::popinst(vm);
	if (ret.get())
		return ret;
	bool success;
	auto val = vm->stack()->popval(success);
	return std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(!success || val->dtype() == sqf::type::NOTHING));
}
