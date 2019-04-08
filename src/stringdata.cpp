#include "stringdata.h"
#include <stdexcept>
#include <vector>
#include <algorithm>

std::string sqf::stringdata::parse_from_sqf(std::string_view s)
{
	if (s.length() == 0)
		return static_cast<std::string>(s);
	size_t count = 0;
	char start = s[0];
	if (start != '"' && start != '\'')
		return static_cast<std::string>(s);
	if (s.length() == 2)
		return "";

    std::vector<char> arr;
    arr.reserve(s.size());
	for (size_t i = 1; i < s.length() - 1; i++)
	{
		char c = s[i];
		if (c == start && s[i + 1] == start) //quote type
		{
			i++;
		}
        arr.emplace_back(c);
	}
	return std::string(arr.data(), arr.size());
}

std::string sqf::stringdata::parse_to_sqf(std::string_view s)
{
	size_t count = std::count(s.begin(), s.end(), '"') + 2;
	size_t size = s.length() + count;
    std::vector<char> arr;
    arr.reserve(size);

    arr.emplace_back('"');//starting quote

	for (char c : s)
	{
        arr.emplace_back(c);
		if (c == '"')
            arr.emplace_back(c);
	}
    arr.emplace_back('"');//ending quote
	return std::string(arr.data(), arr.size());
}
