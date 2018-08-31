#include "arraydata.h"
#include "value.h"
#include "virtualmachine.h"
#include "type.h"
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
		if (mvalue[i]->dtype() == type::STRING && mvalue[i]->dtype() == data->at(i)->dtype())
		{
			if (mvalue[i]->as_string() != data->at(i)->as_string())
			{
				return false;
			}
		} 
		else
		{
			if (!mvalue[i]->equals(data->at(i)))
			{
				return false;
			}
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

std::array<double, 3> sqf::arraydata::as_vec3(void) const
{
	return std::array<double, 3> {at(0)->as_double(), at(1)->as_double(), at(2)->as_double()};
}

std::array<double, 2> sqf::arraydata::as_vec2(void) const
{
	return std::array<double, 2> {at(0)->as_double(), at(1)->as_double()};
}

bool sqf::arraydata::check_type(virtualmachine * vm, type t, size_t len) const
{
	bool errflag = true;
	if (size() != len)
	{
		vm->err() << "Array was expected to have 3 elements of type " << type_str(t) << ". Got " << size() << '.' << std::endl;
		return false;
	}
	for (size_t i = 0; i < len; i++)
	{
		if (at(i)->dtype() != SCALAR)
		{
			vm->err() << "Element " << i << " of array was expected to be of type " << type_str(t) << ". Got " << type_str(at(i)->dtype()) << '.' << std::endl;
			errflag = false;
		}
	}
	return errflag;
}
