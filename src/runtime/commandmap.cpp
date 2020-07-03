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

void sqf::commandmap::add(std::shared_ptr<nularcmd> cmd)
{
	auto key = string_tolower(cmd->name());
	if (mnularcmd.find(key) != mnularcmd.end())
	{
		throw std::runtime_error("Duplicate Command.");
	}
	mnularcmd[key] = cmd;
}
void sqf::commandmap::add(std::shared_ptr<unarycmd> cmd)
{
	auto listsptr = munarycmd[string_tolower(cmd->name())];
	if (!listsptr.get())
	{
		listsptr = munarycmd[string_tolower(cmd->name())] = std::make_shared<std::vector<std::shared_ptr<unarycmd>>>();
	}
	if (std::find_if(listsptr->begin(), listsptr->end(), [cmd](std::shared_ptr<unarycmd> it) -> bool {
		return cmd->rtype() == sqf::type::ANY || it->rtype() == sqf::type::ANY || it->rtype() == cmd->rtype();
		}) != listsptr->end())
	{
		throw std::runtime_error("Duplicate Command.");
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
	if (std::find_if(listsptr->begin(), listsptr->end(), [cmd](std::shared_ptr<binarycmd> it) -> bool {
		auto risconflict = cmd->rtype() == sqf::type::ANY || it->rtype() == sqf::type::ANY || cmd->rtype() == it->rtype();
		auto lisconflict = cmd->ltype() == sqf::type::ANY || it->ltype() == sqf::type::ANY || cmd->ltype() == it->ltype();
		return risconflict && lisconflict;
		}) != listsptr->end())
	{
		throw std::runtime_error("Duplicate Command.");
	}
	listsptr->push_back(cmd);
}

bool sqf::commandmap::remove(std::string str)
{
	auto res = mnularcmd.find(string_tolower(str));
	if (res != mnularcmd.end())
	{
		mnularcmd.erase(res);
		return true;
	}
	return false;
}

bool sqf::commandmap::remove(std::string str, sqf::type rtype)
{
	auto res = munarycmd.find(string_tolower(str));
	if (res != munarycmd.end())
	{
		auto vec = res->second;
		vec->erase(std::remove_if(vec->begin(), vec->end(), [rtype](std::shared_ptr<unarycmd> it) -> bool { return it->matches(type::NA, rtype); }), vec->end());
		return true;
	}
	return false;
}

bool sqf::commandmap::remove(sqf::type ltype, std::string str, sqf::type rtype)
{
	auto res = mbinarycmd.find(string_tolower(str));
	if (res != mbinarycmd.end())
	{
		auto vec = res->second;
		vec->erase(std::remove_if(vec->begin(), vec->end(), [ltype, rtype](std::shared_ptr<binarycmd> it) -> bool { return it->matches(ltype, rtype); }), vec->end());
		return true;
	}
	return false;
}
