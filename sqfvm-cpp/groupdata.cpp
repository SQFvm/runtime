#include "groupdata.h"
#include "virtualmachine.h"

std::shared_ptr<sqf::groupdata> sqf::groupdata::create(sqf::virtualmachine * vm, std::shared_ptr<sqf::sidedata> side)
{
	auto grpptr = new sqf::groupdata(side);
	auto grp = std::shared_ptr<sqf::groupdata>(grpptr);
	grp->mgroupid = vm->get_group_id(side);
	vm->push_group(grp);
	return grp;
}
