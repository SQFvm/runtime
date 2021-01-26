#include "d_array.h"
#include "d_scalar.h"
#include "d_string.h"
#include "d_code.h"
#include "logging.h"
#include "runtime.h"
#include "instruction_blob.hpp"
namespace err = logmessage::runtime;

static std::array<float, 3> as_vec3(const sqf::types::d_array& data)
{
    switch (data.size())
    {
    case 0:
        return std::array<float, 3> { 0, 0, 0 };
    case 1:
        return std::array<float, 3> { data.at(0).data<sqf::types::d_scalar, float>(), 0, 0 };
    case 2:
        return std::array<float, 3> { data.at(0).data<sqf::types::d_scalar, float>(), data.at(1).data<sqf::types::d_scalar, float>(), 0 };
    default:
        return std::array<float, 3> { data.at(0).data<sqf::types::d_scalar, float>(), data.at(1).data<sqf::types::d_scalar, float>(), data.at(2).data<sqf::types::d_scalar, float>() };
    }
}

static std::array<float, 2> as_vec2(const sqf::types::d_array& data)
{
    switch (data.size())
    {
    case 0:
        return std::array<float, 2> { 0, 0 };
    case 1:
        return std::array<float, 2> { data.at(0).data<sqf::types::d_scalar, float>(), 0 };
    default:
        return std::array<float, 2> { data.at(0).data<sqf::types::d_scalar, float>(), data.at(1).data<sqf::types::d_scalar, float>() };
    }
}

float sqf::types::distance3dsqr(const std::shared_ptr<sqf::types::d_array>& l, const std::shared_ptr<sqf::types::d_array>& r)
{
    return distance3d(as_vec3(*l), as_vec3(*r));
}
float sqf::types::distance3dsqr(const sqf::types::d_array* l, const sqf::types::d_array* r)
{
    return distance3d(as_vec3(*l), as_vec3(*r));
}

float sqf::types::distance3d(const std::shared_ptr<sqf::types::d_array>& l, const std::shared_ptr<sqf::types::d_array>& r)
{
    return distance3d(as_vec3(*l), as_vec3(*r));
}
float sqf::types::distance3d(const sqf::types::d_array* l, const sqf::types::d_array* r)
{
    return distance3d(as_vec3(*l), as_vec3(*r));
}

float sqf::types::distance2dsqr(const std::shared_ptr<sqf::types::d_array>& l, const std::shared_ptr<sqf::types::d_array>& r)
{
    return distance2dsqr(as_vec2(*l), as_vec2(*r));
}
float sqf::types::distance2dsqr(const sqf::types::d_array* l, const sqf::types::d_array* r)
{
    return distance2dsqr(as_vec2(*l), as_vec2(*r));
}

float sqf::types::distance2d(const std::shared_ptr<sqf::types::d_array>& l, const std::shared_ptr<sqf::types::d_array>& r)
{
    return distance2d(as_vec2(*l), as_vec2(*r));
}
float sqf::types::distance2d(const sqf::types::d_array* l, const sqf::types::d_array* r)
{
    return distance2d(as_vec2(*l), as_vec2(*r));
}

bool sqf::types::d_array::check_type(sqf::runtime::runtime& runtime, sqf::runtime::type t, size_t min, size_t max) const
{
    bool errflag = true;
    if (size() < min || size() > max)
    {
        if (min == max)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), min, size()));
        }
        else
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), min, max, size()));
        }
        return false;
    }
    for (size_t i = 0; i < size(); i++)
    {
        if (!at(i).is(t))
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), i, t, at(i).type()));
            errflag = false;
        }
    }
    return errflag;

}

void sqf::types::d_array::write(sqf::runtime::instruction_blob& b) const
{
    b.write<sqf::runtime::opcodes>(sqf::runtime::opcodes::meta_array);
    b.write<uint64_t>(uint64_t(size()));
    for (auto& it : m_value)
    {
        it.data()->write(b);
    }
}

std::shared_ptr<sqf::types::d_array> sqf::types::d_array::read(sqf::runtime::instruction_handle& h)
{
    auto len = h.read<uint64_t>();
    std::vector<sqf::runtime::value> values;
    values.resize(len);
    for (auto i = 0; i < len; i++)
    {
        auto opcode = h.read<sqf::runtime::opcodes>();
        switch (opcode)
        {
            case sqf::runtime::opcodes::meta_array: values.emplace_back(sqf::types::d_array::read(h));
            case sqf::runtime::opcodes::meta_code: values.emplace_back(sqf::types::d_code::read(h));
            case sqf::runtime::opcodes::meta_float: values.emplace_back(sqf::types::d_scalar::read(h));
            case sqf::runtime::opcodes::meta_string: values.emplace_back(sqf::types::d_string::read(h));
            case sqf::runtime::opcodes::meta_nil:
            [[fallthrough]]
            default: values.emplace_back();
        }
    }
    return std::make_shared<d_array>(values);
}

bool sqf::types::d_array::check_type(sqf::runtime::runtime& runtime, const sqf::runtime::type* p_t, size_t min, size_t max) const
{
    bool errflag = true;
    if (size() < min || size() > max)
    {
        if (min == max)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), min, size()));
        }
        else
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), min, max, size()));
        }
        return false;
    }
    for (size_t i = 0; i < size(); i++)
    {
        if (!at(i).is(p_t[i]))
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), i, p_t[i], at(i).type()));
            errflag = false;
        }
    }
    return errflag;
}
