#include "commandmap.h"
#include "cmd.h"

std::shared_ptr<sqf::unarycmd> sqf::commandmap::get(std::wstring str, type rtype)
{
	auto listsptr = munarycmd[tolowerstring(str)];
	if (listsptr.get())
	{
		for each (auto it in *listsptr)
		{
			if (it->matches(type::NA, rtype))
			{
				return it;
			}
		}
	}
	return std::shared_ptr<unarycmd>();
}

std::shared_ptr<sqf::binarycmd> sqf::commandmap::get(std::wstring str, type ltype, type rtype)
{
	auto listsptr = mbinarycmd[tolowerstring(str)];
	if (listsptr.get())
	{
		for each (auto it in *listsptr)
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
	for each (auto it in *list)
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
	for each (auto it in *list)
	{
		if (it->matches(ltype, rtype))
		{
			return it;
		}
	}
	return std::shared_ptr<binarycmd>();
}

sqf::commandmap& sqf::commandmap::get(void)
{
	static sqf::commandmap map;
	return map;
}

void sqf::commandmap::add(std::shared_ptr<nularcmd> cmd) { mnularcmd[tolowerstring(cmd->name())] = cmd; }
void sqf::commandmap::add(std::shared_ptr<unarycmd> cmd)
{
	auto listsptr = munarycmd[tolowerstring(cmd->name())];
	if (!listsptr.get())
	{
		listsptr = munarycmd[tolowerstring(cmd->name())] = std::make_shared<std::vector<std::shared_ptr<unarycmd>>>();
	}
	listsptr->push_back(cmd);
}
void sqf::commandmap::add(std::shared_ptr<binarycmd> cmd)
{
	auto listsptr = mbinarycmd[tolowerstring(cmd->name())];
	if (!listsptr.get())
	{
		listsptr = mbinarycmd[tolowerstring(cmd->name())] = std::make_shared<std::vector<std::shared_ptr<binarycmd>>>();
	}
	listsptr->push_back(cmd);
}