#include "../commandmap.h"
#include "../configdata.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include "../value.h"
#include "../codedata.h"
#include "../callstack_configclasses.h"
#include "../callstack_configproperties.h"
#include <algorithm>


using namespace sqf;
namespace
{
	value greaterthengreaterthen_config_string(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto cd = left.data<configdata>();
		auto navnode = right.as_string();
		return cd->navigate(navnode);
	}
	value confignull__(virtualmachine* vm)
	{
		return configdata::configNull();
	}
	value configfile__(virtualmachine* vm)
	{
		return configdata::configFile();
	}
	value configname_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return cd->name();
	}
	value select_config_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto cd = left.data<configdata>();
		auto index = right.as_int();
		if (index >= static_cast<int>(cd->size()) || index < 0)
		{
			vm->wrn() << "Provided index out of config range. Index: " << index << ", ConfigName: " << (cd->is_null() ? "configNull" : cd->name()) << '.' << std::endl;
			return configdata::configNull();
		}
		return (*cd)[index];
	}
	value count_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return cd->size();
	}
	value confighierarchy_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		std::vector<value> parents;
		parents.push_back(right);
		while (cd->has_logical_parent())
		{
			cd = cd->logical_parent().data<configdata>();
			parents.emplace_back(cd);
		}
		std::reverse(parents.begin(), parents.end());
		return sqf::value(parents);
	}
	value inheritsfrom_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return cd->logical_parent();
	}
	value isnumber_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return !cd->is_null() && cd->cfgvalue().dtype() == sqf::type::SCALAR;
	}
	value istext_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return !cd->is_null() && cd->cfgvalue().dtype() == sqf::type::STRING;
	}
	value isclass_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return !cd->is_null() && cd->cfgvalue().dtype() != NOTHING;
	}
	value isarray_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return !cd->is_null() && cd->cfgvalue().dtype() == sqf::type::ARRAY;
	}
	value getnumber_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return !cd->is_null() && cd->cfgvalue().dtype() == sqf::type::SCALAR ? cd->cfgvalue() : value(0);
	}
	value gettext_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return !cd->is_null() && cd->cfgvalue().dtype() == sqf::type::STRING ? cd->cfgvalue() : value("");
	}
	value getarray_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return !cd->is_null() && cd->cfgvalue().dtype() == sqf::type::ARRAY ? cd->cfgvalue() : sqf::value(std::make_shared<sqf::arraydata>());
	}
	value isnull_config(virtualmachine* vm, value::cref right)
	{
		auto cd = right.data<configdata>();
		return cd->is_null();
	}
	value configclasses_code_config(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto exp = left.as_string();
		auto config = right.data<configdata>();


		auto condition_stack = std::make_shared<callstack>(vm->active_vmstack()->stacks_top()->get_namespace());
		vm->parse_sqf(exp, condition_stack);
		auto condition = std::make_shared<codedata>(condition_stack);

		auto cs = std::make_shared<sqf::callstack_configclasses>(vm->active_vmstack()->stacks_top()->get_namespace(), config, condition);
		vm->active_vmstack()->push_back(cs);
		return {};
	}
	value configproperties_array(virtualmachine* vm, value::cref right)
	{
		auto arr = right.data<arraydata>();
		if (!arr->check_type(vm, std::array<sqf::type, 3>{ CONFIG, STRING, type::BOOL }, 1))
		{
			return {};
		}
		auto config = arr->at(0).data<configdata>();
		auto exp = arr->get(1, "true");
		auto include_inherited = arr->get(2, true);


		auto condition_stack = std::make_shared<callstack>(vm->active_vmstack()->stacks_top()->get_namespace());
		vm->parse_sqf(exp, condition_stack);
		auto condition = std::make_shared<codedata>(condition_stack);

		auto cs = std::make_shared<sqf::callstack_configproperties>(vm->active_vmstack()->stacks_top()->get_namespace(), config, condition, include_inherited);
		vm->active_vmstack()->push_back(cs);
		return {};
	}
}
void sqf::commandmap::initconfigcmds()
{
	add(nular("configNull", "A non-existing Config.", confignull__));
	add(nular("configFile", "Return root of config entries hierarchy.", configfile__));
	add(unary("configName", type::CONFIG, "Returns name of config entry.", configname_config));
	add(binary(3, ">>", type::CONFIG, type::STRING, "Returns subentry of config entry with given name.", greaterthengreaterthen_config_string));
	add(binary(7, "/", type::CONFIG, type::STRING, "Returns subentry of config entry with given name.", greaterthengreaterthen_config_string));
	add(binary(4, "select", type::CONFIG, type::SCALAR, "Selects an element at given index from provided config.", select_config_scalar));
	add(unary("count", type::CONFIG, "Returns the number of sub-entries in provided config object.", count_config));
	add(unary("configHierarchy", type::CONFIG, "Returns hierarchy of the given config class.", confighierarchy_config));
	add(unary("inheritsFrom", type::CONFIG, "Returns base entry of config entry.", inheritsfrom_config));
	add(unary("isNumber", type::CONFIG, "Returns base entry of config entry.", isnumber_config));
	add(unary("isText", type::CONFIG, "Check if config entry represents text.", istext_config));
	add(unary("isClass", type::CONFIG, "Check if config entry represents config class.", isclass_config));
	add(unary("isArray", type::CONFIG, "Check if config entry represents array.", isarray_config));
	add(unary("getNumber", type::CONFIG, "Extract number from config entry.", getnumber_config));
	add(unary("getText", type::CONFIG, "Extract text from config entry.", gettext_config));
	add(unary("getArray", type::CONFIG, "Extract array from config entry.", getarray_config));
	add(unary("isNull", type::CONFIG, "Checks whether the tested item is Null.", isnull_config));
	add(binary(4, "configClasses", type::STRING, type::CONFIG, "Returns an array of config entries which meet criteria in condition code. Command iterates through all available config sub classes of the given config class. Current looked at config is stored in _x variable (similar to alternative count command implementation). Condition has to return true in order for the looked at config to be added to the resulting array. Slightly faster than configProperties, but doesn't account for config properties or inherited entries.", configclasses_code_config));
	add(unary("configProperties", type::ARRAY, "Returns an array of config entries which meet criteria in condition code. Command iterates through available classes and config properties for given config entry. If 3rd param is true the search also includes inherited properties. Current looked at config is stored in _x variable (similar to alternative count command implementation). Condition has to return true in order for the looked at property to be added to the resulting array. A bit slower than configClasses but allows to access inherited entires.", configproperties_array));
}
