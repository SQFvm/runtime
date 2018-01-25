#include "full.h"

std::shared_ptr<sqf::unarycmd> sqf::commandmap::get(std::wstring str, type rtype)
{
	auto listsptr = munarycmd[str];
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
	auto listsptr = mbinarycmd[str];
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

sqf::commandmap& sqf::commandmap::get(void)
{
	static sqf::commandmap map;
	return map;
}