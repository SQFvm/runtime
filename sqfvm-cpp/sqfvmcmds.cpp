#include "full.h"
#include <sstream>

using namespace sqf;
namespace
{
	value_s cmds___(const virtualmachine* vm)
	{
		std::wstringstream sstream;
		bool fdone = false;
		sstream << L"[";
		for each (auto pair in commandmap::get().all_n())
		{
			if (fdone) { sstream << L", "; }
			else { fdone = true; }
			sstream << L"[\"n\", \"" << pair.first << L"\"]";
		}
		for each (auto pair in commandmap::get().all_u())
		{
			if (fdone) { sstream << L", "; }
			else { fdone = true; }
			sstream << L"[\"u\", \"" << pair.first << L"\"]";
		}
		for each (auto pair in commandmap::get().all_b())
		{
			if (fdone) { sstream << L", "; }
			else { fdone = true; }
			sstream << L"[\"b\", \"" << pair.first << L"\"]";
		}
		sstream << L"]";
		auto str = sstream.str();
		return std::make_shared<value>(str);
	}
	value_s help___STRING(const virtualmachine* vm, value_s right)
	{
		std::wstringstream sstream;
		auto str = right->as_string();
		for each (auto pair in commandmap::get().all_n())
		{
			if (wstr_cmpi(pair.first.c_str(), pair.first.length(), str.c_str(), str.length()) != 0)
				continue;
			auto cmd = pair.second;
			if(cmd->desc().empty())
				vm->out() << L"NULAR '" << pair.first << L"'\t<" << cmd->name() << L" " << L">" << std::endl;
			else
				vm->out() << L"NULAR '" << pair.first << L"'\t<" << cmd->name() << L" " << L">\t" << cmd->desc() << std::endl;
		}
		for each (auto pair in commandmap::get().all_u())
		{
			if (wstr_cmpi(pair.first.c_str(), pair.first.length(), str.c_str(), str.length()) != 0)
				continue;
			auto cmds = pair.second;
			for each (auto cmd in *cmds)
			{
				if (!cmd->desc().empty())
					vm->out() << L"UNARY '" << pair.first << L"'\t<" << cmd->name() << L" " << sqf::type_str(cmd->rtype()) << L">\t" << cmd->desc() << std::endl;
				else
					vm->out() << L"UNARY '" << pair.first << L"'\t<" << cmd->name() << L" " << sqf::type_str(cmd->rtype()) << L">" << std::endl;
			}
		}
		for each (auto pair in commandmap::get().all_b())
		{
			if (wstr_cmpi(pair.first.c_str(), pair.first.length(), str.c_str(), str.length()) != 0)
				continue;
			auto cmds = pair.second;
			for each (auto cmd in *cmds)
			{
				if (!cmd->desc().empty())
					vm->out() << L"BINARY '" << pair.first << L"'\t<" << sqf::type_str(cmd->ltype()) << L" " << cmd->name() << L" " << sqf::type_str(cmd->rtype()) << L">\t" << cmd->desc() << std::endl;
				else
					vm->out() << L"BINARY '" << pair.first << L"'\t<" << sqf::type_str(cmd->ltype()) << L" " << cmd->name() << L" " << sqf::type_str(cmd->rtype()) << L">" << std::endl;
			}
		}
		return std::make_shared<value>();
	}
}
void sqf::commandmap::initsqfvmcmds(void)
{
	add(nular(L"cmds__", L"Returns a string containing all commands available as string that can be converted to an array.", cmds___));
	add(unary(L"help__", sqf::type::STRING, L"Displays all available information for a single command.", help___STRING));
}
