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


namespace sqf::runtime
{
    struct t_hashmap : public type::extend<t_hashmap> { t_hashmap() : extend() {} static const std::string name() { return "HASHMAP"; } };
}

namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;

namespace
{
    value createhashmap_(runtime& runtime)
    {
        return {};
    }
    value createhashmapfromarray_array(runtime& runtime, value::cref right)
    {
        return {};
    }
    value set_hashmap_array(runtime& runtime, value::cref left, value::cref right)
    {
        return {};
    }
    value get_hashmap_any(runtime& runtime, value::cref left, value::cref right)
    {
        return {};
    }
    value deleteat_hashmap_any(runtime& runtime, value::cref left, value::cref right)
    {
        return {};
    }
    value in_any_hashmap(runtime& runtime, value::cref left, value::cref right)
    {
        return {};
    }
    value count_hashmap(runtime& runtime, value::cref right)
    {
        return {};
    }
    value keys_hashmap(runtime& runtime, value::cref right)
    {
        return {};
    }
    value plus_hashmap(runtime& runtime, value::cref right)
    {
        return {};
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
