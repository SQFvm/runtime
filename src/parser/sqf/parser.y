

// %skeleton "lalr1.cc" /* -*- C++ -*- */
%language "c++"
%require "3.0"

%define api.value.type variant
%define api.token.constructor
%define api.namespace { ::sqf::parser::sqf::bison }
%code top {
    #include "tokenizer.hpp"
    #include <string>
    #include <vector>
}

%code requires
{
     namespace sqf::runtime
     {
          class runtime;
     }
     namespace sqf::parser::sqf
     {
          class parser;
     }
     namespace sqf::parser::sqf::bison
     {
        enum class astkind
        {
            __TOKEN = -1,
            NA = 0,
            ENDOFFILE,
            INVALID,
            M_LINE,
            I_COMMENT_LINE,
            I_COMMENT_BLOCK,
            I_WHITESPACE,

            T_TRUE,
            T_FALSE,
            T_PRIVATE,

            S_CURLYO,
            S_CURLYC,
            S_ROUNDO,
            S_ROUNDC,
            S_EDGEO,
            S_EDGEC,
            S_SEMICOLON,
            S_COMMA,

            T_OPERATOR,
            T_STRING,
            T_IDENT,
            T_NUMBER
        };
        struct astnode
        {
            ::sqf::parser::sqf::tokenizer::token token;
            astkind kind;
            std::vector<astnode> children;

            astnode() : token(), kind(astkind::NA) { }
            astnode(astkind kind) : token(), kind(kind) { }
            astnode(::sqf::parser::sqf::tokenizer::token t) : token(t), kind(astkind::__TOKEN) { }
            astnode(astkind kind, ::sqf::parser::sqf::tokenizer::token t) : token(t), kind(kind) { }

            void append(astnode node)
            {
                children.push_back(node);
            }
            void append_children(const astnode& other)
            { 
                for (auto node : other.children)
                {
                    append(node); 
                } 
            }
        };
     }
}
%code
{
    
    #include "../../runtime/runtime.h"
     namespace sqf::parser::sqf::bison
     {
          // Return the next token.
          parser::symbol_type yylex (::sqf::runtime::runtime &runtime, ::sqf::parser::sqf::tokenizer&);
     }
}

%lex-param { ::sqf::runtime::runtime &runtime }
%lex-param { ::sqf::parser::sqf::tokenizer &tokenizer }
%parse-param { ::sqf::parser::sqf::tokenizer &tokenizer }
%parse-param { ::sqf::parser::sqf::bison::astnode& result }
%parse-param { ::sqf::parser::sqf::parser& actual }
%parse-param { ::sqf::runtime::runtime &runtime }
%parse-param { std::string fpath }
%locations
%define parse.error verbose

%define api.token.prefix {}



/* Tokens */

%token NA 0
%token FALSE                     "false"
%token TRUE                      "true"
%token PRIVATE                   "private"
%token CURLYO                    "{"
%token CURLYC                    "}"
%token ROUNDO                    "("
%token ROUNDC                    ")"
%token SQUAREO                   "["
%token SQUAREC                   "]"
%token SEMICOLON                 ";"
%token COMMA                     ","
%token EQUAL                     "="

%token <tokenizer::token> OPERATOR_0
%token <tokenizer::token> IDENT_0
%token <tokenizer::token> OPERATOR_1
%token <tokenizer::token> IDENT_1
%token <tokenizer::token> OPERATOR_2
%token <tokenizer::token> IDENT_2
%token <tokenizer::token> OPERATOR_3
%token <tokenizer::token> IDENT_3
%token <tokenizer::token> OPERATOR_4
%token <tokenizer::token> IDENT_4
%token <tokenizer::token> OPERATOR_5
%token <tokenizer::token> IDENT_5
%token <tokenizer::token> OPERATOR_6
%token <tokenizer::token> IDENT_6
%token <tokenizer::token> OPERATOR_7
%token <tokenizer::token> IDENT_7
%token <tokenizer::token> OPERATOR_8
%token <tokenizer::token> IDENT_8
%token <tokenizer::token> OPERATOR_9
%token <tokenizer::token> IDENT_9
%token <tokenizer::token> OPERATOR_U
%token <tokenizer::token> IDENT_U
%token <tokenizer::token> IDENT_N
%token <tokenizer::token> IDENT
%token <tokenizer::token> NUMBER
%token <tokenizer::token> STRING

// %type <::sqf::parser::sqf::bison::astnode> statement statements value value_list code array
// %type <::sqf::parser::sqf::bison::astnode> assignment expression exp0 exp1 exp2 exp3 exp4 
// %type <::sqf::parser::sqf::bison::astnode> exp5 exp6 exp7 exp8 exp9 expu

%start start

%%

/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
start: %empty
     | statements
     ;
statements: statement
          | statements ";" statement
          | statements "," statement
          ;
statement: assignment
         | expression
         ;

value: STRING
     | IDENT_N
     | IDENT
     | NUMBER
     | "true"
     | "false"
     | code
     | array
     ;
value_list: value
          | value_list "," value
          ;
code: "{" statements "}"
    | "{" "}"
    ;
array: "[" value_list "]"
     | "[" "]"
     ;
assignment: "private" IDENT "=" expression
          | IDENT "=" expression
          ;
expression: exp0
          ;
exp0: exp1
    | exp1 OPERATOR_0 exp0;
    | exp1 IDENT_0 exp0;
    ;
exp1: exp2
    | exp2 OPERATOR_1 exp1;
    | exp2 IDENT_1 exp1;
    ;
exp2: exp3
    | exp3 OPERATOR_2 exp2;
    | exp3 IDENT_2 exp2;
    ;
exp3: exp4
    | exp4 OPERATOR_3 exp3;
    | exp4 IDENT_3 exp3;
    ;
exp4: exp5
    | exp5 OPERATOR_4 exp4;
    | exp5 IDENT_4 exp4;
    ;
exp5: exp6
    | exp6 OPERATOR_5 exp5;
    | exp6 IDENT_5 exp5;
    ;
exp6: exp7
    | exp7 OPERATOR_6 exp6;
    | exp7 IDENT_6 exp6;
    ;
exp7: exp8
    | exp8 OPERATOR_7 exp7;
    | exp8 IDENT_7 exp7;
    ;
exp8: exp9
    | exp9 OPERATOR_8 exp8;
    | exp9 IDENT_8 exp8;
    ;
exp9: expu
    | expu OPERATOR_9 exp9;
    | expu IDENT_9 exp9;
    ;
expu: OPERATOR_U expu
    | IDENT_U expu
    | "private" expu
    | "(" expression ")"
    | value
    ;


%%

#include "sqf_parser.hpp"
namespace sqf::parser::sqf::bison
{
     void parser::error (const location_type& loc, const std::string& msg)
     {
          actual.__log(logmessage::sqf::ParseError({ fpath, loc.begin.line, loc.begin.column }, msg));
     }
     inline parser::symbol_type yylex (::sqf::runtime::runtime& runtime, ::sqf::parser::sqf::tokenizer& tokenizer)
     {
         auto token = tokenizer.next();
         parser::location_type loc;
         loc.begin.line = token.line;
         loc.begin.column = token.column;
         loc.end.line = token.line;
         loc.end.column = token.column + token.contents.length();

         switch (token.type)
         {
         case tokenizer::etoken::eof: return parser::make_NA(loc);
         case tokenizer::etoken::invalid: return parser::make_NA(loc);
         case tokenizer::etoken::m_line: return yylex(runtime, tokenizer);
         case tokenizer::etoken::i_comment_line: return yylex(runtime, tokenizer);
         case tokenizer::etoken::i_comment_block: return yylex(runtime, tokenizer);
         case tokenizer::etoken::i_whitespace: return yylex(runtime, tokenizer);
         
         case tokenizer::etoken::t_false: return parser::make_FALSE(loc);
         case tokenizer::etoken::t_private: return parser::make_PRIVATE(loc);
         case tokenizer::etoken::t_true: return parser::make_TRUE(loc);

         case tokenizer::etoken::s_curlyo: return parser::make_CURLYO(loc);
         case tokenizer::etoken::s_curlyc: return parser::make_CURLYC(loc);
         case tokenizer::etoken::s_roundo: return parser::make_ROUNDO(loc);
         case tokenizer::etoken::s_roundc: return parser::make_ROUNDC(loc);
         case tokenizer::etoken::s_edgeo: return parser::make_SQUAREO(loc);
         case tokenizer::etoken::s_edgec: return parser::make_SQUAREC(loc);
         case tokenizer::etoken::s_semicolon: return parser::make_SEMICOLON(loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(loc);

         case tokenizer::etoken::t_ident:
         {
             auto key = std::string(token.contents.begin(), token.contents.end());
             if (runtime.sqfop_exists_binary(key))
             {
                auto bres = runtime.sqfop_binary_by_name(key);
                auto p = bres.begin()->get().precedence();
                switch (p)
                {
                case 1: return parser::make_IDENT_0(token, loc);
                case 2: return parser::make_IDENT_1(token, loc);
                case 3: return parser::make_IDENT_2(token, loc);
                case 4: return parser::make_IDENT_3(token, loc);
                case 5: return parser::make_IDENT_4(token, loc);
                case 6: return parser::make_IDENT_5(token, loc);
                case 7: return parser::make_IDENT_6(token, loc);
                case 8: return parser::make_IDENT_7(token, loc);
                case 9: return parser::make_IDENT_8(token, loc);
                case 10: return parser::make_IDENT_9(token, loc);
                }
             }
             else if (runtime.sqfop_exists_unary(key))
             {
                 return parser::make_IDENT_U(token, loc);
             }
             else if (runtime.sqfop_exists(::sqf::runtime::sqfop_nular::key{key}))
             {
                 return parser::make_IDENT_N(token, loc);
             }
             return parser::make_IDENT(token, loc);
         }
         case tokenizer::etoken::t_operator:
         {
             auto key = std::string(token.contents.begin(), token.contents.end());
             if (runtime.sqfop_exists_binary(key))
             {
                 auto bres = runtime.sqfop_binary_by_name(key);
                 auto p = bres.begin()->get().precedence();
                 switch (p)
                 {
                 case 1: return parser::make_OPERATOR_0(token, loc);
                 case 2: return parser::make_OPERATOR_1(token, loc);
                 case 3: return parser::make_OPERATOR_2(token, loc);
                 case 4: return parser::make_OPERATOR_3(token, loc);
                 case 5: return parser::make_OPERATOR_4(token, loc);
                 case 6: return parser::make_OPERATOR_5(token, loc);
                 case 7: return parser::make_OPERATOR_6(token, loc);
                 case 8: return parser::make_OPERATOR_7(token, loc);
                 case 9: return parser::make_OPERATOR_8(token, loc);
                 case 10: return parser::make_OPERATOR_9(token, loc);
                 }
             }
             else if (runtime.sqfop_exists_unary(key))
             {
                 return parser::make_OPERATOR_U(token, loc);
             }
             return parser::make_NA(loc);
         }

         case tokenizer::etoken::t_string_double: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_string_single: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_number: return parser::make_NUMBER(token, loc);
         case tokenizer::etoken::t_hexadecimal: return parser::make_NUMBER(token, loc);
         default:
             return parser::make_NA(loc);
         }
     }
}
