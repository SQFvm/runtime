#include <algorithm>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <optional>
#include "compiletime.h"
#include "parsepreprocessor.h"
#include "virtualmachine.h"
#include "fileio.h"
#include "Entry.h"
#include "logging.h"
#include <iostream>

//#define PRINT_MACRO_CHAIN

#ifdef PRINT_MACRO_CHAIN
#include <iostream>
#endif

namespace err = logmessage::preprocessor;


void sqf::parse::preprocessor::replace_stringify(
	preprocessorfileinfo& local_fileinfo,
	preprocessorfileinfo& original_fileinfo,
	const macro& m,
	std::vector<std::string>& params,
	std::stringstream& sstream,
	const std::unordered_map<std::string, std::string>& param_map)
{
	char c;
	replace_skip(local_fileinfo, sstream);
	c = local_fileinfo.peek();
	if (c == '#')
	{
		local_fileinfo.next();
		replace_concat(local_fileinfo, original_fileinfo, m, params, sstream, param_map);
	}
	else
	{
		auto word_end = replace_find_wordend(local_fileinfo);
		std::string word;
		word.resize(word_end);
		for (int i = 0; i < word_end; i++)
		{
			word[i] = local_fileinfo.next();
		}
		auto param_res = std::find_if(
			m.args.begin(),
			m.args.end(),
			[word](std::string s) -> bool {
				return s.compare(word) == 0;
			}
		);
		if (param_res != m.args.end())
		{
			auto index = param_res - m.args.begin();
			sstream << '"' << params[index] << '"';
		}
		else
		{
			auto macro_res = std::find_if(
				m_macros.begin(),
				m_macros.end(),
				[word](std::unordered_map<std::string, macro>::value_type m) -> bool {
					return m.first.compare(word) == 0;
				}
			);
			if (macro_res == m_macros.end())
			{
				sstream << '"' << word << '"';
			}
			else
			{
				sstream << '"' << handle_macro(local_fileinfo, original_fileinfo, macro_res->second, param_map) << '"';
			}
		}
	}
}
void sqf::parse::preprocessor::replace_concat(
	preprocessorfileinfo& local_fileinfo,
	preprocessorfileinfo& original_fileinfo,
	const macro& m,
	std::vector<std::string>& params,
	std::stringstream& sstream,
	const std::unordered_map<std::string, std::string>& param_map)
{
	char c;
	replace_skip(local_fileinfo, sstream);
	c = local_fileinfo.peek();
	auto word_end = replace_find_wordend(local_fileinfo);
	std::string word;
	word.resize(word_end);
	for (int i = 0; i < word_end; i++)
	{
		word[i] = local_fileinfo.next();
	}
	auto param_res = std::find_if(
		m.args.begin(),
		m.args.end(),
		[word](std::string s) -> bool {
			return s.compare(word) == 0;
		}
	);
	if (param_res != m.args.end())
	{
		auto index = param_res - m.args.begin();
		sstream << params[index];
	}
	else
	{
		auto macro_res = std::find_if(
			m_macros.begin(),
			m_macros.end(),
			[word](std::unordered_map<std::string, macro>::value_type m) -> bool {
				return m.first.compare(word) == 0;
			}
		);
		if (macro_res == m_macros.end())
		{
			sstream << word;
		}
		else
		{
			sstream << handle_macro(local_fileinfo, original_fileinfo, macro_res->second, param_map);
		}
	}
}

size_t sqf::parse::preprocessor::replace_find_wordend(preprocessorfileinfo fileinfo)
{
	auto currentOffset = fileinfo.off;

	while (true)
	{
		char c = fileinfo.next();
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
			continue;
		case '\0':
			return fileinfo.off - currentOffset;
		default:
			return fileinfo.off - currentOffset - 1;
		}
	}
}
void sqf::parse::preprocessor::replace_skip(preprocessorfileinfo& fileinfo, std::stringstream& sstream)
{
	auto currentOffset = fileinfo.off;
	bool flag = true;
	bool in_string = false;
	while (flag)
	{
		if (in_string)
		{
			char c = fileinfo.next();
			if (c == '"')
			{
				in_string = false;
			}
			sstream << c;
		}
		else
		{
			char c = fileinfo.peek();
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
			case '\n': case '\\':
			case '#':
			case '\0':
				flag = false;
				break;
			case '\r':
				break;
			case '"':
				in_string = true;
			default:
				sstream << fileinfo.next();
			}
		}
	}
}
std::string sqf::parse::preprocessor::replace(preprocessorfileinfo& original_fileinfo, const macro& m, std::vector<std::string>& params)
{
	if (m.args.size() != params.size())
	{
		m_errflag = true;
		log(err::ArgCountMissmatch(original_fileinfo));
		return "";
	}
	preprocessorfileinfo local_fileinfo;
	local_fileinfo.content = m.content;
	local_fileinfo.path = m.filepath;
	local_fileinfo.line = m.line;
	if (m.callback)
	{
		return m.callback(m, &local_fileinfo, &original_fileinfo, params, m_vm);
	}

	std::unordered_map<std::string, std::string> parammap;
	for (size_t i = 0; i < params.size(); i++)
	{
		parammap[m.args[i]] = params[i];
	}

	std::stringstream sstream;
		
	char c;
	while (true)
	{
		replace_skip(local_fileinfo, sstream);
		c = local_fileinfo.peek();

		if (c == '#')
		{
			local_fileinfo.next();
			replace_stringify(local_fileinfo, original_fileinfo, m, params, sstream, parammap);
		}
		else if (c == '\n' || c == '\0')
		{
			local_fileinfo.next();
			break;
		}
		else
		{
			auto word_end = replace_find_wordend(local_fileinfo);
			if (word_end == 0)
			{
				sstream << local_fileinfo.next();;
			}
			else
			{
				std::string word;
				word.resize(word_end);
				for (int i = 0; i < word_end; i++)
				{
					word[i] = local_fileinfo.next();
				}
				auto param_res = std::find_if(
					m.args.begin(),
					m.args.end(),
					[word](std::string s) -> bool {
						return s.compare(word) == 0;
					}
				);
				if (param_res != m.args.end())
				{
					auto index = param_res - m.args.begin();
					sstream << params[index];
				}
				else
				{
					// ToDo: Handle args in macro params
					auto macro_res = m_macros.find(word);
					if (macro_res == m_macros.end())
					{
						sstream << word;
					}
					else
					{
						sstream << handle_macro(local_fileinfo, original_fileinfo, macro_res->second, parammap);
					}
				}
			}
		}
	}

	return sstream.str();
}
std::string sqf::parse::preprocessor::handle_arg(preprocessorfileinfo& local_fileinfo, preprocessorfileinfo& original_fileinfo, size_t endindex, const std::unordered_map<std::string, std::string>& param_map)
{
	size_t word_start = local_fileinfo.off;
	bool inside_word = false;
	bool string_mode = false;
	bool part_of_word = false;
	std::stringstream sstream;
	char c;
	while (local_fileinfo.off != endindex && (c = local_fileinfo.next()) != '\0')
	{
		if (string_mode)
		{
			if (c == '"')
			{
				string_mode = false;
			}
			sstream << c;
			continue;
		}
		switch (c)
		{
			case '"':
				string_mode = true;
				sstream << c;
			break;
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
					word_start = local_fileinfo.off - 1;
				}
				if (local_fileinfo.off != endindex)
				{
					break;
				} // Intended conditional fallthrough
				part_of_word = true;
			default:
				if (inside_word)
				{
					inside_word = false;
					auto word = local_fileinfo.content.substr(word_start, local_fileinfo.off - word_start - (!part_of_word ? 1 : 0));
					auto res = contains_macro(word);
					if (res.has_value())
					{
						if (res.value().hasargs)
						{
							local_fileinfo.move_back();
						}
						auto handled = handle_macro(local_fileinfo, original_fileinfo, res.value(), param_map);
						if (m_errflag)
						{
							return "";
						}
						sstream << handled;
						if (!res->hasargs && !part_of_word)
						{
							local_fileinfo.move_back();
						}
					}
					else if (param_map.find(word) != param_map.end())
					{
						sstream << param_map.at(word);
						if (!part_of_word)
						{
							local_fileinfo.move_back();
						}
					}
					else
					{
						sstream << word;
						if (!part_of_word)
						{
							local_fileinfo.move_back();
						}
					}
				}
				else
				{
					sstream << c;
				}
				part_of_word = false;
				break;
		}
	}
	return sstream.str();
}
std::string sqf::parse::preprocessor::handle_macro(preprocessorfileinfo& local_fileinfo, preprocessorfileinfo& original_fileinfo, const macro& m, const std::unordered_map<std::string, std::string>& param_map)
{ // Needs to handle 'NAME(ARG1, ARG2, ARGN)' not more, not less!
	std::vector<std::string> params;
	if (m.hasargs)
	{
		if (local_fileinfo.peek() != '(')
		{
			return m.name;
		}
		local_fileinfo.next(); // Skip the initial '('
		size_t rb_counter = 0;
		size_t cb_counter = 0;
		size_t eb_counter = 0;
		size_t lastargstart = local_fileinfo.off;
		bool exit = false;
		char c;
		bool in_string = false;
		while (!exit && (c = local_fileinfo.next()) != '\0')
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
					local_fileinfo.move_back();
					if (local_fileinfo.off - lastargstart > 0)
					{
						preprocessorfileinfo copy = local_fileinfo;
						copy.off = lastargstart;
						auto handled_param = handle_arg(copy, original_fileinfo, local_fileinfo.off, param_map);
						params.emplace_back(std::move(handled_param));
					}
					local_fileinfo.next();
					lastargstart = local_fileinfo.off;
				}
				break;
			}
		}
	}
#ifdef PRINT_MACRO_CHAIN
	auto replace_result = replace(original_fileinfo, m, params);
	std::cout << "Resolving '" << m.name << "'" << std::endl;
	std::cout << m.name << " - Definition: #define " << m.name;
	if (m.hasargs)
	{
		std::cout << "(";
		for (size_t i = 0; i < m.args.size(); i++)
		{
			if (i != 0)
			{
				std::cout << ",";
			}
			std::cout << m.args[i];
		}
		std::cout << ")";
	}
	std::cout << " " << m.content << std::endl;
	std::cout << m.name << " - Input: " << m.name;
	if (m.hasargs)
	{
		std::cout << "(";
		for (size_t i = 0; i < params.size(); i++)
		{
			if (i != 0)
			{
				std::cout << ",";
			}
			std::cout << params[i];
		}
		std::cout << ")";
	}
	std::cout << std::endl << m.name << " - Result: " << replace_result << std::endl;
	return replace_result;
#else
	return replace(original_fileinfo, m, params);
#endif
}
std::string sqf::parse::preprocessor::parse_ppinstruction(preprocessorfileinfo& fileinfo)
{
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
		if (!m_allowwrite)
		{
			return "\n";
		}
		line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](char c) -> bool {
			return c != '"';
		}));
        auto endIter = std::find_if(line.begin(), line.end(), [](char c) -> bool {
            return c == '"';
        });
        if (std::distance(endIter, line.end()) > 1)
			log(err::UnexpectedDataAfterInclude(fileinfo));
        line.erase(endIter, line.end());
		preprocessorfileinfo otherfinfo;
		try
		{
			otherfinfo.path = m_vm->get_filesystem().get_physical_path(line, fileinfo.path);
			const auto& path = otherfinfo.path;
			auto res = std::find_if(m_path_tree.begin(), m_path_tree.end(), [path](std::string& parent) -> bool { return parent == path; });
			if (res != m_path_tree.end())
			{
				m_errflag = true;
				std::stringstream includeTree;
				for (size_t i = 0; i < m_path_tree.size(); i++)
				{
					includeTree << i << ". " << m_path_tree[i] << std::endl;
				}
				log(err::RecursiveInclude(fileinfo, includeTree.str()));
				return "";
			}
            otherfinfo.content = load_file(path);
		}
		catch (const std::runtime_error& ex)
		{
			m_errflag = true;
			log(err::IncludeFailed(fileinfo, line, ex));
			return "";
		}
		std::string output;
		auto lineInfo = std::to_string(fileinfo.line - 1);
		auto parsedFile = parse_file(otherfinfo);
		output.reserve(
			parsedFile.size() + compiletime::strlen("\n") +
			compiletime::strlen("#line  ") + lineInfo.size() + fileinfo.path.size() + compiletime::strlen("\n")
		);
		output.reserve(
			compiletime::strlen("#line 0 ") + otherfinfo.path.size() + compiletime::strlen("\n") +
			parsedFile.size() + compiletime::strlen("\n") +
			compiletime::strlen("#line ") + lineInfo.size() + compiletime::strlen(" ") + fileinfo.path.size() + compiletime::strlen("\n")
		);
		output.append("#line 0 "); output.append(otherfinfo.path); output.append("\n");
		output.append(parsedFile); output.append("\n");
		output.append("#line "); output.append(lineInfo); output.append(" "); output.append(fileinfo.path); output.append("\n");
		return output;
	}
	else if (inst == "DEFINE")
	{ // #define TEST(A, B, C) A #B##C
		if (!m_allowwrite)
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
			if (m_macros.find(m.name) != m_macros.end())
			{
				log(err::MacroDefinedTwice(fileinfo, m.name));
			}
		}
		else
		{
			if (spaceIndex < bracketsIndex || bracketsIndex == std::string::npos) // std::string::npos does not need to be catched as bracketsIndex always < npos here
			{ // First bracket was found after first space OR is not existing thus we have a simple define with a replace value here
				m.name = line.substr(0, spaceIndex);
				if (m_macros.find(m.name) != m_macros.end())
				{
					log(err::MacroDefinedTwice(fileinfo, m.name));
				}
				m.content = line.substr(line[spaceIndex] == ' ' ? spaceIndex + 1 : spaceIndex); //Special magic for '#define macro\'
				m.hasargs = false;
			}
			else
			{ // We got a define with arguments here
				m.hasargs = true;
				m.name = line.substr(0, bracketsIndex);
				if (m_macros.find(m.name) != m_macros.end())
				{
					log(err::MacroDefinedTwice(fileinfo, m.name));
				}
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
					if (!arg.empty())
					{
						m.args.emplace_back(std::move(arg));
						arg_start_index = arg_index + 1;
					}
				}
				m.content = line.length() <= bracketsEndIndex + 2 ? "" : line.substr(bracketsEndIndex + 2);
					
			}
		}
		m_macros[m.name] = std::move(m);
		return "\n";
	}
	else if (inst == "UNDEF")
	{ // #undef TEST
		if (!m_allowwrite)
		{
			return "\n";
		}
			
		auto res = m_macros.find(line);
		if (res == m_macros.end())
		{
			log(err::MacroNotFound(fileinfo, line));
		}
		else
		{
			m_macros.erase(res);
		}
		return "\n";
	}
	else if (inst == "IFDEF")
	{ // #ifdef TEST
		if (inside_ppif())
		{
			m_errflag = true;
			log(err::UnexpectedIfdef(fileinfo));
			return "";
		}
		else
		{
			inside_ppif(true);
		}
		auto res = m_macros.find(line);
		if (res == m_macros.end())
		{
			m_allowwrite = false;
		}
		else
		{
			m_allowwrite = true;
		}
		return "\n";
	}
	else if (inst == "IFNDEF")
	{ // #ifndef TEST
		if (inside_ppif())
		{
			m_errflag = true;
			log(err::UnexpectedIfndef(fileinfo));
			return "";
		}
		else
		{
			inside_ppif(true);
		}
		auto res = m_macros.find(line);
		if (res == m_macros.end())
		{
			m_allowwrite = true;
		}
		else
		{
			m_allowwrite = false;
		}
		return "\n";
	}
	else if (inst == "ELSE")
	{ // #else
		if (!inside_ppif())
		{
			m_errflag = true;
			log(err::UnexpectedElse(fileinfo));
			return "";
		}
		m_allowwrite = !m_allowwrite;
		return "\n";
	}
	else if (inst == "ENDIF")
	{ // #endif
		if (!inside_ppif())
		{
			m_errflag = true;
			log(err::UnexpectedEndif(fileinfo));
			return "";
		}
		inside_ppif(false);
		m_allowwrite = true;
		return "\n";
	}
	else
	{
		m_errflag = true;
		log(err::UnknownInstruction(fileinfo, inst));
		return "";
	}
}
std::string sqf::parse::preprocessor::parse_file(preprocessorfileinfo& fileinfo)
{
	push_path(fileinfo.path);
	char c;
	std::stringstream sstream;
	std::stringstream wordstream;
	std::unordered_map<std::string, std::string> empty_parammap;
	sstream << "#line 0 " << fileinfo.path << std::endl;
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
            if (m_allowwrite)
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
                if (m_allowwrite)
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
					auto res = parse_ppinstruction(fileinfo);
					if (m_errflag)
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
				if (m_allowwrite)
				{
					if (wordstream.rdbuf()->in_avail() > 0)
					{
						auto word = wordstream.str();
						wordstream.str("");
						auto m = contains_macro(word);
						if (m.has_value())
						{
							fileinfo.move_back();
							auto res = handle_macro(fileinfo, fileinfo, m.value(), empty_parammap);
							if (m_errflag)
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
                if (m_allowwrite)
					wordstream << c;
				was_new_line = false;
			} break;
		}
	}

	auto word = wordstream.str();
	if (!word.empty())
	{
		auto m = contains_macro(word);
		if (m.has_value())
		{
			fileinfo.move_back();
			auto res = handle_macro(fileinfo, fileinfo, m.value(), empty_parammap);
			if (m_errflag)
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
	pop_path(fileinfo);
	return sstream.str();
}

std::string line_macro_callback(
	const sqf::parse::macro& m,
	const sqf::parse::preprocessorfileinfo* local_fileinfo,
	const sqf::parse::preprocessorfileinfo* original_fileinfo,
	const std::vector<std::string>& params,
	sqf::virtualmachine* vm)
{
	return std::to_string(original_fileinfo->line);
}
std::string file_macro_callback(
	const sqf::parse::macro& m,
	const sqf::parse::preprocessorfileinfo* local_fileinfo,
	const sqf::parse::preprocessorfileinfo* original_fileinfo,
	const std::vector<std::string>& params,
	sqf::virtualmachine* vm)
{
	return '"' + original_fileinfo->path + '"';
}
std::string sqf::parse::preprocessor::parse(sqf::virtualmachine* vm, std::string input, bool & errflag, std::string filename)
{
	m_vm = vm;
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = "";
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "__LINE__";
		macro.callback = line_macro_callback;
		m_macros["__LINE__"] = macro;
	}
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = "";
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "__FILE__";
		macro.callback = file_macro_callback;
		m_macros["__FILE__"] = macro;
	}
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = VERSION_FULL;
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "_SQF_VM";
		m_macros["_SQF_VM"] = macro;
	}
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = STR(VERSION_MAJOR);
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "_SQF_VM_MAJOR";
		m_macros["_SQF_VM_MAJOR"] = macro;
	}
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = STR(VERSION_MINOR);
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "_SQF_VM_MINOR";
		m_macros["_SQF_VM_MINOR"] = macro;
	}
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = STR(VERSION_REVISION);
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "_SQF_VM_REVISION";
		m_macros["_SQF_VM_REVISION"] = macro;
	}
	for (auto& macro : vm->preprocessor_macros())
	{
		m_macros[macro.name] = macro;
	}
	preprocessorfileinfo fileinfo;
	fileinfo.content = std::move(input);
	fileinfo.path = std::move(filename);
	auto res = parse_file(fileinfo);
	errflag = m_errflag;
	if (inside_ppif_err_flag())
	{
		return "";
	}
	return res;
}

void sqf::parse::preprocessor::pop_path(preprocessorfileinfo& preprocessorfileinfo)
{
	if (inside_ppif())
	{
		log(logmessage::preprocessor::MissingEndif(preprocessorfileinfo));
		m_inside_ppif_err_flag = true;
	}
	m_path_tree.pop_back();
	m_inside_ppf_tree.pop_back();
}
