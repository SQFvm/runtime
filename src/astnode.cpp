#include <string>
#include <iostream>
#include <vector>
#include "astnode.h"

void print_navigate_ast(std::basic_ostream<char, std::char_traits<char>>* outstreamptr, const astnode& node, const char *(*astkindname)(short), unsigned short level)
{
	if (level == 0)
	{
		*outstreamptr << "Node Type;Offset;Length;Content" << std::endl;
	}
	*outstreamptr << std::string(level, '\t') << astkindname(node.kind) << ';' << node.offset << ';' << node.length << ';' << node.content << std::endl;
	for (auto& it : node.children)
	{
		print_navigate_ast(outstreamptr, it, astkindname, level + 1);
	}
}