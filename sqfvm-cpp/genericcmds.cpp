#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "codedata.h"
#include "arraydata.h"
#include "stringdata.h"
#include "fordata.h"
#include "switchdata.h"
#include "scriptdata.h"
#include "callstack_for.h"
#include "callstack_while.h"
#include "callstack_select.h"
#include "callstack_isnil.h"
#include "callstack_exitwith.h"
#include "callstack_switch.h"
#include "callstack_apply.h"


#define CALLEXTBUFFSIZE 10240
#define CALLEXTVERSIONBUFFSIZE 32
#define RVARGSLIMIT 1024

#if defined(__linux__)
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
			std::make_shared<value>("SQF-VM"), //product name
			std::make_shared<value>("sqf-vm"),
			std::make_shared<value>(2),
			std::make_shared<value>(0),
			std::make_shared<value>("COMMUNITY"),
			std::make_shared<value>(false),
#if _WIN32
			std::make_shared<value>("Windows"),
#elif __linux__
			std::make_shared<value>("Linux"),
#else
			std::make_shared<value>("NA"),
#endif
			std::make_shared<value>("x86")
		};
		return std::make_shared<value>(vec);
	}
	std::shared_ptr<value> call_code(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = std::static_pointer_cast<codedata>(right->data());
		r->loadinto(vm->stack());
		vm->stack()->stacks_top()->setvar("_this", std::make_shared<value>());
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> call_any_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto r = std::static_pointer_cast<codedata>(right->data());
		r->loadinto(vm->stack());
		vm->stack()->stacks_top()->setvar("_this", std::make_shared<value>());
		vm->stack()->pushval(left);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> count_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = std::static_pointer_cast<arraydata>(right->data());
		return std::make_shared<value>(r->size());
	}
	std::shared_ptr<value> compile_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		auto cs = std::make_shared<callstack>();
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
				auto code = std::static_pointer_cast<codedata>(el0->data());
				code->loadinto(vm->stack());
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
				auto code = std::static_pointer_cast<codedata>(el1->data());
				code->loadinto(vm->stack());
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
		auto code = std::static_pointer_cast<codedata>(right->data());
		if (ifcond)
		{
			code->loadinto(vm->stack());
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
		auto code = std::static_pointer_cast<codedata>(right->data());
		if (ifcond)
		{
			auto cs = std::make_shared<callstack_exitwith>();
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
		auto whilecond = std::static_pointer_cast<codedata>(left->data());
		auto execcode = std::static_pointer_cast<codedata>(right->data());

		auto cs = std::make_shared<callstack_while>(whilecond, execcode);
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
		auto fordata = std::static_pointer_cast<sqf::fordata>(left->data());
		auto index = right->as_double();
		fordata->from(index);
		return left;
	}
	std::shared_ptr<value> to_for_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto fordata = std::static_pointer_cast<sqf::fordata>(left->data());
		auto index = right->as_double();
		fordata->to(index);
		return left;
	}
	std::shared_ptr<value> step_for_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto fordata = std::static_pointer_cast<sqf::fordata>(left->data());
		auto index = right->as_double();
		fordata->step(index);
		return left;
	}
	std::shared_ptr<value> do_for_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto fordata = std::static_pointer_cast<sqf::fordata>(left->data());
		auto execcode = std::static_pointer_cast<codedata>(right->data());

		auto cs = std::make_shared<callstack_for>(fordata, execcode);
		vm->stack()->pushcallstack(cs);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> select_array_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->as_vector();
		auto index = right->as_int();

		if ((int)arr.size() <= index || index < 0)
		{
			vm->err() << "Index out of range." << std::endl;
			return std::make_shared<value>();
		}
		if ((int)arr.size() == index)
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
		return flag ? arr[0] : arr[1];
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
		int start = arr[0]->as_int();
		if (start < 0)
		{
			vm->wrn() << "Start index is smaller then 0. Returning empty array." << std::endl;
			return std::make_shared<value>("");
		}
		if (start > (int)vec.size())
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

			return std::make_shared<value>(std::vector<std::shared_ptr<value>>(vec.begin() + start, start + length > (int)vec.size() ? vec.end() : vec.begin() + start + length));
		}
		return std::make_shared<value>(std::vector<std::shared_ptr<value>>(vec.begin() + start, vec.end()));
	}
	std::shared_ptr<value> select_array_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->as_vector();
		if (arr.size() == 0)
			return std::make_shared<value>(std::vector<std::shared_ptr<value>>());
		auto cond = std::static_pointer_cast<codedata>(right->data());
		auto cs = std::make_shared<sqf::callstack_select>(arr, cond);
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
		arr->push_back(right);
		return std::make_shared<value>(newindex);
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
		for (auto it : arr)
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
		return std::make_shared<value>(val->dtype() == sqf::type::NOTHING);
	}
	std::shared_ptr<value> isnil_code(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto cdata = right->data<codedata>();
		auto cs = std::make_shared<callstack_isnil>(vm, cdata);
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
		auto cs = std::make_shared<callstack_switch>(left->data<switchdata>());
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
			r->loadinto(vm->stack());
			return std::shared_ptr<value>();
		}
		return std::make_shared<value>();
	}
	std::shared_ptr<value> apply_array_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->as_vector();
		if (arr.size() == 0)
			return std::make_shared<value>(std::vector<std::shared_ptr<value>>());
		auto cond = std::static_pointer_cast<codedata>(right->data());
		auto cs = std::make_shared<sqf::callstack_apply>(arr, cond);
		vm->stack()->pushcallstack(cs);
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> spawn_any_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto code = right->data<codedata>();
		auto scrpt = std::make_shared<scriptdata>();
		code->loadinto(scrpt->stack());
		vm->push_spawn(scrpt);
		scrpt->stack()->stacks_top()->setvar("_this", left);
		return std::make_shared<value>(scrpt, sqf::type::SCRIPT);
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

		auto index = params[0]->as_int();
		auto val = params[1];
		if ((int)arr->size() <= index)
		{
			arr->resize(index + 1);
		}
		arr->operator[](index) = val;
		return std::make_shared<value>();
	}
	std::shared_ptr<value> plus_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->data<arraydata>();
		return std::make_shared<value>(r->operator std::vector<std::shared_ptr<sqf::value>, std::allocator<std::shared_ptr<sqf::value>>>());
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
			if (!it->path().compare(name))
			{
				return it;
			}
		}
		vm->libraries().push_back(std::make_shared<dlops>(name));
		auto& dl = vm->libraries().back();
		void* sym;
		if (dl->try_resolve("RVExtensionVersion", &sym))
		{
			((RVExtensionVersion)sym)(buffer, CALLEXTVERSIONBUFFSIZE);
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
		try
		{
			auto dl = helpermethod_callextension_loadlibrary(vm, left->as_string());
#if defined(_WIN32) & !defined(_WIN64)
			auto method = (RVExtension)dl->resolve("_RVExtension@12");
#else
			auto method = (RVExtension)dl->resolve("RVExtension");
#endif
			method(buffer, CALLEXTBUFFSIZE, right->as_string().c_str());
			if (buffer[CALLEXTBUFFSIZE - 1] != '\0')
			{
				vm->wrn() << "Library '" << left->as_string() << "' is not terminating RVExtension output buffer with a '\\0'!" << std::endl;
			}
			return std::make_shared<value>(buffer);
		}
		catch (std::runtime_error ex)
		{
			vm->wrn() << "Could not complete command execution due to error with library '" << left->as_string() << "' (RVExtension):" << ex.what() << std::endl;
			return std::make_shared<value>("");
		}
	}
	std::shared_ptr<value> callextension_string_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		static char buffer[CALLEXTBUFFSIZE + 1] = { 0 };
		auto rvec = right->data<arraydata>();
		if (rvec->size() < 2)
		{
			vm->wrn() << "Expected 2 elements in array, got " << rvec->size() << ". Returning error code PARAMS_ERROR_TOO_MANY_ARGS(201)." << std::endl;
			return std::make_shared<value>(std::vector<std::shared_ptr<value>> { std::make_shared<value>(""), std::make_shared<value>(0), std::make_shared<value>(201) });
			return std::shared_ptr<value>();
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
			vm->wrn() << "callExtension SYNTAX_ERROR_WRONG_PARAMS_SIZE(101) error with '" << left->as_string() << "' (RVExtensionArgs)" << std::endl;
			return std::make_shared<value>(std::vector<std::shared_ptr<value>> { std::make_shared<value>(""), std::make_shared<value>(0), std::make_shared<value>(101) });
		}
		std::vector<std::string> argstringvec;

		for (size_t i = 0; i < arr->size(); i++)
		{
			auto& at = arr->at(i);
			switch (at->dtype())
			{
			case type::BOOL:
			case type::STRING:
			case type::SCALAR:
			case type::ARRAY:
				argstringvec.push_back(at->as_string());
				break;
			default:
				vm->wrn() << "callExtension SYNTAX_ERROR_WRONG_PARAMS_TYPE(102) error with '" << left->as_string() << "' (RVExtensionArgs)" << std::endl;
				return std::make_shared<value>(std::vector<std::shared_ptr<value>> { std::make_shared<value>(""), std::make_shared<value>(0), std::make_shared<value>(102) });
			}
		}
		std::vector<const char*> argvec;
		argvec.reserve(argstringvec.size());
		for (size_t i = 0; i < argstringvec.size(); i++)
		{
			argvec.push_back(argstringvec[i].c_str());
		}

		try
		{
			auto dl = helpermethod_callextension_loadlibrary(vm, left->as_string());
#if defined(_WIN32) & !defined(_WIN64)
			auto method = (RVExtensionArgs)dl->resolve("_RVExtensionArgs@20");
#else
			auto method = (RVExtensionArgs)dl->resolve("RVExtensionArgs");
#endif
			auto res = method(buffer, CALLEXTBUFFSIZE, rvec->at(0)->as_string().c_str(), argvec.data(), (int)argvec.size());
			if (buffer[CALLEXTBUFFSIZE - 1] != '\0')
			{
				vm->wrn() << "Library '" << left->as_string() << "' is not terminating RVExtensionArgs output buffer with a '\\0'!" << std::endl;
			}
			return std::make_shared<value>(std::vector<std::shared_ptr<value>> { std::make_shared<value>(buffer), std::make_shared<value>(res), std::make_shared<value>(0) });
		}
		catch (std::runtime_error ex)
		{
			vm->wrn() << "Could not complete command execution due to error with library '" << left->as_string() << "' (RVExtensionArgs):" << ex.what() << std::endl;
			return std::make_shared<value>(std::vector<std::shared_ptr<value>> { std::make_shared<value>(""), std::make_shared<value>(0), std::make_shared<value>(501) });
		}
	}
}
void sqf::commandmap::initgenericcmds(void)
{
	add(nular("ni", "Nil value. This value can be used to undefine existing variables.", nil_));
	add(unary("cal", sqf::type::CODE, "Executes given set of compiled instructions.", call_code));
	add(binary(4, "cal", sqf::type::ANY, sqf::type::CODE, "Executes given set of compiled instructions with an option to pass arguments to the executed Code.", call_any_code));
	add(unary("count", sqf::type::ARRAY, "Can be used to count: the number of elements in array.", count_array));
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

	add(binary(4, "select", type::ARRAY, type::SCALAR, "Selects the element at provided index from array. If the index provided equals the array length, nil will be returned.", select_array_scalar));
	add(binary(4, "select", type::ARRAY, type::BOOL, "Selects the first element if provided boolean is true, second element if it is false.", select_array_bool));
	add(binary(4, "select", type::ARRAY, type::ARRAY, "Selects a range of elements in provided array, starting at element 0 index, ending at either end of the string or the provided element 1 length.", select_array_array));
	add(binary(4, "select", type::ARRAY, type::CODE, "Selects elements from provided array matching provided condition. Current element will be placed in _x variable.", select_array_code));
	add(binary(4, "resize", type::ARRAY, type::SCALAR, "Changes the size of the given array. The command does not return new array, it resizes the source array to the desired number of elements. If the new size is bigger than the current size, the new places are filled with nils.", resize_array_scalar));
	add(binary(4, "pushBack", type::ARRAY, type::ANY, "Insert an element to the back of the given array. This command modifies the original array. Returns the index of the newly added element.", pushback_array_any));
	add(unary("reverse", type::ARRAY, "Reverses given array by reference. Modifies the original array.", reverse_array));
	add(unary("private", type::STRING, "Sets a variable to the innermost scope.", private_string));
	add(unary("private", type::ARRAY, "Sets a bunch of variables to the innermost scope.", private_array));
	add(unary("isNi", type::STRING, "Tests whether the variable defined by the string argument is undefined.", isnil_string));
	add(unary("isNi", type::CODE, "Tests whether an expression result passed as code is undefined.", isnil_code));
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

	add(unary("selectMax", type::ARRAY, "Returns the array element with maximum numerical value. Therefore it is expected that supplied array consists of Numbers only. Booleans however are also supported and will be evaluated as Numbers: true - 1, false - 0. nil value treated as 0. Other non Number elements (not recommended) will be evaluated as 0 and Bad conversion: scalar message will be logged.", selectmax_array));
	add(unary("selectMin", type::ARRAY, "Returns the array element with minimum numerical value. Therefore it is expected that supplied array consists of Numbers only. Booleans however are also supported and will be evaluated as Numbers: true - 1, false - 0. nil value treated as 0. Other non Number elements (not recommended) will be evaluated as 0 and Bad conversion: scalar message will be logged.", selectmin_array));

	add(binary(4, "callExtension", type::STRING, type::STRING, "See https://community.bistudio.com/wiki/callExtension", callextension_string_string));
	add(binary(4, "callExtension", type::STRING, type::ARRAY, "See https://community.bistudio.com/wiki/callExtension", callextension_string_array));
}
