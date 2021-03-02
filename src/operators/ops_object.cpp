#include "ops_object.h"
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
#include "d_config.h"

#include "dlops_storage.h"


#include "object.h"
#include "d_object.h"
#include "group.h"
#include "d_group.h"
#include "d_side.h"


#include <cstdlib>
#include <algorithm>


namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;
using namespace std::string_literals;

namespace
{
    value objnull_(runtime& runtime)
    {
        return value(std::make_shared<d_object>());
    }
    value typeof_object(runtime& runtime, value::cref right)
    {
        auto obj = right.data<d_object>();
        if (obj->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return "";
        }
        else
        {
            return obj->value()->config().name();
        }
    }
    value createvehicle_array(runtime& runtime, value::cref right)
    {
        auto arr = right.data<d_array>();
        if (!arr->check_type(runtime, std::array<sqf::runtime::type, 5>{ t_string(), t_array(), t_array(), t_string(), }))
        {
            return {};
        }
        auto type = arr->at(0).data<d_string, std::string>();
        auto position = arr->at(1).data<d_array>();
        if (!position->check_type(runtime, t_scalar(), 3))
        {
            return {};
        }
        auto radius = arr->at(3).data<d_scalar, float>();
        config conf;
        if (runtime.configuration().enable_classname_check)
        {
            auto configBin = runtime.confighost().root();

            auto cfgVehicles = configBin / "CfgVehicles";
            if (cfgVehicles.empty())
            {
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), std::array<std::string, 2> { "ConfigBin" }, "CfgVehicles"));
                return {};
            }

            auto opt = cfgVehicles / type;
            if (opt.empty())
            {
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), std::array<std::string, 2> { "ConfigBin", "CfgVehicles" }, type));
                return {};
            }
            else
            {
                conf = *opt;
            }
        }
        auto veh = object::create(runtime, conf, true);
        veh->position({
            position->at(0).data<d_scalar, float>() + ((std::rand() % static_cast<int>(radius * 2)) - radius),
            position->at(1).data<d_scalar, float>() + ((std::rand() % static_cast<int>(radius * 2)) - radius),
            position->at(2).data<d_scalar, float>()
            });
        return std::make_shared<d_object>(veh);
    }
    value createvehicle_string_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto type = left.data<d_string, std::string>();
        auto position = right.data<d_array>();
        if (!position->check_type(runtime, t_scalar(), 3))
        {
            return {};
        }
        if (!position->check_type(runtime, t_scalar(), 3))
        {
            return {};
        }
        config conf;
        if (runtime.configuration().enable_classname_check)
        {
            auto configBin = runtime.confighost().root();

            auto cfgVehicles = configBin / "CfgVehicles";
            if (cfgVehicles.empty())
            {
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), std::array<std::string, 2> { "ConfigBin" }, "CfgVehicles"));
                return {};
            }

            auto opt = cfgVehicles / type;
            if (opt.empty())
            {
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), std::array<std::string, 2> { "ConfigBin", "CfgVehicles" }, type));
                return {};
            }
            else
            {
                conf = *opt;
            }
        }
        auto veh = object::create(runtime, conf, true);
        veh->position({
            position->at(0).data<d_scalar, float>(),
            position->at(1).data<d_scalar, float>(),
            position->at(2).data<d_scalar, float>()
            });
        return std::make_shared<d_object>(veh);
    }

    value deletevehicle_array(runtime& runtime, value::cref right)
    {
        auto veh = right.data<d_object>();
        if (veh->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        veh->value()->destroy(runtime);
        return {};
    }
    value position_object(runtime& runtime, value::cref right)
    {
        auto veh = right.data<d_object>();
        if (veh->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto pos = veh->value()->position();
        auto arr = std::make_shared<d_array>();
        arr->push_back(pos.x);
        arr->push_back(pos.y);
        arr->push_back(pos.z);
        return value(arr);
    }
    value setpos_object_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto veh = left.data<d_object>();
        if (veh->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto position = right.data<d_array>();
        position->check_type(runtime, t_scalar(), 3);
        auto inner = veh->value();
        inner->position({
            position->at(0).data<d_scalar, float>(),
            position->at(1).data<d_scalar, float>(),
            position->at(2).data<d_scalar, float>()
            });
        return {};
    }
    value velocity_object(runtime& runtime, value::cref right)
    {
        auto veh = right.data<d_object>();
        if (veh->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto vel = veh->value()->velocity();
        auto arr = std::make_shared<d_array>();
        arr->push_back(vel.x);
        arr->push_back(vel.y);
        arr->push_back(vel.z);
        return value(arr);
    }
    value setvelocity_object_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto veh = left.data<d_object>();
        if (veh->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto velocity = right.data<d_array>();
        velocity->check_type(runtime, t_scalar(), 3);
        auto inner = veh->value();
        inner->velocity({
            velocity->at(0).data<d_scalar, float>(),
            velocity->at(1).data<d_scalar, float>(),
            velocity->at(2).data<d_scalar, float>()
            });
        return {};
    }
    value domove_object_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto obj = left.data<d_object>()->value();
        if (obj->is_vehicle())
        {
            runtime.__logmsg(err::ExpectedUnit(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        setpos_object_array(runtime, left, right);
        return {};
    }
    value domove_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto arr = left.data<d_array>();
        bool errflag = false;
        for (size_t i = 0; i < arr->size(); i++)
        {
            if (!arr->at(i).is<t_object>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), i, t_object(), arr->at(i).type()));
                errflag = true;
            }
            else if (arr->at(i).data<d_object>()->value()->is_vehicle())
            {
                runtime.__logmsg(err::ExpectedUnit(runtime.context_active().current_frame().diag_info_from_position()));
                errflag = true;
            }
        }
        if (errflag)
        {
            return {};
        }
        for (const auto& i : *arr)
        {
            setpos_object_array(runtime, i, right);
        }
        return {};
    }

    value createUnit_group_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto grp = left.data<d_group>();
        auto arr = right.data<d_array>();
        
        if (arr->check_type(runtime, std::array<sqf::runtime::type, 5> { t_string(), t_array(), t_array(), t_scalar(), t_string() }))
        {
            return {};
        }
        auto type = arr->at(0).data<d_string, std::string>();
        auto position = arr->at(1).data<d_array>();
        if (!position->check_type(runtime, t_scalar(), 3))
        {
            return {};
        }
        auto radius = arr->at(3).data<d_scalar, float>();
        config conf;
        if (runtime.configuration().enable_classname_check)
        {
            auto configBin = runtime.confighost().root();

            auto cfgVehicles = configBin / "CfgVehicles";
            if (cfgVehicles.empty())
            {
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), std::array<std::string, 2> { "ConfigBin" }, "CfgVehicles"));
                return {};
            }

            auto opt = cfgVehicles / type;
            if (opt.empty())
            {
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), std::array<std::string, 2> { "ConfigBin", "CfgVehicles" }, type));
                return {};
            }
            else
            {
                conf = *opt;
            }
        }
        auto veh = object::create(runtime, conf, false);
        veh->position({
            position->at(0).data<d_scalar, float>() + ((std::rand() % static_cast<int>(radius * 2)) - radius),
            position->at(1).data<d_scalar, float>() + ((std::rand() % static_cast<int>(radius * 2)) - radius),
            position->at(2).data<d_scalar, float>()
            });
        return std::make_shared<d_object>(veh);
    }
    value createUnit_string_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto type = left.data<d_string, std::string>();
        auto arr = right.data<d_array>();
        double skill = 0.5;
        std::string rank = "PRIVATE";

        if (arr->size() < 2)
        {
            runtime.__logmsg(err::ExpectedMinimumArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, arr->size()));
            return {};
        }
        // Position
        if (!arr->at(0).is< t_array>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 0, t_array(), arr->at(0).type()));
            return {};
        }
        auto position = arr->at(0).data<d_array>();
        if (!position->check_type(runtime, t_scalar(), 3))
        {
            return {};
        }
        // Group
        if (!arr->at(1).is<t_group>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 1, t_group(), arr->at(1).type()));
            return {};
        }
        auto grp = arr->at(1).data<d_group>();

        //Optionals
        //init
        if (arr->size() >= 3)
        {
            if (!arr->at(2).is< t_string>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, t_string(), arr->at(2).type()));
                return {};
            }
            else
            {
                std::string init = arr->at(2).data<d_string, std::string>();
            }
        }
        //skill
        if (arr->size() >= 4)
        {
            if (!arr->at(3).is< t_scalar>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 3, t_scalar(), arr->at(3).type()));
                return {};
            }
            else
            {
                skill = arr->at(3).data<d_scalar, float>();
            }
        }
        //rank
        if (arr->size() >= 5)
        {
            if (!arr->at(4).is<t_string>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 4, t_string(), arr->at(4).type()));
                return {};
            }
            else
            {
                rank = arr->at(4).data<d_string, std::string>();
            }
        }
        config conf;
        if (runtime.configuration().enable_classname_check)
        {
            auto configBin = runtime.confighost().root();

            auto cfgVehicles = configBin / "CfgVehicles";
            if (cfgVehicles.empty())
            {
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), std::array<std::string, 2> { "ConfigBin" }, "CfgVehicles"));
                return {};
            }

            auto opt = cfgVehicles / type;
            if (opt.empty())
            {
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), std::array<std::string, 2> { "ConfigBin", "CfgVehicles" }, type));
                return {};
            }
            else
            {
                conf = *opt;
            }
        }
        auto veh = object::create(runtime, conf, false);
        auto obj = std::make_shared<d_object>(veh);
        if (!grp->is_null())
        {
            grp->value()->push_back(obj);
        }

        veh->position({
            position->at(0).data<d_scalar, float>(),
            position->at(1).data<d_scalar, float>(),
            position->at(2).data<d_scalar, float>()
            });
        return obj;
    }
    value distance_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_array>();
        auto r = right.data<d_array>();
        if (!l->check_type(runtime, t_scalar(), 2, 3) || !r->check_type(runtime, t_scalar(), 2, 3))
        {
            return {};
        }
        return distance3d(l, r);
    }
    value distance_object_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_object>();
        auto r = right.data<d_array>();
        if (l->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (!r->check_type(runtime, t_scalar(), 2, 3))
        {
            return {};
        }
        return l->value()->distance3d(*r);
    }
    value distance_array_object(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_array>();
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (!l->check_type(runtime, t_scalar(), 2, 3))
        {
            return {};
        }
        return r->value()->distance3d(*l);
    }
    value distance_object_object(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_object>();
        auto r = right.data<d_object>();
        if (l->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        return l->value()->distance3d(r->value());
    }
    value distance2d_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_array>();
        auto r = right.data<d_array>();
        if (!l->check_type(runtime, t_scalar(), 2, 3) || !r->check_type(runtime, t_scalar(), 2, 3))
        {
            return {};
        }
        return distance2d(l, r);
    }
    value distance2d_object_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_object>();
        auto r = right.data<d_array>();
        if (l->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (!r->check_type(runtime, t_scalar(), 2, 3))
        {
            return {};
        }
        return l->value()->distance2d(*r);
    }
    value distance2d_array_object(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_array>();
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (!l->check_type(runtime, t_scalar(), 2, 3))
        {
            return {};
        }
        return r->value()->distance2d(*l);
    }
    value distance2d_object_object(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_object>();
        auto r = right.data<d_object>();
        if (l->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        return l->value()->distance2d(r->value());
    }
    class nearestobjects_distancesort3d
    {
        std::array<float, 3> pos;
    public:
        nearestobjects_distancesort3d(std::array<float, 3> p) : pos(p) {}
        nearestobjects_distancesort3d(vec3 p) : pos({ p.x, p.y, p.z }) {}
        bool operator() (value::cref l, value::cref r) const { return l.data<d_object>()->value()->distance3d(pos) < r.data<d_object>()->value()->distance3d(pos); }
    };
    class nearestobjects_distancesort2d
    {
        std::array<float, 2> pos;
    public:
        nearestobjects_distancesort2d(std::array<float, 2> p) : pos(p) {}
        nearestobjects_distancesort2d(vec3 p) : pos({ p.x, p.y }) {}
        bool operator() (value::cref l, value::cref r) const { return l.data<d_object>()->value()->distance2d(pos) < r.data<d_object>()->value()->distance2d(pos); }
    };
    value nearestobjects_array(runtime& runtime, value::cref right)
    {
        auto arr = right.data<d_array>();
        if (arr->size() != 3 && arr->size() != 4)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 3, 4, arr->size()));
            return {};
        }
        vec3 position {0, 0, 0};
        if (arr->at(0).is<t_array>())
        {
            if (!arr->at(0).data<d_array>()->check_type(runtime, t_scalar(), 3))
            {
                return {};
            }
            position = *arr->at(0).data<d_array>();
        }
        else if (arr->at(0).is<t_object>())
        {
            if (arr->at(0).data<d_object>()->is_null())
            {
                runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
                return {};
            }
            position = arr->at(0).data<d_object>()->value()->position();
        }
        else
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 0, std::array<sqf::runtime::type, 2> { t_array(), t_object() }, arr->at(0).type()));
            return {};
        }
        if (!arr->at(1).is<t_array>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 1, t_array(), arr->at(1).type()));
            return {};
        }
        auto filterarr = arr->at(1).data<d_array>();
        for (size_t i = 0; i < filterarr->size(); i++)
        {
            if (!filterarr->at(i).is< t_string>())
                runtime.__logmsg(err::ExpectedSubArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), std::array<size_t, 2> { 1, i }, t_string(), filterarr->at(i).type()));
            {
                return {};
            }
        }
        if (!arr->at(2).is< t_scalar>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, t_scalar(), arr->at(2).type()));
            return {};
        }
        auto radius = arr->at(2).data<d_scalar, float>();
        auto is2ddistance = false;
        if (arr->size() == 4)
        {
            if (!arr->at(3).is<t_boolean>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 4, t_boolean(), arr->at(3).type()));
                return {};
            }
            is2ddistance = arr->at(3).data<d_boolean, bool>();
        }
        auto outputarr = std::make_shared<d_array>();
        if (is2ddistance)
        {
            std::array<float, 2> position2d{ position.x, position.y };
            for (auto& object : runtime.storage<object::object_storage>())
            {
                if (object->distance2d(position2d) > radius) continue;

                bool match = filterarr->empty() || !runtime.configuration().enable_classname_check;
                if (!match)
                {
                    auto cfgObject = object->config().navigate(runtime.confighost());

                    if (!cfgObject.empty())
                    {
                        auto found = std::find_if(filterarr->begin(), filterarr->end(), [&cfgObject](value::cref value) {
                            return cfgObject.has_inherited_with_name(value.data<d_string, std::string>());
                        });
                        match = found != filterarr->end();
                    }
                }
                if (match)
                {
                    outputarr->push_back(value(std::make_shared<d_object>(object)));
                }
            }
            std::sort(outputarr->begin(), outputarr->end(), nearestobjects_distancesort2d(position2d));
        }
        else
        {
            for (auto& object : runtime.storage<object::object_storage>())
            {
                if (object->distance3d(position) > radius) continue;
                
                bool match = filterarr->empty() || !runtime.configuration().enable_classname_check;
                if (!match)
                {
                    auto cfgObject = object->config().navigate(runtime.confighost());
                    if (!cfgObject.empty())
                    {
                        auto found = std::find_if(filterarr->begin(), filterarr->end(), [&cfgObject](value::cref value) {
                            return cfgObject.has_inherited_with_name(value.data<d_string, std::string>());
                            });
                        match = found != filterarr->end();
                    }
                }
                if (match)
                {
                    outputarr->push_back(value(std::make_shared<d_object>(object)));
                }
            }
            std::sort(outputarr->begin(), outputarr->end(), nearestobjects_distancesort3d(position));
        }
        return value(outputarr);
    }
    value isnull_object(runtime& runtime, value::cref right)
    {
        auto obj = right.data<d_object>();
        return obj->is_null();
    }
    value side_object(runtime& runtime, value::cref right)
    {
        auto obj = right.data<d_object>();
        if (obj->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return std::make_shared<d_side>(d_side::side::Empty);
        }
        else
        {
            auto grp = obj->value()->group();
            if (grp->is_null())
            {
                runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
                return std::make_shared<d_side>(d_side::side::Empty);
            }
            else
            {
                return grp->value()->side();
            }
        }
    }
    value allunits_(runtime& runtime)
    {
        auto arr = std::make_shared<d_array>();
        for (auto& object : runtime.storage<object::object_storage>())
        {
            if (object->is_vehicle())
                continue;
            arr->push_back(value(std::make_shared<d_object>(object)));
        }
        return value(arr);
    }
    value iskindof_object_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto obj = left.data<d_object>();
        if (obj->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto base_type_str = right.data<d_string, std::string>();
        return obj->value()->config().navigate(runtime.confighost()).has_inherited_with_name(base_type_str);
    }
    value iskindof_string_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto test_type_str = left.data<d_string, std::string>();
        auto base_type_str = right.data<d_string, std::string>();
        {
            auto configBin = runtime.confighost().root();

            auto cfgVehicles = configBin / "CfgVehicles";
            if (cfgVehicles.empty())
            {
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), std::array<std::string, 2> { "ConfigBin" }, "CfgVehicles"));
                return {};
            }

            auto test_opt = cfgVehicles / test_type_str;
            if (test_opt.empty())
            {
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), std::array<std::string, 2> { "ConfigBin", "CfgVehicles" }, test_type_str));
                return {};
            }

            return test_opt.has_inherited_with_name(base_type_str);
        }
    }
    value iskindof_string_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto test_type_str = left.data<d_string, std::string>();
        auto arr = right.data<d_array>();
        if (!arr->check_type(runtime, std::array<sqf::runtime::type, 2>{ t_string(), t_config() }))
        {
            return {};
        }
        auto base_type_str = arr->at(0).data<d_string, std::string>();
        auto base_conf = arr->at(1).data<d_config, config>();

        {
            auto nav = base_conf.navigate(runtime.confighost());
            auto test_opt = nav / test_type_str;
            if (test_opt.empty())
            {
                // Get navigation path
                std::vector<std::string> path;
                do
                {
                    path.push_back(nav->name);
                    nav = nav.parent_logical();
                } while (nav->id_parent_logical != config::invalid_id);
                runtime.__logmsg(err::ConfigEntryNotFoundWeak(runtime.context_active().current_frame().diag_info_from_position(), path, test_type_str));
                return {};
            }

            return test_opt.has_inherited_with_name(base_type_str);
        }
    }
    value player_(runtime& runtime)
    {
        return std::make_shared<d_object>(runtime.storage<object::object_storage>().player());
    }

    value setdamage_object_scalar(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_object>();
        auto r = right.data<d_scalar, float>();
        if (l->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        l->value()->damage(r);
        return {};
    }
    value getdamage_object(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        return r->value()->damage();
    }
    value alive_object(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return false;
        }
        return r->value()->alive();
    }
    value crew_object(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto arr = std::make_shared<d_array>();
        auto obj = r->value();
        if (!obj->is_vehicle())
        {
            runtime.__logmsg(err::ExpectedVehicleWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyArray(runtime.context_active().current_frame().diag_info_from_position()));
            return value(arr);
        }
        if (!obj->driver()->is_null())
        {
            arr->push_back(value(obj->driver()));
        }
        if (!obj->gunner()->is_null())
        {
            arr->push_back(value(obj->gunner()));
        }
        if (!obj->commander()->is_null())
        {
            arr->push_back(value(obj->commander()));
        }
        for (auto& it : obj->soldiers())
        {
            arr->push_back(value(it));
        }
        return value(arr);
    }
    value vehicle_object(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto obj = r->value();
        if (!obj->is_vehicle())
        {
            runtime.__logmsg(err::ExpectedVehicleWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return right;
        }
        auto parent = obj->parent_object();
        if (parent->is_null() || !parent->value()->is_vehicle())
        {
            return right;
        }
        else
        {
            return value(parent);
        }
    }
    value objectparent_object(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        return value(r->value()->parent_object());
    }
    value driver_object(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto obj = r->value();
        if (!obj->is_vehicle())
        {
            runtime.__logmsg(err::ExpectedVehicleWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return right;
        }
        return value(obj->driver());
    }
    value commander_object(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto obj = r->value();
        if (!obj->is_vehicle())
        {
            runtime.__logmsg(err::ExpectedVehicleWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return right;
        }
        return value(obj->commander());
    }
    value gunner_object(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto obj = r->value();
        if (!obj->is_vehicle())
        {
            runtime.__logmsg(err::ExpectedVehicleWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return right;
        }
        return value(obj->gunner());
    }
    value in_object_object(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_object>();
        if (l->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (l->value()->is_vehicle())
        {
            runtime.__logmsg(err::ExpectedUnitWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningFalse(runtime.context_active().current_frame().diag_info_from_position()));
            return false;
        }
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (!r->value()->is_vehicle())
        {
            runtime.__logmsg(err::ExpectedUnitWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningFalse(runtime.context_active().current_frame().diag_info_from_position()));
            return false;
        }
        auto veh = r->value();
        auto unit = l->value();
        if (veh->driver()->value().get() == unit.get() || veh->commander()->value().get() == unit.get() || veh->gunner()->value().get() == unit.get())
        {
            return true;
        }
        else
        {
            auto res = std::find_if(veh->soldiers().begin(), veh->soldiers().end(), [unit](std::shared_ptr<d_object> data) -> bool {
                return data->value().get() == unit.get();
            });
            return res != veh->soldiers().end();
        }
    }
    value vehiclevarname_object(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_object>();
        if (r->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        return r->value()->varname();
    }
    value setvehiclevarname_object_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_object>();
        if (l->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValue(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto r = right.data<d_string, std::string>();
        l->value()->varname(r);
        return {};
    }
    value allvariables_object(runtime& runtime, value::cref right)
    {
        auto obj = right.data<d_object>();
        if (obj->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyArray(runtime.context_active().current_frame().diag_info_from_position()));
            return std::make_shared<d_array>();
        }
        auto scope = std::static_pointer_cast<value_scope>(obj->value());

        std::vector<value> variable_names;

        for (auto& kvp : *scope)
        {
            variable_names.push_back(kvp.first);
        }
        return variable_names;
    }
    value getVariable_object_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto obj = left.data<d_object>();
        if (obj->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto scope = std::static_pointer_cast<value_scope>(obj->value());
        auto variable = right.data<d_string, std::string>();

        auto res = scope->try_get(variable);
        if (res.has_value())
        {
            return *res;
        }
        return {};
    }
    value getVariable_object_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto obj = left.data<d_object>();
        if (obj->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto scope = std::static_pointer_cast<value_scope>(obj->value());
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
    value setVariable_object_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto obj = left.data<d_object>();
        if (obj->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto scope = std::static_pointer_cast<value_scope>(obj->value());
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
    value units_object(runtime& runtime, value::cref right)
    {
        auto obj = right.data<d_object>();
        if (obj->is_null())
        {
            runtime.__logmsg(err::ExpectedNonNullValueWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return std::make_shared<d_array>();
        }
        else
        {
            auto grp = obj->value()->group();
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
    }
}
void sqf::operators::ops_object(sqf::runtime::runtime& runtime)
{
    using namespace sqf::runtime::sqfop;

    runtime.register_sqfop(unary("units", t_object(), "Returns an array with all the units in the group of the unit. For a destroyed object an empty array is returned.", units_object));
    runtime.register_sqfop(nular("allUnits", "Return a list of all units (all persons except agents) outside and inside vehicles.", allunits_));
    runtime.register_sqfop(nular("objNull", "A non-existent Object. To compare non-existent objects use isNull or isEqualTo.", objnull_));
    runtime.register_sqfop(unary("typeOf", t_object(), "Returns the config class name of given object.", typeof_object));
    runtime.register_sqfop(unary("createVehicle", t_array(), "Creates an empty object of given classname type.", createvehicle_array));
    runtime.register_sqfop(binary(4, "createVehicle", t_string(), t_array(), "Creates an empty object of given classname type.", createvehicle_string_array));
    runtime.register_sqfop(binary(4, "createVehicleLocal", t_any(), t_any(), "Creates an empty object of given classname type.", createvehicle_string_array));
    runtime.register_sqfop(unary("deleteVehicle", t_object(), "Deletes an object.", deletevehicle_array));
    runtime.register_sqfop(unary("position", t_object(), "Returns the object position in format PositionAGLS. Z value is height over the surface underneath.", position_object));
    runtime.register_sqfop(unary("getPos", t_object(), "Returns the object position in format PositionAGLS. Z value is height over the surface underneath.", position_object));
    runtime.register_sqfop(binary(4, "setPos", t_object(), t_array(), "Sets object position.", setpos_object_array));
    runtime.register_sqfop(unary("velocity", t_object(), "Return velocity (speed vector) of Unit as an array with format [x, y, z].", velocity_object));
    runtime.register_sqfop(binary(4, "setVelocity", t_object(), t_array(), "Set velocity (speed vector) of a vehicle. Units are in metres per second.", setvelocity_object_array));
    runtime.register_sqfop(binary(4, "doMove", t_object(), t_array(), "Order the given unit(s) to move to the given position (without radio messages). In SQFVM this command acts like setPos.", domove_object_array));
    runtime.register_sqfop(binary(4, "doMove", t_array(), t_array(), "Order the given unit(s) to move to the given position (without radio messages). In SQFVM this command acts like setPos.", domove_array_array));
    runtime.register_sqfop(binary(4, "createUnit", t_group(), t_array(), "Create unit of a class that's defined in CfgVehicles.", createUnit_group_array));
    runtime.register_sqfop(binary(4, "createUnit", t_string(), t_array(), "Create unit of a class that's defined in CfgVehicles.", createUnit_string_array));
    runtime.register_sqfop(binary(4, "distance", t_array(), t_array(), "Returns a distance in meters between two positions.", distance_array_array));
    runtime.register_sqfop(binary(4, "distance", t_object(), t_array(), "Returns a distance in meters between two positions.", distance_object_array));
    runtime.register_sqfop(binary(4, "distance", t_array(), t_object(), "Returns a distance in meters between two positions.", distance_array_object));
    runtime.register_sqfop(binary(4, "distance", t_object(), t_object(), "Returns a distance in meters between two positions.", distance_object_object));
    runtime.register_sqfop(binary(4, "distance2d", t_array(), t_array(), "Returns a 2d distance in meters between two positions.", distance2d_array_array));
    runtime.register_sqfop(binary(4, "distance2d", t_object(), t_array(), "Returns a 2d distance in meters between two positions.", distance2d_object_array));
    runtime.register_sqfop(binary(4, "distance2d", t_array(), t_object(), "Returns a 2d distance in meters between two positions.", distance2d_array_object));
    runtime.register_sqfop(binary(4, "distance2d", t_object(), t_object(), "Returns a 2d distance in meters between two positions.", distance2d_object_object));
    runtime.register_sqfop(unary("nearestObjects", t_array(), "Returns a list of nearest objects of the given types to the given position or object, within the specified distance. If more than one object is found they will be ordered by proximity, the closest one will be first in the array.", nearestobjects_array));
    runtime.register_sqfop(unary("isNull", t_object(), "Checks whether the tested item is Null.", isnull_object));
    runtime.register_sqfop(unary("side", t_object(), "Returns the side of an object.", side_object));
    runtime.register_sqfop(binary(4, "isKindOf", t_object(), t_string(), "Checks whether the object is (a subtype) of the given type.", iskindof_object_string));
    runtime.register_sqfop(binary(4, "isKindOf", t_string(), t_string(), "Checks whether the object is (a subtype) of the given type. Checks CfgVehicles, CfgAmmo and CfgNonAiVehicles.", iskindof_string_string));
    runtime.register_sqfop(binary(4, "isKindOf", t_string(), t_array(), "Checks whether the object is (a subtype) of the given type.", iskindof_string_array));
    runtime.register_sqfop(nular("player", "Theoretical player object. Practically, just a normal object.", player_));
    runtime.register_sqfop(binary(4, "setDamage", t_object(), t_scalar(), "Damage / repair object. Damage 0 means fully functional, damage 1 means completely destroyed / dead.", setdamage_object_scalar));
    runtime.register_sqfop(unary("getDammage", t_object(), "Return the damage value of an object.", getdamage_object));
    runtime.register_sqfop(unary("damage", t_object(), "Return the damage value of an object.", getdamage_object));
    runtime.register_sqfop(unary("alive", t_object(), "Check if given vehicle/person/building is alive (i.e. not dead or destroyed). alive objNull returns false.", alive_object));
    runtime.register_sqfop(unary("crew", t_object(), "Returns the crew (both dead and alive) of the given vehicle.", crew_object));
    runtime.register_sqfop(unary("vehicle", t_object(), "Vehicle in which given unit is mounted. If none, unit is returned.", vehicle_object));
    runtime.register_sqfop(unary("objectParent", t_object(), "Returns parent of an object if the object is proxy, otherwise objNull.", objectparent_object));
    runtime.register_sqfop(unary("driver", t_object(), "Returns the driver of a vehicle. If provided object is a unit, the unit is returned.", driver_object));
    runtime.register_sqfop(unary("commander", t_object(), "Returns the primary observer. If provided object is a unit, the unit is returned.", commander_object));
    runtime.register_sqfop(unary("gunner", t_object(), "Returns the gunner of a vehicle. If provided object is a unit, the unit is returned.", gunner_object));
    runtime.register_sqfop(binary(4, "in", t_object(), t_object(), "Checks whether unit is in vehicle.", in_object_object));
    runtime.register_sqfop(unary("vehicleVarName", t_object(), "Returns the name of the variable which contains a primary editor reference to this object." "\n"
        "This is the variable given in the Insert Unit dialog / name field, in the editor. It can be changed using setVehicleVarName.", vehiclevarname_object));
    runtime.register_sqfop(binary(4, "setVehicleVarName", t_object(), t_string(), "Sets string representation of an object to a custom string. For example it is possible to return \"MyFerrari\" instead of default \"ce06b00# 164274: offroad_01_unarmed_f.p3d\" when querying object as string", setvehiclevarname_object_string));

    runtime.register_sqfop(unary("allVariables", t_object(), "Returns a list of all variables from desired namespace.", allvariables_object));
    runtime.register_sqfop(binary(4, "getVariable", t_object(), t_string(), "Return the value of variable in the variable space assigned to various data types. Returns nil if variable is undefined.", getVariable_object_string));
    runtime.register_sqfop(binary(4, "getVariable", t_object(), t_array(), "Return the value of variable in the provided variable space. First element is expected to be the variable name as string. Returns second array item if variable is undefined.", getVariable_object_array));
    runtime.register_sqfop(binary(4, "setVariable", t_object(), t_array(), "Sets a variable to given value in the provided variable space. First element is expected to be the variable name as string. Second element is expected to be anything.", setVariable_object_array));
}