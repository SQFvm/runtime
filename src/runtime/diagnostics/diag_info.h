#pragma once
#include "../fileio.h"

#include <string>
#include <string_view>


namespace sqf::runtime::diagnostics
{
    class diag_info
    {
    public:
        // The line this diag_info was created at
        size_t line;
        // The column this diag_info was created at
        size_t column;
        // The adjusted file offset, after preprocessing, this diag_info was created at
        size_t adjusted_offset;
        // The original file offset, before preprocessing, this diag_info was created at.
        size_t file_offset;
        // The original file offset, before preprocessing, this diag_info was created at.
        size_t length;
        // The path of this diag_info
        sqf::runtime::fileio::pathinfo path;
        // A view from the file, that can be used for diagnostics
        std::string code_segment;

        bool operator==(const diag_info& b) const { return line == b.line && column == b.column && file_offset == b.file_offset && path == b.path && code_segment == b.code_segment; }
        bool operator!=(const diag_info& b) const { return line != b.line || column != b.column || file_offset != b.file_offset || path != b.path || code_segment != b.code_segment; }

        diag_info() : diag_info(0, 0, 0, 0, {}, "") {}
        diag_info(
            size_t line,
            size_t column,
            size_t file_offset,
            sqf::runtime::fileio::pathinfo path,
            std::string code_segment) :
            line(line),
            column(column),
            adjusted_offset(file_offset),
            file_offset(file_offset),
            length(0),
            path(path),
            code_segment(code_segment)
        {
        }
        diag_info(
            size_t line,
            size_t column,
            size_t adjusted_offset,
            size_t file_offset,
            sqf::runtime::fileio::pathinfo path,
            std::string code_segment) :
            line(line),
            column(column),
            adjusted_offset(adjusted_offset),
            file_offset(file_offset),
            length(0),
            path(path),
            code_segment(code_segment)
        {
        }

        operator sqf::runtime::fileio::pathinfo() const { return path; }
    };
}