#include "full.h"

sqf::stringdata::stringdata(void)
{
	mvalue = std::wstring();
}

sqf::stringdata::stringdata(std::wstring s)
{
	mvalue = s;
}

std::wstring sqf::stringdata::to_string(void) const
{
	return parse_to_sqf(mvalue);
}

sqf::stringdata::operator const wchar_t*(void) const
{
	return mvalue.c_str();
}

sqf::stringdata::operator std::wstring(void) const
{
	return mvalue;
}

std::wstring sqf::stringdata::parse_from_sqf(std::wstring s)
{
	size_t count = 0;
	wchar_t start = s[0];
	for (auto it = s.begin(); it != s.end(); it++)
	{
		wchar_t c = *it;
		if (c == start && it[1] == start)
		{
			count++;
		}
	}
	size_t size = s.length() - count - 2;
	auto arr = std::make_unique<wchar_t[]>(size);
	for (auto it = s.begin() + 1; it != s.end() - 1; it++)
	{
		wchar_t c = *it;
		if (c == start && it[1] == start)
		{
			it++;
		}
	}
	return std::wstring(arr.get(), size);
}

std::wstring sqf::stringdata::parse_to_sqf(std::wstring s)
{
	size_t count = 2;
	for each (auto c in s)
	{
		if (c == '"')
			count++;
	}
	size_t size = s.length() + count;
	auto arr = std::make_unique<wchar_t[]>(size);
	size_t index = 1;
	arr[0] = L'"';
	arr[size - 1] = L'"';
	for each (auto c in s)
	{
		arr[index++] = c;
		if (c == '"')
		{
			arr[index++] = c;
			index++;
		}
	}
	return std::wstring(arr.get(), size);
}
