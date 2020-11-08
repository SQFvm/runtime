#include "ops_diag.h"
#include "../runtime/value.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"

#include "../runtime/d_string.h"
#include "../runtime/d_scalar.h"
#include "../runtime/d_boolean.h"

#include <chrono>

namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;
namespace
{
    value diag_log_any(runtime& runtime, value::cref right)
    {
        auto r = right.to_string();
        runtime.__logmsg(err::InfoMessage(runtime.context_active().current_frame().diag_info_from_position(), "DIAG_LOG", r));
        return {};
    }
    value diag_tickTime_(runtime& runtime)
    {
        auto curtime = std::chrono::system_clock::now().time_since_epoch();
        auto starttime = runtime.runtime_timestamp().time_since_epoch();
        // Time is since beginning of game so long is fine.
        long long r = static_cast<long long>(std::chrono::duration_cast<std::chrono::milliseconds>(curtime - starttime).count());
        double d = r * 0.001;
        float f = (float)d;
        return { f };
    }
    value assert_bool(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_boolean>()->value();
        if (!r)
        {
            runtime.__logmsg(err::AssertFailed(runtime.context_active().current_frame().diag_info_from_position()));
        }
        return right;
    }
    value halt_(runtime& runtime)
    {
        runtime.execute(sqf::runtime::runtime::action::stop);
        return {};
    }
}
void sqf::operators::ops_diag(::sqf::runtime::runtime& runtime)
{
    runtime.register_sqfop(sqfop::unary("diag_log", t_any(), "Dumps the argument's value to the report file. Each call creates a new line in the file.", diag_log_any));
    runtime.register_sqfop(sqfop::nular("diag_tickTime", "Real time spent from the start of the runtime. Expressed in fractions of second. Resolution of 1 tick is 1 ms.", diag_tickTime_));
    runtime.register_sqfop(sqfop::unary("assert", t_boolean(), "Tests a condition and if the condition is false, displays error on screen.", assert_bool));
    runtime.register_sqfop(sqfop::nular("halt", "Halts the execution if a debugger is attached. If not, warning is logged and execution continues.", halt_));
}