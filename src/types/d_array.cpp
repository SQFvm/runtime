#include "d_array.h"
#include "d_scalar.h"

static std::array<float, 3> as_vec3(const sqf::types::d_array& data)
{
	switch (data.size())
	{
	case 0:
		return std::array<float, 3> { 0, 0, 0 };
	case 1:
		return std::array<float, 3> { data.at(0).data_as<sqf::types::d_scalar, float>(), 0, 0 };
	case 2:
		return std::array<float, 3> { data.at(0).data_as<sqf::types::d_scalar, float>(), data.at(1).data_as<sqf::types::d_scalar, float>(), 0 };
	default:
		return std::array<float, 3> { data.at(0).data_as<sqf::types::d_scalar, float>(), data.at(1).data_as<sqf::types::d_scalar, float>(), data.at(2).data_as<sqf::types::d_scalar, float>() };
	}
}

static std::array<float, 2> as_vec2(const sqf::types::d_array& data)
{
	switch (data.size())
	{
	case 0:
		return std::array<float, 2> { 0, 0 };
	case 1:
		return std::array<float, 2> { data.at(0).data_as<sqf::types::d_scalar, float>(), 0 };
	default:
		return std::array<float, 2> { data.at(0).data_as<sqf::types::d_scalar, float>(), data.at(1).data_as<sqf::types::d_scalar, float>() };
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