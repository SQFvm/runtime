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
			vm->err() << L"Array was expected to have at least a single element." << std::endl;
			return std::make_shared<value>();
		}
		if (arr[0]->dtype() != type::SCALAR)
		{
			vm->err() << L"First element of array was expected to be SCALAR, got " << sqf::type_str(arr[0]->dtype()) << L'.' << std::endl;
			return std::make_shared<value>();
		}
		int start = arr[0]->as_int();
		if (start < 0)
		{
			vm->wrn() << L"Start index is smaller then 0. Returning empty string." << std::endl;
			return std::make_shared<value>(L"");
		}
		if (start >(int)str.length())
		{
			vm->wrn() << L"Start index is larger then string length. Returning empty string." << std::endl;
			return std::make_shared<value>(L"");
		}
		if (arr.size() >= 2)
		{
			if (arr[1]->dtype() != type::SCALAR)
			{
				vm->err() << L"Second element of array was expected to be SCALAR, got " << sqf::type_str(arr[0]->dtype()) << L'.' << std::endl;
				return std::make_shared<value>();
			}
			int length = arr[1]->as_int();
			if (length < 0)
			{
				vm->wrn() << L"Length is smaller then 0. Returning empty string." << std::endl;
				return std::make_shared<value>(L"");
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
			vm->wrn() << L"Empty array passed." << std::endl;
			return std::make_shared<value>(L"");
		}
		if (r[0]->dtype() != type::STRING)
		{
			vm->wrn() << L"First element of array was expected to be of type STRING." << std::endl;
			return std::make_shared<value>(L"");
		}
		auto format = r[0]->as_string();
		auto sstream = std::wstringstream();
		size_t off = 0;
		size_t newoff = 0;
		while ((newoff = format.find(L'%', off)) != std::wstring::npos)
		{
			sstream << format.substr(off, newoff - off);
			newoff++;

			if (!(format[newoff] >= L'0' && format[newoff] <= '9'))
			{
				vm->wrn() << L'\'' << format[newoff] << L"' is no valid placeholder at string index " << newoff << L'.' << std::endl;
				newoff++;
			}
			else
			{
				auto num = std::stoi(format.substr(newoff));
				if (num <= 0)
				{
					vm->wrn() << L"Invalid placeholder index " << num << L" provided at string index " << newoff << L'.' << std::endl;
				}
				else if(num >= (int)r.size())
				{
					vm->wrn() << L"Placeholder index " << num << L" provided at string index " << newoff << L" is out of range." << std::endl;
				}
				else
				{
					sstream << r[num]->tosqf();
				}
				while (format[newoff] >= L'0' && format[newoff] <= '9') newoff++;
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
		/*
		diag_log toString [84];
		diag_log toString [true];
		diag_log toString [false];
		diag_log toString ["something"];
		diag_log toString [{}];
		diag_log toString [[]];
		diag_log str(toArray toString [84]);
		diag_log str(toArray toString [true]);
		diag_log str(toArray toString [false]);
		diag_log str(toArray toString ["something"]);
		diag_log str(toArray toString [{}]);
		diag_log str(toArray toString [[]]);

		19:54:38 "T"
		19:54:38 ""
		19:54:38 ""
		19:54:38 Bad conversion: scalar
		19:54:38 ""
		19:54:38 Bad conversion: scalar
		19:54:38 ""
		19:54:38 Bad conversion: scalar
		19:54:38 ""
		19:54:38 "[84]"
		19:54:38 "[1]"
		19:54:38 "[]"
		19:54:38 Bad conversion: scalar
		19:54:38 "[]"
		19:54:38 Bad conversion: scalar
		19:54:38 "[]"
		19:54:38 Bad conversion: scalar
		19:54:38 "[]"
		*/
		auto r = right->as_vector();
		auto sstream = std::wstringstream();
		for (auto val : r)
		{
			sstream << val->tosqf();
		}
		return std::make_shared<value>(sstream.str());
	}
	std::shared_ptr<value> joinstring_array_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_vector();
		auto r = right->as_string();
		auto sstream = std::wstringstream();
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
		auto sstream = std::wstringstream();
		sstream << l << r;
		return std::make_shared<value>(sstream.str());
	}
}
void sqf::commandmap::initstringcmds(void)
{
	add(unary(L"count", sqf::type::STRING, L"Can be used to count: the number of characters in a string.", count_string));
	add(unary(L"toLower", sqf::type::STRING, L"Converts the supplied string to all lowercase characters.", tolower_string));
	add(unary(L"toUpper", sqf::type::STRING, L"Converts the supplied string to all uppercase characters.", toupper_string));
	add(binary(4, L"select", type::STRING, type::ARRAY, L"Selects a range of characters in provided string, starting at element 0 index, ending at either end of the string or the provided element 1 length.", select_string_array));
	add(unary(L"format", sqf::type::ARRAY, L"Composes a string containing other variables or other variable types. Converts any variable type to a string.", format_string));
	add(unary(L"toArray", sqf::type::STRING, L"Converts the supplied String into an Array of Numbers.", toarray_string));
	add(unary(L"toString", sqf::type::ARRAY, L"Converts the supplied String into an Array of Numbers.", tostring_array));
	add(binary(4, L"joinString", sqf::type::ARRAY, sqf::type::STRING, L"Joins array into String with provided separator. Array can be of mixed types, all elements will be converted to String prior to joining, but the fastest operation is on the array of Strings.", joinstring_array_string));
	add(binary(6, L"+", sqf::type::STRING, sqf::type::STRING, L"Concatinates two strings together.", plus_string_string));
}
