#pragma once
#include <string>
#include <string_view>


namespace sqf::runtime::diagnostics
{
	class diag_info
	{
	private:
		size_t m_fline;
		size_t m_fcolumn;
		std::string m_fpath;
		std::string m_code_segment;
	public:
		size_t line() const { return m_fline; }
		size_t column() const { return m_fcolumn; }
		std::string_view file() const { return m_fpath; }
		std::string_view code_segment() const { return m_code_segment; }

		bool operator==(const diag_info& b) const { return m_fline == b.m_fline && m_fcolumn == b.m_fcolumn && m_fpath == b.m_fpath && m_code_segment == b.m_code_segment; }
		bool operator!=(const diag_info& b) const { return m_fline != b.m_fline || m_fcolumn != b.m_fcolumn || m_fpath != b.m_fpath || m_code_segment != b.m_code_segment; }

		diag_info() = default;

		diag_info(size_t fline, size_t fcolumn, std::string fpath, std::string code_segment) :
			m_fline(fline), m_fcolumn(fcolumn), m_fpath(fpath), m_code_segment(code_segment)
		{
		}
	};
}