#pragma once
#include "../../runtime/parser/sqf.h"
#include "../../runtime/logging.h"
#include "../../runtime/diagnostics/diag_info.h"
#include "../../runtime/fileio.h"
#include "../../runtime/util.h"
#include "../../runtime/value.h"
#include "../../runtime/instruction_set.h"
#include "tokenizer.hpp"

#include <string>
#include <string_view>
#include <vector>



namespace sqf::parser::assembly::bison
{
    struct astnode;
}
namespace sqf::parser::assembly
{
    class parser : public ::sqf::runtime::parser::sqf, public CanLog
    {
    private:
        ::sqf::runtime::value get_value(::sqf::runtime::runtime& runtime, std::string_view contents, const ::sqf::parser::assembly::bison::astnode& node);
        void to_assembly(::sqf::runtime::runtime& runtime, std::string_view contents, const ::sqf::parser::assembly::bison::astnode& node, std::vector<::sqf::runtime::instruction::sptr>& set);
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
        virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;
        virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;
    };
}
