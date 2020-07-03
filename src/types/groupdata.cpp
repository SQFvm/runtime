#include "groupdata.h"
#include "virtualmachine.h"
#include "innerobj.h"

std::shared_ptr<sqf::groupdata> sqf::groupdata::create()
{
	auto grpptr = new sqf::groupdata();
	grpptr->m_isnull = true;
	auto grp = std::shared_ptr<sqf::groupdata>(grpptr);
	return grp;
}
std::shared_ptr<sqf::groupdata> sqf::groupdata::create(sqf::virtualmachine * vm, std::shared_ptr<sqf::sidedata> side)
{
	auto grpptr = new sqf::groupdata(side);
	auto grp = std::shared_ptr<sqf::groupdata>(grpptr);
	grp->m_groupid = vm->get_group_id(side);
	vm->push_group(grp);
	return grp;
}

void sqf::groupdata::add_unit(std::shared_ptr<innerobj> unit)
{
	m_units.push_back(unit);
	unit->group(shared_from_this());
}

void sqf::groupdata::drop_unit(std::shared_ptr<innerobj> unit)
{
	for (size_t i = 0; i < m_units.size(); i++)
	{
		if (unit.get() == m_units[i].get())
		{
			m_units[i] = m_units.back();
			unit->group(std::shared_ptr<groupdata>());
			m_units.pop_back();
			break;
		}
	}
}