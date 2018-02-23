#include "commandmap.h"
#include "configdata.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "value.h"
#include <algorithm>


using namespace sqf;
namespace
{
	std::shared_ptr<value> greaterthengreaterthen_config_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto cd = left->data<configdata>();
		auto navnode = right->as_string();
		return cd->navigate(navnode);
	}
	std::shared_ptr<value> confignull__(virtualmachine* vm)
	{
		return configdata::configNull();
	}
	std::shared_ptr<value> configfile__(virtualmachine* vm)
	{
		return configdata::configFile();
	}
	std::shared_ptr<value> configname_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->name());
	}
	std::shared_ptr<value> select_config_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto cd = left->data<configdata>();
		auto index = right->as_int();
		if (index >= (int)cd->size() || index < 0)
		{
			return configdata::configNull();
		}
		return (*cd)[index];
	}
	std::shared_ptr<value> count_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->size());
	}
	std::shared_ptr<value> confighierarchy_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		std::vector<std::shared_ptr<value>> parents;
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
	std::shared_ptr<value> inheritsfrom_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		return cd->logicparent();
	}
	std::shared_ptr<value> isnumber_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->cfgvalue()->dtype() == sqf::type::SCALAR);
	}
	std::shared_ptr<value> istext_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->cfgvalue()->dtype() == sqf::type::STRING);
	}
	std::shared_ptr<value> isclass_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(!cd->cfgvalue().get());
	}
	std::shared_ptr<value> isarray_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		return std::make_shared<sqf::value>(cd->cfgvalue()->dtype() == sqf::type::ARRAY);
	}
	std::shared_ptr<value> getnumber_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		return cd->cfgvalue()->dtype() == sqf::type::SCALAR ? cd->cfgvalue() : std::make_shared<sqf::value>(0);
	}
	std::shared_ptr<value> gettext_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		return cd->cfgvalue()->dtype() == sqf::type::STRING ? cd->cfgvalue() : std::make_shared<sqf::value>(L"");
	}
	std::shared_ptr<value> getarray_config(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cd = right->data<configdata>();
		return cd->cfgvalue()->dtype() == sqf::type::ARRAY ? cd->cfgvalue() : std::make_shared<sqf::value>(std::make_shared<sqf::arraydata>(), sqf::type::ARRAY);
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
