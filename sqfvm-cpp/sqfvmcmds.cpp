#include "full.h"
#include <sstream>

using namespace sqf;
namespace
{
	value_s help___(const virtualmachine* vm)
	{
		std::wstringstream sstream;
		bool fdone = false;
		sstream << L"[";
		for each (auto cmd in commandmap::get().all_n())
		{
			if (fdone) { sstream << L", "; }
			else { fdone = true; }
			sstream << L'"' << cmd.first << L'"';
		}
		for each (auto cmd in commandmap::get().all_u())
		{
			if (fdone) { sstream << L", "; }
			else { fdone = true; }
			sstream << L'"' << cmd.first << L'"';
		}
		for each (auto cmd in commandmap::get().all_b())
		{
			if (fdone) { sstream << L", "; }
			else { fdone = true; }
			sstream << L'"' << cmd.first << L'"';
		}
		sstream << L"]";
		auto str = sstream.str();
		vm->out() << str << std::endl;
		return std::make_shared<value>(str);
	}
}
void sqf::commandmap::initsqfvmcmds(void)
{
	add(nular(L"help__", L"Displays all commands available with usage, precedence and type.", help___));
}
