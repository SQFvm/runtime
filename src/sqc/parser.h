#pragma once
#include "tokenizer.h"
#include "../runtime/parser/sqf.h"
#include "../cli/colors.h"

#include <functional>
#include <vector>

#include <iostream>
#include <set>

namespace sqf::sqc
{
    class parser : ::sqf::runtime::parser::sqf
    {
    private:
        class instance
        {
            struct error
            {
                sqc::tokenizer::token unmatched;
                sqc::tokenizer::etoken expected;
            };
            sqc::tokenizer& m_tokenizer;
            std::vector<sqc::tokenizer::token> m_tokens;
            using iterator = std::vector<sqc::tokenizer::token>::iterator;
            std::vector<error> m_errors;
            std::vector<std::string_view> m_path;


            struct path__
            {
                instance* i;
                path__(instance* i, const char* s) : i(i) { i->m_path.push_back(s); }
                ~path__() { i->m_path.pop_back(); }
            };


            template<size_t TL> constexpr auto concat_size() { return TL; }
            template<size_t TL, size_t... TVar> constexpr auto concat_size() { return concat_size<TVar...>() + TL; }
            template<typename T, size_t TL, size_t TR>
            constexpr auto concat_(std::array<T, TL> l, std::array<T, TR> r)
            {
                std::array<T, TL + TR> arr;
                for (size_t i = 0; i < TL; i++)
                {
                    arr[i] = l[i];
                }
                for (size_t i = TL; i < TL + TR; i++)
                {
                    arr[i] = r[i - TL];
                }
                return arr;
            }
            template<typename T, size_t TL>
            constexpr auto concat_(std::array<T, TL> l, T r)
            {
                std::array<T, TL + 1> arr;
                for (size_t i = 0; i < TL; i++)
                {
                    arr[i] = l[i];
                }
                arr[TR] = r;
                return arr;
            }
            template<typename T, size_t TR>
            constexpr auto concat_(T l, std::array<T, TR> r)
            {
                std::array<T, 1 + TR> arr;
                arr[0] = l;
                for (size_t i = 1; i < 1 + TR; i++)
                {
                    arr[i] = r[i - 1];
                }
                return arr;
            }
            template<typename T>
            constexpr auto concat(T t)
            {
                return t;
            }
            template<typename T, typename... TVar>
            constexpr auto concat(T t, TVar... args)
            {
                auto arrb = concat<TVar...>(args...);
                auto res = concat_(t, arrb);
                return res;
            }
            bool match(iterator& it, std::initializer_list<std::function<iterator(iterator)>> funcs)
            {
                for (auto func : funcs)
                {
                    auto res = func(it);
                    if (it != res)
                    {
                        it = res;
                        return true;
                    }
                }
                return false;
            }
            template<tokenizer::etoken T>
            bool match(iterator& it)
            {
                auto tab = std::string(it - m_tokens.begin(), ' ');
                if (it->type == T)
                {
                    // std::cout << tab << console::color::foreground_yellow() << m_tokenizer.to_string(it->type) << ": " << console::color::reset();
                    // std::cout << console::color::foreground_green() << "Matched token " << m_tokenizer.to_string(T) << console::color::reset() << " '" << it->contents << "'" << std::endl;
                    ++it;
                    return true;
                }
                // std::cout << tab << console::color::foreground_yellow() << m_tokenizer.to_string(it->type) << ": " << console::color::reset();
                // std::cout << console::color::foreground_red() << "Failed to match token " << m_tokenizer.to_string(T) << console::color::reset() << " '" << it->contents << "'" << std::endl;
                m_errors.push_back({ *it, T });
                return false;
            }
            template<tokenizer::etoken T>
            bool try_match(iterator it)
            {
                if (it->type == T)
                {
                    return true;
                }
                return false;
            }
            bool recover(iterator& it, std::function<bool(iterator& it)> func)
            {
            start:
                m_errors.clear();
                auto res = func(it);
                if (!res)
                {
                    std::cout << "MATCH ERROR ON " << m_tokenizer.to_string(m_errors.front().unmatched.type) << " `" << m_errors.front().unmatched.contents << "`";
                    bool eof = false;
                    std::set<tokenizer::etoken> s;
                    for (size_t i = 0; i < m_errors.size(); i++)
                    {
                        auto unmatched = m_errors[i].unmatched;
                        auto expected = m_errors[i].expected;
                        if (s.find(expected) != s.end()) { continue; }
                        s.insert(expected);
                        if (i != 0)
                        {
                            std::cout << " OR" << std::endl;
                        }
                        std::cout << "[L" << std::setw(5) << unmatched.line
                            << "|C" << std::setw(5) << unmatched.column
                            << "|O" << std::setw(7) << unmatched.offset << "]" <<
                            "    " << "Expected '" << m_tokenizer.to_string(expected) << "'";
                        if (unmatched.type == sqc::tokenizer::etoken::eof)
                        {
                            eof = true;
                        }
                    }
                    std::cout << std::endl;
                    for (auto it : m_path)
                    {
                        std::cout << it << " << ";
                    }
                    std::cout << "HERE" << std::endl;
                    ++it;
                    if (!eof)
                    {
                        goto start;
                    }
                }
                return res;
            }

            // SQC = %empty
            //     | STATEMENTS
            //     ;
            bool r_sqc(iterator& current)
            {
                path__ __p(this, "SQC");
                return m_tokens.empty() || match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_statements(it)) { return current; }
                        return it;
                    }
                    });
            }
            // STATEMENTS = STATEMENT STATEMENTS
            //            | STATEMENT
            //            ;
            bool r_statements(iterator& current)
            {
                path__ __p(this, "STATEMENTS");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_statement(it)) { return current; }
                        if (!r_statements(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_statement(it)) { return current; }
                        return it;
                    }
                    });
            }
            // STATEMENT = VARDECL ";"
            //           | FUNCDECL
            //           | return EXP01 ";"
            //           | return ";"
            //           | throw EXP01 ";"
            //           | IF
            //           | FOR
            //           | WHILE
            //           | TRYCATCH
            //           | SWITCH
            //           | EXP01 ";"
            //           | ASSIGNMENT ";"
            //           | ";"
            //           ;
            bool r_statement(iterator& actual)
            {
                path__ __p(this, "STATEMENT");
                return recover(actual, [&](iterator& current) { return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_vardecl(it)) { return current; }
                        if (!match<tokenizer::etoken::s_semicolon>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_funcdecl(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_return>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_semicolon>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_throw>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_semicolon>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_return>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_semicolon>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_if(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_for(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_while(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_trycatch(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_switch(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_semicolon>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_assignment(it)) { return current; }
                        if (!match<tokenizer::etoken::s_semicolon>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_semicolon>(it)) { return current; }
                        return it;
                    }
                    });
                });
            }
            // ASSIGNMENT = ident "=" EXP01
            //            ;
            bool r_assignment(iterator& current)
            {
                path__ __p(this, "ASSIGNMENT");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_equal>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    }
                    });
            }
            // VARDECL = let ident "=" EXP01
            //         | let ident be EXP01
            //         | private ident "=" EXP01
            //         | private ident be EXP01
            //         ;
            bool r_vardecl(iterator& current)
            {
                path__ __p(this, "VARDECL");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_let>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_equal>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_let>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_be>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_private>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_equal>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_private>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_be>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    });
            }
            // FUNCDECL = function ident FUNCHEAD CODEBLOCK
            //          ;
            bool r_funcdecl(iterator& current)
            {
                path__ __p(this, "FUNCDECL");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_function>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!r_funchead(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    }
                    });
            }
            // FUNCTION = function FUNCHEAD CODEBLOCK
            //          ;
            bool r_function(iterator& current)
            {
                path__ __p(this, "FUNCTION");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_function>(it)) { return current; }
                        if (!r_funchead(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    }
                    });
            }
            // FUNCHEAD = "(" ")"
            //          | "(" ARGLIST ")"
            //          ;
            bool r_funchead(iterator& current)
            {
                path__ __p(this, "FUNCHEAD");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!r_arglist(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        return it;
                    }
                    });
            }
            // ARGLIST = ident "," ARGLIST
            //         | ident ","
            //         | ident
            //         ;
            bool r_arglist(iterator& current)
            {
                path__ __p(this, "ARGLIST");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_comma>(it)) { return current; }
                        if (!r_arglist(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_comma>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        return it;
                    }
                    });
            }
            // CODEBLOCK = "{" "}"
            //           | "{" STATEMENTS "}"
            //           | STATEMENT
            //           ;
            bool r_codeblock(iterator& current)
            {
                path__ __p(this, "CODEBLOCK");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_curlyo>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_curlyc>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_curlyo>(it)) { return current; }
                        if (!r_statements(it)) { return current; }
                        if (!match<tokenizer::etoken::s_curlyc>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_statement(it)) { return current; }
                        return it;
                    },
                    });
            }
            // IF = if "(" EXP01 ")" CODEBLOCK else CODEBLOCK
            //    | if "(" EXP01 ")" CODEBLOCK
            //    ;
            bool r_if(iterator& current)
            {
                path__ __p(this, "IF");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_if>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        if (!match<tokenizer::etoken::t_else>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_if>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    }
                    });
            }
            // FOR = for ident from EXP01 to EXP01 step EXP01 do CODEBLOCK
            //     | for ident from EXP01 to EXP01 CODEBLOCK
            //     | for "(" ident : EXP01 ")" CODEBLOCK
            //     ;
            bool r_for(iterator& current)
            {
                path__ __p(this, "FOR");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_for>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_from>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::t_to>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::t_step>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::t_do>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_for>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_from>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::t_to>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::t_do>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_for>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_colon>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    }
                    });
            }
            // WHILE = "while" "(" EXP01 ")" CODEBLOCK
            //       | "do" CODEBLOCK "while" "(" EXP01 ")"
            //       ;
            bool r_while(iterator& current)
            {
                path__ __p(this, "WHILE");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_while>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_do>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        if (!match<tokenizer::etoken::t_while>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_semicolon>(it)) { return current; }
                        return it;
                    }
                    });
            }
            // TRYCATCH = try CODEBLOCK catch "(" ident ")" CODEBLOCK
            //          ;
            bool r_trycatch(iterator& current)
            {
                path__ __p(this, "TRYCATCH");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_try>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        if (!match<tokenizer::etoken::t_catch>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        if (!match<tokenizer::etoken::t_catch>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    }
                    });
            }
            // SWITCH = switch "(" EXP01 ")" "{" CASELIST "}"
            //        ;
            bool r_switch(iterator& current)
            {
                path__ __p(this, "SWITCH");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_switch>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_curlyo>(it)) { return current; }
                        if (!r_caselist(it)) { return current; }
                        if (!match<tokenizer::etoken::s_curlyc>(it)) { return current; }
                        return it;
                    }
                    });
            }
            // CASELIST = CASE CASELIST
            //          | CASE
            //          ;
            bool r_caselist(iterator& current)
            {
                path__ __p(this, "CASELIST");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_case(it)) { return current; }
                        if (!r_caselist(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_case(it)) { return current; }
                        return it;
                    }
                    });
            }
            // CASE = case EXP01 ":" CODEBLOCK
            //      | case EXP01 ":"
            //      | default ":" CODEBLOCK
            //      | default ":"
            //      ;
            bool r_case(iterator& current)
            {
                path__ __p(this, "CASE");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_case>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_colon>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_case>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_colon>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_default>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_colon>(it)) { return current; }
                        if (!r_codeblock(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_default>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_colon>(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP01: EXP02 "?" EXP01 ":" EXP01
            //      | EXP02
            //      ;
            bool r_exp01(iterator& current)
            {
                path__ __p(this, "EXP01");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp02(it)) { return current; }
                        if (!match<tokenizer::etoken::s_questionmark>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_colon>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp02(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP02: EXP03 "||" EXP01
            //      | EXP03
            //      ;
            bool r_exp02(iterator& current)
            {
                path__ __p(this, "EXP02");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp03(it)) { return current; }
                        if (!match<tokenizer::etoken::s_oror>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp03(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP03: EXP04 "&&" EXP01
            //      | EXP04
            //      ;
            bool r_exp03(iterator& current)
            {
                path__ __p(this, "EXP03");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp04(it)) { return current; }
                        if (!match<tokenizer::etoken::s_andand>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp04(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP04: EXP05 "!=" EXP01
            //      | EXP05 "==" EXP01
            //      | EXP05
            //      ;
            bool r_exp04(iterator& current)
            {
                path__ __p(this, "EXP04");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp05(it)) { return current; }
                        if (!match<tokenizer::etoken::s_notequal>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp05(it)) { return current; }
                        if (!match<tokenizer::etoken::s_equalequal>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp05(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP05: EXP06 ">=" EXP01
            //      | EXP06 "<=" EXP01
            //      | EXP06 "<"  EXP01
            //      | EXP06 ">"  EXP01
            //      | EXP06
            //      ;
            bool r_exp05(iterator& current)
            {
                path__ __p(this, "EXP05");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp06(it)) { return current; }
                        if (!match<tokenizer::etoken::s_greaterthenequal>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp06(it)) { return current; }
                        if (!match<tokenizer::etoken::s_lessthenequal>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp06(it)) { return current; }
                        if (!match<tokenizer::etoken::s_lessthen>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp06(it)) { return current; }
                        if (!match<tokenizer::etoken::s_greaterthen>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp06(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP06: EXP07 "-" EXP01
            //      | EXP07 "+" EXP01
            //      | EXP07
            //      ;
            bool r_exp06(iterator& current)
            {
                path__ __p(this, "EXP06");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp07(it)) { return current; }
                        if (!match<tokenizer::etoken::s_minus>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp07(it)) { return current; }
                        if (!match<tokenizer::etoken::s_plus>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp07(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP07: EXP08 "%" EXP01
            //      | EXP08 "*" EXP01
            //      | EXP08 "/" EXP01
            //      | EXP08
            //      ;
            bool r_exp07(iterator& current)
            {
                path__ __p(this, "EXP07");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp08(it)) { return current; }
                        if (!match<tokenizer::etoken::s_percent>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp08(it)) { return current; }
                        if (!match<tokenizer::etoken::s_star>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp08(it)) { return current; }
                        if (!match<tokenizer::etoken::s_slash>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp08(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP08: "!" EXPP
            //      | EXPP
            //      ;
            bool r_exp08(iterator& current)
            {
                path__ __p(this, "EXP08");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_exclamationmark>(it)) { return current; }
                        if (!r_expp(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_expp(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXPP = "(" EXP01 ")" "." ident "(" EXPLIST ")"
            //      | "(" EXP01 ")"
            //      | ident "(" EXPLIST ")"
            //      | nil
            //      | VALUE "." ident "(" EXPLIST ")"
            //      | VALUE
            //      ;
            bool r_expp(iterator& current)
            {
                path__ __p(this, "EXPP");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_dot>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundo>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_roundc>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_nil>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_value(it)) { return current; }
                        if (!match<tokenizer::etoken::s_dot>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_value(it)) { return current; }
                        return it;
                    }
                    });
            }
            // VALUE = FUNCTION
            //       | string
            //       | ARRAY
            //       | number
            //       | true
            //       | false
            //       | ident
            //       ;
            bool r_value(iterator& current)
            {
                path__ __p(this, "VALUE");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_function(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_string>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_array(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_number>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_true>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_false>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        return it;
                    }
                    });
            }
            // ARRAY = "[" EXPLIST "]"
            //       | "[" "]"
            //       ;
            bool r_array(iterator& current)
            {
                path__ __p(this, "ARRAY");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_edgeo>(it)) { return current; }
                        if (!r_explist(it)) { return current; }
                        if (!match<tokenizer::etoken::s_edgec>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_edgeo>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_edgec>(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXPLIST = EXP01 "," EXPLIST
            //         | EXP01 ","
            //         | EXP01
            //         ;
            bool r_explist(iterator& current)
            {
                path__ __p(this, "EXPLIST");
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_comma>(it)) { return current; }
                        if (!r_arglist(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_comma>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp01(it)) { return current; }
                        return it;
                    }
                    });
            }
        public:
            instance(sqc::tokenizer& tokenizer) :
                m_tokenizer(tokenizer),
                m_tokens()
            {
                sqc::tokenizer::token t;
                while ((t = tokenizer.next()).type != sqc::tokenizer::etoken::eof)
                {
                    switch (t.type)
                    {
                    case sqc::tokenizer::etoken::i_comment_block:
                    case sqc::tokenizer::etoken::i_comment_line:
                    case sqc::tokenizer::etoken::i_whitespace:
                        break;
                    default:
                        m_tokens.push_back(t);
                        break;
                    }
                }
                m_tokens.push_back(t);
            }
            bool test()
            {
                auto it = m_tokens.begin();
                return r_sqc(it);
            }
        };

    public:

        bool test(std::string contents)
        {
            sqc::tokenizer t(contents.begin(), contents.end());
            instance i(t);
            return i.test();
        }


        // Inherited via sqf
        virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;

        virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;

    };
}