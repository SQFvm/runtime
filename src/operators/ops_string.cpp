#include "ops_string.h"
#include "../runtime/value.h"
#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"

#include "../runtime/d_string.h"
#include "../runtime/d_scalar.h"
#include "../runtime/d_array.h"

#include <algorithm>
#include <sstream>
#include <cmath>
#include <cctype>

namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;
using namespace std::string_literals;

namespace
{
    value count_string(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_string, std::string>();
        return r.length();
    }
    value toupper_string(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_string, std::string>();
        std::transform(r.begin(), r.end(), r.begin(), [](char& c) { return (char)std::toupper((int)c); });
        return r;
    }
    value tolower_string(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_string, std::string>();
        std::transform(r.begin(), r.end(), r.begin(), [](char& c) { return (char)std::tolower((int)c); });
        return r;
    }
    value select_string_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto str = left.data<d_string, std::string>();
        auto arr = right.data<d_array>();
        if (arr->empty())
        {
            runtime.__logmsg(err::ExpectedArrayToHaveElements(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (!arr->at(0).is<t_scalar>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 0, t_scalar(), arr->at(0).type()));
            return {};
        }
        int start = static_cast<int>(std::round(arr->at(0).data<d_scalar, float>()));
        if (start < 0)
        {
            runtime.__logmsg(err::NegativeIndexWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return ""s;
        }
        if (start >= static_cast<int>(str.length()))
        {
            runtime.__logmsg(err::IndexOutOfRangeWeak(runtime.context_active().current_frame().diag_info_from_position(), str.length(), start));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return ""s;
        }
        if (arr->size() >= 2)
        {
            if (!arr->at(1).is<t_scalar>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 1, t_scalar(), arr->at(1).type()));
                return {};
            }
            int length = static_cast<int>(std::round(arr->at(1).data<d_scalar, float>()));
            if (length < 0)
            {
                runtime.__logmsg(err::NegativeIndexWeak(runtime.context_active().current_frame().diag_info_from_position()));
                runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
                return ""s;
            }
            return str.substr(start, length);
        }
        return str.substr(start);
    }
    value format_array(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_array>();
        if (r->empty())
        {
            runtime.__logmsg(err::ExpectedArrayToHaveElementsWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return ""s;
        }
        if (!r->at(0).is<t_string>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), 0, t_string(), r->at(0).type()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return ""s;
        }
        auto format = r->at(0).data<d_string, std::string>();
        std::stringstream sstream;
        size_t off = 0;
        size_t newoff;
        while ((newoff = format.find(L'%', off)) != std::string::npos)
        {
            sstream << format.substr(off, newoff - off);
            newoff++;

            if (!(format[newoff] >= '0' && format[newoff] <= '9'))
            {
                runtime.__logmsg(err::FormatInvalidPlaceholder(runtime.context_active().current_frame().diag_info_from_position(), format[newoff], newoff));
                newoff++;
            }
            else
            {
                size_t end;
                for (end = newoff; format[end] >= '0' && format[end] <= '9'; ++end);
                auto num = std::stoi(format.substr(newoff, end - newoff));
                newoff = end;
                if (num >= static_cast<int>(r->size()))
                {
                    runtime.__logmsg(err::IndexOutOfRangeWeak(runtime.context_active().current_frame().diag_info_from_position(), r->size(), num));
                }
                else if (r->at(num).is<t_string>())
                {
                    sstream << r->at(num).data<d_string, std::string>();
                }
                else
                {
                    sstream << r->at(num).to_string_sqf();
                }
            }
            off = newoff;
        }
        sstream << (format.size() >= off ? format.substr(off) : "");
        return sstream.str();
    }
    value toarray_string(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_string, std::string>();
        auto arr = std::vector<value>(r.size());
        for (size_t i = 0; i < r.size(); i++)
        {
            arr[i] = static_cast<int>(r[i]);
        }
        return value(arr);
    }
    value tostring_array(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_array>();
        std::stringstream sstream;
        for (size_t i = 0; i < r->size(); i++)
        {
            auto& val = r->at(i);
            if (val.is<t_scalar>())
            {
                sstream << static_cast<char>(val.data<d_scalar, int>());
            }
            else
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), i, t_scalar(), val.type()));
            }
        }
        return sstream.str();
    }
    value joinstring_array_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_array>();
        auto r = right.data<d_string, std::string>();
        std::stringstream sstream;
        bool separator = false;
        for (auto& it : *l)
        {
            if (separator)
            {
                if (it.is<t_string>())
                {
                    sstream << r << it.data<d_string, std::string>();
                }
                else
                {
                    sstream << r << it.data()->to_string_sqf();
                }
            }
            else
            {
                separator = true;
                if (it.is<t_string>())
                {
                    sstream << it.data<d_string, std::string>();
                }
                else
                {
                    sstream << it.data()->to_string_sqf();
                }
            }
        }
        return sstream.str();
    }
    value plus_string_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_string, std::string>();
        auto r = right.data<d_string, std::string>();
        std::stringstream sstream;
        sstream << l << r;
        return sstream.str();
    }
    value find_string_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_string, std::string>();
        auto r = right.data<d_string, std::string>();
        size_t res = l.find(r);
        return res == std::string::npos ? -1.f : res;
    }
    value splitstring_string_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_string, std::string>();
        auto r = right.data<d_string, std::string>();
        std::vector<value> values;
        if (r.empty())
        {
            for (auto c : l)
            {
                values.push_back(c);
            }
            return values;
        }
        // Avoid using actual strtok method due to concerns regarding the way
        // it works (Reentrancy variance)
        size_t match_length = 0;
        for (size_t i = 0; i < l.length(); i++)
        {
            char lc = l[i];
            bool hit = false;
            for (auto rc : r)
            {
                if (lc == rc)
                {
                    if (match_length > 0)
                    {
                        values.push_back(l.substr(i - match_length, match_length));
                        match_length = 0;
                    }
                    hit = true;
                    break;
                }
            }
            if (!hit)
            {
                match_length++;
            }
        }
        if (match_length != 0)
        {
            values.push_back(l.substr(l.size() - match_length));
        }
        return values;
    }
    value parsenumber_string(runtime& runtime, value::cref right)
    {
        return value(strtof(right.data<d_string, std::string>().c_str(), nullptr));
    }
    value endl_(runtime& runtime)
    {
        return "\r\n"s;
    }
}
void sqf::operators::ops_string(sqf::runtime::runtime& runtime)
{
    using namespace sqf::runtime::sqfop;

    runtime.register_sqfop(nular("endl", "Creates a string containing a line break (`\r\n`).", endl_));
    runtime.register_sqfop(unary("count", t_string(), "Can be used to count: the number of characters in a string.", count_string));
    runtime.register_sqfop(unary("toLower", t_string(), "Converts the supplied string to all lowercase characters.", tolower_string));
    runtime.register_sqfop(unary("toUpper", t_string(), "Converts the supplied string to all uppercase characters.", toupper_string));
    runtime.register_sqfop(binary(4, "select", t_string(), t_array(), "Selects a range of characters in provided string, starting at element 0 index, ending at either end of the string or the provided element 1 length.", select_string_array));
    runtime.register_sqfop(unary("format", t_array(), "Composes a string containing other variables or other variable types. Converts any variable type to a string.", format_array));
    runtime.register_sqfop(unary("toArray", t_string(), "Converts the supplied String into an Array of Numbers.", toarray_string));
    runtime.register_sqfop(unary("toString", t_array(), "Converts the supplied String into an Array of Numbers.", tostring_array));
    runtime.register_sqfop(binary(4, "joinString", t_array(), t_string(), "Joins array into String with provided separator. Array can be of mixed types, all elements will be converted to String prior to joining, but the fastest operation is on the array of Strings.", joinstring_array_string));
    runtime.register_sqfop(binary(6, "+", t_string(), t_string(), "Concatinates two strings together.", plus_string_string));
    runtime.register_sqfop(binary(4, "find", t_string(), t_string(), "Searches for a string within a string. Returns the 0 based index on success or -1 if not found.", find_string_string));
    runtime.register_sqfop(binary(4, "splitString", t_string(), t_string(), "An SQF version of C++ strtok. "
        "Splits given string str into an array of tokens according to given delimiters. "
        "In addition, if empty string "" is used for delimiters, str is split by each character.", splitstring_string_string));
    runtime.register_sqfop(unary("parseNumber", t_string(), "Converts the supplied String into a Scalar.", parsenumber_string));
}