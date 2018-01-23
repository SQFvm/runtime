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
	std::wstring content;
	short kind;
	std::vector<astnode> children;
};

#endif // !_ASTNODE
