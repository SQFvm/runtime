#include "full.h"
#include <sstream>

void sqf::instruction::setdbginf(size_t line, size_t col, std::wstring file, std::wstring segment)
{
	mline = line;
	mcol = col;
	mfile = file;
	msegment = segment;
}

std::wstring sqf::instruction::dbginf(std::wstring tag) const
{
	auto sstream = std::wstringstream();
	sstream << msegment << L'[' << tag << L"][L" << mline << L"|C" << mcol << L"]\t";
	return sstream.str();
}
