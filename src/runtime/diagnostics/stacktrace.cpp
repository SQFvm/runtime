#include "stacktrace.h"
#include "../logging.h"

#include <iomanip>

std::string sqf::runtime::diagnostics::stacktrace::to_string() const
{
    std::stringstream sstream;
    int i = 0;
    if (!value.empty())
    {
        sstream << value.to_string_sqf() << std::endl;
    }
    for (auto& frame : frames)
    {
        sstream <<
            "<" << std::setw(3) << ++i << " of " << frames.size() << "> " <<
            LogLocationInfo((*frame.current())->diag_info()).format() <<
            "[" << (frame.globals_value_scope()->scope_name().empty() ? "SCOPENAME-NA" : frame.globals_value_scope()->scope_name()) << "] " <<
            "[" << (frame.scope_name().empty() ? "SCOPENAME-EMPTY" : frame.scope_name()) << "]" << std::endl <<
            (*frame.current())->diag_info().code_segment << std::endl;
    }
    return sstream.str();
}