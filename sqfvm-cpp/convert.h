#pragma once
#include <memory>
#include "type.h"
#include "data.h"

namespace sqf
{
	std::shared_ptr<sqf::data> convert(std::shared_ptr<sqf::data> val, sqf::type type);
}