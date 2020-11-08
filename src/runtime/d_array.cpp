#include "d_array.h"
#include "d_scalar.h"
#include "logging.h"
#include "runtime.h"
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
