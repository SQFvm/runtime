#include "marker.h"
#include "value.h"
#include "arraydata.h"


sqf::marker::marker()
{
}

std::shared_ptr<sqf::value> sqf::marker::get_text_sqf() const
{
	return std::make_shared<value>(get_text());
}
std::shared_ptr<sqf::value> sqf::marker::get_type_sqf() const
{
	return std::make_shared<value>(get_type());
}
std::shared_ptr<sqf::value> sqf::marker::get_brush_sqf() const
{
	return std::make_shared<sqf::value>(get_brush());
}

std::shared_ptr<sqf::value> sqf::marker::get_color_sqf() const
{
	return std::make_shared<value>(get_color());
}

std::shared_ptr<sqf::value> sqf::marker::get_size_sqf() const
{
	auto val = get_size();
	auto arr = std::make_shared<arraydata>();
	arr->push_back(std::make_shared<value>(val[0]));
	arr->push_back(std::make_shared<value>(val[1]));
	return std::make_shared<value>(arr, sqf::type::ARRAY);
}
std::shared_ptr<sqf::value> sqf::marker::get_pos_sqf() const
{
	auto val = get_pos();
	auto arr = std::make_shared<arraydata>();
	arr->push_back(std::make_shared<value>(val[0]));
	arr->push_back(std::make_shared<value>(val[1]));
	arr->push_back(std::make_shared<value>(val[2]));
	return std::make_shared<value>(arr, sqf::type::ARRAY);
}

std::shared_ptr<sqf::value> sqf::marker::get_alpha_sqf() const
{
	return std::make_shared<value>(get_alpha());
}

std::shared_ptr<sqf::value> sqf::marker::get_direction_sqf() const
{
	return std::make_shared<value>(get_direction());
}
std::shared_ptr<sqf::value> sqf::marker::get_shape_sqf() const
{
	return std::make_shared<sqf::value>(get_shape_string());
}
