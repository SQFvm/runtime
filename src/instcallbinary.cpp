#include "instcallbinary.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "value.h"

void sqf::inst::callbinary::execute(virtualmachine* vm) const
{
	bool flag;
	auto right = vm->active_vmstack()->pop_back_value(flag);
	if (!flag ||right.dtype() == sqf::type::NOTHING)
	{
		if (vm->active_vmstack()->scheduled())
		{
			vm->err() << "callBinary could not receive a value for right arg." << std::endl;
		}
		else
		{
			vm->wrn() << "callBinary could not receive a value for right arg." << std::endl;
		}
		return;
	}
	auto left = vm->active_vmstack()->pop_back_value(flag);
	if (!flag || left.dtype() == sqf::type::NOTHING)
	{
		if (vm->active_vmstack()->scheduled())
		{
			vm->err() << "callBinary could not receive a value for left arg." << std::endl;
		}
		else
		{
			vm->wrn() << "callBinary could not receive a value for left arg." << std::endl;
		}
		return;
	}
	auto cmd = sqf::commandmap::find(mcmds, left.dtype(), right.dtype());
	if (cmd)
	{
		auto val = cmd->execute(vm, left, right);
		vm->active_vmstack()->push_back(val);
	}
	else
	{
		vm->err() << "Unknown input type combination. LType:" << sqf::type_str(left.dtype()) << ", RType: " << sqf::type_str(right.dtype()) << '.' << std::endl;
	}
}

std::string sqf::inst::callbinary::to_string() const
{
	return "CALLBINARY " + (*mcmds->begin())->name();
}
