#include <sstream>
#include <vector>
#include <string>
#include "string_op.h"
#include "compiletime.h"

class macro
{
private:
	std::string mname;
public:
	std::string name() { return mname; }
};
class pphelper
{
private:
	std::wostream* merr;
	std::vector<macro> mmacros;
public:
	std::wostream& err() { return *merr; }
	std::vector<macro>& macros() { return mmacros; }
};


void skiptillaction(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream);
bool iscommandstart(pphelper &helper, const char* source, size_t offset);
void parse_command(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream);
bool isactionstart(pphelper &helper, const char* source, size_t offset);
void parse_action(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream);
void parse_ifdef(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream);
void parse_ifndef(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream);
void parse_undefine(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream);
void parse_define(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream);
void parse_else(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream);
void parse_endif(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream);
void parse_include(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream);

void skiptillaction(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream)
{
	char c;
	while ((c = source[offset]) != '#' && c != '\0' && !isactionstart(helper, source, offset))
	{
		if (c == '\n')
		{
			line++;
			col = 0;
		}
		offset++;
		col++;
	}
}

bool iscommandstart(pphelper &helper, const char* source, size_t offset) { return source[offset] == '#'; }
void parse_command(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream)
{
	offset++;
	col++;
	if (str_cmpi(source, compiletime::strlen("ifdef"), "ifdef", compiletime::strlen("ifdef")) == 0)
	{
		parse_ifdef(helper, source, line, col, offset, sstream);
	}
	else if (str_cmpi(source, compiletime::strlen("ifndef"), "ifndef", compiletime::strlen("ifndef")) == 0)
	{
		parse_ifndef(helper, source, line, col, offset, sstream);
	}
	else if (str_cmpi(source, compiletime::strlen("undefine"), "undefine", compiletime::strlen("undefine")) == 0)
	{
		parse_undefine(helper, source, line, col, offset, sstream);
	}
	else if (str_cmpi(source, compiletime::strlen("define"), "define", compiletime::strlen("define")) == 0)
	{
		parse_define(helper, source, line, col, offset, sstream);
	}
	else if (str_cmpi(source, compiletime::strlen("else"), "else", compiletime::strlen("else")) == 0)
	{
		parse_else(helper, source, line, col, offset, sstream);
	}
	else if (str_cmpi(source, compiletime::strlen("endif"), "endif", compiletime::strlen("endif")) == 0)
	{
		parse_endif(helper, source, line, col, offset, sstream);
	}
	else if (str_cmpi(source, compiletime::strlen("include"), "include", compiletime::strlen("include")) == 0)
	{
		parse_include(helper, source, line, col, offset, sstream);
	}
	else
	{
		helper.err() << "[PPE][" << line << "|C" << col << "]\t" << "Unknown PP Command" << std::endl;
	}
}
bool isactionstart(pphelper &helper, const char* source, size_t offset)
{
	for (auto it : helper.macros())
	{
		if (str_cmpi(source, it.name().length(), it.name().c_str(), it.name().length()) == 0)
		{
			return true;
		}
	} return false;
}
void parse_action(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream) {}

void parse_ifdef(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream) {}
void parse_ifndef(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream) {}
void parse_undefine(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream) {}
void parse_define(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream) {}
void parse_else(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream) {}
void parse_endif(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream) {}
void parse_include(pphelper &helper, const char* source, size_t &line, size_t &col, size_t &offset, std::stringstream &sstream) {}


std::string parse(std::string source)
{
	std::stringstream sstream;
	size_t offset = 0;
	size_t line = 0;
	size_t col = 0;
	pphelper helper;
	while (true)
	{
		skiptillaction(helper, source.c_str(), line, col, offset, sstream);
		if (iscommandstart(helper, source.c_str(), offset))
		{
			parse_command(helper, source.c_str(), line, col, offset, sstream);
		}
		else if (isactionstart(helper, source.c_str(), offset))
		{
			parse_action(helper, source.c_str(), line, col, offset, sstream);
		}
		else
		{
			break;
		}
	}
	return sstream.str();
}