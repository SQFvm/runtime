#include "innerobj.h"
#include "virtualmachine.h"
#include "groupdata.h"
#include "arraydata.h"
#include "configdata.h"
#include "value.h"
#include "string_op.h"
#include "objectdata.h"
#include <sstream>

sqf::innerobj::innerobj(std::string classname, bool isvehicle) :
	m_netid(),
	m_position_x(0),
	m_position_y(0),
	m_position_z(0),
	m_velocity_x(0),
	m_velocity_y(0),
	m_velocity_z(0),
	m_damage(0),
	m_classname(std::move(classname)),
	m_varname(),
	m_group(),
	m_is_vehicle(isvehicle),
	m_has_driver(false),
	m_has_gunner(false),
	m_has_commander(false),
	mtransportSoldier(0),
	m_parent_object(),
	m_driver(),
	m_gunner(),
	m_commander(),
	m_soldiers()
{
	m_driver = objectdata::objnull_data();
	m_gunner = objectdata::objnull_data();
	m_commander = objectdata::objnull_data();
}

std::string sqf::innerobj::tosqf() const
{
	if (m_varname.empty())
	{
		std::stringstream sstream;
		sstream << static_cast<const void*>(this) << "# " << this->m_netid << ": " << m_classname;
		return sstream.str();
	}
	else
	{
		return m_varname;
	}
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
	return arraydata::distance2dsqr(std::array<double, 2> { m_position_x, m_position_y }, otherpos);
}
double sqf::innerobj::distance2d(std::array<double, 2> otherpos) const
{
	return arraydata::distance2d(std::array<double, 2> { m_position_x, m_position_y }, otherpos);
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
	if (m_group)
		m_group->drop_unit(shared_from_this());
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
	vehConfig = vehConfig->navigate(m_classname).data<configdata>();
	if (vehConfig->is_null())
	{
		return false;
	}
	m_has_driver = vehConfig->cfgvalue("hasDriver", false);
	m_has_gunner = vehConfig->cfgvalue("hasGunner", false);
	m_has_commander = vehConfig->cfgvalue("hasCommander", false);
	mtransportSoldier = static_cast<size_t>(vehConfig->cfgvalue("transportSoldier", 0));
	m_soldiers.resize(mtransportSoldier);
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
	obj->m_netid = netid;
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
	if (val->is_null() || val->obj()->is_vehicle() || m_soldiers.size() >= mtransportSoldier)
	{
		return false;
	}
	val->obj()->m_parent_object = std::make_shared<objectdata>(this->shared_from_this());
	m_soldiers.push_back(val);
	return true;
}
void sqf::innerobj::soldiers_pop_back()
{
	auto back = m_soldiers.back();
	if (!back->is_null())
	{
		back->obj()->m_parent_object = objectdata::objnull_data();
	}
	m_soldiers.pop_back();
}
void sqf::innerobj::soldiers_erase(std::shared_ptr<sqf::objectdata> obj)
{
	auto res = std::find(m_soldiers.begin(), m_soldiers.end(), obj);
	if (res != m_soldiers.end())
	{
		if (!(*res)->is_null())
		{
			(*res)->obj()->m_parent_object = objectdata::objnull_data();
		}
		m_soldiers.erase(res);
	}
}
void sqf::innerobj::driver(std::shared_ptr<sqf::objectdata> val)
{
	if (m_has_driver && (val->is_null() || !val->obj()->is_vehicle()))
	{
		if (!m_driver->is_null())
		{
			m_driver->obj()->m_parent_object = objectdata::objnull_data();
		}
		m_driver = val;
		if (!val->is_null())
		{
			m_driver->obj()->m_parent_object = std::make_shared<objectdata>(this->shared_from_this());
		}
	}
}
void sqf::innerobj::gunner(std::shared_ptr<sqf::objectdata> val)
{
	if (m_has_gunner && (val->is_null() || !val->obj()->is_vehicle()))
	{
		if (!m_gunner->is_null())
		{
			m_gunner->obj()->m_parent_object = objectdata::objnull_data();
		}
		m_gunner = val;
		if (!val->is_null())
		{
			m_gunner->obj()->m_parent_object = std::make_shared<objectdata>(this->shared_from_this());
		}
	}
}
void sqf::innerobj::commander(std::shared_ptr<sqf::objectdata> val)
{
	if (m_has_commander && (val->is_null() || !val->obj()->is_vehicle()))
	{
		if (!m_commander->is_null())
		{
			m_commander->obj()->m_parent_object = objectdata::objnull_data();
		}
		m_commander = val;
		if (!val->is_null())
		{
			m_commander->obj()->m_parent_object = std::make_shared<objectdata>(this->shared_from_this());
		}
	}
}
void sqf::innerobj::clear_driver() { driver(objectdata::objnull_data()); }
void sqf::innerobj::clear_gunner() { gunner(objectdata::objnull_data()); }
void sqf::innerobj::clear_commander() { commander(objectdata::objnull_data()); }