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
	add(binary(6, L"+", sqf::type::SCALAR, sqf::type::SCALAR, L"b added to a.", [](const virtualmachine* vm, value_s left, value_s right) -> value_s {
		return std::make_shared<value>((left->as_double()) + (right->as_double()));
	}));
	add(binary(6, L"-", sqf::type::SCALAR, sqf::type::SCALAR, L"Subtracts b from a.", [](const virtualmachine* vm, value_s left, value_s right) -> value_s {
		return std::make_shared<value>((left->as_double()) - (right->as_double()));
	}));
	add(binary(7, L"*", sqf::type::SCALAR, sqf::type::SCALAR, L"Returns the value of a multiplied by b.", [](const virtualmachine* vm, value_s left, value_s right) -> value_s {
		return std::make_shared<value>((left->as_double()) * (right->as_double()));
	}));
	add(binary(7, L"/", sqf::type::SCALAR, sqf::type::SCALAR, L"a divided by b. Division by 0 throws \"Division by zero\" error, however script doesn't stop and the result of such division is assumed to be 0.", [](const virtualmachine* vm, value_s left, value_s right) -> value_s {
		auto r = (right->as_double());
		if (r == 0)
			return std::make_shared<value>(0);
		return std::make_shared<value>((left->as_double()) / r);
	}));
}
