#pragma once
#include <string>
#include <vector>


struct astnode
{
	size_t offset;
	size_t length;
	size_t line;
	size_t col;
	std::wstring content;
	std::wstring file;
	short kind;
	std::vector<astnode> children;
};

void print_navigate_ast(std::basic_ostream<wchar_t, std::char_traits<wchar_t>>* outstreamptr, astnode node, const wchar_t* (*astkindname)(short), unsigned short level = 0);
