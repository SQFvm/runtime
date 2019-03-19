#include "objectdata.h"
#include "innerobj.h"
#include "type.h"
#include "value.h"

std::string sqf::objectdata::tosqf() const { if (mobj.expired()) return std::string("null"); auto sptr = mobj.lock(); return sptr->tosqf(); }

std::shared_ptr<sqf::objectdata> sqf::objectdata::objnull_data()
{
	static std::shared_ptr<sqf::objectdata> cdata = std::make_shared<sqf::objectdata>();
	return cdata;
}
std::shared_ptr<sqf::value> sqf::objectdata::objnull()
{
	return std::make_shared<sqf::value>(objnull_data(), type::OBJECT);
}
