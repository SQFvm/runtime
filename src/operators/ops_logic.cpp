#include "ops_logic.h"
#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"

#include "../runtime/d_scalar.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_code.h"
#include "../runtime/d_array.h"
#include "d_text.h"

#include "../runtime/value.h"

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
            runtime.context_active().push_frame(f);
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
            runtime.context_active().push_frame(f);
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
        if (left.empty() && right.empty())
        {
            return false;
        }
        return left == right;
    }
    value isnotequalto_any_any(runtime& runtime, value::cref left, value::cref right)
    {
        if (left.empty() && right.empty())
        {
            return true;
        }
        return left != right;
    }
    value isequaltype_any_any(runtime& runtime, value::cref left, value::cref right)
    {
        return left.type() == right.type();
    }
    value isequaltypeall_array_any(runtime& runtime, value::cref left, value::cref right)
    {
        auto type = right.type();
        auto arr = left.data<d_array>();

        if (arr->size() == 0)
        {
            return false;
        }

        for (const auto &item : arr->value()) {
            if (item.type() != type) {
                return false;
            }
        }

        return true;
    }
    value isequaltypeany_any_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto type = left.type();
        auto arr = right.data<d_array>();

        for (const auto &item : arr->value()) {
            if (item.type() == type) {
                return true;
            }
        }

        return false;
    }
    value isequaltypearray_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_array>();
        auto r = right.data<d_array>();

        return std::equal(l->value().begin(), l->value().end(), r->value().begin(), r->value().end(),
                          [](value left, value right) { return left.type() == right.type(); });
    }
    value isequaltypeparams_any_array(runtime& runtime, value::cref left, value::cref right)
    {
        if (!left.is(t_array())) {
            return false;
        }

        auto l = left.data<d_array>();
        auto r = right.data<d_array>();

        // Early exit if input does not cover all templates
        if (l->size() < r->size())
        {
            return false;
        }

        // Only input which is defined in templates needs to be validated
        for (size_t i = 0; i < r->size(); i++)
        {
            auto l_value = l->at(i);
            auto r_template = r->at(i);

            // Template which is nil accepts all types
            if (r_template.empty()) {
                continue;
            }

            if (l_value.type() != r_template.type())
            {
                return false;
            }
        }

        return true;
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
    runtime.register_sqfop(binary(4, "isNotEqualTo", t_any(), t_any(), "Check if one value is not equal to another. Both values need to be of the same type.", isnotequalto_any_any));
    runtime.register_sqfop(binary(4, "isEqualType", t_any(), t_any(), "Compares 2 values by their type. A much faster alternative to typeName a == typeName b.", isequaltype_any_any));
    runtime.register_sqfop(binary(4, "isEqualTypeAll", t_array(), t_any(), "Compares types of all elements of an array to the type of a single value.", isequaltypeall_array_any));
    runtime.register_sqfop(binary(4, "isEqualTypeAny", t_any(), t_array(), "Compares type of given value to every type in the given array and if match is found, true is returned.", isequaltypeany_any_array));
    runtime.register_sqfop(binary(4, "isEqualTypeArray", t_array(), t_array(), "Compares types of all elements of one array to types of all elements of another array.", isequaltypearray_array_array));
    runtime.register_sqfop(binary(4, "isEqualTypeParams", t_any(), t_array(), "Compares types of all elements of input array to types of all elements of template array.", isequaltypeparams_any_array));
}
