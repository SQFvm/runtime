#include "innerobj.h"
#include "virtualmachine.h"
#include "groupdata.h"
#include "arraydata.h"
#include "configdata.h"
#include "value.h"
#include "string_op.h"
#include "objectdata.h"
#include <sstream>

sqf::innerobj::innerobj(std::string classname, bool isvehicle) : mdamage(0), mclassname(std::move(classname)), misvehicle(isvehicle)
{
	mdriver = objectdata::objnull_data();
	mgunner = objectdata::objnull_data();
	mcommander = objectdata::objnull_data();
}

std::string sqf::innerobj::tosqf() const
{
	std::stringstream sstream;
	sstream << static_cast<const void*>(this) << "# " << this->mnetid << ": " << mclassname;
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

bool sqf::innerobj::iskindof(std::string_view cfgname) const {
	auto configbin = configdata::configFile().data<configdata>();
	auto cfgVehicles = configbin->navigate("CfgVehicles").data<configdata>();
	if (cfgVehicles->is_null())
	{
		return false;
	}
	auto node = cfgVehicles->navigate(this->classname()).data<configdata>();
	return node->is_kind_of(cfgname);
}

void sqf::innerobj::destroy(sqf::virtualmachine * vm)
{
	if (mgroup)
		mgroup->drop_unit(shared_from_this());
	vm->drop_obj(this);
}

bool sqf::innerobj::update_values_from_configbin()
{ 
	auto configbin = configdata::configFile().data<configdata>();
	auto vehConfig = configbin->navigate("CfgVehicles").data<configdata>();
	if (vehConfig->is_null())
	{
		return false;
	}
	vehConfig = vehConfig->navigate(mclassname).data<configdata>();
	if (vehConfig->is_null())
	{
		return false;
	}
	mhasDriver = vehConfig->cfgvalue("hasDriver", false);
	mhasGunner = vehConfig->cfgvalue("hasGunner", false);
	mhasCommander = vehConfig->cfgvalue("hasCommander", false);
	mtransportSoldier = static_cast<size_t>(vehConfig->cfgvalue("transportSoldier", 0));
	msoldiers.resize(mtransportSoldier);
	return true;
}

std::shared_ptr<sqf::innerobj> sqf::innerobj::create(sqf::virtualmachine* vm, std::string classname, bool isvehicle)
{
	// Fields for whole Crew:
	// hasDriver = true / false;// also refers to a Pilot 
	// hasGunner = true / false;
	// hasCommander = true / false;
	// 
	// // a copilot or codriver is the 1st passenger
	// 
	// total occupancy = hasGunner + hasDriver + hasCommander + transportSoldier;
	auto objptr = new innerobj(classname, isvehicle);
	objptr->update_values_from_configbin();
	auto obj = std::shared_ptr<innerobj>(objptr);
	auto netid = vm->push_obj(obj);
	obj->mnetid = netid;
	return obj;
}







bool sqf::innerobj::soldiers_push_back(sqf::value val)
{
	if (val.dtype() != OBJECT)
	{
		return false;
	}
	return soldiers_push_back(val.data<sqf::objectdata>());
}
bool sqf::innerobj::soldiers_push_back(std::shared_ptr<sqf::objectdata> val)
{
	if (val->is_null() || val->obj()->is_vehicle() || msoldiers.size() >= mtransportSoldier)
	{
		return false;
	}
	val->obj()->mparent_object = std::make_shared<objectdata>(this->shared_from_this());
	msoldiers.push_back(val);
	return true;
}
void sqf::innerobj::soldiers_pop_back()
{
	auto back = msoldiers.back();
	if (!back->is_null())
	{
		back->obj()->mparent_object = objectdata::objnull_data();
	}
	msoldiers.pop_back();
}
void sqf::innerobj::soldiers_erase(std::shared_ptr<sqf::objectdata> obj)
{
	auto res = std::find(msoldiers.begin(), msoldiers.end(), obj);
	if (res != msoldiers.end())
	{
		if (!(*res)->is_null())
		{
			(*res)->obj()->mparent_object = objectdata::objnull_data();
		}
		msoldiers.erase(res);
	}
}
void sqf::innerobj::driver(std::shared_ptr<sqf::objectdata> val)
{
	if (mhasDriver && (val->is_null() || !val->obj()->is_vehicle()))
	{
		if (!mdriver->is_null())
		{
			mdriver->obj()->mparent_object = objectdata::objnull_data();
		}
		mdriver = val;
		if (!val->is_null())
		{
			mdriver->obj()->mparent_object = std::make_shared<objectdata>(this->shared_from_this());
		}
	}
}
void sqf::innerobj::gunner(std::shared_ptr<sqf::objectdata> val)
{
	if (mhasGunner && (val->is_null() || !val->obj()->is_vehicle()))
	{
		if (!mgunner->is_null())
		{
			mgunner->obj()->mparent_object = objectdata::objnull_data();
		}
		mgunner = val;
		if (!val->is_null())
		{
			mgunner->obj()->mparent_object = std::make_shared<objectdata>(this->shared_from_this());
		}
	}
}
void sqf::innerobj::commander(std::shared_ptr<sqf::objectdata> val)
{
	if (mhasCommander && (val->is_null() || !val->obj()->is_vehicle()))
	{
		if (!mcommander->is_null())
		{
			mcommander->obj()->mparent_object = objectdata::objnull_data();
		}
		mcommander = val;
		if (!val->is_null())
		{
			mcommander->obj()->mparent_object = std::make_shared<objectdata>(this->shared_from_this());
		}
	}
}
void sqf::innerobj::clear_driver() { driver(objectdata::objnull_data()); }
void sqf::innerobj::clear_gunner() { gunner(objectdata::objnull_data()); }
void sqf::innerobj::clear_commander() { commander(objectdata::objnull_data()); }