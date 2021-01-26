#include "d_string.h"
#include "instruction_blob.hpp"

void sqf::types::d_string::write(sqf::runtime::instruction_blob& b) const
{
    b.write<sqf::runtime::opcodes>(sqf::runtime::opcodes::meta_string);
    b.write<std::string_view>(value());
}

std::shared_ptr<sqf::types::d_string> sqf::types::d_string::read(sqf::runtime::instruction_handle& h)
{
    return std::make_shared<d_string>(std::string(h.read<std::string_view>()));
}
