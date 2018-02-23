#include "switchdata.h"
#include "value.h"

std::wstring sqf::switchdata::tosqf(void) const
{
	return mvalue->tosqf();
}
