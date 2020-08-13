#pragma once
#include "tokenizer.h"
#include "../runtime/parser/sqf.h"

#include <functional>
#include <deque>

namespace sqf::sqc
{
    class parser : ::sqf::runtime::parser::sqf
    {
    private:
        sqc::tokenizer& m_tokenizer;
        std::deque<sqc::tokenizer::token> m_stack;


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

        /*
            Prefix Description:
                [ c_ ] Describes a group of methods that check if their [ m_ ] can check all possible paths yet.
                [ m_ ] Describes a group of methods that check if their [ r_ ] can be matched yet.
                [ r_ ] Describes a group of methods that represent their ruleset.
        */



        // SQC = %empty
        //     | STATEMENTS
        //     ;
        bool c_sqc() const { return m_stack.size() == 0 && c_statements(); }
        bool m_sqc() const;
        void r_sqc();
        // STATEMENTS = STATEMENT
        //            | STATEMENT STATEMENTS
        //            ;
        bool c_statements() const { return c_statement(); }
        bool m_statements() const;
        void r_statements();
        // STATEMENT = VARDECL ";"
        //           | FUNCDECL
        //           | IF
        //           | FOR
        //           | WHILE
        //           | TRYCATCH
        //           | SWITCH
        //           | EXP0 ";"
        //           | ASSIGNMENT
        //           | ";"
        //           ;
        bool c_statement() const;
        bool m_statement() const;
        void r_statement();
        // ASSIGNMENT = ident "=" EXP0
        //            ;
        bool c_assignment() const;
        bool m_assignment() const;
        void r_assignment();
        // VARDECL = let ident "=" EXP0
        //         | let ident be EXP0
        //         | private ident be EXP0
        //         | private ident "=" EXP0
        //         ;
        bool c_vardecl() const;
        bool m_vardecl() const;
        void r_vardecl();
        // FUNCDECL = function ident FUNCHEAD CODEBLOCK
        //          ;
        bool c_funcdecl() const;
        bool m_funcdecl() const;
        void r_funcdecl();
        // FUNCTION = function FUNCHEAD CODEBLOCK
        //          ;
        bool c_function() const;
        bool m_function() const;
        void r_function();
        // FUNCHEAD = "(" ")"
        //          | "(" ARGLIST ")"
        //          ;
        bool c_funchead() const;
        bool m_funchead() const;
        void r_funchead();
        // ARGLIST = ident
        //         | ident "," ARGLIST
        //         ;
        bool c_arglist() const;
        bool m_arglist() const;
        void r_arglist();
        // CODEBLOCK = STATEMENT ";"
        //           | "{" "}"
        //           | "{" STATEMENTS "}"
        //           ;
        bool c_codeblock() const;
        bool m_codeblock() const;
        void r_codeblock();
        // IF = if "(" EXP01 ")" CODEBLOCK
        //    | if "(" EXP01 ")" CODEBLOCK else "(" EXP01 ")" CODEBLOCK
        //    ;
        bool c_if() const;
        bool m_if() const;
        void r_if();
        // FOR = for "(" EXP01 ";" EXP01 ";" EXP01 ")" CODEBLOCK
        //     | for ident from number to number CODEBLOCK
        //     | for "(" ident : ident ")" CODEBLOCK
        //     ;
        bool c_for() const;
        bool m_for() const;
        void r_for();
        // WHILE = "while" "(" EXP01 ")" CODEBLOCK
        //       | "do" CODEBLOCK "while" "(" EXP01 ")"
        //       ;
        bool c_while() const;
        bool m_while() const;
        void r_while();
        // TRYCATCH = try CODEBLOCK catch "(" ident ")" CODEBLOCK
        //          ;
        bool c_trycatch() const;
        bool m_trycatch() const;
        void r_trycatch();
        // SWITCH = switch "(" EXP01 ")" "{" CASELIST "}"
        //        ;
        bool c_switch() const;
        bool m_switch() const;
        void r_switch();
        // CASELIST = CASE
        //          | CASE CASELIST
        //          ;
        bool c_caselist() const;
        bool m_caselist() const;
        void r_caselist();
        // CASE = case EXP01 ":" CODEBLOCK
        //      | case EXP01 ":"
        //      | default ":" CODEBLOCK
        //      | default ":"
        //      ;
        bool c_case() const;
        bool m_case() const;
        void r_case();
        // EXP01: EXP02
        //      | EXP02 "?" EXP01 ":" EXP01
        //      ;
        bool c_exp01() const;
        bool m_exp01() const;
        void r_exp01();
        // EXP02: EXP03
        //      | EXP03 "||" EXP01
        //      ;
        bool c_exp02() const;
        bool m_exp02() const;
        void r_exp02();
        // EXP03: EXP04
        //      | EXP04 "&&" EXP01
        //      ;
        bool c_exp03() const;
        bool m_exp03() const;
        void r_exp03();
        // EXP04: EXP05
        //      | EXP05 "==" EXP01
        //      | EXP05 "!=" EXP01
        //      ;
        bool c_exp04() const;
        bool m_exp04() const;
        void r_exp04();
        // EXP05: EXP06
        //      | EXP06 "<"  EXP01
        //      | EXP06 "<=" EXP01
        //      | EXP06 ">"  EXP01
        //      | EXP06 ">=" EXP01
        //      ;
        bool c_exp05() const;
        bool m_exp05() const;
        void r_exp05();
        // EXP06: EXP07
        //      | EXP07 "+" EXP01
        //      | EXP07 "-" EXP01
        //      ;
        bool c_exp06() const;
        bool m_exp06() const;
        void r_exp06();
        // EXP07: EXP08
        //      | EXP08 "*" EXP01
        //      | EXP08 "/" EXP01
        //      | EXP08 "%" EXP01
        //      ;
        bool c_exp07() const;
        bool m_exp07() const;
        void r_exp07();
        // EXP08: EXPP
        //      | "!" EXPP
        //      ;
        bool c_exp08() const;
        bool m_exp08() const;
        void r_exp08();
        // EXPP = return EXP01
        //      | return
        //      | throw EXP01
        //      | "(" EXP01 ")" "." ident "(" EXPLIST ")"
        //      | "(" EXP01 ")"
        //      | ident "(" EXPLIST ")"
        //      | null
        //      | nil
        //      | VALUE "." ident "(" EXPLIST ")"
        //      | VALUE
        //      ;
        bool c_expp() const;
        bool m_expp() const;
        void r_expp();
        // VALUE = FUNCTION
        //       | string
        //       | ARRAY
        //       | number
        //       | true
        //       | false
        //       | ident
        //       ;
        bool c_value() const;
        bool m_value() const;
        void r_value();
        // ARRAY = "[" "]"
        //       | "[" EXPLIST "]"
        //       ;
        bool c_array() const;
        bool m_array() const;
        void r_array();
        // EXPLIST = EXP01
        //         | EXP01 "," EXPLIST
        //         ;
        bool c_explist() const;
        bool m_explist() const;
        void r_explist();

    public:
        parser(sqc::tokenizer& tokenizer) :
            m_tokenizer(tokenizer),
            m_stack() {}



        // Inherited via sqf
        virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;

        virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;

    };
}