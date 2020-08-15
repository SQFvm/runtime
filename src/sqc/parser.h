#pragma once
#include "../runtime/parser/sqf.h"
#include "../cli/colors.h"

#include <functional>
#include <vector>

#include <iostream>
#include <set>

namespace sqf::sqc
{
    class parser : ::sqf::runtime::parser::sqf
    {
    public:
        // Inherited via sqf
        virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;

        virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;

    };
}