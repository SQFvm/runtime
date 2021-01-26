#include "d_scalar.h"
#include "instruction_blob.hpp"
#include <sstream>
#include <iomanip>


std::string sqf::types::d_scalar::to_string_sqf() const
{
    if (s_decimals == -1)
    {
        auto bufflen = std::snprintf(nullptr, 0, "%g", m_value) + 1;
        auto buff = new char[bufflen];
        std::snprintf(buff, bufflen, "%g", m_value);
        auto str = std::string(buff, bufflen - 1);
        delete[] buff;
        return str;
    }
    else
    {
        auto bufflen = std::snprintf(nullptr, 0, "%0.*f", s_decimals, m_value) + 1;
        auto buff = new char[bufflen];
        std::snprintf(buff, bufflen, "%0.*f", s_decimals, m_value);
        auto str = std::string(buff, bufflen - 1);
        delete[] buff;
        return str;
    }
}

std::string sqf::types::d_scalar::to_string() const
{
    if (s_decimals == -1)
    {
        auto bufflen = std::snprintf(nullptr, 0, "%g", m_value) + 1;
        auto buff = new char[bufflen];
        std::snprintf(buff, bufflen, "%g", m_value);
        auto str = std::string(buff, bufflen - 1);
        delete[] buff;
        return str;
    }
    else
    {
        auto bufflen = std::snprintf(nullptr, 0, "%0.*f", s_decimals, m_value) + 1;
        auto buff = new char[bufflen];
        std::snprintf(buff, bufflen, "%0.*f", s_decimals, m_value);
        auto str = std::string(buff, bufflen - 1);
        delete[] buff;
        return str;
    }
}

void sqf::types::d_scalar::write(sqf::runtime::instruction_blob& b) const
{
    b.write<sqf::runtime::opcodes>(sqf::runtime::opcodes::meta_string);
    b.write<float>(value());
}

std::shared_ptr<sqf::types::d_scalar> sqf::types::d_scalar::read(sqf::runtime::instruction_handle& h)
{
    return std::make_shared<d_scalar>(h.read<float>());
}
