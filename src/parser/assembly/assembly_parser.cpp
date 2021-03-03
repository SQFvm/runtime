#include "assembly_parser.h"
#include "tokenizer.hpp"
#include "parser.tab.hh"

#include "../../opcodes/common.h"
#include "../../runtime/d_array.h"
#include "../../runtime/d_string.h"
#include "../../runtime/d_scalar.h"
#include "../../runtime/d_boolean.h"
#include "../../runtime/d_code.h"
#include <algorithm>
#include <charconv>
#include <sstream>

using namespace std::string_literals;
using namespace std::string_view_literals;

sqf::runtime::value sqf::parser::assembly::parser::get_value(::sqf::runtime::runtime& runtime, std::string_view contents, const ::sqf::parser::assembly::bison::astnode& node)
{
    switch (node.kind)
    {
        case bison::astkind::HEXNUMBER:
        {
            try
            {
                auto str = std::string(node.token.contents);
                if (str[0] == '$') { str = "0x"s.append(str.substr(1)); }
                auto hexnum = (int64_t)std::stol(str, nullptr, 16);
                return ::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(hexnum));
            }
            catch (std::out_of_range&)
            {
                return ::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(std::nanf("")));
            }
        }
        break;
        case bison::astkind::NUMBER:
        {
            try
            {
                return ::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>((double)std::stod(std::string(node.token.contents))));
            }
            catch (std::out_of_range&)
            {
                return ::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(std::nanf("")));
            }
        }
        break;
        case bison::astkind::STRING:
        {
            return ::sqf::runtime::value(std::make_shared<::sqf::types::d_string>(::sqf::types::d_string::from_sqf(node.token.contents)));
        }
        break;
        case bison::astkind::BOOLEAN_TRUE:
        {
            return ::sqf::runtime::value(true);
        }
        break;
        case bison::astkind::BOOLEAN_FALSE:
        {
            return ::sqf::runtime::value(false);
        }
        break;
        case bison::astkind::CODE:
        {
            std::vector<::sqf::runtime::instruction::sptr> tmp_set;
            bison::astnode previous_node;
            for (size_t i = 0; i < node.children.size(); i++)
            {
                previous_node = node.children[i];
                to_assembly(runtime, contents, previous_node, tmp_set);
            }
            auto inst_set = ::sqf::runtime::instruction_set(tmp_set);
            return ::sqf::runtime::value(std::make_shared<::sqf::types::d_code>(inst_set));
        }
        break;
        case bison::astkind::ARRAY:
        {
            std::vector<::sqf::runtime::value> values;
            for (auto& subnode : node.children)
            {
                values.push_back(get_value(runtime, contents, subnode));
            }
            return ::sqf::runtime::value(std::make_shared<::sqf::types::d_array>(values));
        }
        break;
    }
}
void ::sqf::parser::assembly::parser::to_assembly(::sqf::runtime::runtime& runtime, std::string_view contents, const ::sqf::parser::assembly::bison::astnode& node, std::vector<::sqf::runtime::instruction::sptr>& set)
{

    switch (node.kind)
    {
        case bison::astkind::ASSIGN_TO: {
            auto inst = std::make_shared<::sqf::opcodes::assign_to>(::sqf::types::d_string::from_sqf(node.children[1].token.contents));
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        } break;
        case bison::astkind::ASSIGN_TO_LOCAL: {
            auto inst = std::make_shared<::sqf::opcodes::assign_to_local>(::sqf::types::d_string::from_sqf(node.children[1].token.contents));
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        } break;
        case bison::astkind::GET_VARIABLE: {
            auto inst = std::make_shared<::sqf::opcodes::get_variable>(::sqf::types::d_string::from_sqf(node.children[1].token.contents));
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        } break;
        case bison::astkind::CALL_BINARY: {
            auto str = std::string(node.children[0].token.contents);
            if (!runtime.sqfop_exists_binary(str))
            {
                __log(logmessage::runtime::InvalidAssemblyInstruction(
                    runtime.context_active().current_frame().diag_info_from_position(),
                    str));
            }
            else
            {
                auto binary_ops = runtime.sqfop_binary_by_name(str);
                auto prec = binary_ops.begin()->get().precedence();

                auto inst = std::make_shared<::sqf::opcodes::call_binary>(str, prec);
                inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
                set.push_back(inst);
            }
        } break;
        case bison::astkind::CALL_UNARY: {
            auto inst = std::make_shared<::sqf::opcodes::call_unary>(std::string(node.children[1].token.contents));
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        } break;
        case bison::astkind::CALL_NULAR: {
            auto inst = std::make_shared<::sqf::opcodes::call_nular>(std::string(node.children[1].token.contents));
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        } break;
        case bison::astkind::END_STATEMENT: {
            auto inst = std::make_shared<::sqf::opcodes::end_statement>();
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        } break;
        case bison::astkind::PUSH:

        default:
        {
            bison::astnode previous_node;
            for (size_t i = 0; i < node.children.size(); i++)
            {
                if (i != 0)
                {
                    auto inst = std::make_shared<::sqf::opcodes::end_statement>();
                    inst->diag_info({ previous_node.token.line, previous_node.token.column + previous_node.token.contents.length(), previous_node.token.offset, { *previous_node.token.path, {} }, create_code_segment(contents, previous_node.token.offset, previous_node.token.contents.length()) });
                    set.push_back(inst);
                }
                previous_node = node.children[i];
                to_assembly(runtime, contents, previous_node, set);
            }
        }
    }
}

bool sqf::parser::assembly::parser::get_tree(::sqf::runtime::runtime& runtime, ::sqf::parser::assembly::tokenizer& t, ::sqf::parser::assembly::bison::astnode* out)
{
    ::sqf::parser::assembly::bison::parser p(t, *out, *this, runtime);
    bool success = p.parse() == 0;
    return success;
}

std::optional<sqf::runtime::instruction_set> sqf::parser::assembly::parser::parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file)
{
    tokenizer t(contents.begin(), contents.end(), file.physical);
    ::sqf::parser::assembly::bison::astnode res;
    ::sqf::parser::assembly::bison::parser p(t, res, *this, runtime);
    // p.set_debug_level(1);
    bool success = p.parse() == 0;
    if (!success)
    {
        return {};
    }
    std::vector<::sqf::runtime::instruction::sptr> vec;
    to_assembly(runtime, contents, res, vec);
    return vec;
}

bool ::sqf::parser::assembly::parser::check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file)
{
    tokenizer t(contents.begin(), contents.end(), file.physical);
    ::sqf::parser::assembly::bison::astnode res;
    ::sqf::parser::assembly::bison::parser p(t, res, *this, runtime);
    bool success = p.parse() == 0;
    return success;
}
