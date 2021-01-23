

// %skeleton "lalr1.cc" /* -*- C++ -*- */
%language "c++"
%require "3.0"

%define api.value.type variant
%define api.token.constructor
%define api.namespace { ::sqf::parser::assembly::bison }
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
     namespace sqf::parser::assembly
     {
          class parser;
     }
     #include "astnode.h"
}
%code
{
    
    #include "../../runtime/runtime.h"
     namespace sqf::parser::assembly::bison
     {
          // Return the next token.
          parser::symbol_type yylex (::sqf::runtime::runtime &runtime, ::sqf::parser::assembly::tokenizer&);
     }
}

%lex-param { ::sqf::runtime::runtime &runtime }
%lex-param { ::sqf::parser::assembly::tokenizer &tokenizer }
%parse-param { ::sqf::parser::assembly::tokenizer &tokenizer }
%parse-param { ::sqf::parser::assembly::bison::astnode& result }
%parse-param { ::sqf::parser::assembly::parser& actual }
%parse-param { ::sqf::runtime::runtime &runtime }
%locations
%define parse.error verbose

%define api.token.prefix {}




/* Tokens */

%token END_OF_FILE 0
%token INVALID
%token <tokenizer::token> FALSE                     "false"
%token <tokenizer::token> TRUE                      "true"
%token <tokenizer::token> CURLYO                    "{"
%token <tokenizer::token> CURLYC                    "}"
%token <tokenizer::token> ROUNDO                    "("
%token <tokenizer::token> ROUNDC                    ")"
%token <tokenizer::token> SQUAREO                   "["
%token <tokenizer::token> SQUAREC                   "]"
%token <tokenizer::token> SEMICOLON                 ";"
%token <tokenizer::token> COMMA                     ","
%token <tokenizer::token> EQUAL                     "="

%token <tokenizer::token> ASSIGN_TO
%token <tokenizer::token> ASSIGN_TO_LOCAL
%token <tokenizer::token> GET_VARIABLE
%token <tokenizer::token> CALL_UNARY
%token <tokenizer::token> CALL_NULAR
%token <tokenizer::token> CALL_BINARY
%token <tokenizer::token> PUSH
%token <tokenizer::token> END_STATEMENT
%token <tokenizer::token> IDENT
%token <tokenizer::token> NUMBER
%token <tokenizer::token> HEXNUMBER
%token <tokenizer::token> STRING

%type <::sqf::parser::assembly::bison::astnode> statement statements assignto assigntolocal getvariable
%type <::sqf::parser::assembly::bison::astnode> callunary callnular callbinary push endstatement 
%type <::sqf::parser::assembly::bison::astnode> value exp_list code array

%start start

%%

/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
start: END_OF_FILE                          { result = ::sqf::parser::assembly::bison::astnode{}; }
     | statements                           { result = ::sqf::parser::assembly::bison::astnode{}; result.append($1); }
     ;
statements: statement                       { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::STATEMENTS }; $$.append($1); }
          | statements statement            { $$ = $1; $$.append($2); }
          ;
statement: assignto                     { $$ = $1; }
         | assigntolocal                { $$ = $1; }
         | getvariable                  { $$ = $1; }
         | callunary                    { $$ = $1; }
         | callnular                    { $$ = $1; }
         | callbinary                   { $$ = $1; }
         | push                         { $$ = $1; }
         | endstatement                 { $$ = $1; }
         | error                        { $$ = ::sqf::parser::assembly::bison::astnode{}; }
         ;
assignto: ASSIGN_TO STRING              { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::ASSIGN_TO, $1 }; $$.append(::sqf::parser::assembly::bison::astnode{ astkind::STRING, $1 }); }
        ;
assigntolocal: ASSIGN_TO_LOCAL STRING   { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::ASSIGN_TO_LOCAL, $1 }; $$.append(::sqf::parser::assembly::bison::astnode{ astkind::STRING, $1 });  }
             ;
getvariable: GET_VARIABLE STRING        { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::GET_VARIABLE, $1 }; $$.append(::sqf::parser::assembly::bison::astnode{ astkind::STRING, $1 });  }
           ;
callunary: CALL_UNARY IDENT             { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::CALL_UNARY, $1 }; $$.append(::sqf::parser::assembly::bison::astnode{ astkind::IDENT, $1 }); }
         ;
callnular: CALL_NULAR IDENT             { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::CALL_NULAR, $1 }; $$.append(::sqf::parser::assembly::bison::astnode{ astkind::IDENT, $1 }); }
         ;
callbinary: CALL_BINARY IDENT           { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::CALL_BINARY, $1 }; $$.append(::sqf::parser::assembly::bison::astnode{ astkind::IDENT, $1 }); }
          ;
push: PUSH value                        { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::PUSH, $1 }; $$.append($1); }
    ;
endstatement: END_STATEMENT             { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::END_STATEMENT, $1 }; }
            ;

value: STRING                               { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::STRING, $1 }; }
     | NUMBER                               { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::NUMBER, $1 }; }
     | HEXNUMBER                            { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::HEXNUMBER, $1 }; }
     | "true"                               { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::BOOLEAN_TRUE, $1 }; }
     | "false"                              { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::BOOLEAN_FALSE, $1 }; }
     | code                                 { $$ = $1; }
     | array                                { $$ = $1; }
     ;
exp_list: value                             { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::EXPRESSION_LIST }; $$.append($1); }
        | exp_list "," value                { $$ = $1; $$.append($3); }
        ;
code: "{" statements "}"                    { $$ = ::sqf::parser::assembly::bison::astnode{  astkind::CODE, $1 }; $$.append($2); }
    | "{" "}"                               { $$ = ::sqf::parser::assembly::bison::astnode{  astkind::CODE, $1 }; }
    ;
array: "[" exp_list "]"                     { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::ARRAY, $1 }; $$.append_children($2); }
     | "[" "]"                              { $$ = ::sqf::parser::assembly::bison::astnode{ astkind::ARRAY, $1 }; }
     ;

%%
#ifdef __GNUG__
    #pragma GCC diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#include "assembly_parser.h"
namespace sqf::parser::assembly::bison
{
    void parser::error(const location_type& loc, const std::string& msg)
    {
        actual.__log(logmessage::sqf::ParseError({ *loc.begin.filename, loc.begin.line, loc.begin.column }, msg));
    }
    inline parser::symbol_type yylex (::sqf::runtime::runtime& runtime, ::sqf::parser::assembly::tokenizer& tokenizer)
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
         case tokenizer::etoken::t_true: return parser::make_TRUE(token, loc);
         case tokenizer::etoken::t_assign_to: return parser::make_ASSIGN_TO(token, loc);
         case tokenizer::etoken::t_assign_to_local: return parser::make_ASSIGN_TO_LOCAL(token, loc);
         case tokenizer::etoken::t_get_variable: return parser::make_GET_VARIABLE(token, loc);
         case tokenizer::etoken::t_call_unary: return parser::make_CALL_UNARY(token, loc);
         case tokenizer::etoken::t_call_nular: return parser::make_CALL_NULAR(token, loc);
         case tokenizer::etoken::t_call_binary: return parser::make_CALL_BINARY(token, loc);
         case tokenizer::etoken::t_push: return parser::make_PUSH(token, loc);
         case tokenizer::etoken::t_end_statement: return parser::make_END_STATEMENT(token, loc);

         case tokenizer::etoken::s_curlyo: return parser::make_CURLYO(token, loc);
         case tokenizer::etoken::s_curlyc: return parser::make_CURLYC(token, loc);
         case tokenizer::etoken::s_roundo: return parser::make_ROUNDO(token, loc);
         case tokenizer::etoken::s_roundc: return parser::make_ROUNDC(token, loc);
         case tokenizer::etoken::s_edgeo: return parser::make_SQUAREO(token, loc);
         case tokenizer::etoken::s_edgec: return parser::make_SQUAREC(token, loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(token, loc);

         case tokenizer::etoken::t_operator:
         case tokenizer::etoken::t_ident: return parser::make_IDENT(token, loc);

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
