#include "parser.h"

#include "driver.hh"
#include "astnode.h"


bool sqf::sqc::parser::check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file)
{
    astnode root;
    ::parse::Driver d;
    auto success = d.parse_string(contents, file.physical);
    return success;
}

std::optional<::sqf::runtime::instruction_set> sqf::sqc::parser::parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file)
{
    astnode root;
    ::parse::Driver d(root);
    auto success = d.parse_string(contents, file.physical);
    if (!success)
    {
        return {};
    }
    return std::optional<::sqf::runtime::instruction_set>();
}
