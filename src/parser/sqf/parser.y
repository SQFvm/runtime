

// %skeleton "lalr1.cc" /* -*- C++ -*- */
%language "c++"
%require "3.0"

%define api.value.type variant
%define api.token.constructor
%define api.namespace { ::sqf::parser::sqf::bison }
%code top {
    #ifdef __GNUG__
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wall"
    #endif
    #ifdef _MSC_VER
        #pragma warning(push, 0)
    #endif
    #include "tokenizer.hpp"
    #include <string>
    #include <vector>
    #include <algorithm>
}
%code provides
{
    #ifdef __GNUG__
        #pragma GCC diagnostic pop
    #endif
    #ifdef _MSC_VER
        #pragma warning(pop)
    #endif
}
%code requires
{
    #ifdef __GNUG__
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wall"
    #endif
    #ifdef _MSC_VER
        #pragma warning(push, 0)
    #endif
    namespace sqf::runtime
    {
         class runtime;
    }
    namespace sqf::parser::sqf
    {
         class parser;
    }
    #include "astnode.hpp"
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
%locations
%define parse.error verbose

%define api.token.prefix {}




/* Tokens */

%token END_OF_FILE 0
%token INVALID
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

%token <tokenizer::token> OPERATOR_B_0
%token <tokenizer::token> OPERATOR_B_1
%token <tokenizer::token> OPERATOR_B_2
%token <tokenizer::token> OPERATOR_B_3
%token <tokenizer::token> OPERATOR_B_4
%token <tokenizer::token> OPERATOR_B_5
%token <tokenizer::token> OPERATOR_B_6
%token <tokenizer::token> OPERATOR_B_7
%token <tokenizer::token> OPERATOR_B_8
%token <tokenizer::token> OPERATOR_B_9
%token <tokenizer::token> OPERATOR_BU_0
%token <tokenizer::token> OPERATOR_BU_1
%token <tokenizer::token> OPERATOR_BU_2
%token <tokenizer::token> OPERATOR_BU_3
%token <tokenizer::token> OPERATOR_BU_4
%token <tokenizer::token> OPERATOR_BU_5
%token <tokenizer::token> OPERATOR_BU_6
%token <tokenizer::token> OPERATOR_BU_7
%token <tokenizer::token> OPERATOR_BU_8
%token <tokenizer::token> OPERATOR_BU_9
%token <tokenizer::token> OPERATOR_BN_0
%token <tokenizer::token> OPERATOR_BN_1
%token <tokenizer::token> OPERATOR_BN_2
%token <tokenizer::token> OPERATOR_BN_3
%token <tokenizer::token> OPERATOR_BN_4
%token <tokenizer::token> OPERATOR_BN_5
%token <tokenizer::token> OPERATOR_BN_6
%token <tokenizer::token> OPERATOR_BN_7
%token <tokenizer::token> OPERATOR_BN_8
%token <tokenizer::token> OPERATOR_BN_9
%token <tokenizer::token> OPERATOR_BUN_0
%token <tokenizer::token> OPERATOR_BUN_1
%token <tokenizer::token> OPERATOR_BUN_2
%token <tokenizer::token> OPERATOR_BUN_3
%token <tokenizer::token> OPERATOR_BUN_4
%token <tokenizer::token> OPERATOR_BUN_5
%token <tokenizer::token> OPERATOR_BUN_6
%token <tokenizer::token> OPERATOR_BUN_7
%token <tokenizer::token> OPERATOR_BUN_8
%token <tokenizer::token> OPERATOR_BUN_9
%token <tokenizer::token> OPERATOR_U
%token <tokenizer::token> OPERATOR_N
%token <tokenizer::token> OPERATOR_UN
%token <tokenizer::token> IDENT
%token <tokenizer::token> NUMBER
%token <tokenizer::token> HEXNUMBER
%token <tokenizer::token> STRING

%type <::sqf::parser::sqf::bison::astnode> statement statements value exp_list code array
%type <::sqf::parser::sqf::bison::astnode> assignment expression exp0 exp1 exp2 exp3 exp4 
%type <::sqf::parser::sqf::bison::astnode> exp5 exp6 exp7 exp8 exp9 expu

%start start

%%

/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
start: END_OF_FILE                          { result = ::sqf::parser::sqf::bison::astnode{}; }
     | statements                           { result = ::sqf::parser::sqf::bison::astnode{}; result.append($1); }
     | separators                           { result = ::sqf::parser::sqf::bison::astnode{}; }
     | separators statements                { result = ::sqf::parser::sqf::bison::astnode{}; result.append($2); }
     ;
statements: statement                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::STATEMENTS }; $$.append($1); }
          | statements separators           { $$ = $1; }
          | statements separators statement { $$ = $1; $$.append($3); }
          ;
statement: assignment                       { $$ = $1; }
         | expression                       { $$ = $1; }
         ;
separator: ";"
         | ","
         ;
separators: separator
         | separators separator
         ;

value: STRING                               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::STRING, $1 }; }
     | OPERATOR_N                           { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BN_0                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BN_1                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BN_2                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BN_3                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BN_4                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BN_5                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BN_6                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BN_7                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BN_8                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BN_9                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BUN_0                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BUN_1                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BUN_2                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BUN_3                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BUN_4                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BUN_5                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BUN_6                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BUN_7                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BUN_8                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | OPERATOR_BUN_9                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, $1 }; }
     | IDENT                                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::IDENT, $1 }; }
     | NUMBER                               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::NUMBER, $1 }; }
     | HEXNUMBER                            { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::HEXNUMBER, $1 }; }
     | "true"                               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::BOOLEAN_TRUE, $1 }; }
     | "false"                              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::BOOLEAN_FALSE, $1 }; }
     | code                                 { $$ = $1; }
     | array                                { $$ = $1; }
     ;
exp_list: expression                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPRESSION_LIST }; $$.append($1); }
        | exp_list "," expression           { $$ = $1; $$.append($3); }
        ;
code: "{" statements "}"                    { $$ = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, $1 }; $$.append($2); }
    | "{" separators statements "}"         { $$ = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, $1 }; $$.append($3); }
    | "{" separators "}"                    { $$ = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, $1 }; }
    | "{" "}"                               { $$ = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, $1 }; }
    ;
array: "[" exp_list "]"                     { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::ARRAY, $1 }; $$.append_children($2); }
     | "[" "]"                              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::ARRAY, $1 }; }
     ;
assignment: "private" IDENT "=" expression  { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::ASSIGNMENT_LOCAL, $2 }; $$.append($4); }
          | value "=" expression            { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::ASSIGNMENT, $2 }; $$.append($1); $$.append($3); }
          ;
expression: exp0                            { $$ = $1; }
          ;
exp0: exp1                                  { $$ = $1; }
    | exp0 OPERATOR_B_0 exp1                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, $2 }; $$.append($1); $$.append($3); }
    | exp0 OPERATOR_BUN_0 exp1              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, $2 }; $$.append($1); $$.append($3); }
    | exp0 OPERATOR_BU_0 exp1               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, $2 }; $$.append($1); $$.append($3); }
    | exp0 OPERATOR_BN_0 exp1               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, $2 }; $$.append($1); $$.append($3); }
    ;
exp1: exp2                                  { $$ = $1; }
    | exp1 OPERATOR_B_1 exp2                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, $2 }; $$.append($1); $$.append($3); }
    | exp1 OPERATOR_BU_1 exp2               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, $2 }; $$.append($1); $$.append($3); }
    | exp1 OPERATOR_BN_1 exp2               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, $2 }; $$.append($1); $$.append($3); }
    | exp1 OPERATOR_BUN_1 exp2              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, $2 }; $$.append($1); $$.append($3); }
    ;
exp2: exp3                                  { $$ = $1; }
    | exp2 OPERATOR_B_2 exp3                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, $2 }; $$.append($1); $$.append($3); }
    | exp2 OPERATOR_BU_2 exp3               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, $2 }; $$.append($1); $$.append($3); }
    | exp2 OPERATOR_BN_2 exp3               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, $2 }; $$.append($1); $$.append($3); }
    | exp2 OPERATOR_BUN_2 exp3              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, $2 }; $$.append($1); $$.append($3); }
    ;
exp3: exp4                                  { $$ = $1; }
    | exp3 OPERATOR_B_3 exp4                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, $2 }; $$.append($1); $$.append($3); }
    | exp3 OPERATOR_BU_3 exp4               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, $2 }; $$.append($1); $$.append($3); }
    | exp3 OPERATOR_BN_3 exp4               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, $2 }; $$.append($1); $$.append($3); }
    | exp3 OPERATOR_BUN_3 exp4              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, $2 }; $$.append($1); $$.append($3); }
    ;
exp4: exp5                                  { $$ = $1; }
    | exp4 OPERATOR_B_4 exp5                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, $2 }; $$.append($1); $$.append($3); }
    | exp4 OPERATOR_BU_4 exp5               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, $2 }; $$.append($1); $$.append($3); }
    | exp4 OPERATOR_BN_4 exp5               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, $2 }; $$.append($1); $$.append($3); }
    | exp4 OPERATOR_BUN_4 exp5              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, $2 }; $$.append($1); $$.append($3); }
    ;
exp5: exp6                                  { $$ = $1; }
    | exp5 OPERATOR_B_5 exp6                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, $2 }; $$.append($1); $$.append($3); }
    | exp5 OPERATOR_BU_5 exp6               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, $2 }; $$.append($1); $$.append($3); }
    | exp5 OPERATOR_BN_5 exp6               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, $2 }; $$.append($1); $$.append($3); }
    | exp5 OPERATOR_BUN_5 exp6              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, $2 }; $$.append($1); $$.append($3); }
    ;
exp6: exp7                                  { $$ = $1; }
    | exp6 OPERATOR_B_6 exp7                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, $2 }; $$.append($1); $$.append($3); }
    | exp6 OPERATOR_BU_6 exp7               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, $2 }; $$.append($1); $$.append($3); }
    | exp6 OPERATOR_BN_6 exp7               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, $2 }; $$.append($1); $$.append($3); }
    | exp6 OPERATOR_BUN_6 exp7              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, $2 }; $$.append($1); $$.append($3); }
    ;
exp7: exp8                                  { $$ = $1; }
    | exp7 OPERATOR_B_7 exp8                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, $2 }; $$.append($1); $$.append($3); }
    | exp7 OPERATOR_BU_7 exp8               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, $2 }; $$.append($1); $$.append($3); }
    | exp7 OPERATOR_BN_7 exp8               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, $2 }; $$.append($1); $$.append($3); }
    | exp7 OPERATOR_BUN_7 exp8              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, $2 }; $$.append($1); $$.append($3); }
    ;
exp8: exp9                                  { $$ = $1; }
    | exp8 OPERATOR_B_8 exp9                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, $2 }; $$.append($1); $$.append($3); }
    | exp8 OPERATOR_BU_8 exp9               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, $2 }; $$.append($1); $$.append($3); }
    | exp8 OPERATOR_BN_8 exp9               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, $2 }; $$.append($1); $$.append($3); }
    | exp8 OPERATOR_BUN_8 exp9              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, $2 }; $$.append($1); $$.append($3); }
    ;
exp9: expu                                  { $$ = $1; }
    | exp9 OPERATOR_B_9 expu                { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, $2 }; $$.append($1); $$.append($3); }
    | exp9 OPERATOR_BU_9 expu               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, $2 }; $$.append($1); $$.append($3); }
    | exp9 OPERATOR_BN_9 expu               { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, $2 }; $$.append($1); $$.append($3); }
    | exp9 OPERATOR_BUN_9 expu              { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, $2 }; $$.append($1); $$.append($3); }
    ;
expu: "private" expu                        { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_U expu                       { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_UN expu                      { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BU_0 expu                    { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BU_1 expu                    { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BU_2 expu                    { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BU_3 expu                    { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BU_4 expu                    { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BU_5 expu                    { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BU_6 expu                    { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BU_7 expu                    { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BU_8 expu                    { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BU_9 expu                    { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BUN_0 expu                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BUN_1 expu                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BUN_2 expu                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BUN_3 expu                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BUN_4 expu                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BUN_5 expu                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BUN_6 expu                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BUN_7 expu                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BUN_8 expu                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | OPERATOR_BUN_9 expu                   { $$ = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, $1 }; $$.append($2); }
    | "(" expression ")"                    { $$ = $2; }
    | value                                 { $$ = $1; }
    ;


%%
#ifdef __GNUG__
    #pragma GCC diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#include "sqf_parser.hpp"
namespace sqf::parser::sqf::bison
{
    void parser::error (const location_type& loc, const std::string& msg)
    {
        actual.__log(logmessage::sqf::ParseError({ *loc.begin.filename, loc.begin.line, loc.begin.column }, msg));
    }
    inline parser::symbol_type yylex (::sqf::runtime::runtime& runtime, ::sqf::parser::sqf::tokenizer& tokenizer)
    {
         auto token = tokenizer.next();
         parser::location_type loc;
         loc.begin.line = token.line;
         loc.begin.column = token.column;
         loc.end.line = token.line;
         loc.end.column = token.column + token.contents.length();
         loc.begin.filename = loc.end.filename = token.path;

         switch (token.type)
         {
         case tokenizer::etoken::eof: return parser::make_END_OF_FILE(loc);
         case tokenizer::etoken::invalid: return parser::make_INVALID(loc);
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

         case tokenizer::etoken::t_operator:
         case tokenizer::etoken::t_ident:
         {
             auto key = std::string(token.contents.begin(), token.contents.end());
             short precedence = 0;
             bool binary = false;
             bool unary = false;
             bool nular = false;
             std::transform(key.begin(), key.end(), key.begin(), [](char c) -> char { return (char)std::tolower(c); });
             if (runtime.sqfop_exists_binary(key))
             {
                auto bres = runtime.sqfop_binary_by_name(key);
                precedence = bres.begin()->get().precedence();
                binary = true;
             }
             if (runtime.sqfop_exists_unary(key))
             {
                 unary = true;
             }
             if (runtime.sqfop_exists(::sqf::runtime::sqfop_nular::key{key}))
             {
                 nular = true;
             }
             if (binary && !unary && !nular)
             {
                 
                switch (precedence)
                {
                case 1:  return parser::make_OPERATOR_B_0(token, loc);
                case 2:  return parser::make_OPERATOR_B_1(token, loc);
                case 3:  return parser::make_OPERATOR_B_2(token, loc);
                case 4:  return parser::make_OPERATOR_B_3(token, loc);
                case 5:  return parser::make_OPERATOR_B_4(token, loc);
                case 6:  return parser::make_OPERATOR_B_5(token, loc);
                case 7:  return parser::make_OPERATOR_B_6(token, loc);
                case 8:  return parser::make_OPERATOR_B_7(token, loc);
                case 9:  return parser::make_OPERATOR_B_8(token, loc);
                case 10: return parser::make_OPERATOR_B_9(token, loc);
                }
             }
             else if (binary && !unary && nular)
             {
                switch (precedence)
                {
                case 1:  return parser::make_OPERATOR_BN_0(token, loc);
                case 2:  return parser::make_OPERATOR_BN_1(token, loc);
                case 3:  return parser::make_OPERATOR_BN_2(token, loc);
                case 4:  return parser::make_OPERATOR_BN_3(token, loc);
                case 5:  return parser::make_OPERATOR_BN_4(token, loc);
                case 6:  return parser::make_OPERATOR_BN_5(token, loc);
                case 7:  return parser::make_OPERATOR_BN_6(token, loc);
                case 8:  return parser::make_OPERATOR_BN_7(token, loc);
                case 9:  return parser::make_OPERATOR_BN_8(token, loc);
                case 10: return parser::make_OPERATOR_BN_9(token, loc);
                }
             }
             else if (binary && unary && !nular)
             {
                switch (precedence)
                {
                case 1:  return parser::make_OPERATOR_BU_0(token, loc);
                case 2:  return parser::make_OPERATOR_BU_1(token, loc);
                case 3:  return parser::make_OPERATOR_BU_2(token, loc);
                case 4:  return parser::make_OPERATOR_BU_3(token, loc);
                case 5:  return parser::make_OPERATOR_BU_4(token, loc);
                case 6:  return parser::make_OPERATOR_BU_5(token, loc);
                case 7:  return parser::make_OPERATOR_BU_6(token, loc);
                case 8:  return parser::make_OPERATOR_BU_7(token, loc);
                case 9:  return parser::make_OPERATOR_BU_8(token, loc);
                case 10: return parser::make_OPERATOR_BU_9(token, loc);
                }
             }
             else if (binary && unary && nular)
             {
                switch (precedence)
                {
                case 1:  return parser::make_OPERATOR_BUN_0(token, loc);
                case 2:  return parser::make_OPERATOR_BUN_1(token, loc);
                case 3:  return parser::make_OPERATOR_BUN_2(token, loc);
                case 4:  return parser::make_OPERATOR_BUN_3(token, loc);
                case 5:  return parser::make_OPERATOR_BUN_4(token, loc);
                case 6:  return parser::make_OPERATOR_BUN_5(token, loc);
                case 7:  return parser::make_OPERATOR_BUN_6(token, loc);
                case 8:  return parser::make_OPERATOR_BUN_7(token, loc);
                case 9:  return parser::make_OPERATOR_BUN_8(token, loc);
                case 10: return parser::make_OPERATOR_BUN_9(token, loc);
                }
             }
             else if (!binary && !unary && nular)
             {
                 return parser::make_OPERATOR_N(token, loc);
             }
             else if (!binary && unary && !nular)
             {
                 return parser::make_OPERATOR_U(token, loc);
             }
             else if (!binary && unary && nular)
             {
                 return parser::make_OPERATOR_UN(token, loc);
             }
             
             return token.type == tokenizer::etoken::t_ident ? parser::make_IDENT(token, loc) : parser::make_INVALID(loc);
         }

         case tokenizer::etoken::t_string_double: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_string_single: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_number: return parser::make_NUMBER(token, loc);
         case tokenizer::etoken::t_hexadecimal: return parser::make_HEXNUMBER(token, loc);
         case tokenizer::etoken::s_equal: return parser::make_EQUAL(token, loc);
         default:
             return parser::make_INVALID(loc);
         }
     }
}
