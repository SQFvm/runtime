#pragma once
#include "../fileio.h"
#include "../instruction_set.h"

namespace sqf
{
    namespace runtime
    {
        class runtime;
        class confighost;
        namespace parser
        {
            class config
            {
            public:
                virtual ~config() {}
                /// Checks the syntax of the given contents.
                /// @param  contents    The actual file contents.
                /// @param  pathinfo    The path to the file.
                /// @returns            True on success. False on Failure.
                virtual bool check_syntax(std::string contents, ::sqf::runtime::fileio::pathinfo pathinfo) = 0;
                /// Parses the given contents.
                /// @param  target      The targeted confighost, the config should be parsed into.
                /// @param  contents    The actual file contents.
                /// @param  pathinfo    The path to the file.
                /// @returns            True on success. False on Failure.
                virtual bool parse(::sqf::runtime::confighost& target, std::string contents, ::sqf::runtime::fileio::pathinfo pathinfo) = 0;
            };
        }
    }
    namespace parser::config
    {
        class disabled : public ::sqf::runtime::parser::config
        {
        public:
            virtual ~disabled() override { return; }
            virtual bool check_syntax(std::string contents, ::sqf::runtime::fileio::pathinfo pathinfo) override { return false; }
            virtual bool parse(::sqf::runtime::confighost& target, std::string contents, ::sqf::runtime::fileio::pathinfo pathinfo) override { return false; }
        };
    }
}