#include "marker.h"
#include "value.h"
#include "arraydata.h"


sqf::marker::marker()
{
}

sqf::value sqf::marker::get_text_sqf() const
{
	return get_text();
}
sqf::value sqf::marker::get_type_sqf() const
{
	return value(get_type());
}
sqf::value sqf::marker::get_brush_sqf() const
{
	return get_brush();
}

sqf::value sqf::marker::get_color_sqf() const
{
	return get_color();
}

sqf::value sqf::marker::get_size_sqf() const
{
	auto val = get_size();
	auto arr = std::make_shared<arraydata>();
	arr->push_back(val[0]);
	arr->push_back(val[1]);
	return value(arr);
}
sqf::value sqf::marker::get_pos_sqf() const
{
	auto val = get_pos();
	auto arr = std::make_shared<arraydata>();
	arr->push_back(val[0]);
	arr->push_back(val[1]);
	arr->push_back(val[2]);
	return value(arr);
}

sqf::value sqf::marker::get_alpha_sqf() const
{
	return get_alpha();
}

sqf::value sqf::marker::get_direction_sqf() const
{
	return get_direction();
}
sqf::value sqf::marker::get_shape_sqf() const
{
	return get_shape_string();
}
