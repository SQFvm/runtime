#include "arraydata.h"
#include "value.h"
#include <sstream>
#include <algorithm>

std::string sqf::arraydata::tosqf(void) const
{
	std::stringstream sstream;
	sstream << '[';
	bool first = true;
	for (auto it : mvalue)
	{
		if (first)
		{
			first = false;
		}
		else
		{
			sstream << ", ";
		}
		sstream << it->tosqf();
	}
	sstream << ']';
	return sstream.str();
}

bool sqf::arraydata::equals(std::shared_ptr<data> d) const
{
	auto data = std::dynamic_pointer_cast<arraydata>(d);
	if (mvalue.size() != data->size())
	{
		return false;
	}
	for (size_t i = 0; i < mvalue.size(); i++)
	{
		if (!mvalue[i]->equals(data->at(i)))
		{
			return false;
		}
	}
	return true;
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

void sqf::arraydata::extend(std::vector<std::shared_ptr<value>> other)
{
	mvalue.reserve(mvalue.size() + std::distance(other.begin(), other.end()));
	mvalue.insert(mvalue.end(), other.begin(), other.end());
}

void sqf::arraydata::delete_at(int position)
{
	mvalue.erase(mvalue.begin() + position);
}
