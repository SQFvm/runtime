#include "preprocessor.h"
#include "../runtime.h"
#include "../fileio.h"

std::string sqf::parser::preprocessor::passthrough::preprocess(::sqf::runtime::runtime& runtime, ::sqf::runtime::fileio::pathinfo pathinfo)
{
	return runtime.fileio().read_file(pathinfo);
}