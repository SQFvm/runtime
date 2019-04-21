#include "switchdata.h"
#include "value.h"

std::string sqf::switchdata::tosqf() const
{
	return mvalue.tosqf();
}
