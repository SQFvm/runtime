#pragma once
#include "../runtime/parser/sqf.h"
#include "../runtime/logging.h"
#include "../runtime/diagnostics/diag_info.h"
#include "../runtime/fileio.h"
#include "../runtime/util.h"
#include "../runtime/instruction_set.h"

#include <string>
#include <string_view>
#include <vector>


namespace sqf::sqc::bison
{
    struct astnode;
}
namespace sqf::sqc
{
    namespace util
    {
        class setbuilder;
    }
    class parser : public ::sqf::runtime::parser::sqf, public CanLog
    {
    private:
        struct emplace
        {
            std::string ident;
            std::string replace;
        };
        constexpr static const char* key_scopename_function = "___sqc_func";
        constexpr static const char* key_self = "___self";
        void to_assembly(::sqf::runtime::runtime& runtime, util::setbuilder& set, std::vector<emplace>& locals, const ::sqf::sqc::bison::astnode& current_node);
    public:
        parser(Logger& logger) : CanLog(logger)
        {
        }
        void __log(LogMessageBase&& msg) const
        {
            log(msg);
        }
        virtual ~parser() override { };
        virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;
        virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;

        static std::string to_sqc(const ::sqf::runtime::instruction_set& set);
    };
}
