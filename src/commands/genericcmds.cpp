#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include "../codedata.h"
#include "../arraydata.h"
#include "../stringdata.h"
#include "../fordata.h"
#include "../switchdata.h"
#include "../scriptdata.h"
#include "../callstack_for.h"
#include "../callstack_while.h"
#include "../callstack_select.h"
#include "../callstack_isnil.h"
#include "../callstack_exitwith.h"
#include "../callstack_findif.h"
#include "../callstack_switch.h"
#include "../callstack_apply.h"
#include "../callstack_foreach.h"
#include "../callstack_count.h"
#include "../Entry.h"
#include "../sqfnamespace.h"
#include "../fileio.h"
#include "../parsepreprocessor.h"


#define CALLEXTBUFFSIZE 10240
#define CALLEXTVERSIONBUFFSIZE 32
#define RVARGSLIMIT 1024

#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#include <dlfcn.h>
typedef void(*RVExtensionVersion)(char*, int);
typedef void(*RVExtension)(char*, int, const char*);
typedef int(*RVExtensionArgs)(char*, int, const char*, const char**, int);
#elif defined(_WIN32)
typedef void(__stdcall *RVExtensionVersion)(char*, int);
typedef void(__stdcall *RVExtension)(char*, int, const char*);
typedef int(__stdcall *RVExtensionArgs)(char*, int, const char*, const char**, int);
#else
#error UNSUPPORTED PLATFORM
#endif


using namespace sqf;
namespace
{
	std::shared_ptr<value> productversion_(virtualmachine* vm)
	{
		auto vec = std::vector<std::shared_ptr<value>>{
			std::make_shared<value>("SQF-VM " VERSION_FULL), //product name
			std::make_shared<value>("sqf-vm"),
			std::make_shared<value>(VERSION_MAJORMINOR),
			std::make_shared<value>(VERSION_REVISION),
			std::make_shared<value>("COMMUNITY"),
			std::make_shared<value>(false),
#if _WIN32
			std::make_shared<value>("Windows"),
#elif __linux__
			std::make_shared<value>("Linux"),
#else
			std::make_shared<value>("NA"),
#endif
			std::make_shared<value>(ENVIRONMENTSTR)
		};
		return std::make_shared<value>(vec);
	}
	std::shared_ptr<value> call_code(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->data<codedata>();
		r->loadinto(vm, vm->stack());
		vm->stack()->stacks_top()->setvar("_this", std::make_shared<value>());
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> call_any_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto r = right->data<codedata>();
		r->loadinto(vm, vm->stack());
		vm->stack()->stacks_top()->setvar("_this", left);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> count_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->data<arraydata>();
		return std::make_shared<value>(r->size());
	}
	std::shared_ptr<value> count_code_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->data<codedata>();
		auto r = right->data<arraydata>();
		auto cs = std::make_shared<callstack_count>(vm->stack()->stacks_top()->getnamespace(), l, r);
		vm->stack()->pushcallstack(cs);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> compile_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		auto cs = std::make_shared<callstack>(vm->stack()->stacks_top()->getnamespace());
		vm->parse_sqf(r, cs);
		return std::make_shared<value>(cs);
	}
	std::shared_ptr<value> typename_any(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(type_str(right->dtype()));
	}
	std::shared_ptr<value> str_any(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(std::make_shared<stringdata>(right->tosqf(), false), type::STRING);
	}
	std::shared_ptr<value> nil_(virtualmachine* vm)
	{
		return std::make_shared<value>();
	}
	std::shared_ptr<value> comment_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>();
	}
	std::shared_ptr<value> if_bool(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(right->data(), type::IF);
	}
	std::shared_ptr<value> then_if_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto ifcond = left->as_bool();
		auto arr = right->as_vector();
		if (arr.size() != 2)
		{
			vm->err() << "Expected 2 elements in array." << std::endl;
			return std::make_shared<value>();
		}
		auto el0 = arr[0];
		auto el1 = arr[1];
		if (ifcond)
		{
			if (el1->dtype() != type::CODE)
			{
				vm->wrn() << "Expected element 1 of array to be of type 'CODE' but was '" << sqf::type_str(el1->dtype()) << "'." << std::endl;
			}
			if (el0->dtype() == type::CODE)
			{
				auto code = el0->data<codedata>();
				code->loadinto(vm, vm->stack());
				return std::shared_ptr<value>();
			}
			else
			{
				vm->err() << "Expected element 0 of array to be of type 'CODE' but was '" << sqf::type_str(el0->dtype()) << "'." << std::endl;
				return std::make_shared<value>();
			}
		}
		else
		{
			if (el0->dtype() != type::CODE)
			{
				vm->wrn() << "Expected element 0 of array to be of type 'CODE' but was '" << sqf::type_str(el0->dtype()) << "'." << std::endl;
			}
			if (el1->dtype() == type::CODE)
			{
				auto code = el1->data<codedata>();
				code->loadinto(vm, vm->stack());
				return std::shared_ptr<value>();
			}
			else
			{
				vm->err() << "Expected element 1 of array to be of type 'CODE' but was '" << sqf::type_str(el1->dtype()) << "'." << std::endl;
				return std::make_shared<value>();
			}
		}
	}
	std::shared_ptr<value> then_if_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto ifcond = left->as_bool();
		auto code = right->data<codedata>();
		if (ifcond)
		{
			code->loadinto(vm, vm->stack());
			return std::shared_ptr<value>();
		}
		else
		{
			return std::make_shared<value>();
		}
	}
	std::shared_ptr<value> exitwith_if_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto ifcond = left->as_bool();
		auto code = right->data<codedata>();
		if (ifcond)
		{
			auto cs = std::make_shared<callstack_exitwith>(vm->stack()->stacks_top()->getnamespace());
			code->loadinto(vm->stack(), cs);
			vm->stack()->pushcallstack(cs);
			return std::shared_ptr<value>();
		}
		else
		{
			return std::make_shared<value>();
		}
	}
	std::shared_ptr<value> else_code_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto vec = std::vector<std::shared_ptr<value>>(2);
		vec[0] = left;
		vec[1] = right;
		return std::make_shared<value>(vec);
	}
	std::shared_ptr<value> while_code(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(right->data(), type::WHILE);
	}
	std::shared_ptr<value> do_while_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto whilecond = left->data<codedata>();
		auto execcode = right->data<codedata>();

		auto cs = std::make_shared<callstack_while>(vm->stack()->stacks_top()->getnamespace(), whilecond, execcode);
		vm->stack()->pushcallstack(cs);

		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> for_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto str = right->as_string();
		return std::make_shared<value>(std::make_shared<fordata>(str), type::FOR);
	}
	std::shared_ptr<value> from_for_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto fordata = left->data<sqf::fordata>();
		auto index = right->as_double();
		fordata->from(index);
		return left;
	}
	std::shared_ptr<value> to_for_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto fordata = left->data<sqf::fordata>();
		auto index = right->as_double();
		fordata->to(index);
		return left;
	}
	std::shared_ptr<value> step_for_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto fordata = left->data<sqf::fordata>();
		auto index = right->as_double();
		fordata->step(index);
		return left;
	}
	std::shared_ptr<value> do_for_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto fordata = left->data<sqf::fordata>();
		auto execcode = right->data<codedata>();

		auto cs = std::make_shared<callstack_for>(vm->stack()->stacks_top()->getnamespace(), fordata, execcode);
		vm->stack()->pushcallstack(cs);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> foreach_code_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->data<sqf::codedata>();
		auto r = right->data<sqf::arraydata>();
		auto cs = std::make_shared<callstack_foreach>(vm->stack()->stacks_top()->getnamespace(), l, r);
		vm->stack()->pushcallstack(cs);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> select_array_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->as_vector();
		auto index = right->as_long();

		if (arr.size() < index || index < 0)
		{
			vm->err() << "Index out of range." << std::endl;
			return std::make_shared<value>();
		}
		if (arr.size() == index)
		{
			vm->wrn() << "Index equals range. Returning nil." << std::endl;
			return std::make_shared<value>();
		}
		return arr[index];
	}
	std::shared_ptr<value> select_array_bool(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->as_vector();
		auto flag = right->as_bool();
		if ((!flag && arr.size() < 2) || arr.size() < 1)
		{
			vm->wrn() << "Array should have at least two elements. Returning ni" << std::endl;
			return std::make_shared<value>();
		}
		else if (flag && arr.size() < 2)
		{
			vm->wrn() << "Array should have at least two elements." << std::endl;
		}
		return flag ? arr[1] : arr[0];
	}
	std::shared_ptr<value> select_array_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto vec = left->as_vector();
		auto arr = right->as_vector();
		if (arr.size() < 1)
		{
			vm->err() << "Array was expected to have at least a single element." << std::endl;
			return std::make_shared<value>();
		}
		if (arr[0]->dtype() != type::SCALAR)
		{
			vm->err() << "First element of array was expected to be SCALAR, got " << sqf::type_str(arr[0]->dtype()) << '.' << std::endl;
			return std::make_shared<value>();
		}
		int start = arr[0]->as_long();
		if (start < 0)
		{
			vm->wrn() << "Start index is smaller then 0. Returning empty array." << std::endl;
			return std::make_shared<value>("");
		}
		if (start > vec.size())
		{
			vm->wrn() << "Start index is larger then string length. Returning empty array." << std::endl;
			return std::make_shared<value>("");
		}
		if (arr.size() >= 2)
		{
			if (arr[1]->dtype() != type::SCALAR)
			{
				vm->err() << "Second element of array was expected to be SCALAR, got " << sqf::type_str(arr[0]->dtype()) << '.' << std::endl;
				return std::make_shared<value>();
			}
			int length = arr[1]->as_int();
			if (length < 0)
			{
				vm->wrn() << "Length is smaller then 0. Returning empty array." << std::endl;
				return std::make_shared<value>("");
			}

			return std::make_shared<value>(std::vector<std::shared_ptr<value>>(vec.begin() + start, start + length > static_cast<int>(vec.size()) ? vec.end() : vec.begin() + start + length));
		}
		return std::make_shared<value>(std::vector<std::shared_ptr<value>>(vec.begin() + start, vec.end()));
	}
	std::shared_ptr<value> select_array_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->as_vector();
		if (arr.size() == 0)
			return std::make_shared<value>(std::vector<std::shared_ptr<value>>());
		auto cond = right->data<codedata>();
		auto cs = std::make_shared<sqf::callstack_select>(vm->stack()->stacks_top()->getnamespace(), arr, cond);
		vm->stack()->pushcallstack(cs);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> resize_array_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		if (right->as_int() < 0)
		{
			vm->err() << "New size cannot be smaller then 0." << std::endl;
			return std::make_shared<value>();
		}
		left->data<arraydata>()->resize(right->as_int());
		return std::make_shared<value>();
	}
	std::shared_ptr<value> pushback_array_any(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->data<arraydata>();
		auto newindex = arr->size();
		if (!arr->push_back(right))
		{
			vm->err() << "Array recursion detected." << std::endl;
			return std::shared_ptr<value>();
		}
		return std::make_shared<value>(newindex);
	}
	std::shared_ptr<value> pushbackunique_array_any(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->data<arraydata>();
		int newindex = (int)arr->size();
		auto found = std::find_if(arr->begin(), arr->end(), [right](const std::shared_ptr<value>& val) { return val->equals(right); });
		if (found == arr->end())
		{
			if (!arr->push_back(right))
			{
				vm->err() << "Array recursion detected." << std::endl;
				return std::shared_ptr<value>();
			}
		}
		else
		{
			newindex = -1;
		}
		return std::make_shared<value>(newindex);
	}
	std::shared_ptr<value> findif_array_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->data<arraydata>();
		int newindex = (int)arr->size();
		if (arr->size() == 0)
			return std::make_shared<value>(-1);
		auto cond = right->data<codedata>();
		auto cs = std::make_shared<sqf::callstack_findif>(vm->stack()->stacks_top()->getnamespace(), cond, arr);
		vm->stack()->pushcallstack(cs);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> reverse_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		right->data<arraydata>()->reverse();
		return std::make_shared<value>();
	}
	std::shared_ptr<value> private_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto str = right->as_string();
		vm->stack()->stacks_top()->setvar(str, std::make_shared<value>());
		return std::make_shared<value>();
	}
	std::shared_ptr<value> private_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto arr = right->as_vector();
		bool errflag = false;
		for (size_t i = 0; i < arr.size(); i++)
		{
			auto it = arr[i];
			if (it->dtype() != sqf::type::STRING)
			{
				vm->err() << "Index position " << i << " was expected to be of type 'STRING' but was '" << sqf::type_str(it->dtype()) << "'." << std::endl;
			}
		}
		if (errflag)
		{
			return std::shared_ptr<value>();
		}
		for (auto& it : arr)
		{
			auto str = it->as_string();
			vm->stack()->stacks_top()->setvar(str, std::make_shared<value>());
		}
		return std::make_shared<value>();
	}
	std::shared_ptr<value> isnil_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto varname = right->as_string();
		auto val = vm->stack()->getlocalvar(varname);
		if (val->dtype() == sqf::type::NOTHING)
		{
			val = vm->stack()->stacks_top()->getnamespace()->getvar(varname);
		}
		return std::make_shared<value>(val->dtype() == sqf::type::NOTHING);
	}
	std::shared_ptr<value> isnil_code(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cdata = right->data<codedata>();
		auto cs = std::make_shared<callstack_isnil>(vm->stack()->stacks_top()->getnamespace(), vm, cdata);
		vm->stack()->pushcallstack(cs);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> hint_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		vm->out() << "[HINT]\t" << r << std::endl;
		return std::make_shared<value>();
	}
	std::shared_ptr<value> hint_text(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		vm->out() << "[HINT]\t" << r << std::endl;
		return std::make_shared<value>();
	}
	std::shared_ptr<value> systemchat_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		vm->out() << "[CHAT]\tSYSTEM: " << r << std::endl;
		return std::make_shared<value>();
	}

#define MAGIC_SWITCH "___switch"
	std::shared_ptr<value> switch_any(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(std::make_shared<switchdata>(right), sqf::type::SWITCH);
	}
	std::shared_ptr<value> do_switch_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto r = right->data<codedata>();
		auto cs = std::make_shared<callstack_switch>(vm->stack()->stacks_top()->getnamespace(), left->data<switchdata>());
		vm->stack()->pushcallstack(cs);
		r->loadinto(vm->stack(), cs);
		cs->setvar(MAGIC_SWITCH, left);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> case_any(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto valswtch = vm->stack()->getlocalvar(MAGIC_SWITCH);
		if (valswtch->dtype() != sqf::type::SWITCH)
		{
			vm->err() << "Magic variable '___switch' is not of type 'SWITCH' but was '" << sqf::type_str(valswtch->dtype()) << "'.";
			return std::shared_ptr<value>();
		}
		auto swtch = valswtch->data<switchdata>();

		if (right->equals(swtch->val()))
		{
			swtch->flag(true);
		}
		return std::make_shared<value>(swtch, sqf::type::SWITCH);
	}
	std::shared_ptr<value> default_code(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto valswtch = vm->stack()->getlocalvar(MAGIC_SWITCH);
		if (valswtch->dtype() != sqf::type::SWITCH)
		{
			vm->err() << "Magic variable '___switch' is not of type 'SWITCH' but was '" << sqf::type_str(valswtch->dtype()) << "'.";
			return std::shared_ptr<value>();
		}
		auto swtch = valswtch->data<switchdata>();
		swtch->defaultexec(right->data<codedata>());
		return std::make_shared<value>();
	}
	std::shared_ptr<value> colon_switch_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->data<switchdata>();
		if (l->executed())
		{
			return std::make_shared<value>();
		}
		auto r = right->data<codedata>();
		if (l->flag())
		{
			l->executed(true);
			r->loadinto(vm, vm->stack());
			return std::shared_ptr<value>();
		}
		return std::make_shared<value>();
	}
	std::shared_ptr<value> apply_array_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->as_vector();
		if (arr.empty())
			return std::make_shared<value>(std::vector<std::shared_ptr<value>>());
		auto cond = right->data<codedata>();
		auto cs = std::make_shared<sqf::callstack_apply>(vm->stack()->stacks_top()->getnamespace(), arr, cond);
		vm->stack()->pushcallstack(cs);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> spawn_any_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto code = right->data<codedata>();
		auto script = std::make_shared<scriptdata>();
		code->loadinto(vm, script->stack());
		vm->push_spawn(script);
		script->stack()->stacks_top()->setvar("_this", left);
		return std::make_shared<value>(script, sqf::type::SCRIPT);
	}
	std::shared_ptr<value> scriptdone_script(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->data<scriptdata>();
		return std::make_shared<value>(r->hasfinished());
	}
	std::shared_ptr<value> set_array_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->data<arraydata>();
		auto params = right->as_vector();
		if (params.size() != 2)
		{
			vm->err() << "Expected 2 elements in array, got " << params.size() << ". Returning NIL." << std::endl;
			return std::shared_ptr<value>();
		}
		if (params[0]->dtype() != sqf::type::SCALAR)
		{
			vm->err() << "Index position 0 was expected to be of type 'SCALAR' but was '" << sqf::type_str(params[0]->dtype()) << "'." << std::endl;
			return std::shared_ptr<value>();
		}

		auto index = params[0]->as_long();
		auto val = params[1];
		if (arr->size() <= index)
		{
			arr->resize(index + 1);
		}
		auto oldval = arr->operator[](index);
		arr->operator[](index) = val;
		if (!arr->recursion_test())
		{
			arr->operator[](index) = oldval;
			vm->err() << "Array recursion detected." << std::endl;
			return std::shared_ptr<value>();
		}
		return std::make_shared<value>();
	}
	std::shared_ptr<value> plus_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->data<arraydata>();
		return std::make_shared<value>(r->operator std::vector<std::shared_ptr<sqf::value>, std::allocator<std::shared_ptr<sqf::value>>>());
	}
	std::shared_ptr<value> plus_array_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		// create a copy of left array
		auto result = std::make_shared<value>(left->as_vector());
		result->data<arraydata>()->extend(right->as_vector());
		return result;
	}

	bool value_equals_casesensitive(std::shared_ptr<value> a, std::shared_ptr<value> b)
	{
		// TODO: put this into value class? make use of it elsewhere
		if (a->dtype() == type::STRING && a->dtype() == b->dtype())
		{
			return a->as_string() == b->as_string();
		}
		else
		{
			return a->equals(b);
		}
	}

	std::shared_ptr<value> minus_array_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		// TODO: optimize (e.g. build a hash based set of the right array and check against it)
		auto l = left->data<arraydata>();
		auto r = right->data<arraydata>();
		std::vector<std::shared_ptr<value>> result;

		std::copy_if(l->begin(), l->end(), std::back_inserter(result), [&r, &result](std::shared_ptr<value>& current) {

			auto found = std::find_if(r->begin(), r->end(), [&current](std::shared_ptr<value>& check) {
				return value_equals_casesensitive(current, check);
			});

			//We only want element in output if it doesn't exist in right arg
			return found == r->end();
		});

		return std::make_shared<value>(result);
	}
	std::shared_ptr<value> append_array_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		left->data<arraydata>()->extend(right->as_vector());
		return std::make_shared<value>();
	}
	std::shared_ptr<value> arrayintersect_array_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		// TODO: optimize (e.g. use hash set for checking)
		auto l = left->data<arraydata>();
		auto r = right->data<arraydata>();
		std::vector<std::shared_ptr<value>> result;

		std::copy_if(l->begin(), l->end(), std::back_inserter(result), [&r, &result](std::shared_ptr<value>& current) {
		
			auto found = std::find_if(result.begin(), result.end(), [&current](std::shared_ptr<value>& check) {
				return value_equals_casesensitive(current, check);
			});

			//Result already contains the element. Don't add it (remove duplicates)
			if (found != result.end()) return false;

			found = std::find_if(r->begin(), r->end(), [&current](std::shared_ptr<value>& check) {
				return value_equals_casesensitive(current, check);
			});

			//Only add if right argument also contains the element
			return found != r->end();
		});

		return std::make_shared<value>(result);
	}
	std::shared_ptr<value> deleteat_array_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->data<arraydata>();
		auto index = right->as_int();
		if (index < 0 || index >= l->size())
		{
			vm->err() << "Array index out of bounds." << std::endl;
			return std::make_shared<value>();
		}
		l->delete_at(index);
		return std::make_shared<value>();
	}
	std::shared_ptr<value> find_array_any(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->data<arraydata>();
		auto found = std::find_if(l->begin(), l->end(), [&right](std::shared_ptr<value>& check) {
			return value_equals_casesensitive(check, right);
		});

		if (found != l->end())
			return std::make_shared<value>(static_cast<int>(std::distance(l->begin(), found)));

		return std::make_shared<value>(-1);
	}
	std::shared_ptr<value> selectmax_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->data<arraydata>();
		double max = 0;
		for (size_t i = r->size() - 1; i != ~0; i--)
		{
			auto tmp = r->at(i);
			if (tmp->dtype() == sqf::type::SCALAR)
			{
				if (tmp->as_double() > max)
				{
					max = tmp->as_double();
				}
			}
			else if (tmp->dtype() == sqf::type::BOOL)
			{
				if (tmp->as_bool() ? 1 : 0 > max)
				{
					max = tmp->as_bool() ? 1 : 0;
				}
			}
			else
			{
				if (0 > max)
				{
					max = 0;
				}
				vm->wrn() << "Index position " << i << " was expected to be of type 'SCALAR' or 'BOOL' but was '" << sqf::type_str(tmp->dtype()) << "'." << std::endl;
			}
		}
		return std::make_shared<value>(max);
	}
	std::shared_ptr<value> selectmin_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->data<arraydata>();
		double min = 0;
		for (size_t i = r->size() - 1; i != ~0; i--)
		{
			auto tmp = r->at(i);
			if (tmp->dtype() == sqf::type::SCALAR)
			{
				if (tmp->as_double() < min)
				{
					min = tmp->as_double();
				}
			}
			else if (tmp->dtype() == sqf::type::BOOL)
			{
				if (tmp->as_bool() ? 1 : 0 < min)
				{
					min = tmp->as_bool() ? 1 : 0;
				}
			}
			else
			{
				if (0 < min)
				{
					min = 0;
				}
				vm->wrn() << "Index position " << i << " was expected to be of type 'SCALAR' or 'BOOL' but was '" << sqf::type_str(tmp->dtype()) << "'." << std::endl;
			}
		}
		return std::make_shared<value>(min);
	}

	std::shared_ptr<dlops> helpermethod_callextension_loadlibrary(virtualmachine* vm, std::string name)
	{
		static char buffer[CALLEXTVERSIONBUFFSIZE + 1] = { 0 };
		for (auto it : vm->libraries())
		{
			if (it->path() == name)
			{
				return it;
			}
		}
		#ifdef _WIN32
		vm->libraries().push_back(std::make_shared<dlops>(name));
		#else
		vm->libraries().push_back(std::make_shared<dlops>(name + ".so"));
		#endif
		auto& dl = vm->libraries().back();
		void* sym = nullptr;
		if (dl->try_resolve("RVExtensionVersion", &sym))
		{
			reinterpret_cast<RVExtensionVersion>(sym)(buffer, CALLEXTVERSIONBUFFSIZE);
			if (buffer[CALLEXTVERSIONBUFFSIZE - 1] != '\0')
			{
				vm->wrn() << "Library '" << name << "' is not terminating RVExtensionVersion output buffer with a '\\0'!" << std::endl;
			}
			vm->out() << "[RPT]\tCallExtension loaded: '" << name << "' [" << buffer <<  ']' << std::endl;
		}
		else
		{
			vm->out() << "[RPT]\tCallExtension loaded: '" << name << '\'' << std::endl;
		}
		return dl;
	}
	std::shared_ptr<value> callextension_string_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		static char buffer[CALLEXTBUFFSIZE + 1] = { 0 };
		auto libname = left->as_string();
		if (libname.find('/') != -1 || libname.find('\\') != -1)
		{
			vm->wrn() << "Library name '" << libname << "' is not supported due to containing path characters." << std::endl;
			return std::make_shared<value>("");
		}
		try
		{
			auto dl = helpermethod_callextension_loadlibrary(vm, libname);
#if defined(_WIN32) & !defined(_WIN64)
			auto method = reinterpret_cast<RVExtension>(dl->resolve("_RVExtension@12"));
#else
			auto method = reinterpret_cast<RVExtension>(dl->resolve("RVExtension"));
#endif
			method(buffer, CALLEXTBUFFSIZE, right->as_string().c_str());
			if (buffer[CALLEXTBUFFSIZE - 1] != '\0')
			{
				vm->wrn() << "Library '" << libname << "' is not terminating RVExtension output buffer with a '\\0'!" << std::endl;
			}
			return std::make_shared<value>(buffer);
		}
		catch (const std::runtime_error& ex)
		{
			vm->wrn() << "Could not complete command execution due to error with library '" << libname << "' (RVExtension): " << ex.what() << std::endl;
			return std::make_shared<value>("");
		}
	}
	std::shared_ptr<value> callextension_string_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		static char buffer[CALLEXTBUFFSIZE + 1] = { 0 };
		auto libname = left->as_string();
		if (libname.find('/') != -1 || libname.find('\\') != -1)
		{
			vm->wrn() << "Library name '" << libname << "' is not supported due to containing path characters." << std::endl;
			return std::make_shared<value>("");
		}
		auto rvec = right->data<arraydata>();
		if (rvec->size() < 2)
		{
			vm->wrn() << "Expected 2 elements in array, got " << rvec->size() << ". Returning error code PARAMS_ERROR_TOO_MANY_ARGS(201)." << std::endl;
			return std::make_shared<value>(std::vector<std::shared_ptr<value>> { std::make_shared<value>(""), std::make_shared<value>(0), std::make_shared<value>(201) });
		}
		if (rvec->at(0)->dtype() != type::STRING)
		{
			vm->err() << "Index position 0 was expected to be of type 'STRING' but was '" << sqf::type_str(rvec->at(0)->dtype()) << "'." << std::endl;
			return std::shared_ptr<value>();
		}
		if (rvec->at(1)->dtype() != type::ARRAY)
		{
			vm->err() << "Index position 1 was expected to be of type 'STRING' but was '" << sqf::type_str(rvec->at(1)->dtype()) << "'." << std::endl;
			return std::shared_ptr<value>();
		}

		auto arr = rvec->at(1)->data<arraydata>();
		if (arr->size() > RVARGSLIMIT)
		{
			vm->wrn() << "callExtension SYNTAX_ERROR_WRONG_PARAMS_SIZE(101) error with '" << libname << "' (RVExtensionArgs)" << std::endl;
			return std::make_shared<value>(std::vector<std::shared_ptr<value>> { std::make_shared<value>(""), std::make_shared<value>(0), std::make_shared<value>(101) });
		}
		std::vector<std::string> argstringvec;

		for (const auto& at : *arr) 
		{
			switch (at->dtype())
			{
			case type::BOOL:
			case type::STRING:
			case type::SCALAR:
			case type::ARRAY:
				argstringvec.push_back(at->as_string());
				break;
			default:
				vm->wrn() << "callExtension SYNTAX_ERROR_WRONG_PARAMS_TYPE(102) error with '" << libname << "' (RVExtensionArgs)" << std::endl;
				return std::make_shared<value>(std::vector<std::shared_ptr<value>> { std::make_shared<value>(""), std::make_shared<value>(0), std::make_shared<value>(102) });
			}
		}
		std::vector<const char*> argvec;
		argvec.reserve(argstringvec.size());
		for (auto& it : argstringvec)
		{
			argvec.push_back(it.c_str());
		}

		try
		{
			auto dl = helpermethod_callextension_loadlibrary(vm, libname);
#if defined(_WIN32) & !defined(_WIN64)
			auto method = reinterpret_cast<RVExtensionArgs>(dl->resolve("_RVExtensionArgs@20"));
#else
			auto method = reinterpret_cast<RVExtensionArgs>(dl->resolve("RVExtensionArgs"));
#endif
			auto res = method(buffer, CALLEXTBUFFSIZE, rvec->at(0)->as_string().c_str(), argvec.data(), static_cast<int>(argvec.size()));
			if (buffer[CALLEXTBUFFSIZE - 1] != '\0')
			{
				vm->wrn() << "Library '" << libname << "' is not terminating RVExtensionArgs output buffer with a '\\0'!" << std::endl;
			}
			return std::make_shared<value>(std::vector<std::shared_ptr<value>> { std::make_shared<value>(buffer), std::make_shared<value>(res), std::make_shared<value>(0) });
		}
		catch (const std::runtime_error& ex)
		{
			vm->wrn() << "Could not complete command execution due to error with library '" << libname << "' (RVExtensionArgs): " << ex.what() << std::endl;
			return std::make_shared<value>(std::vector<std::shared_ptr<value>> { std::make_shared<value>(""), std::make_shared<value>(0), std::make_shared<value>(501) });
		}
	}
	std::shared_ptr<value> param_any_array(virtualmachine* vm, std::shared_ptr<value> src, std::shared_ptr<value> trgt)
	{
		std::shared_ptr<sqf::arraydata> elements;
		if (src->dtype())
		{
			elements = src->data<sqf::arraydata>();
		}
		else
		{
			elements = std::make_shared<arraydata>();
			elements->push_back(src);
		}
		auto fels = trgt->as_vector();
		size_t i = 0, j;
		bool flag;
		
		if(fels.size() != 0)
		{
			//validation step
			if (fels.size() >= 1 && fels.at(0)->dtype() != sqf::SCALAR)
			{
				vm->err() << "Param element " << i << " is required to have its third element to be of type " << type_str(ARRAY) << ". Got " << sqf::type_str(fels.at(2)->dtype()) << '.' << std::endl;
				return std::shared_ptr<value>();
			}
			else
			{
				i = fels.at(0)->as_int();
			}
			if (fels.size() >= 3 && fels.at(2)->dtype() != sqf::ARRAY)
			{
				vm->err() << "Param element " << i << " is required to have its third element to be of type " << type_str(ARRAY) << ". Got " << sqf::type_str(fels.at(2)->dtype()) << '.' << std::endl;
				return std::shared_ptr<value>();
			}
			if (fels.size() >= 4 && (fels.at(3)->dtype() != sqf::ARRAY && fels.at(3)->dtype() != sqf::SCALAR))
			{
				vm->err() << "Param element " << i << " is required to have its fourth element to be of type " << type_str(ARRAY) << " or " << type_str(SCALAR) << ". Got " << sqf::type_str(fels.at(2)->dtype()) << '.' << std::endl;
				return std::shared_ptr<value>();
			}
			else if (fels.size() >= 4 && fels.at(3)->dtype() == sqf::ARRAY)
			{
				auto tmp = fels.at(3)->data<arraydata>();
				flag = false;
				for (j = 0; j < tmp->size(); j++)
				{
					if (tmp->at(j)->dtype() != sqf::SCALAR)
					{
						vm->err() << "Param element " << i << " and its inner " << j << ". element was expected to be of type " << type_str(SCALAR) << ". Got " << sqf::type_str(tmp->at(j)->dtype()) << '.' << std::endl;
						flag = true;
						continue;
					}
				}
				if (flag)
				{
					return std::shared_ptr<value>();
				}
			}

			if (i < elements->size())
			{
				auto el = elements->at(i);
				if (fels.size() >= 3 && fels.at(2)->data<arraydata>()->size() != 0)
				{
					auto tmp = fels.at(2)->data<arraydata>();

					auto found = std::find_if(tmp->begin(), tmp->end(), [type = el->dtype()](const std::shared_ptr<value>& val) {
						return type == val->dtype();
					});

					flag = found != tmp->end();

					if (!flag)
					{
						vm->wrn() << "Element " << i << " is not matching provided expected data types. Got " << sqf::type_str(el->dtype()) << '.' << std::endl;
						return fels.at(1);
					}
				}
				if (fels.size() >= 4 && el->dtype() == sqf::ARRAY)
				{
					flag = true;
					if (fels.at(2)->dtype() == sqf::ARRAY)
					{
						auto tmp = fels.at(2)->data<arraydata>();

						auto found = std::find_if(tmp->begin(), tmp->end(), [type = el->dtype()](const std::shared_ptr<value>& val) {
							return type == val->dtype();
						});

						flag = found != tmp->end();
					}
					else if (el->data<arraydata>()->size() != fels.at(3)->as_int())
					{
						flag = false;
					}
					if (!flag)
					{
						vm->wrn() << "Element " << i << " is not matching expected data types. Got " << sqf::type_str(el->dtype()) << '.' << std::endl;
						return fels.at(1);
					}
				}
				return el;
			}
			else
			{
				return fels.size() == 2 ? fels.at(1) : std::make_shared<sqf::value>();
			}
		}
		return std::make_shared<sqf::value>();
	}
	std::shared_ptr<value> param_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto _this = vm->stack()->getlocalvar("_this");
		if (_this->dtype() != ARRAY)
		{
			auto arr = std::make_shared<arraydata>();
			arr->push_back(_this);
			_this = std::make_shared<value>(arr, ARRAY);
		}
		return param_any_array(vm, _this, right);
	}
	std::shared_ptr<value> params_array_array(virtualmachine* vm, std::shared_ptr<value> src, std::shared_ptr<value> trgt)
	{
		auto elements = src->data<sqf::arraydata>();
		auto format = trgt->data<sqf::arraydata>();
		size_t i, j;
		bool flag;
		for (i = 0; i < format->size(); i++)
		{
			auto fel = format->at(i);
			std::vector<std::shared_ptr<sqf::value>> fels;
			if (fel->dtype() == sqf::ARRAY)
			{
				fels = fel->as_vector();
			}
			else
			{
				fels = { fel };
			}
			//validation step
			if (fels.size() < 1 || fels.at(0)->dtype() != sqf::STRING)
			{
				if (fel->dtype() == sqf::ARRAY)
				{
					vm->err() << "Params element " << i << " was expected to be a " << type_str(STRING) << " or an " << type_str(ARRAY) << ". Got " << sqf::type_str(fels.at(0)->dtype()) << '.' << std::endl;
				}
				else
				{
					vm->err() << "Params element " << i << " was expected to have the first element be a " << type_str(STRING) << ". Got " << sqf::type_str(fels.at(0)->dtype()) << '.' << std::endl;
				}
				continue;
			}
			if (fels.size() >= 3 && fels.at(2)->dtype() != sqf::ARRAY)
			{
				vm->err() << "Params element " << i << " is required to have its third element to be of type " << type_str(ARRAY) << ". Got " << sqf::type_str(fels.at(2)->dtype()) << '.' << std::endl;
				continue;
			}
			if (fels.size() >= 4 && (fels.at(3)->dtype() != sqf::ARRAY && fels.at(3)->dtype() != sqf::SCALAR))
			{
				vm->err() << "Params element " << i << " is required to have its fourth element to be of type " << type_str(ARRAY) << " or " << type_str(SCALAR) << ". Got " << sqf::type_str(fels.at(2)->dtype()) << '.' << std::endl;
				continue;
			}
			else if (fels.size() >= 4 && fels.at(3)->dtype() == sqf::ARRAY)
			{
				auto tmp = fels.at(3)->data<arraydata>();
				flag = false;
				for (j = 0; j < tmp->size(); j++)
				{
					if (tmp->at(j)->dtype() != sqf::SCALAR)
					{
						vm->err() << "Params element " << i << " and its inner " << j << ". element was expected to be of type " << type_str(SCALAR) << ". Got " << sqf::type_str(tmp->at(j)->dtype()) << '.' << std::endl;
						flag = true;
						continue;
					}
				}
				if (flag)
				{
					continue;
				}
			}

			if (i < elements->size())
			{
				auto el = elements->at(i);
				if (fels.size() >= 3 && fels.at(2)->data<arraydata>()->size() != 0)
				{
					auto tmp = fels.at(2)->data<arraydata>();

					auto found = std::find_if(tmp->begin(), tmp->end(), [type = el->dtype()](const std::shared_ptr<value>& val) {
						return type == val->dtype();
					});

					flag = found != tmp->end();

					if (!flag)
					{
						vm->wrn() << "Element " << i << " is not matching provided expected data types. Got " << sqf::type_str(el->dtype()) << '.' << std::endl;
						return fels.at(1);
					}
				}
				if (fels.size() >= 4 && el->dtype() == sqf::ARRAY)
				{
					flag = true;
					if (fels.at(2)->dtype() == sqf::ARRAY)
					{
						auto tmp = fels.at(2)->data<arraydata>();

						auto found = std::find_if(tmp->begin(),tmp->end(), [type = el->dtype()](const std::shared_ptr<value>& val) {
							return type == val->dtype();
						});

						flag = found != tmp->end();
					}
					else if (el->data<arraydata>()->size() != fels.at(3)->as_int())
					{
						flag = false;
					}
					if (!flag)
					{
						vm->wrn() << "Element " << i << " is not matching expected data types. Got " << sqf::type_str(el->dtype()) << '.' << std::endl;
						return fels.at(1);
					}
				}
				vm->stack()->stacks_top()->setvar(fels.at(0)->as_string(), el);
			}
			else
			{
				vm->stack()->stacks_top()->setvar(fels.at(0)->as_string(), fels.size() == 2 ? fels.at(1) : std::make_shared<sqf::value>());
			}
		}
		return std::make_shared<sqf::value>();
	}
	std::shared_ptr<value> params_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto _this = vm->stack()->getlocalvar("_this");
		if (_this->dtype() != ARRAY)
		{
			auto arr = std::make_shared<arraydata>();
			arr->push_back(_this);
			_this = std::make_shared<value>(arr, ARRAY);
		}
		return params_array_array(vm, _this, right);
	}
	std::shared_ptr<value> selectrandom_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto arr = right->data<arraydata>();
		return arr->at(rand() % arr->size());
	}
	std::shared_ptr<value> sleep_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		if (!vm->stack()->isscheduled())
		{
			vm->err() << "Cannot suspend in non-scheduled environment." << std::endl;
			return std::shared_ptr<value>();
		}
		auto duration = std::chrono::duration<float>(right->as_float());

		vm->stack()->sleep(std::chrono::duration_cast<std::chrono::milliseconds>(duration));
		return std::make_shared<value>();
	}
	std::shared_ptr<value> cansuspend_(virtualmachine* vm)
	{
		return std::make_shared<value>(vm->stack()->isscheduled());
	}
	std::shared_ptr<value> loadfile_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto res = vm->get_filesystem().try_get_physical_path(right->as_string());
		if (!res.has_value())
		{
			vm->wrn() << "File '" << right->as_string() << "' Not Found." << std::endl;
			return std::make_shared<value>("");
		}
		else
		{
			return std::make_shared<value>(load_file(res.value()));
		}
	}
	std::shared_ptr<value> preprocessfile_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto res = vm->get_filesystem().try_get_physical_path(right->as_string());
		if (!res.has_value())
		{
			vm->wrn() << "File '" << right->as_string() << "' Not Found." << std::endl;
			return std::make_shared<value>("");
		}
		else
		{
			auto filecontents = load_file(res.value());
			bool errflag = false;
			auto parsedcontents = sqf::parse::preprocessor::parse(vm, filecontents, errflag, right->as_string());
			return std::make_shared<value>(parsedcontents);
		}
	}
	std::shared_ptr<value> scriptname_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto str = right->as_string();
		vm->active_vmstack()->script_name(str);
		return std::make_shared<value>();
	}
	std::shared_ptr<value> in_any_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = right->data<arraydata>();
		auto res = std::find_if(arr->begin(), arr->end(), [left](std::shared_ptr<value> it) -> bool {
			return it->equals(left);
		});
		return std::make_shared<value>(res != arr->end());
	}
	std::shared_ptr<value> time_(virtualmachine* vm)
	{
		auto curtime = sqf::virtualmachine::system_time().time_since_epoch();
		auto starttime = vm->get_created_timestamp().time_since_epoch();
		long r = std::chrono::duration_cast<std::chrono::milliseconds>(starttime - curtime).count();
		return std::make_shared<value>(r);
	}
}
void sqf::commandmap::initgenericcmds()
{
	add(nular("time", "Returns time elapsed since virtualmachine start.", time_));
	add(nular("nil", "Nil value. This value can be used to undefine existing variables.", nil_));
	add(unary("call", sqf::type::CODE, "Executes given set of compiled instructions.", call_code));
	add(binary(4, "call", sqf::type::ANY, sqf::type::CODE, "Executes given set of compiled instructions with an option to pass arguments to the executed Code.", call_any_code));
	add(unary("count", sqf::type::ARRAY, "Can be used to count: the number of elements in array.", count_array));
	add(binary(4, "count", sqf::type::CODE, sqf::type::ARRAY, "Can be used to count: the number of elements in array with condition.", count_code_array));
	add(unary("compile", sqf::type::STRING, "Compile expression.", compile_string));
	add(unary("typeName", sqf::type::ANY, "Returns the data type of an expression.", typename_any));
	add(unary("str", sqf::type::ANY, "Converts any value into a string.", str_any));
	add(unary("comment", sqf::type::STRING, "Define a comment. Mainly used in SQF Syntax, as you're able to introduce comment lines with semicolons in a SQS script.", comment_string));
	add(unary("if", sqf::type::BOOL, "This operator creates a If Type which is used in conjunction with the 'then' command.", if_bool));
	add(binary(4, "then", type::IF, type::ARRAY, "First or second element of array is executed depending on left arg. Result of the expression executed is returned as a result (result may be Nothing).", then_if_array));
	add(binary(4, "then", type::IF, type::CODE, "If left arg is true, right arg is executed. Result of the expression executed is returned as a result (result may be Nothing).", then_if_code));
	add(binary(5, "else", type::CODE, type::CODE, "Concats left and right element into a single, 2 element array.", else_code_code));
	add(binary(4, "exitWith", type::IF, type::CODE, "If condition evaluates to true, executes the code in a new scope and exits the current one afterwards.", exitwith_if_code));
	add(unary("while", type::CODE, "Marks code as WHILE type.", while_code));
	add(binary(4, "do", type::WHILE, type::CODE, "Executes provided code as long as while condition evaluates to true.", do_while_code));
	add(unary("for", type::STRING, "Creates a FOR type for usage in 'for <var> from <start> to <end> [ step <stepsize> ] do <code>' construct.", for_string));
	add(binary(4, "from", type::FOR, type::SCALAR, "Sets the start index in a FOR type construct.", from_for_scalar));
	add(binary(4, "to", type::FOR, type::SCALAR, "Sets the end index in a FOR type construct.", to_for_scalar));
	add(binary(4, "step", type::FOR, type::SCALAR, "Sets the step size (default: 1) in a FOR type construct.", step_for_scalar));
	add(binary(4, "do", type::FOR, type::CODE, "Executes provided code as long as the var is smaller then the end index.", do_for_code));
	add(binary(4, "forEach", type::CODE, type::ARRAY, "Executes the given command(s) on every item of an array. The array items are represented by the magic variable _x. The array indices are represented by _forEachIndex.", foreach_code_array));

	add(binary(4, "select", type::ARRAY, type::SCALAR, "Selects the element at provided index from array. If the index provided equals the array length, nil will be returned.", select_array_scalar));
	add(binary(9, "#", type::ARRAY, type::SCALAR, "Selects the element at provided index from array. If the index provided equals the array length, nil will be returned.", select_array_scalar));
	add(unary("selectRandom", type::ARRAY, "Returns a random element from the given array.", selectrandom_array));

	add(binary(4, "select", type::ARRAY, type::BOOL, "Selects the first element if provided boolean is false, second element if it is true.", select_array_bool));
	add(binary(4, "select", type::ARRAY, type::ARRAY, "Selects a range of elements in provided array, starting at element 0 index, ending at either end of the string or the provided element 1 length.", select_array_array));
	add(binary(4, "select", type::ARRAY, type::CODE, "Selects elements from provided array matching provided condition. Current element will be placed in _x variable.", select_array_code));
	add(binary(4, "resize", type::ARRAY, type::SCALAR, "Changes the size of the given array. The command does not return new array, it resizes the source array to the desired number of elements. If the new size is bigger than the current size, the new places are filled with nils.", resize_array_scalar));
	add(binary(4, "pushBack", type::ARRAY, type::ANY, "Insert an element to the back of the given array. This command modifies the original array. Returns the index of the newly added element.", pushback_array_any));
	add(binary(4, "pushBackUnique", type::ARRAY, type::ANY, "Adds element to the back of the given array but only if it is unique to the array. The index of the added element is returned upon success, otherwise -1. This command modifies the original array.", pushbackunique_array_any));
	add(binary(4, "findIf", type::ARRAY, type::CODE, "Searches for an element within array for which the code evaluates to true. Returns the 0 based index on success or -1 if not found. Code on the right side of the command is evaluated for each element of the array, processed element can be referenced in code as _x.", findif_array_code));
	add(unary("reverse", type::ARRAY, "Reverses given array by reference. Modifies the original array.", reverse_array));
	add(unary("private", type::STRING, "Sets a variable to the innermost scope.", private_string));
	add(unary("private", type::ARRAY, "Sets a bunch of variables to the innermost scope.", private_array));
	add(unary("isNil", type::STRING, "Tests whether the variable defined by the string argument is undefined.", isnil_string));
	add(unary("isNil", type::CODE, "Tests whether an expression result passed as code is undefined.", isnil_code));
	add(unary("hint", type::STRING, "Outputs a hint message.", hint_string));
	add(unary("hint", type::TEXT, "Outputs a hint message.", hint_text));
	add(unary("systemChat", type::STRING, "Types text to the system radio channel.", systemchat_string));
	add(nular("productVersion", "Returns basic info about the product.", productversion_));

	add(unary("switch", type::ANY, "Creates a SWITCH type that can be used in 'switch do {...}'.", switch_any));
	add(binary(4, "do", type::SWITCH, type::CODE, "Executes provided code and sets the magic switch variable.", do_switch_code));
	add(unary("case", type::ANY, "Command to create a case inside a switch do construct. Will check if argument matches the one provided in switch strict. Requires a magic variable to be set. Cannot be used outside of switch do codeblock!", case_any));
	add(binary(4, ":", type::SWITCH, type::CODE, "Checks if switch type has the case flag being set and executes provided code then. If another switch got executed already, nothing will be done.", colon_switch_code));
	add(unary("default", type::CODE, "Sets the code to be executed by default if no case matched.", default_code));
	add(binary(4, "apply", type::ARRAY, type::CODE, "Applies given code to each element of the array and returns resulting array. The value of the current array element, to which the code will be applied, is stored in variable _x.", apply_array_code));
	add(binary(4, "spawn", type::ANY, type::CODE, "Adds given code to the scheduler. For SQF-VM, every script is guaranteed to get the same ammount of instructions done before being suspended.", spawn_any_code));
	add(unary("scriptDone", type::SCRIPT, "Check if a script is finished running using the Script_(Handle).", scriptdone_script));


	add(binary(4, "set", type::ARRAY, type::ARRAY, "Changes the element at the given (zero-based) index of the array. If the array size is smaller then the index provided, it is resized to allow for the index to be set.", set_array_array));
	add(unary("+", sqf::type::ARRAY, "Returns a copy of an array.", plus_array));
	add(binary(6, "+", sqf::type::ARRAY, sqf::type::ARRAY, "Concatenates two arrays.", plus_array_array));
	add(binary(6, "-", sqf::type::ARRAY, sqf::type::ARRAY, "Creates a new array with elements from the first parameter, that don't occur in the second one.", minus_array_array));
	add(binary(4, "append", sqf::type::ARRAY, sqf::type::ARRAY, "Appends the second array's element to the first array.", append_array_array));
	add(binary(4, "arrayIntersect", sqf::type::ARRAY, sqf::type::ARRAY, "Creates a new array that contains unique elements that occur in the first and the second array.", arrayintersect_array_array));
	add(binary(4, "deleteAt", sqf::type::ARRAY, sqf::type::SCALAR, "Deletes the item at a given array position.", deleteat_array_scalar));
	add(binary(4, "find", sqf::type::ARRAY, sqf::type::ANY, "Returns the index of an element in the array, or -1.", find_array_any));

	add(unary("selectMax", type::ARRAY, "Returns the array element with maximum numerical value. Therefore it is expected that supplied array consists of Numbers only. Booleans however are also supported and will be evaluated as Numbers: true - 1, false - 0. nil value treated as 0. Other non Number elements (not recommended) will be evaluated as 0 and Bad conversion: scalar message will be logged.", selectmax_array));
	add(unary("selectMin", type::ARRAY, "Returns the array element with minimum numerical value. Therefore it is expected that supplied array consists of Numbers only. Booleans however are also supported and will be evaluated as Numbers: true - 1, false - 0. nil value treated as 0. Other non Number elements (not recommended) will be evaluated as 0 and Bad conversion: scalar message will be logged.", selectmin_array));

	add(binary(4, "callExtension", type::STRING, type::STRING, "See https://community.bistudio.com/wiki/callExtension", callextension_string_string));
	add(binary(4, "callExtension", type::STRING, type::ARRAY, "See https://community.bistudio.com/wiki/callExtension", callextension_string_array));

	add(unary("params", type::ARRAY, "Parses arguments inside of _this into array of private variables.", params_array));
	add(binary(4, "params", type::ARRAY, type::ARRAY, "Parses input argument into array of private variables.", params_array_array));
	add(unary("param", type::ARRAY, "Extracts a single value with given index from _this.", param_array));
	add(binary(4, "param", type::ANY, type::ARRAY, "Extracts a single value with given index from input argument.", param_any_array));
	add(unary("sleep", type::SCALAR, "Suspends code execution for given time in seconds. The delay given is the minimal delay expected.", sleep_scalar));
	add(nular("canSuspend", "Returns true if sleep, uiSleep or waitUntil commands can be used in current scope.", cansuspend_));
	add(unary("loadFile", type::STRING, "", loadfile_string));
	add(unary("preprocessFileLineNumbers", type::STRING, "Reads and processes the content of the specified file. Preprocessor is C-like, supports comments using // or /* and */ and PreProcessor Commands.", preprocessfile_string));
	add(unary("preprocessFile", type::STRING, "Reads and processes the content of the specified file. Preprocessor is C-like, supports comments using // or /* and */ and PreProcessor Commands.", preprocessfile_string));
	add(unary("preprocessFile", type::STRING, "Reads and processes the content of the specified file. Preprocessor is C-like, supports comments using // or /* and */ and PreProcessor Commands.", preprocessfile_string));
	add(unary("scriptName", type::STRING, "Assign a user friendly name to the VM script this command is executed from. Once name is assigned, it cannot be changed.", scriptname_string));
	add(binary(4, "in", type::ANY, type::ARRAY, "Checks whether provided unit is inside of a vehicle. String values will be compared casesensitive.", in_any_array));

}
