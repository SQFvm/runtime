#include "full.h"

using namespace sqf;
namespace
{
	value_s call_code(const virtualmachine* vm, value_s right)
	{
		auto r = std::static_pointer_cast<codedata>(right->data());
		r->loadinto(vm->stack());
		vm->stack()->stacks_top()->setvar(L"_this", std::make_shared<value>());
		return value_s();
	}
	value_s call_any_code(const virtualmachine* vm, value_s left, value_s right)
	{
		auto r = std::static_pointer_cast<codedata>(right->data());
		r->loadinto(vm->stack());
		vm->stack()->stacks_top()->setvar(L"_this", std::make_shared<value>());
		vm->stack()->pushval(left);
		return value_s();
	}
	value_s count_array(const virtualmachine* vm, value_s right)
	{
		auto r = std::static_pointer_cast<arraydata>(right->data());
		return std::make_shared<value>(r->size());
	}
	value_s compile_string(const virtualmachine* vm, value_s right)
	{
		auto r = right->as_string();
		auto cs = std::make_shared<callstack>();
		vm->parse_sqf(r, cs);
		return std::make_shared<value>(cs);
	}
	value_s typename_any(const virtualmachine* vm, value_s right)
	{
		return std::make_shared<value>(type_str(right->dtype()));
	}
	value_s str_any(const virtualmachine* vm, value_s right)
	{
		return std::make_shared<value>(std::make_shared<stringdata>(right->tosqf(), false), type::STRING);
	}
	value_s nil_(const virtualmachine* vm)
	{
		return std::make_shared<value>();
	}
	value_s comment_string(const virtualmachine* vm, value_s right)
	{
		return std::make_shared<value>();
	}
	value_s if_bool(const virtualmachine* vm, value_s right)
	{
		return std::make_shared<value>(right->data(), type::IF);
	}
	value_s then_if_array(const virtualmachine* vm, value_s left, value_s right)
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
				vm->wrn() << L"Expected element 1 of array to be of type code." << std::endl;
			}
			if (el0->dtype() == type::CODE)
			{
				auto code = std::static_pointer_cast<codedata>(el0->data());
				code->loadinto(vm->stack());
				return value_s();
			}
			else
			{
				vm->err() << L"Expected element 0 of array to be of type code." << std::endl;
				return std::make_shared<value>();
			}
		}
		else
		{
			if (el0->dtype() != type::CODE)
			{
				vm->wrn() << L"Expected element 0 of array to be of type code." << std::endl;
			}
			if (el1->dtype() == type::CODE)
			{
				auto code = std::static_pointer_cast<codedata>(el1->data());
				code->loadinto(vm->stack());
				return value_s();
			}
			else
			{
				vm->err() << L"Expected element 1 of array to be of type code." << std::endl;
				return std::make_shared<value>();
			}
		}
	}
	value_s then_if_code(const virtualmachine* vm, value_s left, value_s right)
	{
		auto ifcond = left->as_bool();
		auto code = std::static_pointer_cast<codedata>(right->data());
		if (ifcond)
		{
			code->loadinto(vm->stack());
			return value_s();
		}
		else
		{
			return std::make_shared<value>();
		}
	}
	value_s else_code_code(const virtualmachine* vm, value_s left, value_s right)
	{
		auto vec = std::vector<value_s>(2);
		vec[0] = left;
		vec[1] = right;
		return std::make_shared<value>(vec);
	}
	value_s while_code(const virtualmachine* vm, value_s right)
	{
		return std::make_shared<value>(right->data(), type::WHILE);
	}
	value_s do_while_code(const virtualmachine* vm, value_s left, value_s right)
	{
		auto whilecond = std::static_pointer_cast<codedata>(left->data());
		auto execcode = std::static_pointer_cast<codedata>(right->data());

		auto cs = std::make_shared<callstack_while>(whilecond, execcode);
		vm->stack()->pushcallstack(cs);

		return value_s();
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
	add(unary(L"while", type::CODE, L"Marks code as WHILE type.", while_code));
	add(binary(4, L"do", type::WHILE, type::CODE, L"Executed provided code as long as while condition evaluates to true.", do_while_code));
}
