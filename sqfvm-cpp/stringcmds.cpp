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
}
void sqf::commandmap::initstringcmds(void)
{
	add(unary(L"count", sqf::type::STRING, L"Can be used to count: the number of characters in a string.", count_string));
	add(unary(L"toLower", sqf::type::STRING, L"Converts the supplied string to all lowercase characters.", tolower_string));
	add(unary(L"toUpper", sqf::type::STRING, L"Converts the supplied string to all uppercase characters.", toupper_string));
}
