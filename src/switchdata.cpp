#include "switchdata.h"
#include "value.h"

std::string sqf::switchdata::tosqf(void) const
{
	return mvalue->tosqf();
}
