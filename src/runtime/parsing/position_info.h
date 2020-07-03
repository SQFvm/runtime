#pragma once
#include <string>
#include <string_view>
#include <utility>
namespace sqf
{
	namespace parse
	{
		class position_info
		{
		public:
			size_t line;
			size_t column;
			size_t offset;
			std::string file;

			position_info() : line(0), column(0), offset(0), file("") {}
			position_info(size_t line, size_t column, size_t offset, std::string file) : line(line), column(column), offset(offset), file(std::move(file)) {}
			position_info(size_t line, size_t column, size_t offset, std::string_view file) : line(line), column(column), offset(offset), file(file) {}
		};
	}
}