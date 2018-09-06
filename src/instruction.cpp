#include "instruction.h"
#include <sstream>

void sqf::instruction::setdbginf(size_t line, size_t col, std::string file, std::string segment)
{
	mline = line;
	mcol = col;
	mfile = file;
	msegment = segment;
}

std::string sqf::instruction::dbginf(std::string tag) const
{
	std::stringstream sstream;
	sstream << msegment << '[' << tag << "][" << mline << "|C" << mcol << "]\t";
	return sstream.str();
}
