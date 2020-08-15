#pragma once
#include <vector>

namespace sqf::sqc
{
	enum class nodetype
	{
		NA
	};
	struct astnode
	{
		size_t offset;
		size_t length;
		size_t line;
		size_t col;
		nodetype kind;
		std::vector<astnode> children;

		astnode() : offset(0), length(0), line(0), col(0), kind(nodetype::NA) {}
	};
}