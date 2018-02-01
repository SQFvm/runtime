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
	value_s count_string(const virtualmachine* vm, value_s right)
	{
		auto r = right->as_string();
		return std::make_shared<value>(r.length());
	}
	value_s compile_string(const virtualmachine* vm, value_s right)
	{
		auto r = right->as_string();
		auto cs = std::make_shared<callstack>();
		vm->parse_sqf(r, cs);
		return std::make_shared<value>(cs);
	}
}
void sqf::commandmap::initgenericops(void)
{
	add(unary(L"call", sqf::type::CODE, L"Executes given set of compiled instructions.", call_code));
	add(binary(4, L"call", sqf::type::ANY, sqf::type::CODE, L"Executes given set of compiled instructions with an option to pass arguments to the executed Code.", call_any_code));
	add(unary(L"count", sqf::type::ARRAY, L"Can be used to count: the number of elements in array.", count_array));
	add(unary(L"count", sqf::type::STRING, L"Can be used to count: the number of characters in a string.", count_string));
	add(unary(L"compile", sqf::type::STRING, L"Compile expression.", compile_string));
}
