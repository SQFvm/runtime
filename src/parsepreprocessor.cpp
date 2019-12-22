#include <algorithm>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <optional>
#include <unordered_map>
#include "compiletime.h"
#include "parsepreprocessor.h"
#include "virtualmachine.h"
#include "fileio.h"
#include "Entry.h"

//#define PRINT_MACRO_CHAIN

#ifdef PRINT_MACRO_CHAIN
#include <iostream>
#endif

using namespace sqf::parse::preprocessor;

namespace sqf {
    namespace parse {
        namespace preprocessor {
            namespace settings {
                bool disable_warn_define = false;
				std::vector<macro> predefined_macros = std::vector<macro>();
            }
        }
    }
}

namespace {
	class helper
	{
	private:
		std::vector<std::string> m_path_tree;
		std::vector<bool> m_inside_ppf_tree;
		bool m_inside_ppif_err_flag = false;
	public:
		std::unordered_map<std::string, macro> macros;
		sqf::virtualmachine* vm { nullptr };
		bool errflag = false;
		bool allowwrite = true;

		bool inside_ppif_err_flag()
		{
			return m_inside_ppif_err_flag;
		}
		bool inside_ppif()
		{
			return m_inside_ppf_tree.back();
		}
		void inside_ppif(bool flag)
		{
			m_inside_ppf_tree.back() = flag;
		}
		void push_path(const std::string s)
		{
			m_path_tree.push_back(s);
			m_inside_ppf_tree.push_back(false);
		}
		void pop_path()
		{
			if (inside_ppif())
			{
				vm->wrn() << "[WRN][L0 | C0]\t" << "Unclosed #IFDEF/#IFNDEF in '" << m_path_tree.back() << "'" << std::endl;
				m_inside_ppif_err_flag = true;
			}
			m_path_tree.pop_back();
			m_inside_ppf_tree.pop_back();
		}
		std::vector<std::string>& path_tree()
		{
			return m_path_tree;
		}


		std::optional<macro> contains_macro(std::string mname)
		{
			auto res = macros.find(mname);
			if (res == macros.end())
			{
				return {};
			}
			else
			{
				return res->second;
			}
		}
	};

	std::string handle_macro(helper& h, finfo& local_fileinfo, finfo& original_fileinfo, const macro& m, const std::unordered_map<std::string, std::string>& param_map);
	std::string replace(helper& h, finfo& fileinfo, const macro& m, std::vector<std::string>& params);
	std::string handle_arg(helper& h, finfo& local_fileinfo, finfo& original_fileinfo, size_t endindex, const std::unordered_map<std::string, std::string>& param_map);
	std::string parse_ppinstruction(helper& h, finfo& fileinfo);
	std::string parse_file(helper& h, finfo& fileinfo);
	void replace_stringify(helper& h, finfo& local_fileinfo, finfo& original_fileinfo, const macro& m, std::vector<std::string>& params, std::stringstream& sstream, const std::unordered_map<std::string, std::string>& param_map);
	void replace_concat(helper& h, finfo& local_fileinfo, finfo& original_fileinfo, const macro& m, std::vector<std::string>& params, std::stringstream& sstream, const std::unordered_map<std::string, std::string>& param_map);
	size_t replace_find_wordend(finfo fileinfo);
	void replace_skip(finfo& fileinfo, std::stringstream& sstream);

	void replace_stringify(helper& h, finfo& local_fileinfo, finfo& original_fileinfo, const macro& m, std::vector<std::string>& params, std::stringstream& sstream, const std::unordered_map<std::string, std::string>& param_map)
	{
		char c;
		replace_skip(local_fileinfo, sstream);
		c = local_fileinfo.peek();
		if (c == '#')
		{
			local_fileinfo.next();
			replace_concat(h, local_fileinfo, original_fileinfo, m, params, sstream, param_map);
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
					h.macros.begin(),
					h.macros.end(),
					[word](std::unordered_map<std::string, macro>::value_type m) -> bool {
						return m.first.compare(word) == 0;
					}
				);
				if (macro_res == h.macros.end())
				{
					sstream << '"' << word << '"';
				}
				else
				{
					sstream << '"' << handle_macro(h, local_fileinfo, original_fileinfo, macro_res->second, param_map) << '"';
				}
			}
		}
	}
	void replace_concat(helper& h, finfo& local_fileinfo, finfo& original_fileinfo, const macro& m, std::vector<std::string>& params, std::stringstream& sstream, const std::unordered_map<std::string, std::string>& param_map)
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
				h.macros.begin(),
				h.macros.end(),
				[word](std::unordered_map<std::string, macro>::value_type m) -> bool {
					return m.first.compare(word) == 0;
				}
			);
			if (macro_res == h.macros.end())
			{
				sstream << word;
			}
			else
			{
				sstream << handle_macro(h, local_fileinfo, original_fileinfo, macro_res->second, param_map);
			}
		}
	}

	size_t replace_find_wordend(finfo fileinfo)
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
	void replace_skip(finfo& fileinfo, std::stringstream& sstream)
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
	std::string replace(helper& h, finfo& original_fileinfo, const macro& m, std::vector<std::string>& params)
	{
		if (m.args.size() != params.size())
		{
			h.errflag = true;
			if (original_fileinfo.path.empty())
			{
				h.vm->err() << "[ERR][L" << original_fileinfo.line << "|C" << original_fileinfo.col << "]\t" << "Arg Count Missmatch." << std::endl;
			}
			else
			{
				h.vm->err() << "[ERR][L" << original_fileinfo.line << "|C" << original_fileinfo.col << "|" << original_fileinfo.path << "]\t" << "Arg Count Missmatch." << std::endl;
			}
			return "";
		}
		finfo local_fileinfo;
		local_fileinfo.content = m.content;
		local_fileinfo.path = m.filepath;
		local_fileinfo.line = m.line;
		if (m.callback)
		{
			return m.callback(m, local_fileinfo, original_fileinfo, params);
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
				replace_stringify(h, local_fileinfo, original_fileinfo, m, params, sstream, parammap);
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
						auto macro_res = h.macros.find(word);
						if (macro_res == h.macros.end())
						{
							sstream << word;
						}
						else
						{
							sstream << handle_macro(h, local_fileinfo, original_fileinfo, macro_res->second, parammap);
						}
					}
				}
			}
		}

		return sstream.str();
	}
	std::string handle_arg(helper& h, finfo& local_fileinfo, finfo& original_fileinfo, size_t endindex, const std::unordered_map<std::string, std::string>& param_map)
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
						auto res = h.contains_macro(word);
						if (res.has_value())
						{
							if (res.value().hasargs)
							{
								local_fileinfo.move_back();
							}
							auto handled = handle_macro(h, local_fileinfo, original_fileinfo, res.value(), param_map);
							if (h.errflag)
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
	std::string handle_macro(helper& h, finfo& local_fileinfo, finfo& original_fileinfo, const macro& m, const std::unordered_map<std::string, std::string>& param_map)
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
							finfo copy = local_fileinfo;
							copy.off = lastargstart;
							auto handled_param = handle_arg(h, copy, original_fileinfo, local_fileinfo.off, param_map);
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
		auto replace_result = replace(h, original_fileinfo, m, params);
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
		return replace(h, original_fileinfo, m, params);
#endif
	}
	std::string parse_ppinstruction(helper& h, finfo& fileinfo)
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
				const auto& path = otherfinfo.path;
				auto res = std::find_if(h.path_tree().begin(), h.path_tree().end(), [path](std::string& parent) -> bool { return parent == path; });
				if (res != h.path_tree().end())
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
					for (size_t i = 0; i < h.path_tree().size(); i++)
					{
						h.vm->err() << i << ". " << h.path_tree()[i] << std::endl;
					}
					return "";
				}
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
			std::string output;
			auto lineInfo = std::to_string(fileinfo.line - 1);
			auto parsedFile = parse_file(h, otherfinfo);
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
				if (h.macros.find(m.name) != h.macros.end() && !settings::disable_warn_define)
				{
					if (fileinfo.path.empty())
					{
						h.vm->wrn() << "[WRN][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Macro '" << m.name << "' defined twice." << std::endl;
					}
					else
					{
						h.vm->wrn() << "[WRN][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Macro '" << m.name << "' defined twice." << std::endl;
					}
				}
			}
			else
			{
				if (spaceIndex < bracketsIndex || bracketsIndex == std::string::npos) // std::string::npos does not need to be catched as bracketsIndex always < npos here
				{ // First bracket was found after first space OR is not existing thus we have a simple define with a replace value here
					m.name = line.substr(0, spaceIndex);
					if (h.macros.find(m.name) != h.macros.end() && !settings::disable_warn_define)
					{
						if (fileinfo.path.empty())
						{
							h.vm->wrn() << "[WRN][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Macro '" << m.name << "' defined twice." << std::endl;
						}
						else
						{
							h.vm->wrn() << "[WRN][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Macro '" << m.name << "' defined twice." << std::endl;
						}
					}
					m.content = line.substr(line[spaceIndex] == ' ' ? spaceIndex + 1 : spaceIndex); //Special magic for '#define macro\'
					m.hasargs = false;
				}
				else
				{ // We got a define with arguments here
					m.hasargs = true;
					m.name = line.substr(0, bracketsIndex);
					if (h.macros.find(m.name) != h.macros.end() && !settings::disable_warn_define)
					{
						if (fileinfo.path.empty())
						{
							h.vm->wrn() << "[WRN][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Macro '" << m.name << "' defined twice." << std::endl;
						}
						else
						{
							h.vm->wrn() << "[WRN][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Macro '" << m.name << "' defined twice." << std::endl;
						}
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
			h.macros[m.name] = std::move(m);
			return "\n";
		}
		else if (inst == "UNDEF")
		{ // #undef TEST
			if (!h.allowwrite)
			{
				return "\n";
			}
			
			auto res = h.macros.find(line);
			if (res == h.macros.end())
			{
				if (!settings::disable_warn_define)
				{
					if (fileinfo.path.empty())
					{
						h.vm->wrn() << "[WRN][L" << fileinfo.line << "|C" << fileinfo.col << "]\t" << "Macro '" << line << "' not found." << std::endl;
					}
					else
					{
						h.vm->wrn() << "[WRN][L" << fileinfo.line << "|C" << fileinfo.col << "|" << fileinfo.path << "]\t" << "Macro '" << line << "' not found." << std::endl;
					}
				}
			}
			else
			{
				h.macros.erase(res);
			}
			return "\n";
		}
		else if (inst == "IFDEF")
		{ // #ifdef TEST
			if (h.inside_ppif())
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
				h.inside_ppif(true);
			}
			auto res = h.macros.find(line);
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
			if (h.inside_ppif())
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
				h.inside_ppif(true);
			}
			auto res = h.macros.find(line);
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
			if (!h.inside_ppif())
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
			if (!h.inside_ppif())
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
			h.inside_ppif(false);
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
		h.push_path(fileinfo.path);
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
						auto res = parse_ppinstruction(h, fileinfo);
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
								auto res = handle_macro(h, fileinfo, fileinfo, m.value(), empty_parammap);
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
				auto res = handle_macro(h, fileinfo, fileinfo, m.value(), empty_parammap);
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
		h.pop_path();
		return sstream.str();
	}
}
std::string line_macro_callback(const macro& m, const finfo& local_fileinfo, const finfo& original_fileinfo, const std::vector<std::string>& params)
{
	return std::to_string(original_fileinfo.line);
}
std::string file_macro_callback(const macro& m, const finfo& local_fileinfo, const finfo& original_fileinfo, const std::vector<std::string>& params)
{
	return '"' + original_fileinfo.path + '"';
}
std::string sqf::parse::preprocessor::parse(sqf::virtualmachine* vm, std::string input, bool & errflag, std::string filename)
{
	helper h;
	h.vm = vm;
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = "";
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "__LINE__";
		macro.callback = line_macro_callback;
		h.macros["__LINE__"] = macro;
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
		h.macros["__FILE__"] = macro;
	}
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = VERSION_FULL;
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "_SQF_VM";
		h.macros["_SQF_VM"] = macro;
	}
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = STR(VERSION_MAJOR);
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "_SQF_VM_MAJOR";
		h.macros["_SQF_VM_MAJOR"] = macro;
	}
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = STR(VERSION_MINOR);
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "_SQF_VM_MINOR";
		h.macros["_SQF_VM_MINOR"] = macro;
	}
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = STR(VERSION_REVISION);
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = "_SQF_VM_REVISION";
		h.macros["_SQF_VM_REVISION"] = macro;
	}
	for (auto& it : vm->preprocessor_defines())
	{
		macro macro;
		macro.line = 0;
		macro.column = 0;
		macro.content = it.second;
		macro.filepath = "";
		macro.hasargs = false;
		macro.name = it.first;
		h.macros[it.first] = macro;
	}
	finfo fileinfo;
	fileinfo.content = std::move(input);
	fileinfo.path = std::move(filename);
	auto res = parse_file(h, fileinfo);
	errflag = h.errflag;
	if (h.inside_ppif_err_flag())
	{
		return "";
	}
	return res;
}
