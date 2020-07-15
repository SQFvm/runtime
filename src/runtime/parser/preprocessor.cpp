#include "preprocessor.h"
#include "../runtime.h"

std::optional<std::string> sqf::parser::preprocessor::passthrough::preprocess(::sqf::runtime::runtime& runtime, ::sqf::runtime::fileio::pathinfo pathinfo)
{
	return runtime.fileio().read_file(pathinfo);
}
