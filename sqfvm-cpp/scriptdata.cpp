#include "scriptdata.h"
#include "vmstack.h"

sqf::scriptdata::scriptdata()
{
	mthisstack = std::make_shared<sqf::vmstack>();
}

bool sqf::scriptdata::hasfinished(void)
{
	return mthisstack->isempty();
}
