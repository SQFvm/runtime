#include "full.h"

std::shared_ptr<sqf::unarycmd> sqf::commandmap::get(std::wstring str, type rtype)
{
	auto listsptr = munarycmd[str];
	if (listsptr.get())
	{
		for each (auto it in *listsptr)
		{
			if (it->matches(type::NA, rtype))
			{
				return it;
			}
		}
	}
	return std::shared_ptr<unarycmd>();
}

std::shared_ptr<sqf::binarycmd> sqf::commandmap::get(std::wstring str, type ltype, type rtype)
{
	auto listsptr = mbinarycmd[str];
	if (listsptr.get())
	{
		for each (auto it in *listsptr)
		{
			if (it->matches(ltype, rtype))
			{
				return it;
			}
		}
	}
	return std::shared_ptr<binarycmd>();
}

sqf::commandmap& sqf::commandmap::get(void)
{
	static sqf::commandmap map;
	return map;
}

void sqf::commandmap::init(void)
{
	add(nular(L"ntest", L"foo", [](const virtualmachine* vm) -> value_s { vm->out() << L"ntest" << std::endl; return value_s(); }));
	add(unary(L"utest", sqf::type::ANY, L"foo", [](const virtualmachine* vm, value_s left) -> value_s { vm->out() << L"utest" << std::endl; return value_s(); }));
	add(binary(6, L"+", sqf::type::SCALAR, sqf::type::SCALAR, L"+", [](const virtualmachine* vm, value_s left, value_s right) -> value_s {  return std::make_shared<value>((left->as_double()) + (right->as_double())); }));
	add(binary(6, L"-", sqf::type::SCALAR, sqf::type::SCALAR, L"-", [](const virtualmachine* vm, value_s left, value_s right) -> value_s { return std::make_shared<value>((left->as_double()) - (right->as_double())); }));
	add(binary(7, L"*", sqf::type::SCALAR, sqf::type::SCALAR, L"*", [](const virtualmachine* vm, value_s left, value_s right) -> value_s { return std::make_shared<value>((left->as_double()) * (right->as_double())); }));
	add(binary(7, L"/", sqf::type::SCALAR, sqf::type::SCALAR, L"/", [](const virtualmachine* vm, value_s left, value_s right) -> value_s { return std::make_shared<value>((left->as_double()) / (right->as_double())); }));
}
