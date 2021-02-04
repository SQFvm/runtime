#pragma once
#include "../../runtime/parser/config.h"
#include "../../runtime/logging.h"
#include "../../runtime/diagnostics/diag_info.h"
#include "../../runtime/fileio.h"
#include "../../runtime/util.h"
#include "../../runtime/instruction_set.h"
#include "tokenizer.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <runtime/confighost.h>



namespace sqf::parser::config::bison
{
    struct astnode;
}
namespace sqf::parser::config
{
    class parser : public ::sqf::runtime::parser::config, public CanLog
    {
    private:
        bool apply_to_confighost(::sqf::parser::config::bison::astnode& node, ::sqf::runtime::confighost& confighost, ::sqf::runtime::confignav parent);
    public:
        parser(Logger& logger) : CanLog(logger)
        {
        }
        void __log(LogMessageBase&& msg) const
        {
            log(msg);
        }
        bool get_tree(::sqf::runtime::runtime& runtime, tokenizer& t, bison::astnode* out);
        virtual ~parser() override { };
        virtual bool check_syntax(std::string contents, ::sqf::runtime::fileio::pathinfo pathinfo) override;
        virtual bool parse(::sqf::runtime::confighost& target, std::string contents, ::sqf::runtime::fileio::pathinfo pathinfo) override;
    };
}
