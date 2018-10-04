#include "scriptdata.h"
#include "vmstack.h"

sqf::scriptdata::scriptdata()
{
	mthisstack = std::make_shared<sqf::vmstack>(true);
}

bool sqf::scriptdata::hasfinished()
{
	return mthisstack->isempty();
}
