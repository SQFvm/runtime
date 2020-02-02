#pragma once
#include <string>
namespace sqf
{
	namespace parse
	{
		struct position_info
		{
			size_t line;
			size_t column;
			size_t offset;
			std::string file;
		};
	}
}