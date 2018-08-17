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
		auto str = std::make_shared<sqf::value>("n");
		for (auto pair : commandmap::get().all_n())
		{
			outarr.push_back(std::make_shared<sqf::value>(std::vector<std::shared_ptr<sqf::value>> { str,
				std::make_shared<sqf::value>(pair.first)
			}));
		}
		str = std::make_shared<sqf::value>("u");
		for (auto pair : commandmap::get().all_u())
		{
			for (auto it : *pair.second.get())
			{
				outarr.push_back(std::make_shared<sqf::value>(std::vector<std::shared_ptr<sqf::value>> { str,
					std::make_shared<sqf::value>(pair.first),
					std::make_shared<sqf::value>(sqf::type_str(it->rtype()))
				}));
			}
		}
		str = std::make_shared<sqf::value>("b");
		for (auto pair : commandmap::get().all_b())
		{
			for (auto it : *pair.second.get())
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
		std::stringstream sstream;
		vm->parse_sqf(str, &sstream);
		return std::make_shared<value>(sstream.str());
	}
	std::shared_ptr<value> help___string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		std::stringstream sstream;
		auto str = right->as_string();
		bool wasfound = false;
		for (auto pair : commandmap::get().all_n())
		{
			if (str_cmpi(pair.first.c_str(), pair.first.length(), str.c_str(), str.length()) != 0)
				continue;
			auto cmd = pair.second;
			if (cmd->desc().empty())
				vm->out() << "NULAR '" << pair.first << "'\t<" << cmd->name() << " " << ">" << std::endl;
			else
				vm->out() << "NULAR '" << pair.first << "'\t<" << cmd->name() << " " << ">\t" << cmd->desc() << std::endl;
			wasfound = true;
		}
		for (auto pair : commandmap::get().all_u())
		{
			if (str_cmpi(pair.first.c_str(), pair.first.length(), str.c_str(), str.length()) != 0)
				continue;
			auto cmds = pair.second;
			for (auto cmd : *cmds)
			{
				if (!cmd->desc().empty())
					vm->out() << "UNARY '" << pair.first << "'\t<" << cmd->name() << " " << sqf::type_str(cmd->rtype()) << ">\t" << cmd->desc() << std::endl;
				else
					vm->out() << "UNARY '" << pair.first << "'\t<" << cmd->name() << " " << sqf::type_str(cmd->rtype()) << ">" << std::endl;
				wasfound = true;
			}
		}
		for (auto pair : commandmap::get().all_b())
		{
			if (str_cmpi(pair.first.c_str(), pair.first.length(), str.c_str(), str.length()) != 0)
				continue;
			auto cmds = pair.second;
			for (auto cmd : *cmds)
			{
				if (!cmd->desc().empty())
					vm->out() << "BINARY '" << pair.first << "'\t<" << sqf::type_str(cmd->ltype()) << " " << cmd->name() << " " << sqf::type_str(cmd->rtype()) << ">\t" << cmd->desc() << std::endl;
				else
					vm->out() << "BINARY '" << pair.first << "'\t<" << sqf::type_str(cmd->ltype()) << " " << cmd->name() << " " << sqf::type_str(cmd->rtype()) << ">" << std::endl;
			}
			wasfound = true;
		}
		if (!wasfound)
		{
			vm->out() << "Could not find any command with that name." << std::endl;
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
	add(unary("tree__", sqf::type::STRING, "Returns a string containing the abstract syntax tree for the provided SQF expression.", tree___string));
	add(nular("cmds__", "Returns an array containing all commands available.", cmds___));
	add(unary("help__", sqf::type::STRING, "Displays all available information for a single command.", help___string));
	add(unary("configparse__", sqf::type::STRING, "Parses provided string as config into a new config object.", configparse___string));
	add(binary(4, "merge__", sqf::type::CONFIG, sqf::type::CONFIG, "Merges contents from the right config into the left config. Duplicate entries will be overriden. Contents will not be copied but referenced.", merge___config_config));
}
