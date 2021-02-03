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
    // ToDo: Check if a corresponding config already exists in confighost before creating it to avoid duplicates

    switch (node.kind)
    {
        case nodetype::CONFIGNODE:
        {
            if (!node.children.empty() && node.children.front().kind == nodetype::CONFIGNODE_PARENTIDENT)
            {
                auto curnode = parent.append_or_replace(node.content, node.children.front().content);
                for (size_t i = 1; i < node.children.size(); i++)
                {
                    auto subnode = node.children[i];
                    apply_to_confighost(subnode, confighost, curnode);
                }
            }
            else
            {
                auto curnode = parent.append_or_replace(node.content);
                for (auto subnode : node.children)
                {
                    apply_to_confighost(subnode, confighost, curnode);
                }
            }
        } break;
        case nodetype::DELETENODE:
        {
            parent.delete_inherited_or_replace(node.content);
        } break;
        case nodetype::VALUENODE_ADDARRAY:
        case nodetype::VALUENODE:
        {
            auto curnode = parent.append_or_replace(node.content);

            for (auto& subnode : node.children)
            {
                apply_to_confighost(subnode, confighost, curnode);
            }
            if (node.kind == nodetype::VALUENODE_ADDARRAY)
            {
                auto parent_inherited = curnode.parent_logical().parent_inherited();
                auto valuefield = parent_inherited / node.content;
                if (!valuefield.empty())
                {
                    auto self = curnode->value.data_try<sqf::types::d_array>();
                    auto other = valuefield->value.data_try<sqf::types::d_array>();
                    if (self.get() && other.get())
                    {
                        self->insert(self->end(), other->begin(), other->end());
                    }
                }
            }
        } break;
        case nodetype::STRING:
        parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_string>(node.content)));
        break;
        case nodetype::NUMBER:
        parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_scalar>((double)std::stod(node.content))));
        break;
        case nodetype::HEXNUMBER:
        parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_scalar>((int64_t)std::stol(node.content, nullptr, 16))));
        break;
        case nodetype::LOCALIZATION:
        parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_string>(node.content)));
        break;
        case nodetype::ARRAY:
        {
            std::vector<sqf::runtime::value> values;
            for (auto& subnode : node.children)
            {
                apply_to_confighost(subnode, confighost, parent);
                values.push_back(parent->value);
            }
            parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_array>(values)));
        } break;
        case nodetype::VALUE:
        break;
        default:
        {
            for (auto& subnode : node.children)
            {
                apply_to_confighost(subnode, confighost, parent);
            }
        }
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
