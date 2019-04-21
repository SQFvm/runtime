#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <algorithm>

static std::string string_tolower(std::string_view s)
{
    std::string ret(s);
    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;
}

static std::string& string_tolower_ref(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}