#include "full.h"

sqf::arraydata::arraydata()
{
}

sqf::arraydata::arraydata(std::vector<std::shared_ptr<sqf::value>>)
{
}

std::wstring sqf::arraydata::to_string(void) const
{
	return std::wstring();
}

std::shared_ptr<sqf::value>& sqf::arraydata::operator[](int index)
{
	return mvalue.at(index);
}

std::shared_ptr<sqf::value> sqf::arraydata::operator[](int index) const
{
	return mvalue[index];
}

sqf::arraydata::operator std::vector<sqf::value_s>(void) const
{
	return mvalue;
}
