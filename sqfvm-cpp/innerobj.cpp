#include "innerobj.h"
#include "virtualmachine.h"
#include <sstream>

std::string sqf::innerobj::tosqf(void) const
{
	auto sstream = std::stringstream();
	sstream << (const void*)this << "# " << this->mnetid << ": " << mclassname;
	return sstream.str();
}

std::shared_ptr<sqf::innerobj> sqf::innerobj::create(sqf::virtualmachine* vm, std::string classname, bool isvehicle)
{
	auto objptr = new innerobj(classname, isvehicle);
	auto obj = std::shared_ptr<innerobj>(objptr);
	auto netid = vm->push_obj(obj);
	obj->mnetid = netid;
	return obj;
}
