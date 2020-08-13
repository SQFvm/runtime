#pragma once
#include "tokenizer.h"
#include "../runtime/parser/sqf.h"

#include <functional>
#include <vector>

namespace sqf::sqc
{
    class parser : ::sqf::runtime::parser::sqf
    {
    private:
        class instance
        {
            sqc::tokenizer& m_tokenizer;
            std::vector<sqc::tokenizer::token> m_tokens;
            using iterator = std::vector<sqc::tokenizer::token>::iterator;


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
                if (it->type == T)
                {
                    ++it;
                    return true;
                }
                return false;
            }

            // SQC = %empty
            //     | STATEMENTS
            //     ;
            bool r_sqc(iterator& current)
            {
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
            //           | ASSIGNMENT
            //           | ";"
            //           ;
            bool r_statement(iterator& current)
            {
                return match(current, {
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
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::s_semicolon>(it)) { return current; }
                        return it;
                    }
                    });
            }
            // ASSIGNMENT = ident "=" EXP01
            //            ;
            bool r_assignment(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        if (!match<tokenizer::etoken::s_semicolon>(it)) { return current; }
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
            // ARGLIST = ident
            //         | ident "," ARGLIST
            //         | ident ","
            //         ;
            bool r_arglist(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!match<tokenizer::etoken::t_ident>(it)) { return current; }
                        return it;
                    },
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
                    }
                    });
            }
            // CODEBLOCK = STATEMENT
            //           | "{" "}"
            //           | "{" STATEMENTS "}"
            //           ;
            bool r_codeblock(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_statement(it)) { return current; }
                        return it;
                    },
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
                    }
                    });
            }
            // IF = if "(" EXP01 ")" CODEBLOCK else CODEBLOCK
            //    | if "(" EXP01 ")" CODEBLOCK
            //    ;
            bool r_if(iterator& current)
            {
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
            // EXP01: EXP02
            //      | EXP02 "?" EXP01 ":" EXP01
            //      ;
            bool r_exp01(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp02(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp02(it)) { return current; }
                        if (!match<tokenizer::etoken::s_questionmark>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        if (!match<tokenizer::etoken::s_colon>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP02: EXP03
            //      | EXP03 "||" EXP01
            //      ;
            bool r_exp02(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp03(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp03(it)) { return current; }
                        if (!match<tokenizer::etoken::s_oror>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP03: EXP04
            //      | EXP04 "&&" EXP01
            //      ;
            bool r_exp03(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp04(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp04(it)) { return current; }
                        if (!match<tokenizer::etoken::s_andand>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP04: EXP05
            //      | EXP05 "==" EXP01
            //      | EXP05 "!=" EXP01
            //      ;
            bool r_exp04(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp05(it)) { return current; }
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
                        if (!match<tokenizer::etoken::s_notequal>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP05: EXP06
            //      | EXP06 "<=" EXP01
            //      | EXP06 "<"  EXP01
            //      | EXP06 ">=" EXP01
            //      | EXP06 ">"  EXP01
            //      ;
            bool r_exp05(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp06(it)) { return current; }
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
                        if (!match<tokenizer::etoken::s_greaterthenequal>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp06(it)) { return current; }
                        if (!match<tokenizer::etoken::s_greaterthen>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP06: EXP07
            //      | EXP07 "+" EXP01
            //      | EXP07 "-" EXP01
            //      ;
            bool r_exp06(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp07(it)) { return current; }
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
                        if (!match<tokenizer::etoken::s_minus>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP07: EXP08
            //      | EXP08 "*" EXP01
            //      | EXP08 "/" EXP01
            //      | EXP08 "%" EXP01
            //      ;
            bool r_exp07(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp08(it)) { return current; }
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
                        if (!match<tokenizer::etoken::s_percent>(it)) { return current; }
                        if (!r_exp01(it)) { return current; }
                        return it;
                    }
                    });
            }
            // EXP08: "!" EXPP
            //      | EXPP
            //      ;
            bool r_exp08(iterator& current)
            {
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
                        if (!r_value(it)) { return current; }
                        if (!match<tokenizer::etoken::t_true>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_value(it)) { return current; }
                        if (!match<tokenizer::etoken::t_false>(it)) { return current; }
                        return it;
                    },
                    [&](iterator it) -> iterator
                    {
                        if (!r_value(it)) { return current; }
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
            // EXPLIST = EXP01
            //         | EXP01 "," EXPLIST
            //         | EXP01 ","
            //         ;
            bool r_explist(iterator& current)
            {
                return match(current, {
                    [&](iterator it) -> iterator
                    {
                        if (!r_exp01(it)) { return current; }
                        return it;
                    },
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
                    }
                    });
            }
        public:
            instance(sqc::tokenizer& tokenizer) :
                m_tokenizer(tokenizer),
                m_tokens() {}
        };

    public:



        // Inherited via sqf
        virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;

        virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;

    };
}