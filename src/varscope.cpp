#include "varscope.h"
#include "value.h"

std::shared_ptr<sqf::value> sqf::varscope::get_variable(std::string key)
{
	auto it = get_variable_empty(key); return it.get() ? it : std::make_shared<value>();
}
