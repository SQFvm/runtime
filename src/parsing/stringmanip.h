#pragma once

#include <string_view>

namespace sqf
{
	namespace parse
	{
		static inline std::string_view ltrim(std::string_view str, std::string_view chars = " \t")
		{
			size_t startpos = str.find_first_not_of(chars);
			return str.substr(startpos);
		}

		static inline std::string_view rtrim(std::string_view str, std::string_view chars = " \t")
		{
			size_t endpos = str.find_last_not_of(chars);
			return str.substr(0, endpos);
		}

		static inline std::string_view trim(std::string_view str, std::string_view chars = " \t")
		{
			return ltrim(rtrim(str, chars), chars);
		}

		static inline std::string_view strip_quotes(std::string_view str, std::string_view quotes = "\"'")
		{
			if (str.length() < 2)
			{
				throw std::invalid_argument("String is too short to contain quotes");
			}
			if(quotes.find(str.front()) == std::string_view::npos)
			{
				throw std::invalid_argument("String does not start with a valid quote character");
			}
			char quote_type = str.front();
			if (str.back() != quote_type)
			{
				throw std::invalid_argument("String does not end with the expected quote character");
			}
			return str.substr(1, str.length() - 2);
		}
    }
}