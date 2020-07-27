#pragma once
#include "../fileio.h"

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
		sqf::runtime::fileio::pathinfo path;
		std::string code_segment;

		bool operator==(const diag_info& b) const { return line == b.line && column == b.column && offset == b.offset && path == b.path && code_segment == b.code_segment; }
		bool operator!=(const diag_info& b) const { return line != b.line || column != b.column || offset != b.offset || path != b.path || code_segment != b.code_segment; }

		diag_info() = default;

		diag_info(size_t fline, size_t fcolumn, size_t foffset, sqf::runtime::fileio::pathinfo fpath, std::string code_segment) :
			line(fline), column(fcolumn), offset(foffset), path(fpath), code_segment(code_segment)
		{
		}

		operator sqf::runtime::fileio::pathinfo() const { return path; }
	};
}