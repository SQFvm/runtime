#include "arraydata.h"
#include "value.h"
#include "virtualmachine.h"
#include "type.h"
#include <sstream>
#include <algorithm>

std::string sqf::arraydata::tosqf() const
{
	std::stringstream sstream;
	sstream << '[';
	bool first = true;
	for (auto& it : mvalue)
	{
		if (first)
		{
			first = false;
		}
		else
		{
			sstream << ',';
		}
		sstream << ((it != nullptr) ? it->tosqf() : "nil");
	}
	sstream << ']';
	return sstream.str();
}

bool sqf::arraydata::equals(std::shared_ptr<data> d) const
{
	auto data = std::dynamic_pointer_cast<arraydata>(d);
	if (!data) return false;
	if (mvalue.size() != data->size())
	{
		return false;
	}

	return std::equal(mvalue.begin(),mvalue.end(),data->begin(),data->end(), [](const std::shared_ptr<value>& left, const std::shared_ptr<value>& right) {
		if (left->dtype() == type::STRING && left->dtype() == right->dtype())
			return left->as_string() == right->as_string();
		return left->equals(right);
	});
}

void sqf::arraydata::resize(size_t newsize)
{
	auto cursize = mvalue.size();
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

std::array<double, 3> sqf::arraydata::as_vec3() const
{
	return std::array<double, 3> {at(0)->as_double(), at(1)->as_double(), at(2)->as_double()};
}

std::array<double, 2> sqf::arraydata::as_vec2() const
{
	return std::array<double, 2> {at(0)->as_double(), at(1)->as_double()};
}

bool sqf::arraydata::check_type(virtualmachine * vm, type t, size_t min, size_t max) const
{
	bool errflag = true;
	if (size() < min || size() > max)
	{
		if (min == max)
		{
			vm->err() << "Array was expected to have " << min << " elements of type " << type_str(t) << ". Got " << size() << '.' << std::endl;
		}
		else
		{
			vm->err() << "Array was expected to have " << min << " to " << max << " elements of type " << type_str(t) << ". Got " << size() << '.' << std::endl;
		}
		return false;
	}
	for (size_t i = 0; i < size(); i++)
	{
		if (at(i)->dtype() != t)
		{
			vm->err() << "Element " << i << " of array was expected to be of type " << type_str(t) << ". Got " << type_str(at(i)->dtype()) << '.' << std::endl;
			errflag = false;
		}
	}
	return errflag;
}
bool sqf::arraydata::check_type(virtualmachine * vm, const sqf::type * arr, size_t len) const
{
	bool errflag = true;
	if (size() != len)
	{
		vm->err() << "Array was expected to have " << len << " elements of type combination { ";
		for (size_t i = 0; i < len; i++)
		{
			if (i > 0)
			{
				vm->err() << ", ";
			}
			vm->err() << type_str(arr[i]);
		}
		vm->err() << " }. Got " << size() << '.' << std::endl;
		return false;
	}
	for (size_t i = 0; i < len; i++)
	{
		if (at(i)->dtype() != arr[i])
		{
			vm->err() << "Element " << i << " of array was expected to be of type " << type_str(arr[i]) << ". Got " << type_str(at(i)->dtype()) << '.' << std::endl;
			errflag = false;
		}
	}
	return errflag;
}
bool sqf::arraydata::check_type(virtualmachine * vm, const sqf::type * arr, size_t len, size_t optionalstart) const
{
	bool errflag = true;
	if (size() != len && size() < optionalstart)
	{
		vm->err() << "Array was expected to have at least " << optionalstart << " elements with a maximum of " << len << " and the type combination { ";
		for (size_t i = 0; i < len; i++)
		{
			if (i > 0)
			{
				vm->err() << ", ";
			}
			vm->err() << type_str(arr[i]);
		}
		vm->err() << " }. Got " << size() << '.' << std::endl;
		return false;
	}
	for (size_t i = 0; i < size(); i++)
	{
		if (at(i)->dtype() != arr[i])
		{
			vm->err() << "Element " << i << " of array was expected to be of type " << type_str(arr[i]) << ". Got " << type_str(at(i)->dtype()) << '.' << std::endl;
			errflag = false;
		}
	}
	return errflag;
}

bool sqf::arraydata::recursion_test_helper(std::vector<std::shared_ptr<arraydata>>& visited)
{
	for (auto& it : this->mvalue)
	{
		if (it->dtype() == type::ARRAY)
		{
			auto arr = it->data<arraydata>();
			if (std::find(visited.begin(), visited.end(), arr) != visited.end())
			{
				return false;
			}
			visited.push_back(arr);
			if (!arr->recursion_test_helper(visited))
			{
				return false;
			}
		}
	}
	return true;
}


bool sqf::arraydata::get(size_t index, bool defval)
{
	if (size() <= index || at(index)->dtype() != BOOL)
		return defval;
	return at(index)->as_bool();
}
float sqf::arraydata::get(size_t index, float defval)
{
	if (size() <= index || at(index)->dtype() != SCALAR)
		return defval;
	return at(index)->as_float();
}
std::string sqf::arraydata::get(size_t index, std::string defval)
{
	if (size() <= index || at(index)->dtype() != STRING)
		return defval;
	return at(index)->as_string();
}