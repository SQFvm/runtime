#include "commandmap.h"
#include "cmd.h"
#include <stdexcept>

std::shared_ptr<sqf::unarycmd> sqf::commandmap::get(std::string_view str, type rtype)
{
	auto listsptr = munarycmd[string_tolower(str)];
	if (listsptr)
	{
		for (auto it : *listsptr)
		{
			if (it->matches(type::NA, rtype))
			{
				return it;
			}
		}
	}
	return std::shared_ptr<unarycmd>();
}

std::shared_ptr<sqf::binarycmd> sqf::commandmap::get(std::string_view str, type ltype, type rtype)
{
	auto listsptr = mbinarycmd[string_tolower(str)];
	if (listsptr)
	{
		for (auto it : *listsptr)
		{
			if (it->matches(ltype, rtype))
			{
				return it;
			}
		}
	}
	return std::shared_ptr<binarycmd>();
}

std::shared_ptr<sqf::unarycmd> sqf::commandmap::find(std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> list, type rtype)
{
	for (auto it : *list)
	{
		if (it->matches(type::NA, rtype))
		{
			return it;
		}
	}
	return std::shared_ptr<unarycmd>();
}

std::shared_ptr<sqf::binarycmd> sqf::commandmap::find(std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> list, type ltype, type rtype)
{
	for (auto it : *list)
	{
		if (it->matches(ltype, rtype))
		{
			return it;
		}
	}
	return std::shared_ptr<binarycmd>();
}

sqf::commandmap& sqf::commandmap::get()
{
	static sqf::commandmap map;
	return map;
}

void sqf::commandmap::add(std::shared_ptr<nularcmd> cmd) { mnularcmd[string_tolower(cmd->name())] = cmd; }
void sqf::commandmap::add(std::shared_ptr<unarycmd> cmd)
{
	auto listsptr = munarycmd[string_tolower(cmd->name())];
	if (!listsptr.get())
	{
		listsptr = munarycmd[string_tolower(cmd->name())] = std::make_shared<std::vector<std::shared_ptr<unarycmd>>>();
	}
	listsptr->push_back(cmd);
}
void sqf::commandmap::add(std::shared_ptr<binarycmd> cmd)
{
	if (cmd->precedence() > 10 || cmd->precedence() <= 0)
	{
		throw std::runtime_error("precedence needs to be > 0 & <= 10");
	}
	auto listsptr = mbinarycmd[string_tolower(cmd->name())];
	if (!listsptr.get())
	{
		listsptr = mbinarycmd[string_tolower(cmd->name())] = std::make_shared<std::vector<std::shared_ptr<binarycmd>>>();
	}
	listsptr->push_back(cmd);
}