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
					return next();
				default:
					col++;
					return c;
			}
		}
		bool is_in_block_comment = false;
	public:
		std::string content;
		size_t off = 0;
		size_t line = 0;
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
			if (c == '/' || is_in_block_comment)
			{
				auto pc = peek();
				if (pc == '*' || is_in_block_comment)
				{
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
		std::stringstream sstream;
		std::string actual_content = m.content;
		bool stringify_required = false;
		bool concat_required = false;
		bool inside_string = false;
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
						if ((off + 1 < actual_content.length() ? actual_content[off + 1] : '\0') == '#')
						{
							off++;
							word_start ++;
							sstream << "##";
						}
						else
						{
							stringify_required = true;
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
		/*
		// 2. Stringify
		if (stringify_required)
		{
			bool is_stringify = false;
			for (size_t off = 0; off < actual_content.length(); off++)
			{
				char c = actual_content[off];
				char c_la = off + 1 < actual_content.length() ? actual_content[off + 1] : '\0';
				switch (c)
				{
					case '#':
						if (c_la != '#')
						{
							is_stringify = true;
							sstream << '"';
						}
						else
						{
							off++;
						}
						break;
					case ' ': case '\t':
						if (is_stringify)
						{
							is_stringify = false;
							sstream << '"';
						}
					default:
						sstream << c;
						break;
				}
			}
			if (is_stringify)
			{
				sstream << '"';
			}
			actual_content = sstream.str();
			sstream.str("");
		}
		*/
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
						if ((off + 1 < actual_content.length() ? actual_content[off + 1] : '\0') == '#')
						{
							off++;
							word_start ++;
							sstream << "##";
							concat_required = true;
						}
						else
						{
							stringify_required = true;
							word_start++;
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
				char c_la = off + 1 < actual_content.length() ? actual_content[off + 1] : '\0';
				switch (c)
				{
					case '#':
						if (c_la == '#')
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
		while(fileinfo.off != endindex && (c = fileinfo.next()) != '\0')
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
				h.errflag = true;
				h.vm->err() << "Missing args opening brace on macro." << std::endl;
				return "";
			}
			fileinfo.next(); // Skip the initial '('
			size_t rb_counter = 0;
			size_t cb_counter = 0;
			size_t eb_counter = 0;
			size_t lastargstart = fileinfo.off;
			bool exit = false;
			char c;
			while(!exit && (c = fileinfo.next()) != '\0')
			{
				switch (c)
				{
					case '[': eb_counter++; break;
					case ']': eb_counter--; break;
					case '{': cb_counter++; break;
					case '}': cb_counter--; break;
					case '(': rb_counter++; break;
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
			line.erase(std::find_if(line.rbegin(), line.rend(), [](char c) -> bool {
				return c != '"';
			}).base(), line.end());
			finfo otherfinfo;
			try
			{
				otherfinfo.path = h.vm->get_filesystem().get_physical_path(line, fileinfo.path);
				auto path = otherfinfo.path;
				auto res = std::find_if(h.path_tree.begin(), h.path_tree.end(), [path](std::string& parent) -> bool { return parent == path; });
				if (res != h.path_tree.end())
				{
					h.errflag = true;
					h.vm->err() << "Recursive include detected. Include Tree:" << std::endl;
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
				h.vm->err() << "Failed to include '" << line << "' into file '" << fileinfo.path << "':" << ex.what() << std::endl;
				return "";
			}
			std::stringstream sstream;
			sstream << "#file " << otherfinfo.path << std::endl;
			sstream << "#line 0" << std::endl;
			sstream << parse_file(h, otherfinfo) << std::endl;
			sstream << "#file " << fileinfo.path << std::endl;
			sstream << "#line " << fileinfo.line << std::endl;
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
			auto spaceIndex = line.find(' ');
			if (spaceIndex == -1)
			{ // No Space found thus we just have a define here without anything else
				m.name = line;
			}
			else
			{
				int bracketsIndex = line.find('(');
				if (spaceIndex < bracketsIndex || bracketsIndex == -1)
				{ // First bracket was found after first space OR is not existing thus we have a simple define with a replace value here
					m.name = line.substr(0, spaceIndex);
					m.content = line.substr(spaceIndex + 1);
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
					m.content = line.substr(bracketsEndIndex + 2);
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
				h.vm->err() << "Macro '" << line << "' not found." << std::endl;
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
				h.vm->err() << "Unexpected IFDEF. Already inside of a IFDEF or IFNDEF enclosure." << std::endl;
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
				h.vm->err() << "Unexpected IFNDEF. Already inside of a IFDEF or IFNDEF enclosure." << std::endl;
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
				h.vm->err() << "Unexpected ELSE. Not inside of a IFDEF or IFNDEF enclosure." << std::endl;
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
				h.vm->err() << "Unexpected ENDIF. Not inside inside of a IFDEF or IFNDEF enclosure." << std::endl;
				return "";
			}
			h.inside_ppif = false;
			h.allowwrite = true;
		}
		else
		{
			h.errflag = true;
			h.vm->err() << "Unknown PreProcessor instruction '" << inst << "'." << std::endl;
			return "";
		}
	}

	std::string parse_file(helper& h, finfo& fileinfo)
	{
		char c;
		std::stringstream sstream;
		std::stringstream wordstream;
		bool was_new_line = true;
		while ((c = fileinfo.next()) != '\0')
		{
			switch (c)
			{
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
					if (h.allowwrite)
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
					wordstream << c;
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
/*
class macro;
std::string parse_file(sqf::virtualmachine* vm, std::string input, size_t off, bool& errflag, std::string filename, std::vector<macro>& macrolist, size_t& line, size_t& col, std::vector<std::string>& include_paths, size_t maxindex = ~0);
struct range { size_t index; size_t length; };
class macro
{
public:
	std::string name;
	std::string content;
	std::vector<std::string> args;
	std::string path;
	size_t line;
	size_t column;

	std::string handle(std::vector<std::string> params)
	{
		std::stringstream sstream;
		size_t word_start = 0;
		bool stringify = false;
		size_t off;
		for (off = 0; off < content.length(); off++)
		{
			char c = content[off];
			if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
			{
				continue;
			}
			else
			{
				if (stringify)
				{
					stringify = false;
					sstream << "\"";
				}
				if (word_start == off)
				{
					word_start++;
					sstream << content[off];
				}
				else 
				{
					std::string word = content.substr(word_start, off - word_start);
					auto res = std::find_if(args.begin(), args.end(), [word](std::string& str) -> bool { return str == word; });
					if (res == args.end())
					{
						word_start = off + 1;
						sstream << word;
					}
					else
					{
						size_t index = res - args.begin();
						word_start = off + 1;
						sstream << params[index];
					}
					if (c != '#')
					{
						sstream << content[off];
					}
				}
				if (c == '#')
				{
					if (off + 1 < content.length() && content[off + 1] == '#')
					{
						// Concat
						off++;
					}
					else
					{
						sstream << "\"";
					}
					word_start = off + 1;
				}
			}
		}
		if (word_start != off)
		{
			std::string word = content.substr(word_start, off - word_start);
			auto res = std::find_if(args.begin(), args.end(), [word](std::string& str) -> bool { return str == word; });
			if (res == args.end())
			{
				sstream << word;
			}
			else
			{
				size_t index = res - args.begin();
				sstream << params[index];
			}
		}
		if (stringify)
		{
			sstream << "\"";
		}
		return sstream.str();
	}
	bool handle_insert(sqf::virtualmachine* vm, char& c, size_t& off, size_t maxindex, size_t& line, size_t& col, std::string filename, std::string input, std::stringstream& sstream, std::vector<macro>& macrolist)
	{// So lets check the start being all macro-like
		if (c != '(')
		{
			vm->err() << "Missing round opening brace at L" << line << ", C" << col << " in: '" << filename << "'." << std::endl;
			return false;
		}
		// Now lets find the end of our args chain and all arguments
		bool stringskip = false;
		size_t rbrace_counter = 1;
		size_t cbrace_counter = 0;
		size_t ebrace_counter = 0;
		if (++off >= input.length())
		{
			vm->err() << "Unexpected End-Of-File." << std::endl;
			return false;
		}
		std::vector<range> largs;
		size_t argstart = off++;
		bool exit_loop = false;
		for (c = input[off]; !exit_loop && off < input.length() && off < maxindex; off++, c = input[off])
		{
			if (stringskip)
			{
				if (c == ',')
				{
					vm->err() << "Comma in string detected at L" << line << ", C" << col << " in: '" << filename << "'." << std::endl;
					return false;
				}
				continue;
			}
			switch (c)
			{
				case '"':
					stringskip != stringskip;
					break;
				case ',':
					if (rbrace_counter == 1)
					{
						largs.push_back({ argstart, off - argstart });
						argstart = off + 1;
					}
					break;
				case '(':
					rbrace_counter++;
					break;
				case ')':
					rbrace_counter--;
					if (rbrace_counter == 0)
					{
						// End Reached
						exit_loop = true;
						largs.push_back({ argstart, off - argstart });
					}
					break;
				case '[':
					ebrace_counter++;
					break;
				case ']':
					ebrace_counter--;
					break;
				case '{':
					cbrace_counter++;
					break;
				case '}':
					cbrace_counter--;
					break;
			}
		}

		// Ensure that our arg counts are equal
		if (largs.size() < args.size())
		{
			vm->err() << "Missing arguments for macro '" << name << "' (" << largs.size() << "/" << args.size() << ")." << std::endl;
			return false;
		}
		else if (largs.size() > args.size())
		{
			vm->err() << "Too many for macro '" << name << "' (" << largs.size() << "/" << args.size() << ")." << std::endl;
			return false;
		}

		// Parse each arg individually
		std::vector<std::string> args_parsed;
		for (auto& r : largs)
		{
			bool errflag = false;
			auto arg = parse_file(vm, input, r.index, errflag, filename, macrolist, line, col, std::vector<std::string>(), r.length + r.index);
			auto orig = input.substr(r.index, r.length);
			if (orig == arg && orig.find(',') != std::string::npos)
			{
				vm->err() << "Comma in braces detected at L" << line << ", C" << col << " in: '" << filename << "'." << std::endl;
				errflag = true;
				return false;
			}
			if (errflag)
			{
				return false;
			}
			args_parsed.push_back(arg);
		}
		auto handledcontent = handle(args_parsed);
		bool errflag = false;
		auto parsedcontent = parse_file(vm, handledcontent, 0, errflag, filename, macrolist, line, col, std::vector<std::string>());
		if (errflag)
		{
			return false;
		}
		sstream << handledcontent;
		return true;
	}
};

std::string rest_of_line(std::string input, size_t& off, size_t& line)
{
	size_t from = off;
	auto was_escaped = false;
	std::stringstream sstream;
	for (char c = input[off]; off < input.length(); off++, c = input[off])
	{
		if (c == '\n')
		{
			line++;
		}
		if (c == '\n' && !was_escaped)
		{
			break;
		}
		else if (c == '\\')
		{
			was_escaped = true;
		}
		else if (was_escaped && c != '\n')
		{
			sstream << '\\' << c;
			was_escaped = false;
		}
		else
		{
			was_escaped = false;
			if (c != '\r')
			{
				sstream << c;
			}
		}
	}
	return sstream.str();
}
std::string parse_file(sqf::virtualmachine* vm, std::string input, size_t off, bool& errflag, std::string filename, std::vector<macro>& macrolist, size_t& line, size_t& col, std::vector<std::string>& include_paths, size_t maxindex)
{
	if (input.empty())
	{
		return "";
	}
	size_t word_start = off;
	bool string_start = false;
	std::stringstream sstream;
	bool was_new_line = true;
	bool is_macro = false;
	bool is_inside_if = false;
	bool write_to_output = true;
	bool line_comment = false;
	bool block_comment = false;
	if (input.length() == 0 || off >= input.length())
	{
		vm->err() << "Unexpected End-Of-File." << std::endl;
		errflag = true;
		return "";
	}
	for (char c = input[off]; off < input.length() && off < maxindex; off++)
	{
		c = input[off];
		if (line_comment)
		{
			if (c == '\n')
			{
				line_comment = false;
				line++;
				col = 0;
				was_new_line = true;
				sstream << std::endl;
			}
			word_start++;
			continue;
		}
		if (block_comment)
		{
			if (c == '\n')
			{
				line++;
				col = 0;
				sstream << std::endl;
			}
			else
			{
				col++;
			}
			word_start++;
			if (c == '*' && input.length() > off + 1 && input[off + 1] == '/')
			{
				off++;
				col++;
				word_start++;
				block_comment = false;
			}
			continue;
		}
		if (c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != '#')
		{
			was_new_line = false;
		}
		if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
		{
			col++;
			continue;
		}
		else if (c == '"')
		{
			string_start = true;
		}
		else if (string_start)
		{
			if (c == '"')
			{
				col++;
				continue;
			}
			else
			{
				string_start = false;
			}
		}
		else
		{
			size_t word_len = off - word_start;
			auto word = input.substr(word_start, word_len);
			if (c == '\n')
			{
				col == 0;
				line++;
				was_new_line = true;
			}
			if (c == '#' && was_new_line && !is_macro)
			{
				is_macro = true;
			}
			else if (c == '/' && input.length() > off + 1)
			{
				if (input[off + 1] == '/')
				{
					line_comment = true;
				}
				else if (input[off + 1] == '*')
				{
					block_comment = true;
				}
			}
			else if (is_macro)
			{
				is_macro = false;
				std::transform(word.begin(), word.end(), word.begin(), ::toupper);
				auto restofline = rest_of_line(input, off, line);
				restofline.erase(restofline.begin(), std::find_if(restofline.begin(), restofline.end(), [](char c) -> bool {
					return c != '\t' && c != ' ';
				}));
				restofline.erase(std::find_if(restofline.rbegin(), restofline.rend(), [](char c) -> bool {
					return c != '\t' && c != ' ';
				}).base(), restofline.end());
				if (word == "INCLUDE")
				{ // #include "file/path"
					if (!write_to_output)
					{
						continue;
					}
					//Trim to only filepath in quotes
					restofline.erase(restofline.begin(), std::find_if(restofline.begin(), restofline.end(), [](char c) -> bool {
						return c == '"';
					}));
					restofline.erase(std::find_if(restofline.rbegin(), restofline.rend(), [](char c) -> bool {
						return c == '"';
					}).base(), restofline.end());
					std::string filecontents, virtpath, path;
					try
					{
						virtpath = restofline.substr(1, restofline.length() - 2);
						path = vm->get_filesystem().get_physical_path(virtpath, filename);
						
						auto res = std::find_if(include_paths.begin(), include_paths.end(), [path](std::string& parent) -> bool { return parent == path; });
						if (res != include_paths.end())
						{
							errflag = true;
							vm->err() << "Recursive include detected. Include Tree:" << std::endl;
							for (size_t i = 0; i < include_paths.size(); i++)
							{
								vm->err() << i << ". " << include_paths[i] << std::endl;
							}
							return "";
						}
						filecontents = load_file(path);
					}
					catch (const std::runtime_error& ex)
					{
						errflag = true;
						vm->err() << "Failed to include '" << virtpath << "':" << ex.what() << std::endl;
						return "";
					}
					size_t line = 0;
					size_t col = 0;
					auto res = parse_file(vm, filecontents, 0, errflag, virtpath, macrolist, line, col, include_paths, maxindex);
					if (errflag)
					{
						return "";
					}
					sstream << "#file " << virtpath << std::endl;
					sstream << "#line 0" << std::endl;
					sstream << res << std::endl;
					sstream << "#file " << filename << std::endl;
					sstream << "#line " << line << std::endl;
				}
				else if (word == "DEFINE")
				{ // #define TEST(A, B, C) A #B##C
					macro m;
					m.path = filename;
					m.line = line;
					m.column = col;
					auto spaceIndex = restofline.find(' ');
					if (spaceIndex == -1)
					{ // No Space found thus we just have a define here without anything else
						m.name = restofline;
					}
					else
					{
						int bracketsIndex = restofline.find('(');
						if (spaceIndex < bracketsIndex || bracketsIndex == -1)
						{ // First bracket was found after first space OR is not existing thus we have a simple define with a replace value here
							m.name = restofline.substr(0, spaceIndex);
							m.content = restofline.substr(spaceIndex + 1);
						}
						else
						{
							//we got a define with arguments here
							auto bracketsEndIndex = restofline.find(')');
							auto argumentsString = restofline.substr(bracketsIndex + 1, bracketsEndIndex);

							size_t arg_index = bracketsIndex;
							size_t arg_start_index = bracketsIndex + 1;
							bool ended = false;
							while (!ended)
							{
								arg_index = restofline.find(',', arg_start_index);
								if (arg_index == std::string::npos || arg_index > bracketsEndIndex)
								{
									ended = true;
									arg_index = bracketsEndIndex;
								}
								auto arg = restofline.substr(arg_start_index, arg_index - arg_start_index);
								arg.erase(arg.begin(), std::find_if(arg.begin(), arg.end(), [](char c) -> bool {
									return c != '\t' && c != ' ';
								}));
								arg.erase(std::find_if(arg.rbegin(), arg.rend(), [](char c) -> bool {
									return c != '\t' && c != ' ';
								}).base(), arg.end());
								m.args.push_back(arg);
								arg_start_index = arg_index + 1;
							}
							m.name = restofline.substr(0, bracketsIndex);
							if (m.name == "LOG_SYS_FORMAT")
							{
								m.name = m.name;
							}
							m.content = restofline.substr(bracketsEndIndex + 2);
						}
					}
					macrolist.push_back(m);
					sstream << "\n";
				}
				else if (word == "UNDEFINE")
				{ // #undefine TEST
					auto res = std::find_if(macrolist.begin(), macrolist.end(), [word](macro& m) -> bool {
						return word == m.name;
					});
					if (res == macrolist.end())
					{
						errflag = true;
						vm->err() << "Macro '" << rest_of_line << "' not found." << std::endl;
						return "";
					}
					else
					{
						macrolist.erase(res);
					}
					sstream << "\n";
				}
				else if (word == "IFDEF")
				{ // #ifdef TEST
					if (is_inside_if)
					{
						errflag = true;
						vm->err() << "Unexpected IFNDEF. Already inside of a IFDEF or IFNDEF enclosure." << std::endl;
						return "";
					}
					else
					{
						is_inside_if = true;
					}
					write_to_output = std::find_if(macrolist.begin(), macrolist.end(), [word](macro& m) -> bool {
						return word == m.name;
					}) != macrolist.end();
					sstream << "\n";
				}
				else if (word == "IFNDEF")
				{ // #ifndef TEST
					if (is_inside_if)
					{
						errflag = true;
						vm->err() << "Unexpected IFNDEF. Already inside of a IFDEF or IFNDEF enclosure." << std::endl;
						return "";
					}
					else
					{
						is_inside_if = true;
					}
					write_to_output = std::find_if(macrolist.begin(), macrolist.end(), [word](macro& m) -> bool {
						return word == m.name;
					}) == macrolist.end();
					sstream << "\n";
				}
				else if (word == "ELSE")
				{ // #else
					write_to_output != write_to_output;
					if (!is_inside_if)
					{
						errflag = true;
						vm->err() << "Unexpected ELSE. Currently, not in a IFDEF or IFNDEF enclosure." << std::endl;
						return "";
					}
					sstream << "\n";
				}
				else if (word == "ENDIF")
				{ // #endif
					write_to_output = true;
					if (is_inside_if)
					{
						is_inside_if = false;
					}
					else
					{
						errflag = true;
						vm->err() << "Unexpected ENDIF. Currently, not in a IFDEF or IFNDEF enclosure." << std::endl;
						return "";
					}
					sstream << "\n";
				}
				word_start = off;
				was_new_line = true;
			}
			else if (word_start == off)
			{
				if (write_to_output && (c != '\n' || c != '\r'))
				{
					sstream << input[off];
				}
			}
			else if(write_to_output)
			{
				auto res = std::find_if(macrolist.begin(), macrolist.end(), [word](macro& m) -> bool { return m.name == word; });
				if (res == macrolist.end())
				{
					sstream << word;
				}
				else
				{
					// We hit a macro <3
					if (res->args.empty())
					{
						sstream << res->content;
					}
					else
					{
						// Our macro has args :O
						if (!res->handle_insert(vm, c, off, maxindex, line, col, filename, input, sstream, macrolist))
						{
							errflag = true;
							return "";
						}
						if (c == '\n')
						{
							line++;
							col = 0;
							sstream << "\n";
						}
					}
				}
				word_start = off;
				if (c == '/' && input.length() > off + 1)
				{
					if (input[off + 1] == '/')
					{
						line_comment = true;
					}
					else if (input[off + 1] == '*')
					{
						block_comment = true;
					}
				}
				else if(off != maxindex)
				{
					sstream << c;
				}
			}
			word_start++;
			col++;
		}
	}
	if (word_start != off)
	{
		auto word = input.substr(word_start, off - word_start);
		auto res = std::find_if(macrolist.begin(), macrolist.end(), [word](macro& m) -> bool { return m.name == word; });
		if (res == macrolist.end())
		{
			sstream << word;
		}
		else
		{
			// We hit a macro <3
			if (res->args.empty())
			{
				sstream << res->content;
			}
			else
			{
				// Our macro has args :O
				char c = '\0';
				if (!res->handle_insert(vm, c, off, maxindex, line, col, filename, input, sstream, macrolist))
				{
					errflag = true;
					return "";
				}
			}
		}
	}
	return sstream.str();
}

std::string sqf::parse::preprocessor::parse(sqf::virtualmachine* vm, std::string input, bool & errflag, std::string filename)
{
	std::vector<macro> macros;
	size_t off = 0;
	size_t line = 0;
	size_t col = 0;
	auto output = parse_file(vm, input, off, errflag, filename, macros, line, col, std::vector<std::string>());
	return output;
}
*/