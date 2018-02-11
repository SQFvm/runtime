#include "full.h"
#include <sstream>

std::wstring sqf::configdata::tosqf(void) const
{
	std::wstringstream sstream;
	for (size_t i = logicalparentcount(); i != ~0; i--)
	{
		auto node = std::shared_ptr<configdata>((configdata*)this, [](configdata*) {});;
		for (size_t j = 0; j < i; j++)
		{
			node = node->mlogicparent.lock();
		}
		sstream << node->mname;
		if (i != 0)
		{
			sstream << L'/';
		}
	}
	return sstream.str();
}

void sqf::configdata::mergeinto(std::shared_ptr<configdata> cd)
{
	for each (auto val in innervector())
	{
		if (val->dtype() != sqf::type::CONFIG)
			continue;
		auto subcd = val->data<configdata>();
		auto othercd = cd->navigate_unsafe(subcd->mname);
		if (othercd.get())
		{
			subcd->mergeinto(othercd->data<configdata>());
		}
		else
		{
			cd->push_back(val);
		}
	}
}

sqf::value_s sqf::configdata::configFile(void)
{
	static std::shared_ptr<sqf::configdata> cdata = std::make_shared<sqf::configdata>();
	return std::make_shared<sqf::value>(cdata, sqf::type::CONFIG);
}

sqf::value_s sqf::configdata::configNull(void)
{
	static std::shared_ptr<sqf::configdata> cdata = std::make_shared<sqf::configdata>(L"");
	return std::make_shared<sqf::value>(cdata, sqf::type::CONFIG);
}
