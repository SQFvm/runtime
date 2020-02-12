#pragma once
#include "parsing/astnode.h"
#include <vector>
#include <string>
namespace sqf
{
	class virtualmachine;
	namespace linting
	{
		enum check
		{
			private_var_usage
		};
		void add_to(virtualmachine* vm, check type);
	}
}