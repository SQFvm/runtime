#include "stringdata.h"

std::string sqf::stringdata::parse_from_sqf(std::string s)
{
	if (s.length() == 0)
		return s;
	size_t count = 0;
	char start = s[0];
	if (start != '"' && start != '\'')
		return s;
	if (s.length() == 2)
		return "";
	for (size_t i = 0; i < s.length(); i++)
	{
		char c = s[i];
		if (c == start && s[i + 1] == start)
		{
			count++;
		}
	}
	size_t size = s.length() - count - 2;
	size_t index = 0;
	auto arr = std::make_unique<char[]>(size);
	for (size_t i = 1; i < s.length() - 1; i++)
	{
		char c = s[i];
		if (c == start && s[i + 1] == start)
		{
			i++;
		}
		arr[index] = c;
		index++;
	}
	return std::string(arr.get(), size);
}

std::string sqf::stringdata::parse_to_sqf(std::string s)
{
	size_t count = 2;
	for (size_t i = 0; i < s.length(); i++)
	{
		char c = s[i];
		if (c == '"')
			count++;
	}
	size_t size = s.length() + count;
	auto arr = std::make_unique<char[]>(size);
	size_t index = 1;
	arr[0] = '"';
	arr[size - 1] = '"';
	for (size_t i = 0; i < s.length(); i++)
	{
		char c = s[i];
		arr[index] = c;
		index++;
		if (c == '"')
		{
			arr[index] = c;
			index++;
		}
	}
	return std::string(arr.get(), size);
}
