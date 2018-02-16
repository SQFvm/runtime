#include "varscope.h"
#include "value.h"

std::shared_ptr<sqf::value> sqf::varscope::getvar(std::wstring key)
{
	auto it = mvarmap.find(tolowerstring(key)); return it == mvarmap.end() ? std::make_shared<value>() : it->second;
}
