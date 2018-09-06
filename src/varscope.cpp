#include "varscope.h"
#include "value.h"

std::shared_ptr<sqf::value> sqf::varscope::getvar(std::string key)
{
	auto it = getvar_empty(key); return it.get() ? it : std::make_shared<value>();
}
