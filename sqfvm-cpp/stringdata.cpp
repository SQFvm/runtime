#include "full.h"

sqf::stringdata::stringdata(void)
{
	mvalue = std::wstring();
	mvalue_parsed = std::wstring();
}

sqf::stringdata::stringdata(std::wstring s)
{
	mvalue = s;
	mvalue_parsed = parse_from_sqf(s);
}

std::wstring sqf::stringdata::to_string(void) const
{
	return mvalue;
}

sqf::stringdata::operator const wchar_t*(void) const
{
	return mvalue_parsed.c_str();
}

sqf::stringdata::operator std::wstring(void) const
{
	return mvalue_parsed;
}

std::wstring sqf::stringdata::parse_from_sqf(std::wstring s)
{
	size_t count = 0;
	wchar_t start = s[0];
	for (size_t i = 0; i < s.length(); i++)
	{
		wchar_t c = s[i];
		if (c == start && s[i + 1] == start)
		{
			count++;
		}
	}
	size_t size = s.length() - count - 2;
	size_t index = 0;
	auto arr = std::make_unique<wchar_t[]>(size);
	for (size_t i = 1; i < s.length() - 1; i++)
	{
		wchar_t c = s[i];
		if (c == start && s[i + 1] == start)
		{
			i++;
		}
		arr[index] = c;
		index++;
	}
	return std::wstring(arr.get(), size);
}

std::wstring sqf::stringdata::parse_to_sqf(std::wstring s)
{
	size_t count = 2;
	for (size_t i = 0; i < s.length(); i++)
	{
		wchar_t c = s[i];
		if (c == L'"')
			count++;
	}
	size_t size = s.length() + count;
	auto arr = std::make_unique<wchar_t[]>(size);
	size_t index = 1;
	arr[0] = L'"';
	arr[size - 1] = L'"';
	for (size_t i = 0; i < s.length(); i++)
	{
		wchar_t c = s[i];
		arr[index] = c;
		index++;
		if (c == L'"')
		{
			arr[index] = c;
			index++;
		}
	}
	return std::wstring(arr.get(), size);
}
