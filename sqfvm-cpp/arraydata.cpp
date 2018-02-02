#include "full.h"
#include <sstream>

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
	std::wstringstream sstream;
	sstream << L'[';
	bool first = true;
	for each (auto it in mvalue)
	{
		if (first)
		{
			first = false;
		}
		else
		{
			sstream << L", ";
		}
		sstream << it->to_string();
	}
	sstream << L']';
	return sstream.str();
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
