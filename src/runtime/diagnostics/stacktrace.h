#pragma once
#include "../frame.h"
#include "../value.h"

#include <vector>
#include <string>
#include <sstream>

namespace sqf::runtime::diagnostics
{
    struct stacktrace
    {
        std::vector<sqf::runtime::frame> frames;
        sqf::runtime::value value;

        stacktrace() {}
        stacktrace(std::vector<sqf::runtime::frame> f) : frames(f) {}

        std::string to_string() const;
    };
}