#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include "compiletime.h"
#include "parsepreprocessor.h"
#include "virtualmachine.h"
#include "fileio.h"

class macro;
std::string parse_file(sqf::virtualmachine* vm, std::string input, size_t off, bool& errflag, std::string filename, std::vector<macro>& macrolist, size_t& line, size_t& col, size_t maxindex = ~0);
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
		std::vector<range> args;
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
						args.push_back({ argstart, off - argstart });
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
						args.push_back({ argstart, off - argstart });
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
		if (args.size() < args.size())
		{
			vm->err() << "Missing arguments for macro '" << name << "' (" << args.size() << "/" << args.size() << ")." << std::endl;
			return false;
		}
		else if (args.size() > args.size())
		{
			vm->err() << "Too many for macro '" << name << "' (" << args.size() << "/" << args.size() << ")." << std::endl;
			return false;
		}

		// Parse each arg individually
		std::vector<std::string> args_parsed;
		for (auto& r : args)
		{
			bool errflag = false;
			auto arg = parse_file(vm, input, r.index, errflag, filename, macrolist, line, col, r.length + r.index);
			auto orig = input.substr(r.index, r.length);
			if (orig == arg && orig.find(',') != std::string::npos)
			{
				vm->err() << "Comma in braces detected at L" << line << ", C" << col << " in: '" << filename << "'." << std::endl;
				errflag = true;
				return "";
			}
			if (errflag)
			{
				return false;
			}
			args_parsed.push_back(arg);
		}
		auto handledcontent = handle(args_parsed);
		sstream << handledcontent;
	}
};

std::string rest_of_line(std::string input, size_t& off, size_t& line)
{
	size_t from = off;
	auto was_escaped = false;
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
		else
		{
			was_escaped = false;
		}
	}
	auto output = input.substr(from, off - from);
	output.erase(std::remove_if(output.begin(), output.end(), [](char c) -> bool { return c == '\\' || c == '\n' || c == '\r'; }), output.end());
	return output;
}
std::string parse_file(sqf::virtualmachine* vm, std::string input, size_t off, bool& errflag, std::string filename, std::vector<macro>& macrolist, size_t& line, size_t& col, size_t maxindex)
{
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
				was_new_line = false;
			}
			if (c == '#' && was_new_line && !is_macro)
			{
				is_macro = true;
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
					auto virtpath = restofline.substr(1, restofline.length() - 2);
					auto path = vm->get_filesystem().get_physical_path(virtpath);
					auto filecontents = load_file(path);
					size_t line = 0;
					size_t col = 0;
					auto res = parse_file(vm, filecontents, 0, errflag, virtpath, macrolist, line, col);
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
								if (arg_index == std::string::npos)
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
							m.content = restofline.substr(bracketsEndIndex + 2);
						}
					}
					auto actualContent = parse_file(vm, m.content, 0, errflag, filename, macrolist, line, col);
					if (errflag)
					{
						return "";
					}
					m.content = actualContent;
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
						res->handle_insert(vm, c, off, maxindex, line, col, filename, input, sstream, macrolist);
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
				else
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
				res->handle_insert(vm, c, off, maxindex, line, col, filename, input, sstream, macrolist);
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
	auto output = parse_file(vm, input, off, errflag, filename, macros, line, col);
	return output;
}
