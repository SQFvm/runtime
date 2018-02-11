#include "full.h"
#include <algorithm>


using namespace sqf;
namespace
{
	value_s greaterthengreaterthen_config_string(const virtualmachine* vm, value_s left, value_s right)
	{
		auto cd = left->data<configdata>();
		auto navnode = right->as_string();
		return cd->navigate(navnode);
	}
	value_s confignull__(const virtualmachine* vm)
	{
		return configdata::configNull();
	}
	value_s configfile__(const virtualmachine* vm)
	{
		return configdata::configFile();
	}
	value_s configname_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->name());
	}
	value_s select_config_scalar(const virtualmachine* vm, value_s left, value_s right)
	{
		auto cd = left->data<configdata>();
		auto index = right->as_int();
		if (index >= cd->size() || index < 0)
		{
			return configdata::configNull();
		}
		return (*cd)[index];
	}
	value_s count_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->size());
	}
	value_s confighierarchy_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		std::vector<value_s> parents;
		parents.push_back(right);
		while (cd->haslogicparent())
		{
			right = cd->logicparent();
			cd = right->data<configdata>();
			parents.push_back(right);
		}
		std::reverse(parents.begin(), parents.end());
		return std::make_shared<sqf::value>(parents);
	}
	value_s inheritsfrom_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->logicparent());
	}
	value_s isnumber_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->value()->dtype() == sqf::type::SCALAR);
	}
	value_s istext_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->value()->dtype() == sqf::type::STRING);
	}
	value_s isclass_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(!cd->value().get());
	}
	value_s isarray_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->value()->dtype() == sqf::type::ARRAY);
	}
	value_s getnumber_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		return cd->value()->dtype() == sqf::type::SCALAR ? cd->value() : std::make_shared<sqf::value>(0);
	}
	value_s gettext_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		return cd->value()->dtype() == sqf::type::STRING ? cd->value() : std::make_shared<sqf::value>(L"");
	}
	value_s getarray_config(const virtualmachine* vm, value_s right)
	{
		auto cd = right->data<configdata>();
		return cd->value()->dtype() == sqf::type::ARRAY ? cd->value() : std::make_shared<sqf::value>(std::make_shared<sqf::arraydata>(), sqf::type::ARRAY);
	}
}
void sqf::commandmap::initconfigcmds(void)
{
	add(nular(L"configNull", L"A non-existing Config.", confignull__));
	add(nular(L"configFile", L"Return root of config entries hierarchy.", configfile__));
	add(unary(L"configName", type::CONFIG, L"Returns name of config entry.", configname_config));
	add(binary(3, L">>", type::CONFIG, type::STRING, L"Returns subentry of config entry with given name.", greaterthengreaterthen_config_string));
	add(binary(7, L"/", type::CONFIG, type::STRING, L"Returns subentry of config entry with given name.", greaterthengreaterthen_config_string));
	add(binary(4, L"select", type::CONFIG, type::SCALAR, L"Selects an element at given index from provided config.", select_config_scalar));
	add(unary(L"count", type::CONFIG, L"Returns the number of sub-entries in provided config object.", count_config));
	add(unary(L"configHierarchy", type::CONFIG, L"Returns hierarchy of the given config class.", confighierarchy_config));
	add(unary(L"inheritsFrom", type::CONFIG, L"Returns base entry of config entry.", inheritsfrom_config));
	add(unary(L"isNumber", type::CONFIG, L"Returns base entry of config entry.", isnumber_config));
	add(unary(L"isText", type::CONFIG, L"Check if config entry represents text.", istext_config));
	add(unary(L"isClass", type::CONFIG, L"Check if config entry represents config class.", isclass_config));
	add(unary(L"isArray", type::CONFIG, L"Check if config entry represents array.", isarray_config));
	add(unary(L"getNumber", type::CONFIG, L"Extract number from config entry.", getnumber_config));
	add(unary(L"getText", type::CONFIG, L"Extract text from config entry.", gettext_config));
	add(unary(L"getArray", type::CONFIG, L"Extract array from config entry.", getarray_config));
}
