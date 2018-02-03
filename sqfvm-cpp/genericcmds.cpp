#include "full.h"

using namespace sqf;
namespace
{
	value_s call_code(const virtualmachine* vm, value_s right)
	{
		auto r = std::static_pointer_cast<codedata>(right->as_data());
		r->loadinto(vm->stack());
		vm->stack()->stacks_top()->setvar(L"_this", std::make_shared<value>());
		return value_s();
	}
	value_s call_any_code(const virtualmachine* vm, value_s left, value_s right)
	{
		auto r = std::static_pointer_cast<codedata>(right->as_data());
		r->loadinto(vm->stack());
		vm->stack()->stacks_top()->setvar(L"_this", std::make_shared<value>());
		vm->stack()->pushval(left);
		return value_s();
	}
	value_s count_array(const virtualmachine* vm, value_s right)
	{
		auto r = std::static_pointer_cast<arraydata>(right->as_data());
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
		return std::make_shared<value>(type_str(right->as_type()));
	}
	value_s str_any(const virtualmachine* vm, value_s right)
	{
		return std::make_shared<value>(std::make_shared<stringdata>(right->to_string(), false), type::STRING);
	}
	value_s nil_(const virtualmachine* vm)
	{
		return std::make_shared<value>();
	}
	value_s comment_string(const virtualmachine* vm, value_s right)
	{
		return std::make_shared<value>();
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
}
