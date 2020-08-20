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
		size_t file_offset;
		sqf::runtime::fileio::pathinfo path;
		std::string code_segment;

		bool operator==(const diag_info& b) const { return line == b.line && column == b.column && file_offset == b.file_offset && path == b.path && code_segment == b.code_segment; }
		bool operator!=(const diag_info& b) const { return line != b.line || column != b.column || file_offset != b.file_offset || path != b.path || code_segment != b.code_segment; }

		diag_info() : diag_info(0, 0, 0, 0, {}, "") {}
		diag_info(size_t fline, size_t fcolumn, size_t foffset, sqf::runtime::fileio::pathinfo fpath, std::string code_segment) :
			line(fline), column(fcolumn), offset(foffset), file_offset(offset), path(fpath), code_segment(code_segment)
		{
		}
		diag_info(size_t fline, size_t fcolumn, size_t foffset, size_t faoffset, sqf::runtime::fileio::pathinfo fpath, std::string code_segment) :
			line(fline), column(fcolumn), offset(foffset), file_offset(faoffset), path(fpath), code_segment(code_segment)
		{
		}

		operator sqf::runtime::fileio::pathinfo() const { return path; }
	};
}