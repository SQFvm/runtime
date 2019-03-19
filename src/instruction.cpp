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
	if (mfile.empty())
	{
		sstream << msegment << '[' << tag << "][L" << mline << "|C" << mcol << "]\t";
	}
	else
	{
		sstream << msegment << '[' << tag << "][L" << mline << "|C" << mcol << "|" << mfile << "]\t";
	}
	return sstream.str();
}
