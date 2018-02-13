#include "full.h"
#include <sstream>
#include <algorithm>

std::wstring sqf::arraydata::tosqf(void) const
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
		sstream << it->tosqf();
	}
	sstream << L']';
	return sstream.str();
}

void sqf::arraydata::resize(int newsize)
{
	auto cursize = (int)mvalue.size();
	mvalue.resize(newsize);
	if (newsize > cursize)
	{
		for (; cursize < newsize; cursize++)
		{
			mvalue[cursize] = std::make_shared<sqf::value>();
		}
	}
}

void sqf::arraydata::reverse()
{
	std::reverse(mvalue.begin(), mvalue.end());
}
