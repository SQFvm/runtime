#include "scriptdata.h"
#include "vmstack.h"
#include "callstack.h"

sqf::scriptdata::scriptdata()
{
	mthisstack = std::make_shared<sqf::vmstack>(true);
	auto cs = std::make_shared<callstack>();
	cs->set_variable("_thisScript", this);
	mthisstack->push_back(cs);
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
