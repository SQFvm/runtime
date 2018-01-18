#include "full.h"

sqf::arraydata::arraydata()
{
	msize = 0;
}

sqf::arraydata::arraydata(size_t t)
{
	msize = t;
	mvalue.resize(t);
}

sqf::arraydata::arraydata(std::vector<std::shared_ptr<sqf::value>> v)
{
	msize = v.size();
	mvalue = v;
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
	if (index < 0 || index >= (int)mvalue.size())
	{
		return value_s();
	}
	return mvalue[index];
}

sqf::arraydata::operator std::vector<sqf::value_s>(void) const
{
	return mvalue;
}
