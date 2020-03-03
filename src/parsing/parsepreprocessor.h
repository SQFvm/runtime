#pragma once
#include "macro.h"
#include "logging.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace sqf
{
	class virtualmachine;
	namespace parse
	{
		class preprocessorfileinfo
		{
		private:
			size_t last_col;
			bool is_in_string;
			bool is_in_block_comment;
			// Handles correct progression of line, col and off
			char _next()
			{
				if (off >= content.length())
				{
					return '\0';
				}
				char c = content[off++];
				switch (c)
				{
				case '\n':
					line++;
					last_col = col;
					col = 0;
					return c;
				case '\r':
					return _next();
				default:
					col++;
					return c;
				}
			}
		public:
			preprocessorfileinfo()
			{
				last_col = 0;
				is_in_string = false;
				is_in_block_comment = false;
			}
			std::string content;
			size_t off = 0;
			size_t line = 1;
			size_t col = 0;
			std::string path;
			// Returns the next character.
			// Will not take into account to skip eg. comments or simmilar things!
			char peek(size_t len = 0)
			{
				if (off + len >= content.length())
				{
					return '\0';
				}
				return content[off + len];
			}
			// Will return the next character in the file.
			// Comments will be skipped automatically.
			char next()
			{
				char c = _next();
				if (!is_in_string && (c == '/' || is_in_block_comment))
				{
					if (c == '\n')
					{
						return c;
					}
					auto pc = peek();
					if (is_in_block_comment && c == '*' && pc == '/')
					{
						_next();
						c = _next();
						is_in_block_comment = false;
						return c;
					}
					else if (pc == '*' || is_in_block_comment)
					{
						if (!is_in_block_comment)
						{
							_next();
						}
						is_in_block_comment = true;
						while ((c = _next()) != '\0')
						{
							if (c == '\n')
							{
								break;
							}
							else if (c == '*' && peek() == '/')
							{
								_next();
								is_in_block_comment = false;
								c = next();
								break;
							}
						}
					}
					else if (pc == '/')
					{
						while ((c = _next()) != '\0' && c != '\n');
					}
				}
				if (c == '\\')
				{
					auto pc1 = peek(0);
					auto pc2 = peek(1);
					if (pc1 == '\r' && pc2 == '\n')
					{
						_next();
						_next();
						return next();
					}
					else if (pc1 == '\n')
					{
						_next();
						return next();
					}
				}
				if (c == '"')
				{
					is_in_string = !is_in_string;
				}
				return c;
			}

			std::string get_word()
			{
				char c;
				size_t off_start = off;
				size_t off_end = off;
				while (
					(c = next()) != '\0' && (
					(c >= 'A' && c <= 'Z') ||
						(c >= 'a' && c <= 'z') ||
						(c >= '0' && c <= '9') ||
						c == '_'
						))
				{
					off_end = off;
				}
				move_back();
				return content.substr(off_start, off_end - off_start);
			}

			std::string get_line(bool catchEscapedNewLine)
			{
				char c;
				size_t off_start = off;
				bool escaped = false;
				bool exit = false;
				if (catchEscapedNewLine)
				{
					std::string outputString;
					outputString.reserve(64);
					while (!exit && (c = next()) != '\0')
					{
						switch (c)
						{
						case '\\':
							escaped = true;
							break;
						case '\n':
							if (!escaped)
							{
								exit = true;
							}
							escaped = false;
							break;
						default:
							if (escaped)
							{
								outputString.push_back('\\');
								escaped = false;
							}
							outputString.push_back(c);
							break;
						}
					}
					outputString.shrink_to_fit();
					return outputString;
				}
				else
				{
					while ((c = next()) != '\0' && c != '\n') {}
				}
				return content.substr(off_start, off - off_start);
			}
			// Moves one character backwards and updates
			// porgression of line, col and off according
			// col will only be tracked for one line!
			// Not supposed to be used more then once!
			void move_back()
			{
				if (off == 0)
				{
					return;
				}
				char c = content[--off];
				switch (c)
				{
				case '\n':
					line--;
					col = last_col;
					break;
				case '\r':
					move_back();
					break;
				default:
					col--;
					break;
				}
			}
		};
		class preprocessor : public CanLog
		{
		public:
		private:
			std::vector<std::string> m_path_tree;
			std::vector<bool> m_inside_ppf_tree;
			bool m_inside_ppif_err_flag = false;
			std::unordered_map<std::string, macro> m_macros;
			bool m_errflag = false;
			bool m_allowwrite = true;
			virtualmachine* m_vm;

			void replace_stringify(
				preprocessorfileinfo& local_fileinfo,
				preprocessorfileinfo& original_fileinfo,
				const macro& m,
				std::vector<std::string>& params,
				std::stringstream& sstream,
				const std::unordered_map<std::string, std::string>& param_map);

			void replace_concat(
				preprocessorfileinfo& local_fileinfo,
				preprocessorfileinfo& original_fileinfo,
				const macro& m,
				std::vector<std::string>& params,
				std::stringstream& sstream,
				const std::unordered_map<std::string, std::string>& param_map);

			std::string handle_macro(
				preprocessorfileinfo& local_fileinfo,
				preprocessorfileinfo& original_fileinfo,
				const macro& m,
				const std::unordered_map<std::string, std::string>& param_map);

			std::string replace(
				preprocessorfileinfo& fileinfo,
				const macro& m,
				std::vector<std::string>& params);

			std::string handle_arg(
				preprocessorfileinfo& local_fileinfo,
				preprocessorfileinfo& original_fileinfo,
				size_t endindex,
				const std::unordered_map<std::string, std::string>& param_map);

			std::string parse_ppinstruction(preprocessorfileinfo& fileinfo);

			std::string parse_file(preprocessorfileinfo& fileinfo);

			size_t replace_find_wordend(preprocessorfileinfo fileinfo);

			void replace_skip(preprocessorfileinfo& fileinfo, std::stringstream& sstream);
		public:
			preprocessor(Logger& ref_logger, virtualmachine* vm);
			std::string parse(::sqf::virtualmachine* vm, std::string input, bool &errflag, std::string filename);
			bool inside_ppif_err_flag() { return m_inside_ppif_err_flag; }
			bool inside_ppif() { return m_inside_ppf_tree.back(); }
			bool errflag() { return m_errflag; }
			void inside_ppif(bool flag) { m_inside_ppf_tree.back() = flag; }
			void push_path(const std::string s)
			{
				m_path_tree.push_back(s);
				m_inside_ppf_tree.push_back(false);
			}
			void pop_path(preprocessorfileinfo& preprocessorfileinfo);


			std::optional<macro> contains_macro(std::string mname)
			{
				auto res = m_macros.find(mname);
				if (res == m_macros.end())
				{
					return {};
				}
				else
				{
					return res->second;
				}
			}
		};
	}
}