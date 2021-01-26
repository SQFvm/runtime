#include "instruction_blob.hpp"
#include "runtime.h"

#include "d_array.h"
#include "d_code.h"
#include "d_scalar.h"
#include "d_string.h"


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
