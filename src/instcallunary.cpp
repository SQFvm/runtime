#include "instcallunary.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "value.h"

void sqf::inst::callunary::execute(virtualmachine* vm) const
{
	bool flag;
	auto right = vm->stack()->popval(flag);
	if (!flag || right == nullptr || right->dtype() == sqf::type::NOTHING)
	{
		vm->err() << "callUnary could not receive a value for right arg." << std::endl;
		return;
	}
	auto cmd = sqf::commandmap::find(mcmds, right->dtype()); 
	if (cmd)
	{
		auto val = cmd->execute(vm, std::shared_ptr<value>(), right);
		if (val)
			vm->stack()->pushval(val);
	}
	else
	{
		vm->err() << "Unknown input type combination. RType:" << sqf::type_str(right->dtype()) << '.' << std::endl;
	}
}

std::string sqf::inst::callunary::to_string() const
{
	return "CALLUNARY " + (*mcmds->begin())->name();
}
