#include "default.h"
#include "../../runtime/util.h"
#include "../../runtime/runtime.h"
#include "../../runtime/fileio.h"
#include "../../runtime/value.h"

#include <algorithm>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <optional>
#include <iostream>

//#define PRINT_MACRO_CHAIN

#ifdef PRINT_MACRO_CHAIN
#include <iostream>
#endif

namespace err = logmessage::preprocessor;
using namespace std::string_literals;

void sqf::parser::preprocessor::default::replace_stringify(
	::sqf::runtime::runtime & runtime,
	preprocessorfileinfo & local_fileinfo,
	preprocessorfileinfo & original_fileinfo,
	const ::sqf::runtime::parser::macro & m,
	std::vector<std::string> & params,
	std::stringstream & sstream,
	const std::unordered_map<std::string, std::string> & param_map)
{
	char c;
	replace_skip(runtime, local_fileinfo, sstream);
	c = local_fileinfo.peek();
	if (c == '#')
	{
		local_fileinfo.next();
		replace_concat(runtime, local_fileinfo, original_fileinfo, m, params, sstream, param_map);
	}
	else
	{
		auto word_end = replace_find_wordend(runtime, local_fileinfo);
		std::string word;
		word.resize(word_end);
		for (size_t i = 0; i < word_end; i++)
		{
			word[i] = local_fileinfo.next();
		}
		auto param_res = std::find_if(
			m.args().begin(),
			m.args().end(),
			[word](std::string s) -> bool {
				return s.compare(word) == 0;
			}
		);
		if (param_res != m.args().end())
		{
			auto index = param_res - m.args().begin();
			sstream << '"' << params[index] << '"';
		}
		else
		{
			auto macro_res = std::find_if(
				m_macros.begin(),
				m_macros.end(),
				[word](std::unordered_map<std::string, ::sqf::runtime::parser::macro>::value_type m) -> bool {
					return m.first.compare(word) == 0;
				}
			);
			if (macro_res == m_macros.end())
			{
				sstream << '"' << word << '"';
			}
			else
			{
				sstream << '"' << handle_macro(runtime, local_fileinfo, original_fileinfo, macro_res->second, param_map) << '"';
			}
		}
	}
}
void sqf::parser::preprocessor::default::replace_concat(
	::sqf::runtime::runtime & runtime,
	preprocessorfileinfo & local_fileinfo,
	preprocessorfileinfo & original_fileinfo,
	const ::sqf::runtime::parser::macro & m,
	std::vector<std::string> & params,
	std::stringstream & sstream,
	const std::unordered_map<std::string, std::string> & param_map)
{
	char c;
	replace_skip(runtime, local_fileinfo, sstream);
	c = local_fileinfo.peek();
	auto word_end = replace_find_wordend(runtime, local_fileinfo);
	std::string word;
	word.resize(word_end);
	for (size_t i = 0; i < word_end; i++)
	{
		word[i] = local_fileinfo.next();
	}
	auto param_res = std::find_if(
		m.args().begin(),
		m.args().end(),
		[word](std::string s) -> bool {
			return s.compare(word) == 0;
		}
	);
	if (param_res != m.args().end())
	{
		auto index = param_res - m.args().begin();
		sstream << params[index];
	}
	else
	{
		auto macro_res = std::find_if(
			m_macros.begin(),
			m_macros.end(),
			[word](std::unordered_map<std::string, ::sqf::runtime::parser::macro>::value_type m) -> bool {
				return m.first.compare(word) == 0;
			}
		);
		if (macro_res == m_macros.end())
		{
			sstream << word;
		}
		else
		{
			sstream << handle_macro(runtime, local_fileinfo, original_fileinfo, macro_res->second, param_map);
		}
	}
}

size_t sqf::parser::preprocessor::default::replace_find_wordend(::sqf::runtime::runtime & runtime, preprocessorfileinfo fileinfo)
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
void sqf::parser::preprocessor::default::replace_skip(::sqf::runtime::runtime & runtime, preprocessorfileinfo & fileinfo, std::stringstream & sstream)
{
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
std::string sqf::parser::preprocessor::default::replace(::sqf::runtime::runtime & runtime, preprocessorfileinfo & original_fileinfo, const ::sqf::runtime::parser::macro & m, std::vector<std::string> & params)
{
	if (m.args().size() != params.size())
	{
		m_errflag = true;
		log(err::ArgCountMissmatch(m.diag_info()));
		return "";
	}
	preprocessorfileinfo local_fileinfo(m.diag_info());
	local_fileinfo.content = m.content();
	local_fileinfo.line = m.diag_info().line;
	if (m.has_callback())
	{
		return m(original_fileinfo, original_fileinfo, params, runtime);
	}

	std::unordered_map<std::string, std::string> parammap;
	for (size_t i = 0; i < params.size(); i++)
	{
		parammap[m.args()[i]] = params[i];
	}

	std::stringstream sstream;

	char c;
	while (true)
	{
		replace_skip(runtime, local_fileinfo, sstream);
		c = local_fileinfo.peek();

		if (c == '#')
		{
			local_fileinfo.next();
			replace_stringify(runtime, local_fileinfo, original_fileinfo, m, params, sstream, parammap);
		}
		else if (c == '\n' || c == '\0')
		{
			local_fileinfo.next();
			break;
		}
		else
		{
			auto word_end = replace_find_wordend(runtime, local_fileinfo);
			if (word_end == 0)
			{
				sstream << local_fileinfo.next();;
			}
			else
			{
				std::string word;
				word.resize(word_end);
				for (size_t i = 0; i < word_end; i++)
				{
					word[i] = local_fileinfo.next();
				}
				auto param_res = std::find_if(
					m.args().begin(),
					m.args().end(),
					[word](std::string s) -> bool {
						return s.compare(word) == 0;
					}
				);
				if (param_res != m.args().end())
				{
					auto index = param_res - m.args().begin();
					sstream << params[index];
				}
				else
				{
					// ToDo: Handle args in ::sqf::runtime::parser::macro params
					auto macro_res = m_macros.find(word);
					if (macro_res == m_macros.end())
					{
						sstream << word;
					}
					else
					{
						sstream << handle_macro(runtime, local_fileinfo, original_fileinfo, macro_res->second, parammap);
					}
				}
			}
		}
	}

	return sstream.str();
}
std::string sqf::parser::preprocessor::default::handle_arg(::sqf::runtime::runtime & runtime, preprocessorfileinfo & local_fileinfo, preprocessorfileinfo & original_fileinfo, size_t endindex, const std::unordered_map<std::string, std::string> & param_map)
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
				auto res = get_try(word);
				if (res.has_value())
				{
					if (!res.value().args().empty())
					{
						local_fileinfo.move_back();
					}
					auto handled = handle_macro(runtime, local_fileinfo, original_fileinfo, res.value(), param_map);
					if (m_errflag)
					{
						return "";
					}
					sstream << handled;
					if (res.value().args().empty() && !part_of_word)
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
std::string sqf::parser::preprocessor::default::handle_macro(::sqf::runtime::runtime & runtime, preprocessorfileinfo & local_fileinfo, preprocessorfileinfo & original_fileinfo, const ::sqf::runtime::parser::macro & m, const std::unordered_map<std::string, std::string> & param_map)
{ // Needs to handle 'NAME(ARG1, ARG2, ARGN)' not more, not less!
	std::vector<std::string> params;
	if (!m.args().empty())
	{
		if (local_fileinfo.peek() != '(')
		{
			return std::string(m.name());
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
						auto handled_param = handle_arg(runtime, copy, original_fileinfo, local_fileinfo.off, param_map);
						params.emplace_back(std::move(handled_param));
					}
					else if (m.args().size() != 0)
					{
						params.emplace_back("");
						log(err::EmptyArgument(original_fileinfo.operator ::sqf::runtime::diagnostics::diag_info()));
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
		for (size_t i = 0; i < m.args().size(); i++)
		{
			if (i != 0)
			{
				std::cout << ",";
			}
			std::cout << m.args[i];
		}
		std::cout << ")";
	}
	std::cout << " " << m.content() << std::endl;
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
	return replace(runtime, original_fileinfo, m, params);
#endif
}

namespace {
	const std::string WHITESPACE = " \t";

	std::string_view ltrim(std::string_view s)
	{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string_view::npos) ? "" : s.substr(start);
	}

	std::string_view rtrim(std::string_view s)
	{
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string_view::npos) ? "" : s.substr(0, end + 1);
	}

	std::string_view trim(std::string_view s)
	{
		return rtrim(ltrim(s));
	}
}

std::string sqf::parser::preprocessor::default::parse_ppinstruction(::sqf::runtime::runtime & runtime, preprocessorfileinfo & fileinfo)
{
	auto inst = fileinfo.get_word();
	std::string line{ trim(fileinfo.get_line(true)) };
	std::transform(inst.begin(), inst.end(), inst.begin(), std::toupper);
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
			log(err::UnexpectedDataAfterInclude(fileinfo.operator ::sqf::runtime::diagnostics::diag_info()));
		line.erase(endIter, line.end());
		try
		{
			auto include_path_info = runtime.fileio().get_info(line, fileinfo);
			if (!include_path_info.has_value())
			{
				m_errflag = true;
				log(err::IncludeFailed(fileinfo.operator ::sqf::runtime::diagnostics::diag_info(), line, "FileIO returned no file."));
				return "";
			}
			const auto& physical = include_path_info->physical;
			auto res = std::find_if(m_path_tree.begin(), m_path_tree.end(), [physical](std::string& parent) -> bool { return parent == physical; });
			if (res != m_path_tree.end())
			{
				m_errflag = true;
				std::stringstream includeTree;
				for (size_t i = 0; i < m_path_tree.size(); i++)
				{
					includeTree << i << ". " << m_path_tree[i] << std::endl;
				}
				log(err::RecursiveInclude(fileinfo.operator ::sqf::runtime::diagnostics::diag_info(), includeTree.str()));
				return "";
			}
			preprocessorfileinfo otherfinfo(*include_path_info);
			otherfinfo.content = runtime.fileio().read_file(*include_path_info);


			std::string output;
			auto lineInfo = std::to_string(fileinfo.line - 1);
			auto parsedFile = parse_file(runtime, otherfinfo);
			output.reserve(
				::sqf::runtime::util::strlen("#line 1 \"") + physical.size() + ::sqf::runtime::util::strlen("\"\n") +
				parsedFile.size() + ::sqf::runtime::util::strlen("\n") +
				::sqf::runtime::util::strlen("#line ") + lineInfo.size() + ::sqf::runtime::util::strlen(" \"") + fileinfo.pathinf.physical.size() + ::sqf::runtime::util::strlen("\"\n")
			);
			output.append("#line 1 \""); output.append(physical); output.append("\"\n");
			output.append(parsedFile); output.append("\n");
			output.append("#line "); output.append(lineInfo); output.append(" \""); output.append(fileinfo.pathinf.physical); output.append("\"\n");
			return output;
		}
		catch (const std::runtime_error& ex)
		{
			m_errflag = true;
			log(err::IncludeFailed(fileinfo.operator ::sqf::runtime::diagnostics::diag_info(), line, ex));
			return "";
		}
	}
	else if (inst == "DEFINE")
	{ // #define TEST(A, B, C) A #B##C
		if (!m_allowwrite)
		{
			return "\n";
		}
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
			if (get_try(line).has_value())
			{
				log(err::MacroDefinedTwice(fileinfo.operator ::sqf::runtime::diagnostics::diag_info(), line));
			}
			m_macros[line] = { fileinfo, line };
		}
		else
		{
			if (spaceIndex < bracketsIndex || bracketsIndex == std::string::npos) // std::string::npos does not need to be catched as bracketsIndex always < npos here
			{ // First bracket was found after first space OR is not existing thus we have a simple define with a replace value here
				auto name_tmp = line.substr(0, spaceIndex);
				if (get_try(name_tmp).has_value())
				{
					log(err::MacroDefinedTwice(fileinfo.operator ::sqf::runtime::diagnostics::diag_info(), name_tmp));
				}
				auto content = trim(line.substr(line[spaceIndex] == ' ' ? spaceIndex + 1 : spaceIndex)); //Special magic for '#define ::sqf::runtime::parser::macro\'
				m_macros[line] = { fileinfo, name_tmp, std::string(content) };
			}
			else
			{ // We got a define with arguments here
				auto name_tmp = line.substr(0, bracketsIndex);
				if (get_try(name_tmp).has_value())
				{
					log(err::MacroDefinedTwice(fileinfo.operator ::sqf::runtime::diagnostics::diag_info(), name_tmp));
				}
				auto bracketsEndIndex = line.find(')');
				auto argumentsString = line.substr(bracketsIndex + 1, bracketsEndIndex);

				size_t arg_index = bracketsIndex;
				size_t arg_start_index = bracketsIndex + 1;
				bool ended = false;
				std::vector<std::string> args;
				while (!ended)
				{
					arg_index = line.find(',', arg_start_index);
					if (arg_index == std::string::npos || arg_index > bracketsEndIndex)
					{
						ended = true;
						arg_index = bracketsEndIndex;
					}
					std::string arg{ trim(line.substr(arg_start_index, arg_index - arg_start_index)) };
					if (!arg.empty())
					{
						args.emplace_back(std::move(arg));
						arg_start_index = arg_index + 1;
					}
				}
				auto content = trim(line.length() <= bracketsEndIndex + 1 ? "" : line.substr(bracketsEndIndex + 1));

				m_macros[line] = { fileinfo, name_tmp, std::string(content) };
			}
		}
		return "\n";
	}
	else if (inst == "UNDEF")
	{ // #undef TEST
		if (!m_allowwrite)
		{
			return "\n";
		}

		auto res = m_macros.find(static_cast<std::string>(line));
		if (res == m_macros.end())
		{
			log(err::MacroNotFound(fileinfo.operator ::sqf::runtime::diagnostics::diag_info(), line));
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
			log(err::UnexpectedIfdef(fileinfo.operator ::sqf::runtime::diagnostics::diag_info()));
			return "";
		}
		else
		{
			inside_ppif(true);
		}
		auto res = m_macros.find(static_cast<std::string>(line));
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
			log(err::UnexpectedIfndef(fileinfo.operator ::sqf::runtime::diagnostics::diag_info()));
			return "";
		}
		else
		{
			inside_ppif(true);
		}
		auto res = m_macros.find(static_cast<std::string>(line));
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
			log(err::UnexpectedElse(fileinfo.operator ::sqf::runtime::diagnostics::diag_info()));
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
			log(err::UnexpectedEndif(fileinfo.operator ::sqf::runtime::diagnostics::diag_info()));
			return "";
		}
		inside_ppif(false);
		m_allowwrite = true;
		return "\n";
	}
	else
	{
		m_errflag = true;
		log(err::UnknownInstruction(fileinfo.operator ::sqf::runtime::diagnostics::diag_info(), inst));
		return "";
	}
}
std::string sqf::parser::preprocessor::default::parse_file(::sqf::runtime::runtime & runtime, preprocessorfileinfo & fileinfo)
{
	push_path(fileinfo.pathinf.physical);
	char c;
	std::stringstream sstream;
	std::stringstream wordstream;
	std::unordered_map<std::string, std::string> empty_parammap;
	sstream << "#line 0 \"" << fileinfo.pathinf.physical << "\"\n";
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
				auto res = parse_ppinstruction(runtime, fileinfo);
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
					auto m = get_try(word);
					if (m.has_value())
					{
						fileinfo.move_back();
						auto res = handle_macro(runtime, fileinfo, fileinfo, m.value(), empty_parammap);
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
		auto m = get_try(word);
		if (m.has_value())
		{
			fileinfo.move_back();
			auto res = handle_macro(runtime, fileinfo, fileinfo, m.value(), empty_parammap);
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
	const ::sqf::runtime::parser::macro & m,
	const ::sqf::runtime::diagnostics::diag_info dinf,
	const ::sqf::runtime::fileio::pathinfo local,
	const std::vector<std::string> & params,
	::sqf::runtime::runtime & runtime)
{
	return std::to_string(dinf.line);
}
std::string file_macro_callback(
	const ::sqf::runtime::parser::macro & m,
	const ::sqf::runtime::diagnostics::diag_info dinf,
	const ::sqf::runtime::fileio::pathinfo local,
	const std::vector<std::string> & params,
	::sqf::runtime::runtime & runtime)
{
	return '"' + local.physical + '"';
}
std::string eval_macro_callback(
	const ::sqf::runtime::parser::macro & m,
	const ::sqf::runtime::diagnostics::diag_info dinf,
	const ::sqf::runtime::fileio::pathinfo local,
	const std::vector<std::string> & params,
	::sqf::runtime::runtime & runtime)
{
	if (params.empty())
	{
		return "";
	}
	bool success = false;
	// Cannot wait for stop here as either we are most of the time already 
	// running inside the VM and thus cannot give way for the evaluate_expression method.
	// ToDo: Fix "edge case" where the user uses a running VM to preprocess a file that contains __EVAL to not break the SQF-VM execution.
	auto res = runtime.evaluate_expression(params[0], success, false);
	return success ? res.data()->to_string_sqf() : "";
}


void sqf::parser::preprocessor::default::pop_path(preprocessorfileinfo & preprocessorfileinfo)
{
	if (inside_ppif())
	{
		log(logmessage::preprocessor::MissingEndif(preprocessorfileinfo.operator ::sqf::runtime::diagnostics::diag_info()));
		m_inside_ppif_err_flag = true;
	}
	m_path_tree.pop_back();
	m_inside_ppf_tree.pop_back();
}
sqf::parser::preprocessor::default::default(::Logger & logger) : CanLog(logger)
{
	m_macros["__LINE__"s] = { "__LINE__"s, line_macro_callback };
	m_macros["__EXEC"s] = { "__EXEC"s, { "EXPRESSION"s }, eval_macro_callback };
	m_macros["__EVAL"s] = { "__EVAL"s, { "EXPRESSION"s }, eval_macro_callback };
	m_macros["_SQFVM"s] = { "_SQFVM"s };
	m_macros["_SQFVM_RUNTIME_VERSION_MAJOR"s] = { "_SQFVM_RUNTIME_VERSION_MAJOR"s, STR(SQFVM_RUNTIME_VERSION_MAJOR) };
	m_macros["_SQFVM_RUNTIME_VERSION_MINOR"s] = { "_SQFVM_RUNTIME_VERSION_MINOR"s, STR(SQFVM_RUNTIME_VERSION_MINOR) };
	m_macros["_SQFVM_RUNTIME_VERSION_REVISION"s] = { "_SQFVM_RUNTIME_VERSION_REVISION"s, STR(SQFVM_RUNTIME_VERSION_REVISION) };
}
std::optional<std::string> sqf::parser::preprocessor::default::preprocess(::sqf::runtime::runtime& runtime, std::string_view view, ::sqf::runtime::fileio::pathinfo pathinfo)
{
	preprocessorfileinfo fileinfo(pathinfo);
	fileinfo.content = view;
	auto res = parse_file(runtime, fileinfo);
	if (inside_ppif_err_flag())
	{
		return {};
	}
	return res;
}
