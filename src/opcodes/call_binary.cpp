#include "instcallbinary.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "value.h"

void sqf::inst::call_binary::execute(virtualmachine* vm) const
{
	bool flag;
	auto right = vm->active_vmstack()->pop_back_value(flag);
	if (!flag ||right.dtype() == sqf::type::NOTHING)
	{
		if (vm->active_vmstack()->scheduled())
		{
			vm->logmsg(logmessage::runtime::NoValueFoundForRightArgumentWeak(*this));
		}
		else
		{
			vm->logmsg(logmessage::runtime::NoValueFoundForRightArgument(*this));
		}
		return;
	}
	auto left = vm->active_vmstack()->pop_back_value(flag);
	if (!flag || left.dtype() == sqf::type::NOTHING)
	{
		if (vm->active_vmstack()->scheduled())
		{
			vm->logmsg(logmessage::runtime::NoValueFoundForLeftArgumentWeak(*this));
		}
		else
		{
			vm->logmsg(logmessage::runtime::NoValueFoundForLeftArgument(*this));
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
		vm->logmsg(logmessage::runtime::UnknownInputTypeCombinationBinary(*this, left.dtype(), mcmds->front()->name(), right.dtype()));
	}
}

std::string sqf::inst::call_binary::to_string() const
{
	return "CALLBINARY " + (*mcmds->begin())->name();
}
