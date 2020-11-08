#include "preprocessor.h"
#include "../runtime.h"

std::optional<std::string> sqf::parser::preprocessor::passthrough::preprocess(::sqf::runtime::runtime& runtime, std::string_view view, ::sqf::runtime::fileio::pathinfo pathinfo)
{
    return std::string(view);
}

std::optional<std::string> sqf::runtime::parser::preprocessor::preprocess(::sqf::runtime::runtime& runtime, ::sqf::runtime::fileio::pathinfo pathinfo)
{
    auto contents = runtime.fileio().read_file(pathinfo);
    return preprocess(runtime, contents, pathinfo);
}
