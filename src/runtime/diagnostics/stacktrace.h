#pragma once

#include "../frame.h"

#include <vector>
#include <string>

namespace sqf::runtime::diagnostics
{

	struct stacktrace
	{
		std::vector<sqf::runtime::frame> frames;

		stacktrace() {}
		stacktrace(std::vector<sqf::runtime::frame> f) : frames(f) {}
	};
}