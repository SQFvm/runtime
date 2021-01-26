#include "instruction_blob.hpp"
#include "runtime.h"

#include "d_array.h"
#include "d_code.h"
#include "d_scalar.h"
#include "d_string.h"


using namespace std::string_view_literals;

void sqf::runtime::instructions::assign_to::execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const
{
    auto& context = vm.context_active();

    auto value = vm.context_active().pop_value();
    if (!value.has_value())
    {
        if (context.weak_error_handling())
        {
            vm.__logmsg(logmessage::runtime::FoundNoValueWeak(diag_info));
        }
        else
        {
            vm.__logmsg(logmessage::runtime::FoundNoValue(diag_info));
        }
        return;
    }
    else if (value->is<sqf::types::t_nothing>())
    {
        vm.__logmsg(logmessage::runtime::AssigningNilValue(diag_info, variable_name));
    }
    if (variable_name.empty()) { return; }
    if (variable_name[0] == '_')
    {
        for (auto it = context.frames_rbegin(); it != context.frames_rend(); ++it)
        {
            if (it->contains(std::string(variable_name)))
            {
                (*it)[std::string(variable_name)] = *value;
                return;
            }
        }
        context.current_frame()[std::string(variable_name)] = *value;
    }
    else
    {
        context.current_frame().globals_value_scope()->at(std::string(variable_name)) = *value;
    }
}
void sqf::runtime::instructions::assign_to_local::execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const
{
    auto& context = vm.context_active();

    auto value = vm.context_active().pop_value();
    if (variable_name.empty()) { return; }
    if (!value.has_value())
    {
        if (context.weak_error_handling())
        {
            vm.__logmsg(logmessage::runtime::FoundNoValueWeak(diag_info));
        }
        else
        {
            vm.__logmsg(logmessage::runtime::FoundNoValue(diag_info));
        }
        return;
    }
    else if (value->is<sqf::types::t_nothing>())
    {
        vm.__logmsg(logmessage::runtime::AssigningNilValue(diag_info, variable_name));
    }

    context.current_frame()[std::string(variable_name)] = *value;
}
void sqf::runtime::instructions::get_variable::execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const
{
    if (variable_name[0] == '_')
    { // local variable
        auto opt = vm.context_active().get_variable(std::string(variable_name));
        if (opt.has_value())
        {
            vm.context_active().push_value(*opt);
        }
        else
        {
            vm.__logmsg(logmessage::runtime::VariableNotFound(diag_info, variable_name));
            vm.context_active().push_value({});
        }
    }
    else
    { // global variable
        auto global_scope = vm.context_active().current_frame().globals_value_scope();
        if (global_scope->contains(std::string(variable_name)))
        {
            vm.context_active().push_value(global_scope->at(std::string(variable_name)));
        }
        else
        {
            vm.__logmsg(logmessage::runtime::VariableNotFound(diag_info, variable_name));
            vm.context_active().push_value({});
        }
    }
}
void sqf::runtime::instructions::call_nular::execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const
{
    auto& context = vm.context_active();
    sqf::runtime::sqfop_nular::key key = { std::string(operator_name) };
    if (!vm.sqfop_exists(key))
    {
        vm.__logmsg(logmessage::runtime::UnknownInputTypeCombinationNular(diag_info, key.name));
        return;
    }
    auto op = vm.sqfop_at(key);
    auto return_value = op.execute(vm);

    context.push_value(return_value);
}
void sqf::runtime::instructions::end_statement::execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const
{
    vm.context_active().clear_values();
}
void sqf::runtime::instructions::make_array::execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const
{
    auto vec = std::vector<sqf::runtime::value>(size);
    for (size_t i = size - 1; i != (size_t)~0; i--)
    {
        auto opt = vm.context_active().pop_value();
        if (!opt.has_value())
        {
            vm.__logmsg(logmessage::runtime::StackCorruptionMissingValues(diag_info, size, i));
            break;
        }
        else
        {
            vec[i] = *opt;
        }
    }
    vm.context_active().push_value(std::make_shared<sqf::types::d_array>(vec));
}
void sqf::runtime::instructions::push::execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const
{
    vm.context_active().push_value(data);
}
void sqf::runtime::instructions::call_binary::execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const
{
    auto& context = vm.context_active();

    auto right_value = vm.context_active().pop_value();
    if (!right_value.has_value())
    {
        if (context.weak_error_handling())
        {
            vm.__logmsg(logmessage::runtime::NoValueFoundForRightArgumentWeak(diag_info));
        }
        else
        {
            vm.__logmsg(logmessage::runtime::NoValueFoundForRightArgument(diag_info));
        }
        return;
    }
    else if (right_value->is<sqf::types::t_nothing>())
    {
        vm.__logmsg(logmessage::runtime::NilValueFoundForRightArgumentWeak(diag_info));
        return;
    }



    auto left_value = vm.context_active().pop_value();
    if (!left_value.has_value())
    {
        if (context.weak_error_handling())
        {
            vm.__logmsg(logmessage::runtime::NoValueFoundForRightArgumentWeak(diag_info));
        }
        else
        {
            vm.__logmsg(logmessage::runtime::NoValueFoundForRightArgument(diag_info));
        }
        return;
    }
    else if (left_value->is<sqf::types::t_nothing>())
    {
        vm.__logmsg(logmessage::runtime::NilValueFoundForRightArgumentWeak(diag_info));
        return;
    }

    auto tleft = left_value->type();
    auto tright = right_value->type();
    sqf::runtime::sqfop_binary::key key = { std::string(operator_name), tleft, tright };

    if (!vm.sqfop_exists(key))
    {
        key = { std::string(operator_name), sqf::types::t_any(), tright };
        if (!vm.sqfop_exists(key))
        {
            key = { std::string(operator_name), tleft, sqf::types::t_any() };
            if (!vm.sqfop_exists(key))
            {
                key = { std::string(operator_name), sqf::types::t_any(), sqf::types::t_any() };
                if (!vm.sqfop_exists(key))
                {
                    vm.__logmsg(logmessage::runtime::UnknownInputTypeCombinationBinary(diag_info, tleft, key.name, tright));
                    return;
                }
            }
        }
    }
    auto op = vm.sqfop_at(key);
    auto return_value = op.execute(vm, *left_value, *right_value);

    context.push_value(return_value);
}
void sqf::runtime::instructions::call_unary::execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const
{
    auto& context = vm.context_active();

    auto right_value = vm.context_active().pop_value();
    if (!right_value.has_value())
    {
        if (context.weak_error_handling())
        {
            vm.__logmsg(logmessage::runtime::NoValueFoundForRightArgumentWeak(diag_info));
        }
        else
        {
            vm.__logmsg(logmessage::runtime::NoValueFoundForRightArgument(diag_info));
        }
        return;
    }
    else if (right_value->is<sqf::types::t_nothing>())
    {
        vm.__logmsg(logmessage::runtime::NilValueFoundForRightArgumentWeak(diag_info));
        return;
    }

    auto tright = right_value->operator sqf::runtime::type();
    sqf::runtime::sqfop_unary::key key = { std::string(operator_name), tright };
    if (!vm.sqfop_exists(key))
    {
        key = { std::string(operator_name), sqf::types::t_any() };
        if (!vm.sqfop_exists(key))
        {
            vm.__logmsg(logmessage::runtime::UnknownInputTypeCombinationUnary(diag_info, key.name, tright));
            return;
        }
    }
    auto op = vm.sqfop_at(key);
    auto return_value = op.execute(vm, *right_value);

    context.push_value(return_value);
}

inline void sqf::runtime::instructions::push::write(sqf::runtime::instruction_blob& b) const
{
    b.write<opcodes>(opcodes::push);
    data.data()->write(b);
}
sqf::runtime::instructions::push sqf::runtime::instructions::push::read(sqf::runtime::instruction_handle& h)
{
    auto opcode = h.read<opcodes>();
    switch (opcode)
    {
        case sqf::runtime::opcodes::meta_array: return { sqf::types::d_array::read(h) };
        case sqf::runtime::opcodes::meta_code: return { sqf::types::d_code::read(h) };
        case sqf::runtime::opcodes::meta_float: return { sqf::types::d_scalar::read(h) };
        case sqf::runtime::opcodes::meta_string: return { sqf::types::d_string::read(h) };
        case sqf::runtime::opcodes::meta_nil:
        [[fallthrough]]
        default: return { {} };
    }
}

std::string sqf::runtime::instruction_blob::reconstruct_sqf() const
{
    auto h = create_handle();
    std::vector<std::string> strings;
    while (!h.eos())
    {
        auto op = h.read<opcodes>();
        switch (op)
        {
            case opcodes::assign_to: {
                if (strings.empty()) { break; }
                auto op = instructions::assign_to::read(h);
                std::string str;
                str.reserve(
                    op.variable_name.length() +
                    " = "sv.length() +
                    strings.back().length()
                );
                str.append(op.variable_name);
                str.append(" = "sv);
                str.append(strings.back());
                strings.pop_back();
                strings.push_back(str);
            } break;
            case opcodes::assign_to_local: {
                if (strings.empty()) { break; }
                auto op = instructions::assign_to_local::read(h);
                std::string str;
                str.reserve(
                    "private "sv.length() +
                    op.variable_name.length() +
                    " = "sv.length() +
                    strings.back().length()
                );
                str.append("private "sv);
                str.append(op.variable_name);
                str.append(" = "sv);
                str.append(strings.back());
                strings.pop_back();
                strings.push_back(str);
            } break;
            case opcodes::get_variable: {
                auto op = instructions::get_variable::read(h);
                strings.push_back(std::string(op.variable_name));
            } break;
            case opcodes::call_nular: {
                auto op = instructions::call_nular::read(h);
                strings.push_back(std::string(op.operator_name));
            } break;
            case opcodes::call_unary: {
                if (strings.empty()) { break; }
                auto op = instructions::call_unary::read(h);
                auto r = strings.back();
                strings.pop_back();


                std::string str;
                str.reserve(
                    op.operator_name.length() +
                    " "sv.length() +
                    r.length()
                );
                str.append(op.operator_name);
                str.append(" "sv);
                str.append(r);
                strings.push_back(str);
            } break;
            case opcodes::call_binary: {
                auto op = instructions::call_unary::read(h);
                if (strings.empty()) { break; }
                auto r = strings.back();
                strings.pop_back();
                if (strings.empty()) { break; }
                auto l = strings.back();
                strings.pop_back();


                std::string str;
                str.reserve(
                    l.length() +
                    " "sv.length() +
                    op.operator_name.length() +
                    " "sv.length() +
                    r.length()
                );
                str.append(l);
                str.append(" "sv);
                str.append(op.operator_name);
                str.append(" "sv);
                str.append(r);
                strings.push_back(str);
            } break;
            case opcodes::push:
            {
                auto op = instructions::push::read(h);
                strings.push_back(op.data.to_string_sqf());
            } break;
            case opcodes::make_array:
            {
                auto op = instructions::make_array::read(h);
                if (strings.size() < op.size) { break; }
                size_t len = 1; // [
                for (auto rit = strings.rbegin(); rit != rbegin() + op.size; ++rit)
                {
                    len += rit->length();
                    len += ","sv.length(); // No need to check for last as "]"sv.length() == 1 too.
                }
                std::string str = "[";
                str.reserve(len);
                for (size_t i = 0; i < op.size; i++)
                {
                    str.append(strings.back());
                    strings.pop_back();
                    str.append(",");
                }
                str.append("]");
                strings.push_back(str);
            } break;
            case opcodes::end_statement:
            {
                auto op = instructions::push::read(h);
                strings.push_back("; ");
            } break;
        }
    }
    size_t len = 0;
    for (auto it : strings)
    {
        len += it.length();
    }
    std::string output;
    output.reserve(len);
    for (auto it : strings)
    {
        output.append(it);
    }
    return output;
}

std::string sqf::runtime::instruction_blob::reconstruct_assembly() const
{
    auto h = create_handle();
    std::vector<std::string> strings;
    while (!h.eos())
    {
        auto op = h.read<opcodes>();
        switch (op)
        {
            case opcodes::assign_to:
            {
                auto op = instructions::assign_to::read(h);
                std::string str;
                str.reserve(
                    "assignTo "sv.length() +
                    op.variable_name.length()
                );
                str.append("assignTo "sv);
                str.append(op.variable_name);
                strings.push_back(str);
            } break;
            case opcodes::assign_to_local:
            {
                auto op = instructions::assign_to_local::read(h);
                std::string str;
                str.reserve(
                    "assignToLocal "sv.length() +
                    op.variable_name.length()
                );
                str.append("assignToLocal "sv);
                str.append(op.variable_name);
                strings.push_back(str);
            } break;
            case opcodes::get_variable:
            {
                auto op = instructions::get_variable::read(h);
                std::string str;
                str.reserve(
                    "getVariable "sv.length() +
                    op.variable_name.length()
                );
                str.append("getVariable "sv);
                str.append(op.variable_name);
                strings.push_back(str);
            } break;
            case opcodes::call_nular:
            {
                auto op = instructions::call_nular::read(h);
                std::string str;
                str.reserve(
                    "callNular "sv.length() +
                    op.operator_name.length()
                );
                str.append("getVariable "sv);
                str.append(op.operator_name);
                strings.push_back(str);
            } break;
            case opcodes::call_unary:
            {
                auto op = instructions::call_unary::read(h);
                std::string str;
                str.reserve(
                    "callUnary "sv.length() +
                    op.operator_name.length()
                );
                str.append("getVariable "sv);
                str.append(op.operator_name);
                strings.push_back(str);
            } break;
            case opcodes::call_binary:
            {
                auto op = instructions::call_binary::read(h);
                std::string str;
                str.reserve(
                    "callBinary "sv.length() +
                    op.operator_name.length()
                );
                str.append("callBinary "sv);
                str.append(op.operator_name);
                strings.push_back(str);
            } break;
            case opcodes::push:
            {
                auto op = instructions::push::read(h);
                auto content = op.data.to_string();
                std::string str;
                str.reserve(
                    "push "sv.length() +
                    content.length()
                );
                str.append("push "sv);
                str.append(content);
                strings.push_back(str);
            } break;
            case opcodes::make_array:
            {
                auto op = instructions::make_array::read(h);
                std::string content = std::to_string(op.size);
                std::string str;
                str.reserve(
                    "makeArray "sv.length() +
                    content.length()
                );
                str.append("makeArray "sv);
                str.append(content);
                strings.push_back(str);
            } break;
            case opcodes::end_statement:
            {
                auto op = instructions::push::read(h);
                strings.push_back("endStatement");
            } break;
        }
    }
    size_t len = 0;
    for (auto it : strings)
    {
        len += it.length();
        len++;
    }
    std::string output;
    output.reserve(len);
    for (auto it : strings)
    {
        output.append(it);
        output.append("\n"sv);
    }
    return output;
}
