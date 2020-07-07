#include "d_array.h"

static std::array<double, 3> as_vec3(const sqf::types::d_array*) const
{
	switch (size())
	{
	case 0:
		return std::array<double, 3> { 0, 0, 0 };
	case 1:
		return std::array<double, 3> { at(0).as_double(), 0, 0 };
	case 2:
		return std::array<double, 3> { at(0).as_double(), at(1).as_double(), 0 };
	default:
		return std::array<double, 3> { at(0).as_double(), at(1).as_double(), at(2).as_double() };
	}
}

static std::array<double, 2> as_vec2(const sqf::types::d_array*) const
{
	switch (size())
	{
	case 0:
		return std::array<double, 2> { 0, 0 };
	case 1:
		return std::array<double, 2> { at(0).as_double(), 0 };
	default:
		return std::array<double, 2> { at(0).as_double(), at(1).as_double() };
	}
}

double sqf::types::distance3dsqr(const std::shared_ptr<sqf::types::d_array>& l, const std::shared_ptr<sqf::types::d_array>& r)
{ return distance3d(as_vec3(l), as_vec3(r)); }
double sqf::types::distance3dsqr(const sqf::types::d_array* l, const sqf::types::d_array* r)
{ return distance3d(as_vec3(l), as_vec3(r)); }

double sqf::types::distance3d(const std::shared_ptr<sqf::types::d_array>& l, const std::shared_ptr<sqf::types::d_array>& r)
{ return distance3d(as_vec3(l), as_vec3(r)); }
double sqf::types::distance3d(const sqf::types::d_array* l, const sqf::types::d_array* r)
{ return distance3d(as_vec3(l), as_vec3(r)); }

double sqf::types::distance2dsqr(const std::shared_ptr<sqf::types::d_array>& l, const std::shared_ptr<sqf::types::d_array>& r)
{ return distance2dsqr(as_vec2(l), as_vec2(r)); }
double sqf::types::distance2dsqr(const sqf::types::d_array* l, const sqf::types::d_array* r)
{ return distance2dsqr(as_vec2(l), as_vec2(r)); }

double sqf::types::distance2d(const std::shared_ptr<sqf::types::d_array>& l, const std::shared_ptr<sqf::types::d_array>& r)
{ return distance2d(as_vec2(l), as_vec2(r)); }
double sqf::types::distance2d(const sqf::types::d_array* l, const sqf::types::d_array* r)
{ return distance2d(as_vec2(l), as_vec2(r)); }