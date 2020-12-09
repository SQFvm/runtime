#include "ops_hashmap.h"
#include "../runtime/value.h"
#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"
#include "../runtime/util.h"

#include "../runtime/d_string.h"
#include "../runtime/d_scalar.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_array.h"

#include <array>



namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;

namespace
{
    value createhashmap_(runtime& runtime)
    {
        return std::make_shared<d_hashmap>();
    }
    value createhashmapfromarray_array(runtime& runtime, value::cref right)
    {
        std::unordered_map<sqf::runtime::value, sqf::runtime::value> hashmap;
        auto arr = right.data<d_array>();
        for (size_t i = 0; i < arr->size(); i++)
        {
            auto& it = arr->at(i);
            if (it.is<t_array>())
            {
                auto subArr = it.data<d_array>();
                if (subArr->size() == 2)
                {
                    auto& key = subArr->at(0);
                    auto& value = subArr->at(1);
                    // ToDo: Check key-type matches
                    hashmap[key] = value;
                }
                else
                {
                    runtime.__logmsg(err::ExpectedArraySizeMissmatch(
                        runtime.context_active().current_frame().diag_info_from_position(),
                        2,
                        2,
                        subArr->size()));
                }
            }
            else
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(
                    runtime.context_active().current_frame().diag_info_from_position(),
                    i,
                    t_array(),
                    it.type()));
            }
        }
        return std::make_shared<d_hashmap>(hashmap);
    }
    value set_hashmap_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto data = left.data<d_hashmap>();
        auto arr = right.data<d_array>();
        if (arr->size() == 2)
        {
            auto& key = arr->at(0);
            auto& value = arr->at(1);
            // ToDo: Check key-type matches
            data->map()[key] = value;
        }
        else
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(
                runtime.context_active().current_frame().diag_info_from_position(),
                2,
                2,
                arr->size()));
        }
        return {};
    }
    value get_hashmap_any(runtime& runtime, value::cref left, value::cref right)
    {
        auto data = left.data<d_hashmap>();
        auto res = data->map().find(right);
        if (res != data->map().end())
        {
            return res->second;
        }
        else
        {
            // ToDo: Log warning about key not found
        }
        return {};
    }
    value deleteat_hashmap_any(runtime& runtime, value::cref left, value::cref right)
    {
        auto data = left.data<d_hashmap>();
        auto res = data->map().find(right);
        if (res != data->map().end())
        {
            auto val = *res;
            data->map().erase(right);
            return val.second;
        }
        else
        {
            // ToDo: Log warning about key not found
        }
        return {};
    }
    value in_any_hashmap(runtime& runtime, value::cref left, value::cref right)
    {
        auto data = right.data<d_hashmap>();
        return data->map().find(left) != data->map().end();
    }
    value count_hashmap(runtime& runtime, value::cref right)
    {
        return right.data<d_hashmap>()->map().size();
    }
    value keys_hashmap(runtime& runtime, value::cref right)
    {
        std::vector<value> keys;
        auto data = right.data<d_hashmap>();
        for (auto& it : data->map())
        {
            keys.push_back(it.first);
        }
        return std::make_shared<d_array>(keys);
    }
    value plus_hashmap(runtime& runtime, value::cref right)
    {
        std::unordered_map<sqf::runtime::value, sqf::runtime::value> hashmap = right.data<d_hashmap>()->map();
        return std::make_shared<d_hashmap>(hashmap);
    }
}

void sqf::operators::ops_hashmap(::sqf::runtime::runtime& runtime)
{
    using namespace sqf::runtime::sqfop;
    runtime.register_sqfop(nular("createHashMap", "Creates a hashmap.", createhashmap_));
    runtime.register_sqfop(unary("createHashMapFromArray", t_array(), "Creates a hashmap from an key-value pair array ([[key, value], ...]).", createhashmapfromarray_array));
    runtime.register_sqfop(binary(4, "set", t_hashmap(), t_array(), "Assigns a value to a hashmap.", set_hashmap_array));
    runtime.register_sqfop(binary(4, "get", t_hashmap(), t_any(), "Receives a value from a hashmap.", get_hashmap_any));
    runtime.register_sqfop(binary(4, "deleteAt", t_hashmap(), t_any(), "Removes a key-value pair from a hashmap.", deleteat_hashmap_any));
    runtime.register_sqfop(binary(4, "in", t_any(), t_hashmap(), "Checks if a given key is inside a value.", in_any_hashmap));
    runtime.register_sqfop(unary("count", t_hashmap(), "Returns the number of elements inside a hashmap.", count_hashmap));
    runtime.register_sqfop(unary("keys", t_hashmap(), "Returns the keys of a hashmap.", keys_hashmap));
    runtime.register_sqfop(unary("+", t_hashmap(), "Returns a copy of the hashmap.", plus_hashmap));
}
