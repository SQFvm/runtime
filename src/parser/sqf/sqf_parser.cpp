#include "sqf_parser.hpp"
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

namespace sqf::parser::sqf::util
{
    static std::string strip_formatted(std::string_view sview)
    {
        if (sview.length() == 0) { return {}; }
        if (sview.length() == 2) { return {}; }

        std::vector<char> arr;
        arr.reserve(sview.size());
        for (size_t i = 1; i < sview.length() - 1; i++)
        {
            char c = sview[i];
            if (::sqf::parser::util::is_match_repeated<2, '"'>(sview.data() + i) ||
                ::sqf::parser::util::is_match_repeated<2, '{'>(sview.data() + i) ||
                ::sqf::parser::util::is_match_repeated<2, '}'>(sview.data() + i))
            { // Double "", {{, }} have to be escaped
                i++;
            }
            arr.emplace_back(c);
        }
        return std::string(arr.data(), arr.size());
    }
}

void ::sqf::parser::sqf::parser::to_assembly(std::string_view contents, const ::sqf::parser::sqf::bison::astnode& node, std::vector<::sqf::runtime::instruction::sptr>& set)
{

    switch (node.kind)
    {
    case bison::astkind::EXP0:
    case bison::astkind::EXP1:
    case bison::astkind::EXP2:
    case bison::astkind::EXP3:
    case bison::astkind::EXP4:
    case bison::astkind::EXP5:
    case bison::astkind::EXP6:
    case bison::astkind::EXP7:
    case bison::astkind::EXP8:
    case bison::astkind::EXP9:
    {
        to_assembly(contents, node.children[0], set);
        to_assembly(contents, node.children[1], set);
        auto s = std::string(node.token.contents);
        std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return (char)std::tolower((int)c); });
        auto inst = std::make_shared<::sqf::opcodes::call_binary>(s, (short)(((short)node.kind - (short)bison::astkind::EXP0) + 1));
        inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
        set.push_back(inst);
    }
    break;
    case bison::astkind::EXPU:
    {
        auto s = std::string(node.token.contents);
        to_assembly(contents, node.children[0], set);
        if (node.children[0].kind == bison::astkind::NUMBER && (s == "+" || s == "-"))
        {
            if (s == "-")
            {
                auto child = std::static_pointer_cast<::sqf::opcodes::push>(set.back());
                auto scalar = child->value().data<::sqf::types::d_scalar>();
                scalar->value(-scalar->value());
            }
        }
        else
        {
            std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return (char)std::tolower((int)c); });
            auto inst = std::make_shared<::sqf::opcodes::call_unary>(s);
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        }
    }
    break;
    case bison::astkind::EXPN:
    {
        auto s = std::string(node.token.contents);
        std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return (char)std::tolower((int)c); });
        auto inst = std::make_shared<::sqf::opcodes::call_nular>(s);
        inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
        set.push_back(inst);
    }
    break;
    case bison::astkind::HEXNUMBER:
    {
        try
        {
            auto str = std::string(node.token.contents);
            if (str[0] == '$') { str = "0x"s.append(str.substr(1)); }
            auto hexnum = (int64_t)std::stol(str, nullptr, 16);
            auto inst = std::make_shared<::sqf::opcodes::push>(
                ::sqf::runtime::value(
                    std::make_shared<::sqf::types::d_scalar>(
                        hexnum)));
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        }
        catch (std::out_of_range&)
        {
            auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(std::nanf(""))));
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            __log(logmessage::assembly::NumberOutOfRange(inst->diag_info()));
            set.push_back(inst);
        }
    }
    break;
    case bison::astkind::NUMBER:
    {
        try
        {
            auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>((double)std::stod(std::string(node.token.contents)))));
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        }
        catch (std::out_of_range&)
        {
            auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(std::nanf(""))));
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            __log(logmessage::assembly::NumberOutOfRange(inst->diag_info()));
            set.push_back(inst);
        }
    }
    break;
    case bison::astkind::STRING:
    {
        auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_string>(::sqf::types::d_string::from_sqf(node.token.contents))));
        inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
        set.push_back(inst);
    }
    break;
    case bison::astkind::BOOLEAN_TRUE:
    {
        auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(true));
        inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
        set.push_back(inst);
    }
    break;
    case bison::astkind::BOOLEAN_FALSE:
    {
        auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(false));
        inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
        set.push_back(inst);
    }
    break;
    case bison::astkind::CODE:
    {
        std::vector<::sqf::runtime::instruction::sptr> tmp_set;
        bison::astnode previous_node;
        for (size_t i = 0; i < node.children.size(); i++)
        {
            if (i != 0)
            {
                auto inst = std::make_shared<::sqf::opcodes::end_statement>();
                inst->diag_info({ previous_node.token.line, previous_node.token.column + previous_node.token.contents.length(), previous_node.token.offset, { *previous_node.token.path, {} }, create_code_segment(contents, previous_node.token.offset, previous_node.token.contents.length()) });
                tmp_set.push_back(inst);
            }
            previous_node = node.children[i];
            to_assembly(contents, previous_node, tmp_set);
        }
        auto inst_set = ::sqf::runtime::instruction_set(tmp_set);
        auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_code>(inst_set)));
        inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
        set.push_back(inst);
    }
    break;
    case bison::astkind::ARRAY:
    {
        for (auto& subnode : node.children)
        {
            to_assembly(contents, subnode, set);
        }
        auto inst = std::make_shared<::sqf::opcodes::make_array>(node.children.size());
        inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
        set.push_back(inst);
    }
    break;
    case bison::astkind::ASSIGNMENT:
    {
        to_assembly(contents, node.children[1], set);
        if (node.children[0].children.empty() && node.children[0].token.type == tokenizer::etoken::t_ident)
        {
            auto inst = std::make_shared<::sqf::opcodes::assign_to>(node.children[0].token.contents);
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        }
        else
        {
            auto inst = std::make_shared<::sqf::opcodes::assign_to>(""s);
            inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
            set.push_back(inst);
        }
    }
    break;
    case bison::astkind::ASSIGNMENT_LOCAL:
    {
        to_assembly(contents, node.children[0], set);
        auto inst = std::make_shared<::sqf::opcodes::assign_to_local>(node.token.contents);
        inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
        set.push_back(inst);
    }
    break;
    case bison::astkind::IDENT:
    {
        auto inst = std::make_shared<::sqf::opcodes::get_variable>(node.token.contents);
        inst->diag_info({ node.token.line, node.token.column, node.token.offset, { *node.token.path, {} }, create_code_segment(contents, node.token.offset, node.token.contents.length()) });
        set.push_back(inst);
    }
    break;
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
            to_assembly(contents, previous_node, set);
        }
    }
    }
}

bool sqf::parser::sqf::parser::get_tree(::sqf::runtime::runtime& runtime, ::sqf::parser::sqf::tokenizer& t, ::sqf::parser::sqf::bison::astnode* out)
{
    ::sqf::parser::sqf::bison::parser p(t, *out, *this, runtime);
    bool success = p.parse() == 0;
    return success;
}

std::optional<sqf::runtime::instruction_set> sqf::parser::sqf::parser::parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file)
{
    tokenizer t(contents.begin(), contents.end(), file.physical);
    ::sqf::parser::sqf::bison::astnode res;
    ::sqf::parser::sqf::bison::parser p(t, res, *this, runtime);
    // p.set_debug_level(1);
    bool success = p.parse() == 0;
    if (!success)
    {
        return {};
    }
    std::vector<::sqf::runtime::instruction::sptr> vec;
    to_assembly(contents, res, vec);
    return vec;
}

bool ::sqf::parser::sqf::parser::check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file)
{
    tokenizer t(contents.begin(), contents.end(), file.physical);
    ::sqf::parser::sqf::bison::astnode res;
    ::sqf::parser::sqf::bison::parser p(t, res, *this, runtime);
    bool success = p.parse() == 0;
    return success;
}
