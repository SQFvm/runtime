#include "full.h"
#include <sstream>

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