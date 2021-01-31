#pragma once
#include "ops_config.h"
#include "ops_diag.h"
#include "ops_generic.h"
#include "ops_group.h"
#include "ops_logic.h"
#include "ops_markers.h"
#include "ops_math.h"
#include "ops_namespace.h"
#include "ops_object.h"
#include "ops_sqfvm.h"
#include "ops_string.h"
#include "ops_text.h"
#include "ops_dummy_nular.h"
#include "ops_dummy_unary.h"
#include "ops_dummy_binary.h"
#include "ops_osspecific.h"
#include "ops_hashmap.h"

namespace sqf
{
    namespace runtime
    {
        class runtime;
    }

    namespace operators
    {
        inline void ops(::sqf::runtime::runtime& runtime)
        {
            sqf::operators::ops_config(runtime);
            sqf::operators::ops_diag(runtime);
            sqf::operators::ops_generic(runtime);
            sqf::operators::ops_group(runtime);
            sqf::operators::ops_logic(runtime);
            sqf::operators::ops_markers(runtime);
            sqf::operators::ops_math(runtime);
            sqf::operators::ops_namespace(runtime);
            sqf::operators::ops_object(runtime);
            sqf::operators::ops_sqfvm(runtime);
            sqf::operators::ops_string(runtime);
            sqf::operators::ops_text(runtime);
            sqf::operators::ops_dummy_nular(runtime);
            sqf::operators::ops_dummy_unary(runtime);
            sqf::operators::ops_dummy_binary(runtime);
            sqf::operators::ops_osspecific(runtime);
            sqf::operators::ops_hashmap(runtime);
        }
    }
}