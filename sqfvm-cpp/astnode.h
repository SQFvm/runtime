#ifndef _ASTNODE
#define _ASTNODE 1

#if !defined(_STRING) & !defined(_STRING_)
#error virtualmachine requires <string> header
#endif // !_STRING
#if !defined(_VECTOR) & !defined(_VECTOR_)
#error virtualmachine requires <vector> header
#endif // !_VECTOR


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

void print_navigate_ast(std::wostream* outstreamptr, astnode node, const wchar_t* (*astkindname)(short), unsigned short level = 0);

#endif // !_ASTNODE
