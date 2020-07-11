#pragma once

#include <string_view>

namespace sqf::runtime::util
{
	inline std::string_view ltrim(std::string_view str, std::string_view chars = " \t")
	{
		size_t startpos = str.find_first_not_of(chars);
		return str.substr(startpos);
	}

	inline std::string_view rtrim(std::string_view str, std::string_view chars = " \t")
	{
		size_t endpos = str.find_last_not_of(chars);
		return str.substr(0, endpos);
	}

	inline std::string_view trim(std::string_view str, std::string_view chars = " \t")
	{
		return ltrim(rtrim(str, chars), chars);
	}

	inline std::string_view strip_quotes(std::string_view str, std::string_view quotes = "\"'")
	{
		if (str.length() < 2)
		{
			return {};
		}
		if (quotes.find(str.front()) == std::string_view::npos)
		{
			return {};
		}
		char quote_type = str.front();
		if (str.back() != quote_type)
		{
			return {};
		}
		return str.substr(1, str.length() - 2);
	}

	static unsigned int constexpr strlen(const char* str)
	{
		return str[0] ? 1 + strlen(str + 1) : 0;
	}
	static unsigned int constexpr wcslen(const wchar_t* str)
	{
		return str[0] ? 1 + wcslen(str + 1) : 0;
	}
	static constexpr double pi()
	{
		return 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665;
	}
}