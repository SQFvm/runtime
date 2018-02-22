#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "configdata.h"
#include <sstream>

using namespace sqf;
namespace
{
	std::shared_ptr<value> cmds___(virtualmachine* vm)
	{
		std::vector<std::shared_ptr<sqf::value>> outarr;
		auto str = std::make_shared<sqf::value>(L"n");
		for (auto pair : commandmap::get().all_n())
		{
			outarr.push_back(std::make_shared<sqf::value>(std::vector<std::shared_ptr<sqf::value>> { str,
				std::make_shared<sqf::value>(pair.first)
			}));
		}
		str = std::make_shared<sqf::value>(L"u");
		for (auto pair : commandmap::get().all_u())
		{
			for each (auto it in *pair.second.get())
			{
				outarr.push_back(std::make_shared<sqf::value>(std::vector<std::shared_ptr<sqf::value>> { str,
					std::make_shared<sqf::value>(pair.first),
					std::make_shared<sqf::value>(sqf::type_str(it->rtype()))
				}));
			}
		}
		str = std::make_shared<sqf::value>(L"b");
		for (auto pair : commandmap::get().all_b())
		{
			for each (auto it in *pair.second.get())
			{
				outarr.push_back(std::make_shared<sqf::value>(std::vector<std::shared_ptr<sqf::value>> { str,
					std::make_shared<sqf::value>(sqf::type_str(it->ltype())),
					std::make_shared<sqf::value>(pair.first),
					std::make_shared<sqf::value>(sqf::type_str(it->rtype()))
				}));
			}
		}
		return std::make_shared<sqf::value>(outarr);
	}
	std::shared_ptr<value> tree___string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto str = right->as_string();
		auto sstream = std::wstringstream();
		vm->parse_sqf(str, &sstream);
		return std::make_shared<value>(sstream.str());
	}
	std::shared_ptr<value> help___string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		std::wstringstream sstream;
		auto str = right->as_string();
		for (auto pair : commandmap::get().all_n())
		{
			if (wstr_cmpi(pair.first.c_str(), pair.first.length(), str.c_str(), str.length()) != 0)
				continue;
			auto cmd = pair.second;
			if (cmd->desc().empty())
				vm->out() << L"NULAR '" << pair.first << L"'\t<" << cmd->name() << L" " << L">" << std::endl;
			else
				vm->out() << L"NULAR '" << pair.first << L"'\t<" << cmd->name() << L" " << L">\t" << cmd->desc() << std::endl;
		}
		for (auto pair : commandmap::get().all_u())
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
		for (auto pair : commandmap::get().all_b())
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
	std::shared_ptr<value> configparse___string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto str = right->as_string();
		auto cd = std::make_shared<sqf::configdata>();
		vm->parse_config(str, cd);
		return std::make_shared<value>(cd, type::CONFIG);
	}
	std::shared_ptr<value> merge___config_config(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto target = left->data<configdata>();
		auto source = right->data<configdata>();
		source->mergeinto(target);
		return std::make_shared<value>();
	}
}
void sqf::commandmap::initsqfvmcmds(void)
{
	add(unary(L"tree__", sqf::type::STRING, L"Returns a string containing the abstract syntax tree for the provided SQF expression.", tree___string));
	add(nular(L"cmds__", L"Returns a string containing all commands available as string that can be converted to an array.", cmds___));
	add(unary(L"help__", sqf::type::STRING, L"Displays all available information for a single command.", help___string));
	add(unary(L"configparse__", sqf::type::STRING, L"Parses provided string as config into a new config object.", configparse___string));
	add(binary(4, L"merge__", sqf::type::CONFIG, sqf::type::CONFIG, L"Merges contents from the right config into the left config. Duplicate entries will be overriden. Contents will not be copied but referenced.", merge___config_config));
}
