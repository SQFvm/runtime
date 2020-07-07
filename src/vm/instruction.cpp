#include "instruction.h"
#include <sstream>

void sqf::instruction::setdbginf(size_t line, size_t col, std::string file, std::string segment)
{
	mline = line;
	mcol = col;
	mfile = std::move(file);
	msegment = std::move(segment);
}