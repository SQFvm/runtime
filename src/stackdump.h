#pragma once
#include <string>

namespace sqf
{
	class sqfnamespace;
	namespace diagnostics
	{
		struct stackdump
		{
			std::shared_ptr<sqf::sqfnamespace> namespace_used;
			size_t line;
			size_t column;
			std::string file;
			std::string dbginf;
			std::string callstack_name;
			std::string scope_name;
		};
	}
}