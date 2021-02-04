#include "config_parser.hpp"
#include "tokenizer.hpp"
#include "parser.tab.hh"

#include "../../runtime/d_array.h"
#include "../../runtime/d_string.h"
#include "../../runtime/d_scalar.h"
#include <algorithm>
#include <charconv>
#include <sstream>


#ifdef DF__SQF_CONFIG__REPORT_PROGRESS_BY_LINE
#include <iostream>
#endif // DF__SQF_CONFIG__REPORT_PROGRESS_BY_LINE



namespace err = logmessage::config;

bool sqf::parser::config::parser::apply_to_confighost(::sqf::parser::config::bison::astnode& node, ::sqf::runtime::confighost& confighost, ::sqf::runtime::confignav parent)
{
    using namespace std::string_literals;
    // ToDo: Check if a corresponding config already exists in confighost before creating it to avoid duplicates

    switch (node.kind)
    {
        
           case ::sqf::parser::config::bison::astkind::CLASS_DEF: {
               parent.append_or_replace(node.children[0].token.contents);
           } break;
           case ::sqf::parser::config::bison::astkind::CLASS_DEF_EXT: {
               auto nav = parent.append_or_replace(node.children[0].token.contents, node.children[1].token.contents);
               if (nav.parent_inherited().empty())
               {
                   __log(err::InheritedParentNotFound({ *node.token.path, node.token.line, node.token.column }, node.children[0].token.contents, node.children[1].token.contents));
               }
           } break;
           case ::sqf::parser::config::bison::astkind::CLASS: {
               auto nav = parent.append_or_replace(node.children[0].token.contents);
               for (auto subnode : node.children[1].children)
               {
                   apply_to_confighost(subnode, confighost, nav);
               }
           } break;
           case ::sqf::parser::config::bison::astkind::CLASS_EXT: {
               auto nav = parent.append_or_replace(node.children[0].token.contents, node.children[1].token.contents);
               if (nav.parent_inherited().empty())
               {
                   __log(err::InheritedParentNotFound({ *node.token.path, node.token.line, node.token.column }, node.children[0].token.contents, node.children[1].token.contents));
               }
               for (auto subnode : node.children[2].children)
               {
                   apply_to_confighost(subnode, confighost, nav);
               }
           } break;
           case ::sqf::parser::config::bison::astkind::DELETE_CLASS: {
               parent.delete_inherited_or_replace(node.children[0].token.contents);
           } break;
           case ::sqf::parser::config::bison::astkind::FIELD: {
               auto nav = parent.append_or_replace(node.children[0].token.contents);
               apply_to_confighost(node.children[1], confighost, nav);
           } break;
           case ::sqf::parser::config::bison::astkind::FIELD_ARRAY: {
               auto nav = parent.append_or_replace(node.children[0].token.contents);
               apply_to_confighost(node.children[1], confighost, nav);
           } break;
           case ::sqf::parser::config::bison::astkind::FIELD_ARRAY_APPEND: {
               auto nav = parent.append_or_replace(node.children[0].token.contents);
               apply_to_confighost(node.children[1], confighost, nav);

               auto parent_inherited = nav.parent_logical().parent_inherited();
               auto inherited_value_field = parent_inherited / std::string(node.children[0].token.contents.data(), node.children[0].token.contents.length());
               if (!inherited_value_field.empty())
               {
                   auto self = nav->value.data_try<sqf::types::d_array>();
                   auto inherited = inherited_value_field->value.data_try<sqf::types::d_array>();
                   if (self.get() && inherited.get())
                   {
                       self->insert(self->begin(), inherited->begin(), inherited->end());
                   }
               }
           } break;
           case ::sqf::parser::config::bison::astkind::NUMBER_DECIMAL: {
               try
               {
                   auto d = (double)std::stod(std::string(node.token.contents));
                   parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_scalar>(d)));
               }
               catch (std::out_of_range&)
               {
                   parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_scalar>(std::nanf(""))));
               }
           } break;
           case ::sqf::parser::config::bison::astkind::NUMBER_HEXADECIMAL: {
               try
               {
                   auto str = std::string(node.token.contents);
                   if (str[0] == '$') { str = "0x"s.append(str.substr(1)); }
                   auto hexnum = (int64_t)std::stol(str, nullptr, 16);
                   parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_scalar>(hexnum)));
               }
               catch (std::out_of_range&)
               {
                   parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_scalar>(std::nanf(""))));
               }
           } break;
           case ::sqf::parser::config::bison::astkind::STRING: {
               auto transformed = sqf::types::d_string::from_sqf(node.token.contents);
               parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_string>(transformed)));
           } break;
           case ::sqf::parser::config::bison::astkind::ARRAY: {
               std::vector<sqf::runtime::value> values;
               for (auto& subnode : node.children)
               {
                   // Abuse parents value for setting a local value that is not the actual value
                   apply_to_confighost(subnode, confighost, parent);
                   values.push_back(parent->value);
               }
               // Set parents value to the actual value
               parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_array>(values)));
           } break;
           case ::sqf::parser::config::bison::astkind::ANYSTRING: {
               auto start = node.children.front().token.contents.data();
               auto end = node.children.back().token.contents.data() + node.children.back().token.contents.length();
               std::string str = { start, end };
               parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_string>(str)));
           } break;



           case ::sqf::parser::config::bison::astkind::IDENT:
           case ::sqf::parser::config::bison::astkind::ANY: {
               auto start = node.token.contents.data();
               auto end = node.token.contents.data() + node.token.contents.length();
               std::string str = { start, end };
               parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_string>(str)));
           } break;
           case ::sqf::parser::config::bison::astkind::STATEMENTS:
           case ::sqf::parser::config::bison::astkind::ENDOFFILE:
           case ::sqf::parser::config::bison::astkind::INVALID:
           case ::sqf::parser::config::bison::astkind::__TOKEN:
           case ::sqf::parser::config::bison::astkind::NA:
        default:
        for (auto& subnode : node.children)
        {
            apply_to_confighost(subnode, confighost, parent);
        }
        break;
    }
    return true;
}


bool sqf::parser::config::parser::check_syntax(std::string contents, ::sqf::runtime::fileio::pathinfo pathinfo)
{
    tokenizer t(contents.begin(), contents.end(), pathinfo.physical);
    ::sqf::parser::config::bison::astnode res;
    ::sqf::parser::config::bison::parser p(t, res, *this);
    bool success = p.parse() == 0;
    return success;
}

bool sqf::parser::config::parser::parse(::sqf::runtime::confighost& target, std::string contents, ::sqf::runtime::fileio::pathinfo pathinfo)
{
    tokenizer t(contents.begin(), contents.end(), pathinfo.physical);
    ::sqf::parser::config::bison::astnode res;
    ::sqf::parser::config::bison::parser p(t, res, *this);
    // p.set_debug_level(1);
    bool success = p.parse() == 0;
    if (!success)
    {
        return {};
    }
    std::vector<::sqf::runtime::instruction::sptr> vec;
    auto root = target.root();
    return apply_to_confighost(res, target, root);
}
