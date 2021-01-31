#include "ops_text.h"
#include "../runtime/value.h"
#include "../runtime/logging.h"
#include "../runtime/runtime.h"

#include "../runtime/d_string.h"
#include "../runtime/d_array.h"
#include "d_text.h"

namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;
using namespace std::string_literals;

namespace
{
    value composetext_array(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_array>();
        std::stringstream sstream;
        for (auto& it : *r)
        {
            sstream << it.data<d_string, std::string>();
        }
        return std::make_shared<d_text>(sstream.str());
    }
    value linebreak_(runtime& runtime)
    {
        return std::make_shared<d_text>("\r\n"s);
    }
    value parsetext_string(runtime& runtime, value::cref right)
    {
        return std::make_shared<d_text>(right.data<d_string, std::string>());
    }
    value text_string(runtime& runtime, value::cref right)
    {
        return std::make_shared<d_text>(right.data<d_string, std::string>());
    }
}
void sqf::operators::ops_text(sqf::runtime::runtime& runtime)
{
    using namespace sqf::runtime::sqfop;

    runtime.register_sqfop(unary("composeText", t_array(), "Creates a structured text containing a line break.", composetext_array));
    runtime.register_sqfop(nular("lineBreak", "Creates a structured text containing a line break.", linebreak_));
    runtime.register_sqfop(unary("parseText", t_string(), "Creates a structured text by parsing the given XML description.", parsetext_string));
    runtime.register_sqfop(unary("text", t_string(), "Creates a structured text containing the given plain text if argument is String.", text_string));
}
