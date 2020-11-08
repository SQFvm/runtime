#pragma once
#include "../fileio.h"
#include "../instruction_set.h"

#include <optional>
#include <algorithm>
#include <string_view>

namespace sqf
{
    namespace runtime
    {
        class runtime;
        namespace parser
        {
            class sqf
            {
            public:
                virtual ~sqf() {};
                virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) = 0;
                virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) = 0;
                static std::string create_code_segment(std::string_view view, size_t off, size_t length)
                {
                    size_t i = off < 15 ? 0 : off - 15;
                    size_t len = 30 + length;
                    if (i < 0)
                    {
                        len += i;
                        i = 0;
                    }
                    for (size_t j = i; j < i + len && j < view.length(); j++)
                    {
                        char wc = view[j];
                        if (wc == '\n')
                        {
                            if (j < off)
                            {
                                i = j + 1;
                            }
                            else
                            {
                                len = j - i;
                                break;
                            }
                        }
                    }

                    std::string spacing(off - i, ' ');
                    std::string postfix(std::max<size_t>(1, length), '^');

                    std::string txt;
                    txt.reserve(len + 1 + spacing.length() + postfix.length() + 1);
                    txt.append(view.substr(i, len));
                    txt.append("\n");
                    txt.append(spacing);
                    txt.append(postfix);
                    txt.append("\n");
                    return txt;
                }
            };
        }
    }
    namespace parser::sqf
    {
        class disabled : public ::sqf::runtime::parser::sqf
        {
        public:
            virtual ~disabled() override { return; };
            virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override { return false; }
            virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override { return {}; }
        };
    }
}