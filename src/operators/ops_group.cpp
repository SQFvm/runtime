#include "ops_group.h"
#include "../runtime/value.h"
#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../runtime/version.h"
#include "../runtime/sqfop.h"
#include "../runtime/util.h"
#include "../runtime/instruction_set.h"
#include "../runtime/d_string.h"
#include "../runtime/d_scalar.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_array.h"
#include "../runtime/d_code.h"

#include "dlops_storage.h"


#include "group.h"
#include "d_object.h"
#include "d_group.h"
#include "d_side.h"


#include <sstream>
#include <algorithm>


namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;
using namespace std::string_literals;

namespace
{
    value grpnull_(runtime& runtime)
    {
        return std::make_shared<d_group>();
    }
    value creategroup_side(runtime& runtime, value::cref right)
    {
        auto side = right.data<d_side>();
        auto grp = group::create(runtime);
        grp->side(side->value());
        return std::make_shared<d_group>(grp);
    }
    value groupid_group(runtime& runtime, value::cref right)
    {
        auto grp = right.data<d_group>();
        if (grp->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return "";
        }
        else
        {
            return grp->value()->group_id();
        }
    }
    value units_group(runtime& runtime, value::cref right)
    {
        auto grp = right.data<d_group>();
        if (grp->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return std::make_shared<d_array>();
        }
        else
        {
            return std::vector<value>(grp->value()->begin(), grp->value()->end());
        }
    }
    value deletegroup_group(runtime& runtime, value::cref right)
    {
        auto grp = right.data<d_group>();
        if (grp->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return std::make_shared<d_array>();
        }
        else if (grp->value()->empty())
        {
            return std::vector<value>(grp->value()->begin(), grp->value()->end());
        }
        else
        {
            runtime.__logmsg(logmessage::runtime::GroupNotEmpty(runtime.context_active().current_frame().diag_info_from_position(), std::string(grp->value()->group_id())));
            return {};
        }
    }
    value isnull_group(runtime& runtime, value::cref right)
    {
        auto grp = right.data<d_group>();
        return grp->is_null();
    }
    value side_group(runtime& runtime, value::cref right)
    {
        auto grp = right.data<d_group>();
        if (grp->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        else
        {
            return std::make_shared<d_side>(grp->value()->side());
        }
    }
    value selectleader_group_object(runtime& runtime, value::cref left, value::cref right)
    {
        auto grp = left.data<d_group>();
        if (grp->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto leader = right.data<d_object>();
        if (leader->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto grp_ = grp->value();
        auto res = std::find(grp_->begin(), grp_->end(), leader->value());
        if (res == grp_->end())
        {
            runtime.__logmsg(err::GroupLeaderNotPartOfGroup(runtime.context_active().current_frame().diag_info_from_position()));
        }
        grp_->leader(leader);
        return {};
    }
    value allvariables_group(runtime& runtime, value::cref right)
    {
        auto grp = right.data<d_group>();
        if (grp->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyArray(runtime.context_active().current_frame().diag_info_from_position()));
            return std::make_shared<d_array>();
        }
        auto scope = std::static_pointer_cast<value_scope>(grp->value());

        std::vector<value> variable_names;

        for (auto& kvp : *scope)
        {
            variable_names.push_back(kvp.first);
        }
        return variable_names;
    }
    value getVariable_group_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto grp = right.data<d_group>();
        if (grp->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto scope = std::static_pointer_cast<value_scope>(grp->value());
        auto variable = right.data<d_string, std::string>();
        
        auto res = scope->try_get(variable);
        if (res.has_value())
        {
            return *res;
        }
        return {};
    }
    value getVariable_group_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto grp = right.data<d_group>();
        if (grp->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto scope = std::static_pointer_cast<value_scope>(grp->value());
        auto r = right.data<d_array>();
        if (r->size() != 2)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, r->size()));
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (!r->at(0).is<t_string>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, t_string(), r->at(0).type()));
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        
        auto res = scope->try_get(r->at(0).data<d_string, std::string>());
        if (res.has_value())
        {
            return *res;
        }
        return r->at(1);
    }
    value setVariable_group_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto grp = right.data<d_group>();
        if (grp->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto scope = std::static_pointer_cast<value_scope>(grp->value());
        auto r = right.data<d_array>();
        if (r->size() != 2)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, r->size()));
            return {};
        }
        if (!r->at(0).is<t_string>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, t_string(), r->at(0).type()));
            return {};
        }

        scope->at(r->at(0).data<d_string, std::string>()) = r->at(1);
        return {};
    }
}
void sqf::operators::ops_group(sqf::runtime::runtime& runtime)
{
    using namespace sqf::runtime::sqfop;

    runtime.register_sqfop(nular("blufor", "Western side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::West)); }));
    runtime.register_sqfop(nular("west", "Western side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::West)); }));
    runtime.register_sqfop(nular("opfor", "Eastern side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::East)); }));
    runtime.register_sqfop(nular("east", "Eastern side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::East)); }));
    runtime.register_sqfop(nular("resistance", "Guerilla side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::Guerilla)); }));
    runtime.register_sqfop(nular("independent", "Guerilla side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::Guerilla)); }));
    runtime.register_sqfop(nular("civilian", "Civilian side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::Civilian)); }));
    runtime.register_sqfop(nular("sideEmpty", "Empty side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::Empty)); }));
    runtime.register_sqfop(nular("sideEnemy", "Enemy side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::Enemy)); }));
    runtime.register_sqfop(nular("sideFriendly", "Friendly side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::Friendly)); }));
    runtime.register_sqfop(nular("sideLogic", "Logic side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::Logic)); }));
    runtime.register_sqfop(nular("sideUnknown", "Unknown side.", [](::sqf::runtime::runtime& runtime) -> value { return value(std::make_shared<d_side>(d_side::side::Unknown)); }));

    runtime.register_sqfop(nular("grpNull", "A non-existing Group. To compare non-existent groups use isNull or isEqualTo.", grpnull_));
    runtime.register_sqfop(unary("createGroup", t_side(), "Creates a new Group for the given Side.", creategroup_side));
    runtime.register_sqfop(unary("groupId", t_group(), "Returns group name.", groupid_group));
    runtime.register_sqfop(unary("units", t_group(), "Returns an array with all the units in the group.", units_group));
    runtime.register_sqfop(unary("deleteGroup", t_group(), "Destroys the given group. Group must be empty.", deletegroup_group));
    runtime.register_sqfop(unary("isNull", t_group(), "Checks whether the tested item is Null.", isnull_group));
    runtime.register_sqfop(unary("side", t_group(), "Returns the side of a group.", side_group));
    runtime.register_sqfop(binary(4, "selectLeader", t_group(), t_object(), "Select the group's leader.", selectleader_group_object));



    runtime.register_sqfop(unary("allVariables", t_group(), "Returns a list of all variables from desired namespace.", allvariables_group));
    runtime.register_sqfop(binary(4, "getVariable", t_group(), t_string(), "Return the value of variable in the variable space assigned to various data types. Returns nil if variable is undefined.", getVariable_group_string));
    runtime.register_sqfop(binary(4, "getVariable", t_group(), t_array(), "Return the value of variable in the provided variable space. First element is expected to be the variable name as string. Returns second array item if variable is undefined.", getVariable_group_array));
    runtime.register_sqfop(binary(4, "setVariable", t_group(), t_array(), "Sets a variable to given value in the provided variable space. First element is expected to be the variable name as string. Second element is expected to be anything.", setVariable_group_array));
}