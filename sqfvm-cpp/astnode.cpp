#include <string>
#include <iostream>
#include <vector>
#include "astnode.h"

void print_navigate_ast(std::wostream* outstreamptr, astnode node, const wchar_t *(*astkindname)(short), unsigned short level)
{
	if (level == 0)
	{
		std::wcout << L"Node Type;Offset;Length;Content" << std::endl;
	}
	*outstreamptr << std::wstring(level, L'\t') << astkindname(node.kind) << L';' << node.offset << L';' << node.length << L';' << node.content << std::endl;
	for each (auto it in node.children)
	{
		print_navigate_ast(outstreamptr, it, astkindname, level + 1);
	}
}