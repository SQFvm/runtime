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

using namespace sqf;
namespace
{
	std::shared_ptr<value> productversion_(virtualmachine* vm)
	{
		auto vec = std::vector<std::shared_ptr<value>> {
			std::make_shared<value>(L"SQF-VM"), //product name
			std::make_shared<value>(L"sqf-vm"),
			std::make_shared<value>(2),
			std::make_shared<value>(0),
			std::make_shared<value>(L"COMMUNITY"),
			std::make_shared<value>(false),
#if _WIN32
			std::make_shared<value>(L"Windows"),
#elif __linux__
			std::make_shared<value>(L"Linux"),
#else
			std::make_shared<value>(L"NA"),
#endif
			std::make_shared<value>(L"x86")
		};
		return std::make_shared<value>(vec);
	}
	std::shared_ptr<value> call_code(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = std::static_pointer_cast<codedata>(right->data());
		r->loadinto(vm->stack());
		vm->stack()->stacks_top()->setvar(L"_this", std::make_shared<value>());
		return std::shared_ptr<value>();
	}
	std::shared_ptr<value> call_any_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto r = std::static_pointer_cast<codedata>(right->data());
		r->loadinto(vm->stack());
		vm->stack()->stacks_top()->setvar(L"_this", std::make_shared<value>());
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
			vm->err() << L"Expected 2 elements in array." << std::endl;
			return std::make_shared<value>();
		}
		auto el0 = arr[0];
		auto el1 = arr[1];
		if (ifcond)
		{
			if (el1->dtype() != type::CODE)
			{
				vm->wrn() << L"Expected element 1 of array to be of type 'CODE' but was '" << sqf::type_str(el1->dtype()) << L"'." << std::endl;
			}
			if (el0->dtype() == type::CODE)
			{
				auto code = std::static_pointer_cast<codedata>(el0->data());
				code->loadinto(vm->stack());
				return std::shared_ptr<value>();
			}
			else
			{
				vm->err() << L"Expected element 0 of array to be of type 'CODE' but was '" << sqf::type_str(el0->dtype()) << L"'." << std::endl;
				return std::make_shared<value>();
			}
		}
		else
		{
			if (el0->dtype() != type::CODE)
			{
				vm->wrn() << L"Expected element 0 of array to be of type 'CODE' but was '" << sqf::type_str(el0->dtype()) << L"'." << std::endl;
			}
			if (el1->dtype() == type::CODE)
			{
				auto code = std::static_pointer_cast<codedata>(el1->data());
				code->loadinto(vm->stack());
				return std::shared_ptr<value>();
			}
			else
			{
				vm->err() << L"Expected element 1 of array to be of type 'CODE' but was '" << sqf::type_str(el1->dtype()) << L"'." << std::endl;
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
			vm->err() << L"Index out of range." << std::endl;
			return std::make_shared<value>();
		}
		if ((int)arr.size() == index)
		{
			vm->wrn() << L"Index equals range. Returning nil." << std::endl;
			return std::make_shared<value>();
		}
		return arr[index];
	}
	std::shared_ptr<value> select_array_bool(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto arr = left->as_vector();
		auto flag = right->as_bool();
		if (!flag && arr.size() < 2 || arr.size() < 1)
		{
			vm->wrn() << L"Array should have at least two elements. Returning nil" << std::endl;
			return std::make_shared<value>();
		}
		else if (flag && arr.size() < 2)
		{
			vm->wrn() << L"Array should have at least two elements." << std::endl;
		}
		return flag ? arr[0] : arr[1];
	}
	std::shared_ptr<value> select_array_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto vec = left->as_vector();
		auto arr = right->as_vector();
		if (arr.size() < 1)
		{
			vm->err() << L"Array was expected to have at least a single element." << std::endl;
			return std::make_shared<value>();
		}
		if (arr[0]->dtype() != type::SCALAR)
		{
			vm->err() << L"First element of array was expected to be SCALAR, got " << sqf::type_str(arr[0]->dtype()) << L'.' << std::endl;
			return std::make_shared<value>();
		}
		int start = arr[0]->as_int();
		if (start < 0)
		{
			vm->wrn() << L"Start index is smaller then 0. Returning empty array." << std::endl;
			return std::make_shared<value>(L"");
		}
		if (start > (int)vec.size())
		{
			vm->wrn() << L"Start index is larger then string length. Returning empty array." << std::endl;
			return std::make_shared<value>(L"");
		}
		if (arr.size() >= 2)
		{
			if (arr[1]->dtype() != type::SCALAR)
			{
				vm->err() << L"Second element of array was expected to be SCALAR, got " << sqf::type_str(arr[0]->dtype()) << L'.' << std::endl;
				return std::make_shared<value>();
			}
			int length = arr[1]->as_int();
			if (length < 0)
			{
				vm->wrn() << L"Length is smaller then 0. Returning empty array." << std::endl;
				return std::make_shared<value>(L"");
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
			vm->err() << L"New size cannot be smaller then 0." << std::endl;
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
				vm->err() << L"Index position " << i << L" was expected to be of type 'STRING' but was '" << sqf::type_str(it->dtype()) << L"'." << std::endl;
			}
		}
		if (errflag)
		{
			return std::shared_ptr<value>();
		}
		for each (auto it in arr)
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
		vm->out() << L"[HINT]\t" << r << std::endl;
		return std::make_shared<value>();
	}
	std::shared_ptr<value> hint_text(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		vm->out() << L"[HINT]\t" << r << std::endl;
		return std::make_shared<value>();
	}
	std::shared_ptr<value> systemchat_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		vm->out() << L"[CHAT]\tSYSTEM: " << r << std::endl;
		return std::make_shared<value>();
	}
	std::shared_ptr<value> exitwith_if_code(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		vm->out() << L"[CHAT]\tSYSTEM: " << r << std::endl;
		return std::make_shared<value>();
	}

#define MAGIC_SWITCH L"___switch"
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
			vm->err() << L"Magic variable '___switch' is not of type 'SWITCH' but was '" << sqf::type_str(valswtch->dtype()) << L"'.";
			return std::shared_ptr<value>();
		}
		auto swtch = valswtch->data<switchdata>();
		
		if (right->equals(swtch->value()))
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
			vm->err() << L"Magic variable '___switch' is not of type 'SWITCH' but was '" << sqf::type_str(valswtch->dtype()) << L"'.";
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
		scrpt->stack()->stacks_top()->setvar(L"_this", left);
		return std::make_shared<value>(scrpt, sqf::type::SCRIPT);
	}
	std::shared_ptr<value> scriptdone_script(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->data<scriptdata>();
		return std::make_shared<value>(r->hasfinished());
	}
}
void sqf::commandmap::initgenericcmds(void)
{
	add(nular(L"nil", L"Nil value. This value can be used to undefine existing variables.", nil_));
	add(unary(L"call", sqf::type::CODE, L"Executes given set of compiled instructions.", call_code));
	add(binary(4, L"call", sqf::type::ANY, sqf::type::CODE, L"Executes given set of compiled instructions with an option to pass arguments to the executed Code.", call_any_code));
	add(unary(L"count", sqf::type::ARRAY, L"Can be used to count: the number of elements in array.", count_array));
	add(unary(L"compile", sqf::type::STRING, L"Compile expression.", compile_string));
	add(unary(L"typeName", sqf::type::ANY, L"Returns the data type of an expression.", typename_any));
	add(unary(L"str", sqf::type::ANY, L"Converts any value into a string.", str_any));
	add(unary(L"comment", sqf::type::STRING, L"Define a comment. Mainly used in SQF Syntax, as you're able to introduce comment lines with semicolons in a SQS script.", comment_string));
	add(unary(L"if", sqf::type::BOOL, L"This operator creates a If Type which is used in conjunction with the 'then' command.", if_bool));
	add(binary(4, L"then", type::IF, type::ARRAY, L"First or second element of array is executed depending on left arg. Result of the expression executed is returned as a result (result may be Nothing).", then_if_array));
	add(binary(4, L"then", type::IF, type::CODE, L"If left arg is true, right arg is executed. Result of the expression executed is returned as a result (result may be Nothing).", then_if_code));
	add(binary(5, L"else", type::CODE, type::CODE, L"Concats left and right element into a single, 2 element array.", else_code_code));
	add(binary(4, L"exitWith", type::IF, type::CODE, L"If condition evaluates to true, executes the code in a new scope and exits the current one afterwards.", exitwith_if_code));
	add(unary(L"while", type::CODE, L"Marks code as WHILE type.", while_code));
	add(binary(4, L"do", type::WHILE, type::CODE, L"Executes provided code as long as while condition evaluates to true.", do_while_code));
	add(unary(L"for", type::STRING, L"Creates a FOR type for usage in 'for <var> from <start> to <end> [ step <stepsize> ] do <code>' construct.", for_string));
	add(binary(4, L"from", type::FOR, type::SCALAR, L"Sets the start index in a FOR type construct.", from_for_scalar));
	add(binary(4, L"to", type::FOR, type::SCALAR, L"Sets the end index in a FOR type construct.", to_for_scalar));
	add(binary(4, L"step", type::FOR, type::SCALAR, L"Sets the step size (default: 1) in a FOR type construct.", step_for_scalar));
	add(binary(4, L"do", type::FOR, type::CODE, L"Executes provided code as long as the var is smaller then the end index.", do_for_code));

	add(binary(4, L"select", type::ARRAY, type::SCALAR, L"Selects the element at provided index from array. If the index provided equals the array length, nil will be returned.", select_array_scalar));
	add(binary(4, L"select", type::ARRAY, type::BOOL, L"Selects the first element if provided boolean is true, second element if it is false.", select_array_bool));
	add(binary(4, L"select", type::ARRAY, type::ARRAY, L"Selects a range of elements in provided array, starting at element 0 index, ending at either end of the string or the provided element 1 length.", select_array_array));
	add(binary(4, L"select", type::ARRAY, type::CODE, L"Selects elements from provided array matching provided condition. Current element will be placed in _x variable.", select_array_code));
	add(binary(4, L"resize", type::ARRAY, type::SCALAR, L"Changes the size of the given array. The command does not return new array, it resizes the source array to the desired number of elements. If the new size is bigger than the current size, the new places are filled with nils.", resize_array_scalar));
	add(binary(4, L"pushBack", type::ARRAY, type::ANY, L"Insert an element to the back of the given array. This command modifies the original array. Returns the index of the newly added element.", pushback_array_any));
	add(unary(L"reverse", type::ARRAY, L"Reverses given array by reference. Modifies the original array.", reverse_array));
	add(unary(L"private", type::STRING, L"Sets a variable to the innermost scope.", private_string));
	add(unary(L"private", type::ARRAY, L"Sets a bunch of variables to the innermost scope.", private_array));
	add(unary(L"isNil", type::STRING, L"Tests whether the variable defined by the string argument is undefined.", isnil_string));
	add(unary(L"isNil", type::CODE, L"Tests whether an expression result passed as code is undefined.", isnil_code));
	add(unary(L"hint", type::STRING, L"Outputs a hint message.", hint_string));
	add(unary(L"hint", type::TEXT, L"Outputs a hint message.", hint_text));
	add(unary(L"systemChat", type::STRING, L"Types text to the system radio channel.", systemchat_string));
	add(nular(L"productVersion", L"Returns basic info about the product.", productversion_));

	add(unary(L"switch", type::ANY, L"Creates a SWITCH type that can be used in 'switch do {...}'.", switch_any));
	add(binary(4, L"do", type::SWITCH, type::CODE, L"Executes provided code and sets the magic switch variable.", do_switch_code));
	add(unary(L"case", type::ANY, L"Command to create a case inside a switch do construct. Will check if argument matches the one provided in switch strict. Requires a magic variable to be set. Cannot be used outside of switch do codeblock!", case_any));
	add(binary(4, L":", type::SWITCH, type::CODE, L"Checks if switch type has the case flag being set and executes provided code then. If another switch got executed already, nothing will be done.", colon_switch_code));
	add(unary(L"default", type::CODE, L"Sets the code to be executed by default if no case matched.", default_code));
	add(binary(4, L"apply", type::ARRAY, type::CODE, L"Applies given code to each element of the array and returns resulting array. The value of the current array element, to which the code will be applied, is stored in variable _x.", apply_array_code));
	add(binary(4, L"spawn", type::ANY, type::CODE, L"Adds given code to the scheduler. For SQF-VM, every script is guaranteed to get the same ammount of instructions done before being suspended.", spawn_any_code));
	add(unary(L"scriptDone", type::SCRIPT, L"Check if a script is finished running using the Script_(Handle).", scriptdone_script));

}
