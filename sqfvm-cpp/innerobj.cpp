#include "innerobj.h"
#include "virtualmachine.h"
#include "groupdata.h"
#include "arraydata.h"
#include <sstream>

std::string sqf::innerobj::tosqf(void) const
{
	std::stringstream sstream;
	sstream << (const void*)this << "# " << this->mnetid << ": " << mclassname;
	return sstream.str();
}

double sqf::innerobj::distance3dsqr(std::array<double, 3> otherpos) const
{
	return arraydata::distance3dsqr(pos(), otherpos);
}
double sqf::innerobj::distance3d(std::array<double, 3> otherpos) const
{
	return arraydata::distance3d(pos(), otherpos);
}
double sqf::innerobj::distance2dsqr(std::array<double, 2> otherpos) const
{
	return arraydata::distance2dsqr(std::array<double, 2> { mposx, mposy }, otherpos);
}
double sqf::innerobj::distance2d(std::array<double, 2> otherpos) const
{
	return arraydata::distance2d(std::array<double, 2> { mposx, mposy }, otherpos);
}

void sqf::innerobj::destroy(sqf::virtualmachine * vm)
{
	if (mgroup.get())
	{
		mgroup->drop_unit(shared_from_this());
	}
	vm->drop_obj(this);
}

std::shared_ptr<sqf::innerobj> sqf::innerobj::create(sqf::virtualmachine* vm, std::string classname, bool isvehicle)
{
	auto objptr = new innerobj(classname, isvehicle);
	auto obj = std::shared_ptr<innerobj>(objptr);
	auto netid = vm->push_obj(obj);
	obj->mnetid = netid;
	return obj;
}
