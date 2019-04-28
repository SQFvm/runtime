#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include "../codedata.h"

using namespace sqf;
namespace
{
	value and_bool_bool(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_bool();
		auto r = right.as_bool();
		return l && r;
	}
	value or_bool_bool(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_bool();
		auto r = right.as_bool();
		return l || r;
	}
	value and_bool_code(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_bool();
		if (l)
		{
			auto r = right.data<codedata>();
			r->loadinto(vm, vm->active_vmstack());
			return {};
		}
		else
		{
			return false;
		}
	}
	value or_bool_code(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_bool();
		if (l)
		{
			return true;
		}
		else
		{
			auto r = right.data<codedata>();
			r->loadinto(vm, vm->active_vmstack());
			return {};
		}
	}

	value greaterthen_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_double();
		auto r = right.as_double();
		return l > r;
	}
	value greaterthenorequal_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_double();
		auto r = right.as_double();
		return l >= r;
	}
    value lessthen_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_double();
		auto r = right.as_double();
		return l < r;
	}
    value lessthenorequal_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_double();
		auto r = right.as_double();
		return l <= r;
	}
	value equals_any_any(virtualmachine* vm, value::cref left, value::cref right)
	{
		return left.equals(right);
	}
	value notequals_any_any(virtualmachine* vm, value::cref left, value::cref right)
	{
		return !left.equals(right);
	}
	value isequalto_any_any(virtualmachine* vm, value::cref left, value::cref right)
	{
		if (left.dtype() == type::STRING && left.dtype() == right.dtype())
		{
			return left.as_string() == right.as_string();
		}
		else
		{
			return left.equals(right);
		}
	}
	value isequaltype_any_any(virtualmachine* vm, value::cref left, value::cref right)
	{
		return left.dtype() == right.dtype();
	}

	value true_(virtualmachine* vm)
	{
		return true;
	}
	value false_(virtualmachine* vm)
	{
		return false;
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
	
	add(binary(3, "!=", sqf::type::SCALAR, sqf::type::SCALAR, "Returns whether one value is not equal to another.", notequals_any_any));
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
