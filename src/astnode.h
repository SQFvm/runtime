#pragma once
#include <string>
#include <vector>


struct astnode
{
	size_t offset;
	size_t length;
	size_t line;
	size_t col;
	std::string content;
	std::string file;
	short kind;
	std::vector<astnode> children;
};

void print_navigate_ast(std::basic_ostream<char, std::char_traits<char>>* outstreamptr, const astnode& node, const char* (*astkindname)(short), unsigned short level = 0);
