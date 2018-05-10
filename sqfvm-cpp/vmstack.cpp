#include "vmstack.h"
#include "value.h"

std::shared_ptr<sqf::value> sqf::vmstack::getlocalvar(std::string varname)
{
	for (auto it = stacks_begin(); it != stacks_end(); it++)
	{
		if (it->get()->containsvar(varname))
		{
			return it->get()->getvar(varname);
			break;
		}
	}
	return std::make_shared<sqf::value>();
}
