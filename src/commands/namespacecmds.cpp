#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include "../varscope.h"
#include "../sqfnamespace.h"
#include "../codedata.h"
#include "../callstack.h"
#include "../objectdata.h"
#include "../innerobj.h"
#include "../arraydata.h"
#include <algorithm>


using namespace sqf;
namespace
{
	value allvariables_namespace(virtualmachine* vm, value::cref right)
	{
		std::shared_ptr<varscope> r;
		if (right.dtype() == OBJECT)
		{
			auto obj = right.data<objectdata>();
			if (obj->is_null())
			{
				vm->wrn() << "Attempted to use command on NULL object." << std::endl;
				return value(std::make_shared<arraydata>());
			}
			r = std::dynamic_pointer_cast<varscope>(obj->obj());
		}
		else
		{
			r = std::dynamic_pointer_cast<varscope>(right.data());
		}
		std::vector<value> arr(r->get_variable_map().size());
		transform(r->get_variable_map().begin(), r->get_variable_map().end(), arr.begin(), [](auto pair) { return value(pair.first); });
		return value(arr);
	}
	value with_namespace(virtualmachine* vm, value::cref right)
	{
		auto r = right.data<sqfnamespace>();
		return value(std::make_shared<sqfwith>(r));
	}
	value do_with_code(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<sqfwith>();
		auto r = right.data<codedata>();
		auto cs = std::make_shared<callstack>(l);
		r->loadinto(vm, vm->active_vmstack());
		return {};
	}
	value getVariable_namespace_string(virtualmachine* vm, value::cref left, value::cref right)
	{
		std::shared_ptr<varscope> l;
		if (left.dtype() == OBJECT)
		{
			auto obj = left.data<objectdata>();
			if (obj->is_null())
			{
				vm->wrn() << "Attempted to use command on NULL object." << std::endl;
				return value(std::make_shared<arraydata>());
			}
			l = std::dynamic_pointer_cast<varscope>(obj->obj());
		}
		else
		{
			l = std::dynamic_pointer_cast<varscope>(left.data());
		}
		auto r = right.as_string();
        return l->get_variable(r);
	}
	value getVariable_namespace_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		std::shared_ptr<varscope> l;
		if (left.dtype() == OBJECT)
		{
			auto obj = left.data<objectdata>();
			if (obj->is_null())
			{
				vm->wrn() << "Attempted to use command on NULL object." << std::endl;
				return value(std::make_shared<arraydata>());
			}
			l = std::dynamic_pointer_cast<varscope>(obj->obj());
		}
		else
		{
			l = std::dynamic_pointer_cast<varscope>(left.data());
		}
		auto r = right.as_vector();
		if (r.size() != 2)
		{
			vm->err() << "Expected 2 elements in array, got " << r.size() << ". Returning NIL." << std::endl;
			return {};
		}
		if (r[0].dtype() != sqf::type::STRING)
		{
			vm->err() << "Index position 0 was expected to be of type 'STRING' but was '" << sqf::type_str(r[0].dtype()) << "'." << std::endl;
			return {};
		}
		bool success = false;
		auto var = l->get_variable(r[0].as_string(), success);
		if (success)
		{
			return var;
		}
		else
		{
			return r[1];
		}
	}
	value setVariable_namespace_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		std::shared_ptr<varscope> l;
		if (left.dtype() == OBJECT)
		{
			auto obj = left.data<objectdata>();
			if (obj->is_null())
			{
				vm->wrn() << "Attempted to use command on NULL object." << std::endl;
				return value(std::make_shared<arraydata>());
			}
			l = std::dynamic_pointer_cast<varscope>(obj->obj());
		}
		else
		{
			l = std::dynamic_pointer_cast<varscope>(left.data());
		}
		auto r = right.as_vector();
		if (r.size() != 2 && r.size() != 3)
		{
			vm->err() << "Expected 2 elements in array, got " << r.size() << ". Returning NIL." << std::endl;
			return {};
		}
		//Third element is ignored due to no networking in sqf-vm
		if (r[0].dtype() != sqf::type::STRING)
		{
			vm->err() << "Index position 0 was expected to be of type 'STRING' but was '" << sqf::type_str(r[0].dtype()) << "'." << std::endl;
			return {};
		}
		auto val = r[1];
		l->set_variable(r[0].as_string(), val);
		return {};
	}
}
void sqf::commandmap::initnamespacecmds()
{
	add(nular("missionNamespace", "Returns the global namespace attached to mission.",
		[](virtualmachine* vm) -> value { return value(vm->missionnamespace()); }));
	add(nular("uiNamespace", "The value of PI.",
		[](virtualmachine* vm) -> value { return value(vm->uinamespace()); }));
	add(nular("parsingNamespace", "Returns the global namespace attached to config parser.",
		[](virtualmachine* vm) -> value { return value(vm->parsingnamespace()); }));
	add(nular("profileNamespace", "Returns the global namespace attached to the active user profile. Use setVariable and getVariable to save and load data to and from this Namespace. A variable can be deleted by setting its value to nil. By default the variables set in this namespace will exist while the game is running. In order to make variables save permanently, use saveProfileNamespace before the game is over.",
		[](virtualmachine* vm) -> value { return value(vm->profilenamespace()); }));
	add(unary("allVariables", type::NAMESPACE, "Returns a list of all variables from desired namespace.", allvariables_namespace));
	add(unary("allVariables", type::OBJECT, "Returns a list of all variables from desired namespace.", allvariables_namespace));
	add(unary("allVariables", type::GROUP, "Returns a list of all variables from desired namespace.", allvariables_namespace));
	add(unary("with", type::NAMESPACE, "Creates a WITH type that is used inside a do construct in order to execute code inside a given namespace.", with_namespace));
	add(binary(4, "do", type::WITH, type::CODE, "Executes code in the namespace provided via the WITH parameter.", do_with_code));
	add(binary(4, "getVariable", type::NAMESPACE, type::STRING, "Return the value of variable in the variable space assigned to various data types. Returns nil if variable is undefined.", getVariable_namespace_string));
	add(binary(4, "getVariable", type::NAMESPACE, type::ARRAY, "Return the value of variable in the provided variable space. First element is expected to be the variable name as string. Returns second array item if variable is undefined.", getVariable_namespace_array));
	add(binary(4, "setVariable", type::NAMESPACE, type::ARRAY, "Sets a variable to given value in the provided variable space. First element is expected to be the variable name as string. Second element is expected to be anything.", setVariable_namespace_array));
	add(binary(4, "getVariable", type::OBJECT, type::STRING, "Return the value of variable in the variable space assigned to various data types. Returns nil if variable is undefined.", getVariable_namespace_string));
	add(binary(4, "getVariable", type::OBJECT, type::ARRAY, "Return the value of variable in the provided variable space. First element is expected to be the variable name as string. Returns second array item if variable is undefined.", getVariable_namespace_array));
	add(binary(4, "setVariable", type::OBJECT, type::ARRAY, "Sets a variable to given value in the provided variable space. First element is expected to be the variable name as string. Second element is expected to be anything.", setVariable_namespace_array));
	add(binary(4, "getVariable", type::GROUP, type::STRING, "Return the value of variable in the variable space assigned to various data types. Returns nil if variable is undefined.", getVariable_namespace_string));
	add(binary(4, "getVariable", type::GROUP, type::ARRAY, "Return the value of variable in the provided variable space. First element is expected to be the variable name as string. Returns second array item if variable is undefined.", getVariable_namespace_array));
	add(binary(4, "setVariable", type::GROUP, type::ARRAY, "Sets a variable to given value in the provided variable space. First element is expected to be the variable name as string. Second element is expected to be anything.", setVariable_namespace_array));
}
