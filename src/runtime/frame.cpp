#include "frame.h"
#include "runtime.h"
#include "context.h"

void sqf::runtime::frame::clear_values_helper(runtime& runtime)
{

    runtime.context_active().clear_values();
}
