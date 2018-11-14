#include "configdata.h"
#include "value.h"
#include <sstream>

std::shared_ptr<sqf::value> sqf::configdata::parent_unsafe() const 
{
	std::weak_ptr<configdata> lparent = mlogicparent;
	while (!lparent.expired())
	{
		auto lockparent = lparent.lock();
		lparent = lockparent->mlogicparent;
		auto res = navigate_unsafe(mname);
		if (res)
		{
			return res;
		}
	}
	return std::shared_ptr<sqf::value>();
}

std::shared_ptr<sqf::value> sqf::configdata::navigate_unsafe(std::string_view nextnode) const
{
	for (auto it : innervector())
	{
		if (it->dtype() != type::CONFIG)
			continue;
		auto cd = it->data<sqf::configdata>();
		if (str_cmpi(cd->mname.c_str(), -1, nextnode.data(), -1) == 0)
			return it;
	}
	return std::shared_ptr<sqf::value>();
}

std::shared_ptr<sqf::value> sqf::configdata::navigate_full_unsafe(std::string_view nextnode) const
{
	auto it = navigate_unsafe(nextnode);
	if (it)
	{
		return it;
	}
	else
	{
		std::shared_ptr<sqf::value> p;
		while ((p = parent_unsafe()).get())
		{
			it = navigate_full_unsafe(nextnode);
			if (it)
				return it;
		}
		return std::shared_ptr<sqf::value>();
	}
}

std::string sqf::configdata::tosqf() const
{
	std::stringstream sstream;
	for (size_t i = logicalparentcount(); i != (size_t)~0; i--)
	{
		auto node = std::shared_ptr<configdata>(const_cast<configdata*>(this), [](configdata*) {});
		for (size_t j = 0; j < i; j++)
		{
			node = node->mlogicparent.lock();
		}
		sstream << node->mname;
		if (i != 0)
		{
			sstream << '/';
		}
	}
	return sstream.str();
}

bool sqf::configdata::is_kind_of(std::string_view s) const
{
	if (str_cmpi(mname.c_str(), -1, s.data(), -1) == 0)
	{
		return true;
	}
	auto node = this->parent()->data<configdata>();
	while (!node->is_null())
	{
		if (str_cmpi(node->name().c_str(), -1, s.data(), -1) == 0)
		{
			return true;
		}
		node = node->parent()->data<configdata>();
	}
	return false;
}

std::shared_ptr<sqf::value> sqf::configdata::logicparent() const
{
	return mlogicparent.expired() ? configNull() : std::make_shared<sqf::value>(mlogicparent.lock(), type::CONFIG);
}

bool sqf::configdata::cfgvalue(std::string_view key, bool def) const
{
	auto node = navigate(key)->data<configdata>();
	if (node->is_null() || (node->cfgvalue()->dtype() != type::BOOL && node->cfgvalue()->dtype() != type::SCALAR))
	{
		return def;
	}
	return node->cfgvalue()->as_bool();
}
float sqf::configdata::cfgvalue(std::string_view key, float def) const
{
	auto node = navigate(key)->data<configdata>();
	if (node->is_null() || node->cfgvalue()->dtype() != type::SCALAR)
	{
		return def;
	}
	return node->cfgvalue()->as_float();
}
std::string sqf::configdata::cfgvalue(std::string_view key, std::string def) const
{
	auto node = navigate(key)->data<configdata>();
	if (node->is_null() || node->cfgvalue()->dtype() != type::SCALAR)
	{
		return def;
	}
	return node->cfgvalue()->as_string();
}

void sqf::configdata::mergeinto(std::shared_ptr<configdata> cd)
{
	for (auto& val : innervector())
	{
		if (val->dtype() != sqf::type::CONFIG)
			continue;
		auto subcd = val->data<configdata>();
		auto othercd = cd->navigate_unsafe(subcd->mname);
		if (othercd)
		{
			subcd->mergeinto(othercd->data<configdata>());
		}
		else
		{
			cd->push_back(val);
		}
	}
}

std::shared_ptr<sqf::value> sqf::configdata::configFile()
{
	static std::shared_ptr<sqf::configdata> cdata = std::make_shared<sqf::configdata>();
	return std::make_shared<sqf::value>(cdata, sqf::type::CONFIG);
}

std::shared_ptr<sqf::value> sqf::configdata::configNull()
{
	static std::shared_ptr<sqf::configdata> cdata = std::make_shared<sqf::configdata>("");
	return std::make_shared<sqf::value>(cdata, sqf::type::CONFIG);
}
