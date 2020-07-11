#pragma once
#include <string>
#include <string_view>


namespace sqf::runtime::diagnostics
{
	class diag_info
	{
	public:
		size_t line;
		size_t column;
		size_t offset;
		std::string file;
		std::string code_segment;

		bool operator==(const diag_info& b) const { return line == b.line && column == b.column && offset == b.offset && file == b.file && code_segment == b.code_segment; }
		bool operator!=(const diag_info& b) const { return line != b.line || column != b.column || offset != b.offset || file != b.file || code_segment != b.code_segment; }

		diag_info() = default;

		diag_info(size_t fline, size_t fcolumn, size_t foffset, std::string fpath, std::string code_segment) :
			line(fline), column(fcolumn), offset(foffset), file(fpath), code_segment(code_segment)
		{
		}
	};
}