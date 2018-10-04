#include "innerobj.h"
#include "virtualmachine.h"
#include "groupdata.h"
#include "arraydata.h"
#include "configdata.h"
#include "value.h"
#include "string_op.h"
#include <sstream>

std::string sqf::innerobj::tosqf() const
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

bool sqf::innerobj::iskindof(std::string cfgname)
{
	auto configbin = configdata::configFile()->data<configdata>();
	auto cfgVehicles = configbin->navigate("CfgVehicles")->data<configdata>();
	if (cfgVehicles->is_null())
	{
		return false;
	}
	auto node = cfgVehicles->navigate(this->classname())->data<configdata>();
	while (!node->is_null())
	{
		if (str_cmpi(node->name().c_str(), -1, cfgname.c_str(), -1) == 0)
		{
			return true;
		}
		node = node->parent()->data<configdata>();
	}
	return false;
}

void sqf::innerobj::destroy(sqf::virtualmachine * vm)
{
	if (mgroup)
		mgroup->drop_unit(shared_from_this());
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
