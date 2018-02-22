#include <sstream>
#include <vector>
#include <string>
extern "C" {
#include "wstring_op.h"
}
#include "compiletime.h"

class macro
{
private:
	std::wstring mname;
public:
	std::wstring name(void) { return mname; }
};
class pphelper
{
private:
	std::wostream* merr;
	std::vector<macro> mmacros;
public:
	std::wostream& err(void) { return *merr; }
	std::vector<macro>& macros(void) { return mmacros; }
};


void skiptillaction(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream);
bool iscommandstart(pphelper &helper, const wchar_t* source, size_t offset);
void parse_command(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream);
bool isactionstart(pphelper &helper, const wchar_t* source, size_t offset);
void parse_action(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream);
void parse_ifdef(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream);
void parse_ifndef(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream);
void parse_undefine(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream);
void parse_define(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream);
void parse_else(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream);
void parse_endif(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream);
void parse_include(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream);

void skiptillaction(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream)
{
	wchar_t c;
	while ((c = source[offset]) != L'#' && c != L'\0' && !isactionstart(helper, source, offset))
	{
		if (c == L'\n')
		{
			line++;
			col = 0;
		}
		offset++;
		col++;
	}
}

bool iscommandstart(pphelper &helper, const wchar_t* source, size_t offset) { return source[offset] == L'#'; }
void parse_command(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream)
{
	offset++;
	col++;
	if (wstr_cmpi(source, compiletime::strlen(L"ifdef"), L"ifdef", compiletime::strlen(L"ifdef")) == 0)
	{
		parse_ifdef(helper, source, line, col, offset, sstream);
	}
	else if (wstr_cmpi(source, compiletime::strlen(L"ifndef"), L"ifndef", compiletime::strlen(L"ifndef")) == 0)
	{
		parse_ifndef(helper, source, line, col, offset, sstream);
	}
	else if (wstr_cmpi(source, compiletime::strlen(L"undefine"), L"undefine", compiletime::strlen(L"undefine")) == 0)
	{
		parse_undefine(helper, source, line, col, offset, sstream);
	}
	else if (wstr_cmpi(source, compiletime::strlen(L"define"), L"define", compiletime::strlen(L"define")) == 0)
	{
		parse_define(helper, source, line, col, offset, sstream);
	}
	else if (wstr_cmpi(source, compiletime::strlen(L"else"), L"else", compiletime::strlen(L"else")) == 0)
	{
		parse_else(helper, source, line, col, offset, sstream);
	}
	else if (wstr_cmpi(source, compiletime::strlen(L"endif"), L"endif", compiletime::strlen(L"endif")) == 0)
	{
		parse_endif(helper, source, line, col, offset, sstream);
	}
	else if (wstr_cmpi(source, compiletime::strlen(L"include"), L"include", compiletime::strlen(L"include")) == 0)
	{
		parse_include(helper, source, line, col, offset, sstream);
	}
	else
	{
		helper.err() << L"[PPE][L" << line << L"|C" << col << L"]\t" << L"Unknown PP Command" << std::endl;
	}
}
bool isactionstart(pphelper &helper, const wchar_t* source, size_t offset)
{
	for (auto it : helper.macros())
	{
		if (wstr_cmpi(source, it.name().length(), it.name().c_str(), it.name().length()) == 0)
		{
			return true;
		}
	} return false;
}
void parse_action(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream) {}

void parse_ifdef(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream) {}
void parse_ifndef(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream) {}
void parse_undefine(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream) {}
void parse_define(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream) {}
void parse_else(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream) {}
void parse_endif(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream) {}
void parse_include(pphelper &helper, const wchar_t* source, size_t &line, size_t &col, size_t &offset, std::wstringstream &sstream) {}


std::wstring parse(std::wstring source)
{
	std::wstringstream sstream;
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