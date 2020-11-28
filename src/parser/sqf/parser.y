

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
            ENDOFFILE = -3,
            INVALID = -2,
            __TOKEN = -1,
            NA = 0,
            STATEMENTS,
            STATEMENT,
            IDENT,
            NUMBER,
            HEXNUMBER,
            STRING,
            BOOLEAN,
            VALUE_LIST,
            CODE,
            ARRAY,
            ASSIGNMENT,
            ASSIGNMENT_LOCAL,
            EXPN,
            EXP0,
            EXP1,
            EXP2,
            EXP3,
            EXP4,
            EXP5,
            EXP6,
            EXP7,
            EXP8,
            EXP9,
            EXPU
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
%token <tokenizer::token> FALSE                     "false"
%token <tokenizer::token> TRUE                      "true"
%token <tokenizer::token> PRIVATE                   "private"
%token <tokenizer::token> CURLYO                    "{"
%token <tokenizer::token> CURLYC                    "}"
%token <tokenizer::token> ROUNDO                    "("
%token <tokenizer::token> ROUNDC                    ")"
%token <tokenizer::token> SQUAREO                   "["
%token <tokenizer::token> SQUAREC                   "]"
%token <tokenizer::token> SEMICOLON                 ";"
%token <tokenizer::token> COMMA                     ","
%token <tokenizer::token> EQUAL                     "="

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
%token <tokenizer::token> HEXNUMBER
%token <tokenizer::token> STRING

%type <::sqf::parser::sqf::bison::astnode> statement statements value value_list code array
%type <::sqf::parser::sqf::bison::astnode> assignment expression exp0 exp1 exp2 exp3 exp4 
%type <::sqf::parser::sqf::bison::astnode> exp5 exp6 exp7 exp8 exp9 expu

%start start

%%

/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
start: %empty                               { result = ::sqf::parser::sqf::bison::astnode{}; }
     | statements                           { result = ::sqf::parser::sqf::bison::astnode{}; result.append($1); }
     ;
statements: statement                       { $$ = ::sqf::parser::sqf::bison::astnode{}; $$.append($1); }
          | statements ";" statement        { $$ = $1; $$.append($3); }
          | statements "," statement        { $$ = $1; $$.append($3); }
          ;
statement: assignment                       { $$ = $1; }
         | expression                       { $$ = $1; }
         ;

value: STRING                               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::STRING, $1 }; }
     | IDENT_N                              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | IDENT                                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::IDENT, $1 }; }
     | NUMBER                               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::NUMBER, $1 }; }
     | HEXNUMBER                            { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::HEXNUMBER, $1 }; }
     | "true"                               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::BOOLEAN, $1 }; }
     | "false"                              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::BOOLEAN, $1 }; }
     | code                                 { $$ = $1; }
     | array                                { $$ = $1; }
     ;
value_list: value                           { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::VALUE_LIST }; $$.append($1); }
          | value_list "," value            { $$ = $1; $$.append($3); }
          ;
code: "{" statements "}"                    { $$ = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, $1 }; $$.append($2); }
    | "{" "}"                               { $$ = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, $1 }; }
    ;
array: "[" value_list "]"                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::ARRAY, $1 }; $$.append_children($2); }
     | "[" "]"                              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::ARRAY, $1 }; }
     ;
assignment: "private" IDENT "=" expression  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::ASSIGNMENT, $2 }; $$.append($4); }
          | IDENT "=" expression            { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::ASSIGNMENT_LOCAL, $1 }; $$.append($3); }
          ;
expression: exp0                            { $$ = $1; }
          ;
exp0: exp1                                  { $$ = $1; }
    | exp1 OPERATOR_0 exp0                  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, $2 }; $$.append($1); $$.append($3); }
    | exp1 IDENT_0 exp0                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, $2 }; $$.append($1); $$.append($3); }
    ;
exp1: exp2                                  { $$ = $1; }
    | exp2 OPERATOR_1 exp1                  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, $2 }; $$.append($1); $$.append($3); }
    | exp2 IDENT_1 exp1                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, $2 }; $$.append($1); $$.append($3); }
    ;
exp2: exp3                                  { $$ = $1; }
    | exp3 OPERATOR_2 exp2                  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, $2 }; $$.append($1); $$.append($3); }
    | exp3 IDENT_2 exp2                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, $2 }; $$.append($1); $$.append($3); }
    ;
exp3: exp4                                  { $$ = $1; }
    | exp4 OPERATOR_3 exp3                  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, $2 }; $$.append($1); $$.append($3); }
    | exp4 IDENT_3 exp3                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, $2 }; $$.append($1); $$.append($3); }
    ;
exp4: exp5                                  { $$ = $1; }
    | exp5 OPERATOR_4 exp4                  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, $2 }; $$.append($1); $$.append($3); }
    | exp5 IDENT_4 exp4                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, $2 }; $$.append($1); $$.append($3); }
    ;
exp5: exp6                                  { $$ = $1; }
    | exp6 OPERATOR_5 exp5                  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, $2 }; $$.append($1); $$.append($3); }
    | exp6 IDENT_5 exp5                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, $2 }; $$.append($1); $$.append($3); }
    ;
exp6: exp7                                  { $$ = $1; }
    | exp7 OPERATOR_6 exp6                  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, $2 }; $$.append($1); $$.append($3); }
    | exp7 IDENT_6 exp6                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, $2 }; $$.append($1); $$.append($3); }
    ;
exp7: exp8                                  { $$ = $1; }
    | exp8 OPERATOR_7 exp7                  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, $2 }; $$.append($1); $$.append($3); }
    | exp8 IDENT_7 exp7                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, $2 }; $$.append($1); $$.append($3); }
    ;
exp8: exp9                                  { $$ = $1; }
    | exp9 OPERATOR_8 exp8                  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, $2 }; $$.append($1); $$.append($3); }
    | exp9 IDENT_8 exp8                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, $2 }; $$.append($1); $$.append($3); }
    ;
exp9: expu                                  { $$ = $1; }
    | expu OPERATOR_9 exp9                  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, $2 }; $$.append($1); $$.append($3); }
    | expu IDENT_9 exp9                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, $2 }; $$.append($1); $$.append($3); }
    ;
expu: OPERATOR_U expu                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | IDENT_U expu                          { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | "private" expu                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | "(" expression ")"                    { $$ = $2; }
    | value                                 { $$ = $1; }
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
         
         case tokenizer::etoken::t_false: return parser::make_FALSE(token, loc);
         case tokenizer::etoken::t_private: return parser::make_PRIVATE(token, loc);
         case tokenizer::etoken::t_true: return parser::make_TRUE(token, loc);

         case tokenizer::etoken::s_curlyo: return parser::make_CURLYO(token, loc);
         case tokenizer::etoken::s_curlyc: return parser::make_CURLYC(token, loc);
         case tokenizer::etoken::s_roundo: return parser::make_ROUNDO(token, loc);
         case tokenizer::etoken::s_roundc: return parser::make_ROUNDC(token, loc);
         case tokenizer::etoken::s_edgeo: return parser::make_SQUAREO(token, loc);
         case tokenizer::etoken::s_edgec: return parser::make_SQUAREC(token, loc);
         case tokenizer::etoken::s_semicolon: return parser::make_SEMICOLON(token, loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(token, loc);

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
         case tokenizer::etoken::t_hexadecimal: return parser::make_HEXNUMBER(token, loc);
         default:
             return parser::make_NA(loc);
         }
     }
}
