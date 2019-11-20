#include "scriptdata.h"
#include "vmstack.h"

sqf::scriptdata::scriptdata()
{
	mthisstack = std::make_shared<sqf::vmstack>(true);
}

bool sqf::scriptdata::hasfinished() const
{
	return mthisstack->empty();
}

std::string sqf::scriptdata::tosqf() const
{
	if (this->mthisstack->script_name().empty())
	{
		return "SCRIPT";
	}
	else
	{
		return this->mthisstack->script_name();
	}
}
