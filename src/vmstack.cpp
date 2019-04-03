#include "vmstack.h"
#include "value.h"
#include "virtualmachine.h"

void sqf::vmstack::pushinst(sqf::virtualmachine * vm, std::shared_ptr<instruction> inst)
{
	if (mstacks.empty())
	{
		mstacks.push_back(std::make_shared<callstack>(vm->missionnamespace()));
	}
	mstacks.back()->pushinst(inst);
}

std::shared_ptr<sqf::value> sqf::vmstack::getlocalvar(std::string varname)
{
	for (auto it = stacks_begin(); it != stacks_end(); ++it)
	{
		if (it->get()->containsvar(varname))
		{
			return it->get()->getvar(varname);
		}
	}
	return std::make_shared<sqf::value>();
}

void sqf::vmstack::sleep(std::chrono::milliseconds ms)
{
	mwakeupstamp = sqf::virtualmachine::system_time() + ms;
	misasleep = true;
}
