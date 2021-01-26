#include "d_code.h"

void sqf::types::d_code::write(sqf::runtime::instruction_blob& b) const
{
    b.write<sqf::runtime::opcodes>(sqf::runtime::opcodes::meta_code);
    b.write<uint64_t>(uint64_t(value().size()));
    b.append(value().begin(), value().end());
}

std::shared_ptr<sqf::types::d_code> sqf::types::d_code::read(sqf::runtime::instruction_handle& h)
{
    auto len = h.read<uint64_t>();
    return std::make_shared<sqf::types::d_code>(sqf::runtime::instruction_blob(h.pc(), h.pc() + len));
}
