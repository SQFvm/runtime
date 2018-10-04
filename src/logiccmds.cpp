#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "codedata.h"

using namespace sqf;
namespace
{
	std::shared_ptr<value> and_bool_bool(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_bool();
		auto r = right->as_bool();
		return std::make_shared<value>(l && r);
	}
	std::shared_ptr<value> or_bool_bool(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_bool();
		auto r = right->as_bool();
		return std::make_shared<value>(l || r);
	}
	std::shared_ptr<value> and_bool_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_bool();
		if (l)
		{
			auto r = right->data<codedata>();
			r->loadinto(vm, vm->stack());
			return std::shared_ptr<value>();
		}
		else
		{
			return std::make_shared<value>(false);
		}
	}
	std::shared_ptr<value> or_bool_code(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_bool();
		if (l)
		{
			return std::make_shared<value>(true);
		}
		else
		{
			auto r = right->data<codedata>();
			r->loadinto(vm, vm->stack());
			return std::shared_ptr<value>();
		}
	}

	std::shared_ptr<value> greaterthen_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_double();
		auto r = right->as_double();
		return std::make_shared<value>(l > r);
	}
	std::shared_ptr<value> greaterthenorequal_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_double();
		auto r = right->as_double();
		return std::make_shared<value>(l >= r);
	}
	std::shared_ptr<value> lessthen_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_double();
		auto r = right->as_double();
		return std::make_shared<value>(l < r);
	}
	std::shared_ptr<value> lessthenorequal_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_double();
		auto r = right->as_double();
		return std::make_shared<value>(l <= r);
	}
	std::shared_ptr<value> equals_any_any(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(left->equals(right));
	}
	std::shared_ptr<value> notequals_any_any(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(!left->equals(right));
	}
	std::shared_ptr<value> isequalto_any_any(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		if (left->dtype() == type::STRING && left->dtype() == right->dtype())
		{
			return std::make_shared<value>(left->as_string() == right->as_string());
		}
		else
		{
			return std::make_shared<value>(left->equals(right));
		}
	}
	std::shared_ptr<value> isequaltype_any_any(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(left->dtype() == right->dtype());
	}

	std::shared_ptr<value> true_(virtualmachine* vm)
	{
		return std::make_shared<value>(true);
	}
	std::shared_ptr<value> false_(virtualmachine* vm)
	{
		return std::make_shared<value>(false);
	}
}

void sqf::commandmap::initlogiccmds()
{
	add(nular("true", "Always true.", true_));
	add(nular("false", "Always false.", false_));

	add(binary(2, "&&", sqf::type::BOOL, sqf::type::BOOL, "Returns true only if both conditions are true. Both sides are always evaluated.", and_bool_bool));
	add(binary(2, "and", sqf::type::BOOL, sqf::type::BOOL, "Returns true only if both conditions are true. Both sides are always evaluated.", and_bool_bool));
	add(binary(1, "||", sqf::type::BOOL, sqf::type::BOOL, "Returns true only if one or both conditions are true. Both sides are always evaluated.", or_bool_bool));
	add(binary(1, "or", sqf::type::BOOL, sqf::type::BOOL, "Returns true only if one or both conditions are true. Both sides are always evaluated.", or_bool_bool));
	add(binary(2, "&&", sqf::type::BOOL, sqf::type::CODE, "Returns true only if both conditions are true. Left side is always evaluated. Right side only will get evaluated if left side evaluates to true.", and_bool_code));
	add(binary(2, "and", sqf::type::BOOL, sqf::type::CODE, "Returns true only if both conditions are true. Left side is always evaluated. Right side only will get evaluated if left side evaluates to true.", and_bool_code));
	add(binary(1, "||", sqf::type::BOOL, sqf::type::CODE, "Returns true only if both conditions are true. Left side is always evaluated. Right side only will get evaluated if left side evaluates to false.", or_bool_code));
	add(binary(1, "or", sqf::type::BOOL, sqf::type::CODE, "Returns true only if both conditions are true. Left side is always evaluated. Right side only will get evaluated if left side evaluates to false.", or_bool_code));

	add(binary(3, ">", sqf::type::SCALAR, sqf::type::SCALAR, "Returns true if a is greater than b, else returns false.", greaterthen_scalar_scalar));
	add(binary(3, ">=", sqf::type::SCALAR, sqf::type::SCALAR, "Returns true if a is greater than or equal to b, else returns false.", greaterthenorequal_scalar_scalar));
	add(binary(3, "<", sqf::type::SCALAR, sqf::type::SCALAR, "Returns true if a is less than b, else returns false.", lessthen_scalar_scalar));
	add(binary(3, "<=", sqf::type::SCALAR, sqf::type::SCALAR, "Returns true if a is less than or equal to b, else returns false.", lessthenorequal_scalar_scalar));

	add(binary(3, "==", sqf::type::SCALAR, sqf::type::SCALAR, "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	add(binary(3, "==", sqf::type::SIDE, sqf::type::SIDE, "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	add(binary(3, "==", sqf::type::STRING, sqf::type::STRING, "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	add(binary(3, "==", sqf::type::OBJECT, sqf::type::OBJECT, "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	add(binary(3, "==", sqf::type::GROUP, sqf::type::GROUP, "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	add(binary(3, "==", sqf::type::TEXT, sqf::type::TEXT, "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	add(binary(3, "==", sqf::type::CONFIG, sqf::type::CONFIG, "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	add(binary(3, "==", sqf::type::DISPLAY, sqf::type::DISPLAY, "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	add(binary(3, "==", sqf::type::CONTROL, sqf::type::CONTROL, "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	add(binary(3, "==", sqf::type::LOCATION, sqf::type::LOCATION, "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	add(binary(3, "!=", sqf::type::ANY, sqf::type::ANY, "Returns whether one value is not equal to another.", notequals_any_any));
	add(binary(3, "!=", sqf::type::SIDE, sqf::type::SIDE, "Returns whether one value is not equal to another.", notequals_any_any));
	add(binary(3, "!=", sqf::type::STRING, sqf::type::STRING, "Returns whether one value is not equal to another.", notequals_any_any));
	add(binary(3, "!=", sqf::type::OBJECT, sqf::type::OBJECT, "Returns whether one value is not equal to another.", notequals_any_any));
	add(binary(3, "!=", sqf::type::GROUP, sqf::type::GROUP, "Returns whether one value is not equal to another.", notequals_any_any));
	add(binary(3, "!=", sqf::type::TEXT, sqf::type::TEXT, "Returns whether one value is not equal to another.", notequals_any_any));
	add(binary(3, "!=", sqf::type::CONFIG, sqf::type::CONFIG, "Returns whether one value is not equal to another.", notequals_any_any));
	add(binary(3, "!=", sqf::type::DISPLAY, sqf::type::DISPLAY, "Returns whether one value is not equal to another.", notequals_any_any));
	add(binary(3, "!=", sqf::type::CONTROL, sqf::type::CONTROL, "Returns whether one value is not equal to another.", notequals_any_any));
	add(binary(3, "!=", sqf::type::LOCATION, sqf::type::LOCATION, "Returns whether one value is not equal to another.", notequals_any_any));
	add(binary(4, "isEqualTo", sqf::type::ANY, sqf::type::ANY, "Check if one value is equal to another. Both values need to be of the same type.", isequalto_any_any));
	add(binary(4, "isEqualType", sqf::type::ANY, sqf::type::ANY, "Compares 2 values by their type. A much faster alternative to typeName a == typeName b.", isequaltype_any_any));
}
