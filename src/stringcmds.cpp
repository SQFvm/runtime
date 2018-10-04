#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include <algorithm>
#include <sstream>

using namespace sqf;
namespace
{
	std::shared_ptr<value> count_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		return std::make_shared<value>(r.length());
	}
	std::shared_ptr<value> toupper_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		std::transform(r.begin(), r.end(), r.begin(), ::toupper);
		return std::make_shared<value>(r);
	}
	std::shared_ptr<value> tolower_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		std::transform(r.begin(), r.end(), r.begin(), ::toupper);
		return std::make_shared<value>(r);
	}
	std::shared_ptr<value> select_string_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto str = left->as_string();
		auto arr = right->as_vector();
		if (arr.size() < 1)
		{
			vm->err() << "Array was expected to have at least a single element." << std::endl;
			return std::make_shared<value>();
		}
		if (arr[0]->dtype() != type::SCALAR)
		{
			vm->err() << "First element of array was expected to be SCALAR, got " << sqf::type_str(arr[0]->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		int start = arr[0]->as_int();
		if (start < 0)
		{
			vm->wrn() << "Start index is smaller then 0. Returning empty string." << std::endl;
			return std::make_shared<value>("");
		}
		if (start >(int)str.length())
		{
			vm->wrn() << "Start index is larger then string length. Returning empty string." << std::endl;
			return std::make_shared<value>("");
		}
		if (arr.size() >= 2)
		{
			if (arr[1]->dtype() != type::SCALAR)
			{
				vm->err() << "Second element of array was expected to be SCALAR, got " << sqf::type_str(arr[0]->dtype()) << '.' << std::endl;
				return std::make_shared<value>();
			}
			int length = arr[1]->as_int();
			if (length < 0)
			{
				vm->wrn() << "Length is smaller then 0. Returning empty string." << std::endl;
				return std::make_shared<value>("");
			}
			return std::make_shared<value>(str.substr(start, length));
		}
		return std::make_shared<value>(str.substr(start));
	}
	std::shared_ptr<value> format_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_vector();
		if (r.size() == 0)
		{
			vm->wrn() << "Empty array passed." << std::endl;
			return std::make_shared<value>("");
		}
		if (r[0]->dtype() != type::STRING)
		{
			vm->wrn() << "First element of array was expected to be of type STRING." << std::endl;
			return std::make_shared<value>("");
		}
		auto format = r[0]->as_string();
		std::stringstream sstream;
		size_t off = 0;
		size_t newoff = 0;
		while ((newoff = format.find(L'%', off)) != std::string::npos)
		{
			sstream << format.substr(off, newoff - off);
			newoff++;

			if (!(format[newoff] >= '0' && format[newoff] <= '9'))
			{
				vm->wrn() << '\'' << format[newoff] << "' is no valid placeholder at string index " << newoff << '.' << std::endl;
				newoff++;
			}
			else
			{
				auto num = std::stoi(format.substr(newoff));
				if (num <= 0)
				{
					vm->wrn() << "Invalid placeholder index " << num << " provided at string index " << newoff << '.' << std::endl;
				}
				else if(num >= (int)r.size())
				{
					vm->wrn() << "Placeholder index " << num << " provided at string index " << newoff << " is out of range." << std::endl;
				}
				else
				{
					sstream << r[num]->tosqf();
				}
				while (format[newoff] >= '0' && format[newoff] <= '9') newoff++;
			}
			off = newoff;
		}
		sstream << format.substr(off);
		return std::make_shared<value>(sstream.str());
	}
	std::shared_ptr<value> toarray_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		auto arr = std::vector<std::shared_ptr<value>>(r.size());
		for (size_t i = 0; i < r.size(); i++)
		{
			arr[i] = std::make_shared<value>((int)(r[i]));
		}
		return std::make_shared<value>(arr);
	}
	std::shared_ptr<value> tostring_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_vector();
		std::stringstream sstream;
		for (size_t i = 0; i < r.size(); i++)
		{
			auto& val = r[i];
			if (val->dtype() == SCALAR)
			{
				sstream << (char)val->as_int();
			}
			else
			{
				vm->err() << "Element " << i << " of input array was not of type SCALAR. Got " << sqf::type_str(val->dtype()) << '.' << std::endl;
			}
		}
		return std::make_shared<value>(sstream.str());
	}
	std::shared_ptr<value> joinstring_array_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_vector();
		auto r = right->as_string();
		std::stringstream sstream;
		bool separator = false;
		for (auto it : l)
		{
			if (separator)
			{
				if (it->dtype() == sqf::type::STRING)
				{
					sstream << r << it->as_string();
				}
				else
				{
					sstream << r << it->tosqf();
				}
			}
			else
			{
				separator = true;
				if (it->dtype() == sqf::type::STRING)
				{
					sstream << it->as_string();
				}
				else
				{
					sstream << it->tosqf();
				}
			}
		}
		return std::make_shared<value>(sstream.str());
	}
	std::shared_ptr<value> plus_string_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_string();
		auto r = right->as_string();
		std::stringstream sstream;
		sstream << l << r;
		return std::make_shared<value>(sstream.str());
	}
	std::shared_ptr<value> find_string_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_string();
		auto r = right->as_string();
		int res = l.find(r);
		return std::make_shared<value>(res == std::string::npos ? -1 : res);
	}
}
void sqf::commandmap::initstringcmds()
{
	add(unary("count", sqf::type::STRING, "Can be used to count: the number of characters in a string.", count_string));
	add(unary("toLower", sqf::type::STRING, "Converts the supplied string to all lowercase characters.", tolower_string));
	add(unary("toUpper", sqf::type::STRING, "Converts the supplied string to all uppercase characters.", toupper_string));
	add(binary(4, "select", type::STRING, type::ARRAY, "Selects a range of characters in provided string, starting at element 0 index, ending at either end of the string or the provided element 1 length.", select_string_array));
	add(unary("format", sqf::type::ARRAY, "Composes a string containing other variables or other variable types. Converts any variable type to a string.", format_string));
	add(unary("toArray", sqf::type::STRING, "Converts the supplied String into an Array of Numbers.", toarray_string));
	add(unary("toString", sqf::type::ARRAY, "Converts the supplied String into an Array of Numbers.", tostring_array));
	add(binary(4, "joinString", sqf::type::ARRAY, sqf::type::STRING, "Joins array into String with provided separator. Array can be of mixed types, all elements will be converted to String prior to joining, but the fastest operation is on the array of Strings.", joinstring_array_string));
	add(binary(6, "+", sqf::type::STRING, sqf::type::STRING, "Concatinates two strings together.", plus_string_string));
	add(binary(4, "find", sqf::type::STRING, sqf::type::STRING, "Searches for a string within a string. Returns the 0 based index on success or -1 if not found.", find_string_string));
}
