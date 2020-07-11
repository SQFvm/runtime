#include "instcallunary.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "value.h"

void sqf::inst::call_unary::execute(virtualmachine* vm) const
{
	bool flag;
	auto right = vm->active_vmstack()->pop_back_value(flag);
	if (!flag || right.dtype() == sqf::type::NOTHING)
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
	auto cmd = sqf::commandmap::find(mcmds, right.dtype()); 
	if (cmd)
	{
        value dummy;
		auto val = cmd->execute(vm, dummy, right);
		vm->active_vmstack()->push_back(val);
	}
	else
	{
		vm->logmsg(logmessage::runtime::UnknownInputTypeCombinationBinary(*this, mcmds->front()->name(), right.dtype()));
	}
}

std::string sqf::inst::call_unary::to_string() const
{
	return "CALLUNARY " + (*mcmds->begin())->name();
}
