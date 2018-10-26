#pragma once
#include <string>

namespace sqf
{
	class virtualmachine;
	namespace parse
	{
		namespace preprocessor
		{
			std::string parse(sqf::virtualmachine* vm, std::string, bool &errflag, std::string filename);
		}
	}
}