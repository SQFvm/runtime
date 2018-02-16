#include "innerobj.h"
#include "virtualmachine.h"
#include <sstream>

std::wstring sqf::innerobj::tosqf(void) const
{
	auto sstream = std::wstringstream();
	sstream << (const void*)this << L"# " << this->mnetid << L": " << mclassname;
	return sstream.str();
}

std::shared_ptr<sqf::innerobj> sqf::innerobj::create(sqf::virtualmachine* vm, std::wstring classname, bool isvehicle)
{
	auto obj = std::make_shared<innerobj>(classname, isvehicle);
	auto netid = vm->push_obj(obj);
	obj->mnetid = netid;
	return obj;
}
