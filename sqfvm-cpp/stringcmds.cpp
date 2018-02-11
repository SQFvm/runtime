#include "full.h"
#include <algorithm>

using namespace sqf;
namespace
{
	value_s count_string(const virtualmachine* vm, value_s right)
	{
		auto r = right->as_string();
		return std::make_shared<value>(r.length());
	}
	value_s toupper_string(const virtualmachine* vm, value_s right)
	{
		auto r = right->as_string();
		std::transform(r.begin(), r.end(), r.begin(), ::toupper);
		return std::make_shared<value>(r);
	}
	value_s tolower_string(const virtualmachine* vm, value_s right)
	{
		auto r = right->as_string();
		std::transform(r.begin(), r.end(), r.begin(), ::toupper);
		return std::make_shared<value>(r);
	}
	value_s select_string_array(const virtualmachine* vm, value_s left, value_s right)
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
}
void sqf::commandmap::initstringcmds(void)
{
	add(unary(L"count", sqf::type::STRING, L"Can be used to count: the number of characters in a string.", count_string));
	add(unary(L"toLower", sqf::type::STRING, L"Converts the supplied string to all lowercase characters.", tolower_string));
	add(unary(L"toUpper", sqf::type::STRING, L"Converts the supplied string to all uppercase characters.", toupper_string));
	add(binary(4, L"select", type::STRING, type::ARRAY, L"Selects a range of characters in provided string, starting at element 0 index, ending at either end of the string or the provided element 1 length.", select_string_array));
}
