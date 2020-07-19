#include "ops_logic.h"
#include "../runtime/value.h"
#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"

#include "../runtime/d_scalar.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_code.h"

namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;

namespace
{
	value and_bool_bool(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_boolean, bool>();
		auto r = right.data<d_boolean, bool>();
		return l && r;
	}
	value or_bool_bool(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_boolean, bool>();
		auto r = right.data<d_boolean, bool>();
		return l || r;
	}
	value and_bool_code(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_boolean, bool>();
		if (l)
		{
			frame f(runtime.default_value_scope(), right.data<d_code, instruction_set>());
			runtime.active_context().push_frame(f);
			return {};
		}
		else
		{
			return false;
		}
	}
	value or_bool_code(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_boolean, bool>();
		if (l)
		{
			return true;
		}
		else
		{
			frame f(runtime.default_value_scope(), right.data<d_code, instruction_set>());
			runtime.active_context().push_frame(f);
			return {};
		}
	}

	value greaterthen_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_scalar, float>();
		auto r = right.data<d_scalar, float>();
		return l > r;
	}
	value greaterthenorequal_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_scalar, float>();
		auto r = right.data<d_scalar, float>();
		return l >= r;
	}
    value lessthen_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_scalar, float>();
		auto r = right.data<d_scalar, float>();
		return l < r;
	}
    value lessthenorequal_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_scalar, float>();
		auto r = right.data<d_scalar, float>();
		return l <= r;
	}
	value equals_any_any(runtime& runtime, value::cref left, value::cref right)
	{
		return left.data()->equals(right.data(), true);
	}
	value notequals_any_any(runtime& runtime, value::cref left, value::cref right)
	{
		return !left.data()->equals(right.data(), true);
	}
	value isequalto_any_any(runtime& runtime, value::cref left, value::cref right)
	{
		return left == right;
	}
	value isequaltype_any_any(runtime& runtime, value::cref left, value::cref right)
	{
		return left.data()->type() == right.data()->type();
	}

	value true_(runtime& runtime)
	{
		return true;
	}
	value false_(runtime& runtime)
	{
		return false;
	}
}


#include "../runtime/d_string.h"
#include "d_config.h"
#include "d_group.h"
#include "d_side.h"
#include "d_object.h"

void sqf::operators::ops_logic(sqf::runtime::runtime& runtime)
{
	using namespace sqf::runtime::sqfop;
	runtime.register_sqfop(nular("true", "Always true.", true_));
	runtime.register_sqfop(nular("false", "Always false.", false_));

	runtime.register_sqfop(binary(2, "&&", t_boolean(), t_boolean(), "Returns true only if both conditions are true. Both sides are always evaluated.", and_bool_bool));
	runtime.register_sqfop(binary(2, "and", t_boolean(), t_boolean(), "Returns true only if both conditions are true. Both sides are always evaluated.", and_bool_bool));
	runtime.register_sqfop(binary(1, "||", t_boolean(), t_boolean(), "Returns true only if one or both conditions are true. Both sides are always evaluated.", or_bool_bool));
	runtime.register_sqfop(binary(1, "or", t_boolean(), t_boolean(), "Returns true only if one or both conditions are true. Both sides are always evaluated.", or_bool_bool));
	runtime.register_sqfop(binary(2, "&&", t_boolean(), t_code(), "Returns true only if both conditions are true. Left side is always evaluated. Right side only will get evaluated if left side evaluates to true.", and_bool_code));
	runtime.register_sqfop(binary(2, "and", t_boolean(), t_code(), "Returns true only if both conditions are true. Left side is always evaluated. Right side only will get evaluated if left side evaluates to true.", and_bool_code));
	runtime.register_sqfop(binary(1, "||", t_boolean(), t_code(), "Returns true only if both conditions are true. Left side is always evaluated. Right side only will get evaluated if left side evaluates to false.", or_bool_code));
	runtime.register_sqfop(binary(1, "or", t_boolean(), t_code(), "Returns true only if both conditions are true. Left side is always evaluated. Right side only will get evaluated if left side evaluates to false.", or_bool_code));

	runtime.register_sqfop(binary(3, ">", t_scalar(), t_scalar(), "Returns true if a is greater than b, else returns false.", greaterthen_scalar_scalar));
	runtime.register_sqfop(binary(3, ">=", t_scalar(), t_scalar(), "Returns true if a is greater than or equal to b, else returns false.", greaterthenorequal_scalar_scalar));
	runtime.register_sqfop(binary(3, "<", t_scalar(), t_scalar(), "Returns true if a is less than b, else returns false.", lessthen_scalar_scalar));
	runtime.register_sqfop(binary(3, "<=", t_scalar(), t_scalar(), "Returns true if a is less than or equal to b, else returns false.", lessthenorequal_scalar_scalar));

	runtime.register_sqfop(binary(3, "==", t_scalar(), t_scalar(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	runtime.register_sqfop(binary(3, "==", t_side(), t_side(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	runtime.register_sqfop(binary(3, "==", t_string(), t_string(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	runtime.register_sqfop(binary(3, "==", t_object(), t_object(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	runtime.register_sqfop(binary(3, "==", t_group(), t_group(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	runtime.register_sqfop(binary(3, "==", t_text(), t_text(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	runtime.register_sqfop(binary(3, "==", t_config(), t_config(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	runtime.register_sqfop(binary(3, "==", t_display(), t_display(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	runtime.register_sqfop(binary(3, "==", t_control(), t_control(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	runtime.register_sqfop(binary(3, "==", t_location(), t_location(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	runtime.register_sqfop(binary(3, "==", t_boolean(), t_boolean(), "Check if one value is equal to another. Both values need to be of the same type.", equals_any_any));
	
	runtime.register_sqfop(binary(3, "!=", t_scalar(), t_scalar(), "Returns whether one value is not equal to another.", notequals_any_any));
	runtime.register_sqfop(binary(3, "!=", t_side(), t_side(), "Returns whether one value is not equal to another.", notequals_any_any));
	runtime.register_sqfop(binary(3, "!=", t_string(), t_string(), "Returns whether one value is not equal to another.", notequals_any_any));
	runtime.register_sqfop(binary(3, "!=", t_object(), t_object(), "Returns whether one value is not equal to another.", notequals_any_any));
	runtime.register_sqfop(binary(3, "!=", t_group(), t_group(), "Returns whether one value is not equal to another.", notequals_any_any));
	runtime.register_sqfop(binary(3, "!=", t_text(), t_text(), "Returns whether one value is not equal to another.", notequals_any_any));
	runtime.register_sqfop(binary(3, "!=", t_config(), t_config(), "Returns whether one value is not equal to another.", notequals_any_any));
	runtime.register_sqfop(binary(3, "!=", t_display(), t_display(), "Returns whether one value is not equal to another.", notequals_any_any));
	runtime.register_sqfop(binary(3, "!=", t_control(), t_control(), "Returns whether one value is not equal to another.", notequals_any_any));
	runtime.register_sqfop(binary(3, "!=", t_location(), t_location(), "Returns whether one value is not equal to another.", notequals_any_any));
	runtime.register_sqfop(binary(4, "isEqualTo", t_any(), t_any(), "Check if one value is equal to another. Both values need to be of the same type.", isequalto_any_any));
	runtime.register_sqfop(binary(4, "isEqualType", t_any(), t_any(), "Compares 2 values by their type. A much faster alternative to typeName a == typeName b.", isequaltype_any_any));
}
