#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <optional>
#include "compiletime.h"
#include "parsepreprocessor.h"
#include "virtualmachine.h"
#include "fileio.h"

namespace {
	class helper;
	class finfo;
	class macro {
	public:
		std::string name;
		std::string content;
		std::vector<std::string> args;
		std::string filepath;
		size_t line;
		size_t column;

	};
	class helper
	{
	public:
		std::vector<macro> macros;
		std::vector<std::string> path_tree;
		sqf::virtualmachine* vm;
		bool errflag = false;
		bool allowwrite = true;
		bool inside_ppif = false;

		std::optional<macro> contains_macro(std::string mname)
		{
			auto res = std::find_if(macros.begin(), macros.end(), [mname](macro& m) -> bool
			{
				return m.name == mname;
			});
			if (res == macros.end())
			{
				return {};
			}
			return *res;
		}

	};
	class finfo
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
		finfo()
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
							c = _next();
							break;
						}
					}
				}
				else if (pc == '/')
				{
					while ((c = _next()) != '\0' && c != '\n');
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
				std::stringstream sstream;
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
								sstream << '\\';
								escaped = false;
							}
							sstream << c;
							break;
					}
				}
				return sstream.str();
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
	std::string handle_macro(helper& h, finfo& fileinfo, const macro& m);
	std::string replace(helper& h, finfo& fileinfo, const macro& m, std::vector<std::string> params);
	std::string handle_arg(helper& h, finfo& fileinfo, size_t endindex);
	std::string parse_macro(helper& h, finfo& fileinfo);
	std::string parse_file(helper& h, finfo& fileinfo);

	void replace_helper_step1(helper& h, finfo& fileinfo, const macro& m, std::vector<std::string>& params, bool& stringify_required, std::string& actual_content, std::stringstream& sstream, size_t& word_start, size_t off = ~0)
	{
		if (word_start != off)
		{
			auto word = actual_content.substr(word_start, off - word_start);
			auto res = std::find_if(
				m.args.begin(),
				m.args.end(),
				[word](std::string s) { return word == s; }
			);
			if (res != m.args.end())
			{
				size_t index = res - m.args.begin();
				if (stringify_required)
				{
					sstream << '"' << params[index] << '"';
					stringify_required = false;
				}
				else
				{
					sstream << params[index];
				}
			}
			else
			{
				if (stringify_required)
				{
					sstream << '#' << word;
					stringify_required = false;
				}
				else
				{
					sstream << word;
				}
			}
		}
		word_start = off + 1;
	}
	void replace_helper_step3(helper& h, finfo& fileinfo, const macro& m, std::vector<std::string>& params, bool& stringify_required, std::string& actual_content, std::stringstream& sstream, size_t& word_start, size_t& off)
	{
		if (word_start != off)
		{
			auto word = actual_content.substr(word_start, off - word_start);
			auto res = h.contains_macro(word);
			if (res.has_value())
			{
				finfo handlefinfo;
				handlefinfo.off = off;
				handlefinfo.line = m.line;
				handlefinfo.col = off;
				handlefinfo.path = m.filepath;
				handlefinfo.content = actual_content;
				auto handled = handle_macro(h, handlefinfo, res.value());
				if (stringify_required)
				{
					stringify_required = false;
					sstream << '"' << handled << '"';
				}
				else
				{
					sstream << handled;
				}
				off = handlefinfo.off - 1;
			}
			else
			{
				if (stringify_required)
				{
					stringify_required = false;
					sstream << '"' << word << '"';
				}
				else
				{
					sstream << word;
				}
			}
		}
		word_start = off + 1;
	}
	std::string replace(helper& h, finfo& fileinfo, const macro& m, std::vector<std::string> params)
	{
		if (m.args.size() != params.size())
		{
			h.errflag = true;
			if (fileinfo.path.empty())
			{
				h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Arg Count Missmatch." << std::endl;
			}
			else
			{
				h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Arg Count Missmatch." << std::endl;
			}
			return "";
		}
		std::stringstream sstream;
		std::string actual_content = m.content;
		bool stringify_required = false;
		bool concat_required = false;
		bool inside_string = false;
		size_t last_stringify_required = -2;
		// 1. Replace & scan for replace/concat hash
		{
			size_t word_start = 0;
			for (size_t off = 0; off < actual_content.length(); off++)
			{
				char c = actual_content[off];
				switch (c)
				{
					case 'a': case 'b': case 'c': case 'd': case 'e':
					case 'f': case 'g': case 'h': case 'i': case 'j':
					case 'k': case 'l': case 'm': case 'n': case 'o':
					case 'p': case 'q': case 'r': case 's': case 't':
					case 'u': case 'v': case 'w': case 'x': case 'y':
					case 'z': case 'A': case 'B': case 'C': case 'D':
					case 'E': case 'F': case 'G': case 'H': case 'I':
					case 'J': case 'K': case 'L': case 'M': case 'N':
					case 'O': case 'P': case 'Q': case 'R': case 'S':
					case 'T': case 'U': case 'V': case 'W': case 'X':
					case 'Y': case 'Z': case '0': case '1': case '2':
					case '3': case '4': case '5': case '6': case '7':
					case '8': case '9': case '_':
						break;
					case '#':
						if (inside_string)
						{
							break;
						}
						replace_helper_step1(h, fileinfo, m, params, stringify_required, actual_content, sstream, word_start, off);
						
						{
							auto peek1 = off + 1 < actual_content.length() ? actual_content[off + 1] : '\0';
							auto peek2 = off + 2 < actual_content.length() ? actual_content[off + 2] : '\0';
							if (peek1 == '#' && peek2 == '#')
							{
								off++;
								sstream << "#";
								word_start++;
							}
							if (peek1 == '#')
							{
								off++;
								word_start++;
								sstream << "##";
							}
							else
							{
								last_stringify_required = off;
								stringify_required = true;
							}
						}
						break;
					case '"':
						if (!inside_string)
						{
							inside_string = true;
						}
						else
						{
							inside_string = false;
							auto word = actual_content.substr(word_start, off - word_start);
							sstream << word << c;
							word_start = off + 1;
						}
						break;
					default:
						if (inside_string)
						{
							break;
						}
						if (stringify_required && off - 1 == last_stringify_required)
						{
							stringify_required = false;
							word_start--;
						}
						replace_helper_step1(h, fileinfo, m, params, stringify_required, actual_content, sstream, word_start, off);
						sstream << c;
				}
			}

			replace_helper_step1(h, fileinfo, m, params, stringify_required, actual_content, sstream, word_start, actual_content.length());
			if (stringify_required)
			{
				stringify_required = false;
				sstream << '#';
			}
			actual_content = sstream.str();
			sstream.str("");
		}
		// 3. Execute nested macros
		{
			size_t word_start = 0;
			bool is_inside_string = false;
			stringify_required = false;
			for (size_t off = 0; off < actual_content.length(); off++)
			{
				char c = actual_content[off];
				switch (c)
				{
					case 'a': case 'b': case 'c': case 'd': case 'e':
					case 'f': case 'g': case 'h': case 'i': case 'j':
					case 'k': case 'l': case 'm': case 'n': case 'o':
					case 'p': case 'q': case 'r': case 's': case 't':
					case 'u': case 'v': case 'w': case 'x': case 'y':
					case 'z': case 'A': case 'B': case 'C': case 'D':
					case 'E': case 'F': case 'G': case 'H': case 'I':
					case 'J': case 'K': case 'L': case 'M': case 'N':
					case 'O': case 'P': case 'Q': case 'R': case 'S':
					case 'T': case 'U': case 'V': case 'W': case 'X':
					case 'Y': case 'Z': case '0': case '1': case '2':
					case '3': case '4': case '5': case '6': case '7':
					case '8': case '9': case '_':
						break;
					case '#':
						if (inside_string)
						{
							break;
						}
						replace_helper_step3(h, fileinfo, m, params, stringify_required, actual_content, sstream, word_start, off);
						{
							auto peek1 = off + 1 < actual_content.length() ? actual_content[off + 1] : '\0';
							auto peek2 = off + 2 < actual_content.length() ? actual_content[off + 2] : '\0';
							if (peek1 == '#' && peek2 == '#')
							{
								off++;
								sstream << "#";
								word_start++;
							}
							if (peek1 == '#')
							{
								off++;
								word_start++;
								sstream << "##";
								concat_required = true;
							}
							else
							{
								stringify_required = true;
								word_start++;
							}
						}
						break;
					case '"':
						if (!inside_string)
						{
							inside_string = true;
						}
						else
						{
							inside_string = false;
							auto word = actual_content.substr(word_start, off - word_start);
							if (stringify_required)
							{
								sstream << c << word << c << c;
								stringify_required = false;
							}
							else
							{
								sstream << word << c;
							}
							word_start = off + 1;
							break;
						}
					default:
						if (inside_string)
						{
							break;
						}
						size_t offold = off;
						replace_helper_step3(h, fileinfo, m, params, stringify_required, actual_content, sstream, word_start, off);
						if (off == offold)
						{
							sstream << c;
						}
						break;
				}
			}
			size_t len = actual_content.length();
			replace_helper_step3(h, fileinfo, m, params, stringify_required, actual_content, sstream, word_start, len);
			if (stringify_required)
			{
				stringify_required = false;
				sstream << '#';
			}
			actual_content = sstream.str();
			sstream.str("");
		}
		// 4. Concat everything
		if (concat_required)
		{
			for (size_t off = 0; off < actual_content.length(); off++)
			{
				char c = actual_content[off];
				char c_la1 = off + 1 < actual_content.length() ? actual_content[off + 1] : '\0';
				char c_la2 = off + 2 < actual_content.length() ? actual_content[off + 2] : '\0';
				switch (c)
				{
					case '#':
						if (c_la1 == '#' && c_la2 == '#')
						{
							sstream << c;
						}
						else if (c_la1 == '#')
						{
							off++;
						}
						break;
					default:
						sstream << c;
						break;
				}
			}
			actual_content = sstream.str();
		}
		return actual_content;
	}
	std::string handle_arg(helper& h, finfo& fileinfo, size_t endindex)
	{
		size_t word_start = fileinfo.off;
		bool inside_word = false;
		std::stringstream sstream;
		char c;
		while (fileinfo.off != endindex && (c = fileinfo.next()) != '\0')
		{
			switch (c)
			{
				case 'a': case 'b': case 'c': case 'd': case 'e':
				case 'f': case 'g': case 'h': case 'i': case 'j':
				case 'k': case 'l': case 'm': case 'n': case 'o':
				case 'p': case 'q': case 'r': case 's': case 't':
				case 'u': case 'v': case 'w': case 'x': case 'y':
				case 'z': case 'A': case 'B': case 'C': case 'D':
				case 'E': case 'F': case 'G': case 'H': case 'I':
				case 'J': case 'K': case 'L': case 'M': case 'N':
				case 'O': case 'P': case 'Q': case 'R': case 'S':
				case 'T': case 'U': case 'V': case 'W': case 'X':
				case 'Y': case 'Z': case '0': case '1': case '2':
				case '3': case '4': case '5': case '6': case '7':
				case '8': case '9': case '_':
					if (!inside_word)
					{
						inside_word = true;
						word_start = fileinfo.off - 1;
					}
					if (fileinfo.off != endindex)
					{
						break;
					} // Intended conditional fallthrough
				default:
					if (inside_word)
					{
						inside_word = false;
						auto word = fileinfo.content.substr(word_start, fileinfo.off - word_start - (fileinfo.off != endindex ? 1 : 0));
						auto res = h.contains_macro(word);
						if (res.has_value())
						{
							if (!res.value().args.empty())
							{
								fileinfo.move_back();
							}
							auto handled = handle_macro(h, fileinfo, res.value());
							if (h.errflag)
							{
								return "";
							}
							sstream << handled;
						}
						else
						{
							sstream << word;
							if (fileinfo.off != endindex)
							{
								sstream << c;
							}
						}
					}
					else
					{
						sstream << c;
					}
					break;
			}
		}
		return sstream.str();
	}
	std::string handle_macro(helper& h, finfo& fileinfo, const macro& m)
	{ // Needs to handle 'NAME(ARG1, ARG2, ARGN)' not more, not less!
		std::vector<std::string> params;
		if (!m.args.empty())
		{
			if (fileinfo.peek() != '(')
			{
				return m.name;
			}
			fileinfo.next(); // Skip the initial '('
			size_t rb_counter = 0;
			size_t cb_counter = 0;
			size_t eb_counter = 0;
			size_t lastargstart = fileinfo.off;
			bool exit = false;
			char c;
			bool in_string = false;
			while (!exit && (c = fileinfo.next()) != '\0')
			{
				if (in_string)
				{
					if (c == '"')
					{
						in_string = false;
					}
					continue;
				}
				switch (c)
				{
					case '[': eb_counter++; break;
					case ']': eb_counter--; break;
					case '{': cb_counter++; break;
					case '}': cb_counter--; break;
					case '(': rb_counter++; break;
					case '"': in_string = true; break;
					case ')': if (rb_counter != 0) { rb_counter--; break; }
							  else { exit = true; /* goto case ',' */ }
					case ',':
						if (rb_counter == 0 && eb_counter == 0 && cb_counter == 0)
						{
							fileinfo.move_back();
							auto param = fileinfo.content.substr(lastargstart, fileinfo.off - lastargstart);
							if (!param.empty())
							{
								finfo copy = fileinfo;
								copy.off = lastargstart;
								auto handled_param = handle_arg(h, copy, fileinfo.off);
								params.push_back(handled_param);
							}
							fileinfo.next();
							lastargstart = fileinfo.off;
						}
						break;
				}
			}
		}
		return replace(h, fileinfo, m, params);
	}

	std::string parse_macro(helper& h, finfo& fileinfo)
	{
		char c;
		bool was_new_line = true;
		auto inst = fileinfo.get_word();
		auto line = fileinfo.get_line(true);
		line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](char c) -> bool {
			return c != ' ' && c != '\t';
		}));
		line.erase(std::find_if(line.rbegin(), line.rend(), [](char c) -> bool {
			return c != ' ' && c != '\t';
		}).base(), line.end());
		std::transform(inst.begin(), inst.end(), inst.begin(), ::toupper);
		if (inst == "INCLUDE")
		{ // #include "file/path"
		  // Trim
			line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](char c) -> bool {
				return c != '"';
			}));
            auto endIter = std::find_if(line.begin(), line.end(), [](char c) -> bool {
                return c == '"';
            });
            if (std::distance(endIter, line.end()) > 1) {
                if (fileinfo.path.empty())
                    h.vm->wrn() << "[WARN][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Unexpected data after include path." << std::endl;
                else
                    h.vm->wrn() << "[WARN][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Unexpected data after include path." << std::endl;
            }
            line.erase(endIter, line.end());
			finfo otherfinfo;
			try
			{
				otherfinfo.path = h.vm->get_filesystem().get_physical_path(line, fileinfo.path);
				auto path = otherfinfo.path;
				auto res = std::find_if(h.path_tree.begin(), h.path_tree.end(), [path](std::string& parent) -> bool { return parent == path; });
				if (res != h.path_tree.end())
				{
					h.errflag = true;
					if (fileinfo.path.empty())
					{
						h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Recursive include detected. Include Tree." << std::endl;
					}
					else
					{
						h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Recursive include detected. Include Tree." << std::endl;
					}
					for (size_t i = 0; i < h.path_tree.size(); i++)
					{
						h.vm->err() << i << ". " << h.path_tree[i] << std::endl;
					}
					return "";
				}
				h.path_tree.push_back(path);
				otherfinfo.content = load_file(path);
			}
			catch (const std::runtime_error& ex)
			{
				h.errflag = true;
				if (fileinfo.path.empty())
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Failed to include '" << line << "' into file '" << fileinfo.path << "]\t" << "':" << ex.what() << std::endl;
				}
				else
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Failed to include '" << line << "' into file '" << fileinfo.path << "]\t" << "':" << ex.what() << std::endl;
				}
				return "";
			}
			std::stringstream sstream;
			sstream << "#file " << otherfinfo.path << std::endl;
			sstream << "#line 0" << std::endl;
			sstream << parse_file(h, otherfinfo) << std::endl;
			sstream << "#file " << fileinfo.path << std::endl;
			sstream << "#line " << fileinfo.line - 1 << std::endl;
			return sstream.str();
		}
		else if (inst == "DEFINE")
		{ // #define TEST(A, B, C) A #B##C
			if (!h.allowwrite)
			{
				return "\n";
			}
			macro m;
			m.filepath = fileinfo.path;
			m.line = fileinfo.line;
			m.column = fileinfo.col;
			auto bracketsIndex = line.find('(');
            auto spaceIter = std::find_if(line.begin(), line.end(), [](char ch) {
                    return !(ch == '_' ||
                        (ch >= 'a' && ch <= 'z') ||
                        (ch >= 'A' && ch <= 'Z') ||
                        (ch >= '0' && ch <= '9'));
                });
			auto spaceIndex = spaceIter == line.end() ? std::string::npos : std::distance(line.begin(), spaceIter);
			if (bracketsIndex == std::string::npos && spaceIndex == std::string::npos)
			{ // Empty define
				m.name = line;
			}
			else
			{
				if (spaceIndex < bracketsIndex || bracketsIndex == std::string::npos) // std::string::npos does not need to be catched as bracketsIndex always < npos here
				{ // First bracket was found after first space OR is not existing thus we have a simple define with a replace value here
					m.name = line.substr(0, spaceIndex);
                    if (m.name == "DEBUG_MODE_FULL") __debugbreak();
					m.content = line.substr(line[spaceIndex] == ' ' ? spaceIndex + 1 : spaceIndex); //Special magic for #define macro\ 
				}
				else
				{ // We got a define with arguments here
					auto bracketsEndIndex = line.find(')');
					auto argumentsString = line.substr(bracketsIndex + 1, bracketsEndIndex);

					size_t arg_index = bracketsIndex;
					size_t arg_start_index = bracketsIndex + 1;
					bool ended = false;
					while (!ended)
					{
						arg_index = line.find(',', arg_start_index);
						if (arg_index == std::string::npos || arg_index > bracketsEndIndex)
						{
							ended = true;
							arg_index = bracketsEndIndex;
						}
						auto arg = line.substr(arg_start_index, arg_index - arg_start_index);
						arg.erase(arg.begin(), std::find_if(arg.begin(), arg.end(), [](char c) -> bool {
							return c != '\t' && c != ' ';
						}));
						arg.erase(std::find_if(arg.rbegin(), arg.rend(), [](char c) -> bool {
							return c != '\t' && c != ' ';
						}).base(), arg.end());
						m.args.push_back(arg);
						arg_start_index = arg_index + 1;
					}
					m.name = line.substr(0, bracketsIndex);
					m.content = line.length() <= bracketsEndIndex + 2 ? "" : line.substr(bracketsEndIndex + 2);
				}
			}
			h.macros.push_back(m);
			return "\n";
		}
		else if (inst == "UNDEF")
		{ // #undef TEST
			if (!h.allowwrite)
			{
				return "\n";
			}
			auto res = std::find_if(h.macros.begin(), h.macros.end(), [line](macro& m) -> bool {
				return line == m.name;
			});
			if (res == h.macros.end())
			{
				h.errflag = true;
				if (fileinfo.path.empty())
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Macro '" << line << "' not found." << std::endl;
				}
				else
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Macro '" << line << "' not found." << std::endl;
				}
				return "";
			}
			else
			{
				h.macros.erase(res);
			}
			return "\n";
		}
		else if (inst == "IFDEF")
		{ // #ifdef TEST
			if (h.inside_ppif)
			{
				h.errflag = true;
				if (fileinfo.path.empty())
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Unexpected IFDEF. Already inside of a IFDEF or IFNDEF enclosure." << std::endl;
				}
				else
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Unexpected IFDEF. Already inside of a IFDEF or IFNDEF enclosure." << std::endl;
				}
				return "";
			}
			else
			{
				h.inside_ppif = true;
			}
			auto res = std::find_if(h.macros.begin(), h.macros.end(), [line](macro& m) -> bool {
				return line == m.name;
			});
			if (res == h.macros.end())
			{
				h.allowwrite = false;
			}
			else
			{
				h.allowwrite = true;
			}
			return "\n";
		}
		else if (inst == "IFNDEF")
		{ // #ifndef TEST
			if (h.inside_ppif)
			{
				h.errflag = true;
				if (fileinfo.path.empty())
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Unexpected IFNDEF. Already inside of a IFDEF or IFNDEF enclosure." << std::endl;
				}
				else
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Unexpected IFNDEF. Already inside of a IFDEF or IFNDEF enclosure." << std::endl;
				}
				return "";
			}
			else
			{
				h.inside_ppif = true;
			}
			auto res = std::find_if(h.macros.begin(), h.macros.end(), [line](macro& m) -> bool {
				return line == m.name;
			});
			if (res == h.macros.end())
			{
				h.allowwrite = true;
			}
			else
			{
				h.allowwrite = false;
			}
			return "\n";
		}
		else if (inst == "ELSE")
		{ // #else
			if (!h.inside_ppif)
			{
				h.errflag = true;
				if (fileinfo.path.empty())
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Unexpected ELSE. Not inside of a IFDEF or IFNDEF enclosure." << std::endl;
				}
				else
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Unexpected ELSE. Not inside of a IFDEF or IFNDEF enclosure." << std::endl;
				}
				return "";
			}
			h.allowwrite = !h.allowwrite;
			return "\n";
		}
		else if (inst == "ENDIF")
		{ // #endif
			if (!h.inside_ppif)
			{
				h.errflag = true;
				if (fileinfo.path.empty())
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Unexpected ENDIF. Not inside inside of a IFDEF or IFNDEF enclosure." << std::endl;
				}
				else
				{
					h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Unexpected ENDIF. Not inside inside of a IFDEF or IFNDEF enclosure." << std::endl;
				}
				return "";
			}
			h.inside_ppif = false;
			h.allowwrite = true;
			return "\n";
		}
		else
		{
			h.errflag = true;
			if (fileinfo.path.empty())
			{
				h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Unknown PreProcessor instruction '" << inst << "'." << std::endl;
			}
			else
			{
				h.vm->err() << "[ERR][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Unknown PreProcessor instruction '" << inst << "'." << std::endl;
			}
			return "";
		}
	}

	std::string parse_file(helper& h, finfo& fileinfo)
	{
		char c;
		std::stringstream sstream;
		std::stringstream wordstream;
		bool was_new_line = true;
		bool is_in_string = false;
		while ((c = fileinfo.next()) != '\0')
		{
			if (is_in_string)
			{
				if (c == '"')
				{
					is_in_string = false;
				}
                if (h.allowwrite)
				    sstream << c;
				continue;
			}
			switch (c)
			{
				case '"':
				{
					is_in_string = true;
					auto word = wordstream.str();
					wordstream.str("");
                    if (h.allowwrite)
					    sstream << word << c;
				} break;
				case '\n':
				{
					was_new_line = true;
					// Fall Through
				}
				case '#':
				{
					if (c == '#' && was_new_line)
					{
						auto res = parse_macro(h, fileinfo);
						if (h.errflag)
						{
							return res;
						}
						sstream << res;
						break;
					}
				}
				default:
				{
					if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
					{
						was_new_line = false;
					}
					if (h.allowwrite)
					{
						if (wordstream.rdbuf()->in_avail() > 0)
						{
							auto word = wordstream.str();
							wordstream.str("");
							auto m = h.contains_macro(word);
							if (m.has_value())
							{
								fileinfo.move_back();
								auto res = handle_macro(h, fileinfo, m.value());
								if (h.errflag)
								{
									return res;
								}
								sstream << res;
							}
							else
							{
								sstream << word << c;
							}
						}
						else
						{
							sstream << c;
						}
					}
					else if (c == '\n')
					{
						sstream << c;
					}
				} break;
				case 'a': case 'b': case 'c': case 'd': case 'e':
				case 'f': case 'g': case 'h': case 'i': case 'j':
				case 'k': case 'l': case 'm': case 'n': case 'o':
				case 'p': case 'q': case 'r': case 's': case 't':
				case 'u': case 'v': case 'w': case 'x': case 'y':
				case 'z': case 'A': case 'B': case 'C': case 'D':
				case 'E': case 'F': case 'G': case 'H': case 'I':
				case 'J': case 'K': case 'L': case 'M': case 'N':
				case 'O': case 'P': case 'Q': case 'R': case 'S':
				case 'T': case 'U': case 'V': case 'W': case 'X':
				case 'Y': case 'Z': case '0': case '1': case '2':
				case '3': case '4': case '5': case '6': case '7':
				case '8': case '9': case '_':
				{
                    if (h.allowwrite)
					    wordstream << c;
					was_new_line = false;
				} break;
			}
		}

		auto word = wordstream.str();
		if (!word.empty())
		{
			auto m = h.contains_macro(word);
			if (m.has_value())
			{
				fileinfo.move_back();
				auto res = handle_macro(h, fileinfo, m.value());
				if (h.errflag)
				{
					return res;
				}
				sstream << res;
			}
			else
			{
				sstream << word;
			}
		}
		return sstream.str();
	}

}
std::string sqf::parse::preprocessor::parse(sqf::virtualmachine* vm, std::string input, bool & errflag, std::string filename)
{
	helper h;
	h.vm = vm;
	finfo fileinfo;
	fileinfo.content = input;
	fileinfo.path = filename;
	auto res = parse_file(h, fileinfo);
	errflag = h.errflag;
	return res;
}