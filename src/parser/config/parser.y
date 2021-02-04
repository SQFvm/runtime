

// %skeleton "lalr1.cc" /* -*- C++ -*- */
%language "c++"
%require "3.0"

%define api.value.type variant
%define api.token.constructor
%define api.namespace { ::sqf::parser::config::bison }
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
     namespace sqf::parser::config
     {
          class parser;
     }
     namespace sqf::parser::config::bison
     {
        enum class astkind
        {
            ENDOFFILE = -3,
            INVALID = -2,
            __TOKEN = -1,
            NA = 0,
            STATEMENTS,
            CLASS_DEF,
            CLASS_DEF_EXT,
            CLASS,
            CLASS_EXT,
            DELETE_CLASS,
            FIELD,
            FIELD_ARRAY,
            FIELD_ARRAY_APPEND,
            NUMBER_DECIMAL,
            NUMBER_HEXADECIMAL,
            STRING,
            IDENT,
            ARRAY,
            ANYSTRING,
            ANY
        };
        struct astnode
        {
            ::sqf::parser::config::tokenizer::token token;
            astkind kind;
            std::vector<astnode> children;

            astnode() : token(), kind(astkind::NA) { }
            astnode(astkind kind) : token(), kind(kind) { }
            astnode(::sqf::parser::config::tokenizer::token t) : token(t), kind(astkind::__TOKEN) { }
            astnode(astkind kind, ::sqf::parser::config::tokenizer::token t) : token(t), kind(kind) { }

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
     namespace sqf::parser::config::bison
     {
          // Return the next token.
          parser::symbol_type yylex (::sqf::parser::config::tokenizer&);
     }
}

%lex-param { ::sqf::parser::config::tokenizer &tokenizer }
%parse-param { ::sqf::parser::config::tokenizer &tokenizer }
%parse-param { ::sqf::parser::config::bison::astnode& result }
%parse-param { ::sqf::parser::config::parser& actual }
%locations
%define parse.error verbose

%define api.token.prefix {}




/* Tokens */

%token END_OF_FILE 0
%token INVALID
%token <tokenizer::token> DELETE                    "delete"
%token <tokenizer::token> CLASS                     "class"
%token <tokenizer::token> CURLYO                    "{"
%token <tokenizer::token> CURLYC                    "}"
%token <tokenizer::token> ROUNDO                    "("
%token <tokenizer::token> ROUNDC                    ")"
%token <tokenizer::token> SQUAREO                   "["
%token <tokenizer::token> SQUAREC                   "]"
%token <tokenizer::token> COLON                     ":"
%token <tokenizer::token> SEMICOLON                 ";"
%token <tokenizer::token> COMMA                     ","
%token <tokenizer::token> PLUSEQUAL                 "+="
%token <tokenizer::token> EQUAL                     "="

%token <tokenizer::token> IDENT
%token <tokenizer::token> NUMBER
%token <tokenizer::token> HEXNUMBER
%token <tokenizer::token> STRING
%token <tokenizer::token> ANY

%type <::sqf::parser::config::bison::astnode> topstatements topstatement statements statement arrayvalue
%type <::sqf::parser::config::bison::astnode> classdef deleteclass classbody field string number array
%type <::sqf::parser::config::bison::astnode> arrayvaluelist anyval anyarr anyp anyarray anyvalue ident

%start start

%%

/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
start: END_OF_FILE                                      { result = ::sqf::parser::config::bison::astnode{}; }
     | topstatements                                    { result = ::sqf::parser::config::bison::astnode{}; result.append($1); }
     | separators                                       { result = ::sqf::parser::config::bison::astnode{}; }
     | separators topstatements                         { result = ::sqf::parser::config::bison::astnode{}; result.append($2); }
     ;
separators: ";"
          | separators ";"
          ;
topstatements: topstatement                             { $$ = ::sqf::parser::config::bison::astnode{ astkind::STATEMENTS }; $$.append($1); }
             | topstatements separators                 { $$ = $1; }
             | topstatements separators topstatement    { $$ = $1; $$.append($3); }
             ;
topstatement: classdef                                  { $$ = $1; }
            | deleteclass                               { $$ = $1; }
            ;
statements: statement                                   { $$ = ::sqf::parser::config::bison::astnode{ astkind::STATEMENTS }; $$.append($1); }
          | statements separators                       { $$ = $1; }
          | statements separators statement             { $$ = $1; $$.append($3); }
          ;
statement: classdef                                     { $$ = $1; }
         | field                                        { $$ = $1; }
         | deleteclass                                  { $$ = $1; }
         ;

classdef: "class" ident                                 { $$ = ::sqf::parser::config::bison::astnode{ astkind::CLASS_DEF, $1 }; $$.append($2); }
        | "class" ident ":" ident                       { $$ = ::sqf::parser::config::bison::astnode{ astkind::CLASS_DEF_EXT, $1 }; $$.append($2); $$.append($4); }
        | "class" ident classbody                       { $$ = ::sqf::parser::config::bison::astnode{ astkind::CLASS, $1 }; $$.append($2); $$.append($3); }
        | "class" ident ":" ident classbody             { $$ = ::sqf::parser::config::bison::astnode{ astkind::CLASS_EXT, $1 }; $$.append($2); $$.append($4); $$.append($5); }
        ;
deleteclass: "delete" ident                             { $$ = ::sqf::parser::config::bison::astnode{ astkind::DELETE_CLASS, $1 }; $$.append($2); }
           ;

classbody: "{" "}"                                      { $$ = ::sqf::parser::config::bison::astnode{ astkind::STATEMENTS }; }
         | "{" statements "}"                           { $$ = $2; }
         ;

field: ident "=" anyvalue
     {
         $$ = ::sqf::parser::config::bison::astnode{ astkind::FIELD, $2 };
         $$.append($1);
         if ($3.children.size() == 1 && $3.children[0].kind != astkind::ANY)
         {
            $$.append($3.children[0]);
         }
         else
         {
            $$.append($3);
         }
     }
     | ident "[" "]" "=" array                          { $$ = ::sqf::parser::config::bison::astnode{ astkind::FIELD_ARRAY, $4 }; $$.append($1); $$.append($5); }
     | ident "[" "]" "+=" array                         { $$ = ::sqf::parser::config::bison::astnode{ astkind::FIELD_ARRAY_APPEND, $4 }; $$.append($1); $$.append($5); }
     ;

ident: IDENT                                            { $$ = ::sqf::parser::config::bison::astnode{ astkind::IDENT, $1 }; }
     ;
string: STRING                                          { $$ = ::sqf::parser::config::bison::astnode{ astkind::STRING, $1 }; }
      ;
number: NUMBER                                          { $$ = ::sqf::parser::config::bison::astnode{ astkind::NUMBER_DECIMAL, $1 }; }
      | HEXNUMBER                                       { $$ = ::sqf::parser::config::bison::astnode{ astkind::NUMBER_HEXADECIMAL, $1 }; }
      ;
array: "{" "}"                                          { $$ = ::sqf::parser::config::bison::astnode{ astkind::ARRAY }; }
     | "{" arrayvaluelist "}"                           { $$ = $2; }
     ;
arrayvalue: array                                       { $$ = $1; }
          | anyarray
          {
              if ($1.children.size() == 1 && $1.children[0].kind != astkind::ANY)
              {
                 $$ = $1.children[0];
              }
              else
              {
                 $$ = $1;
              }
          }
          ;
arrayvaluelist: arrayvalue                              { $$ = ::sqf::parser::config::bison::astnode{ astkind::ARRAY }; $$.append($1); }
              | arrayvaluelist "," arrayvalue           { $$ = $1; $$.append($3); }
              ;
anyval: anyp                                            { $$ = $1; }
      | "{"                                             { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANY, $1 }; }
      | "}"                                             { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANY, $1 }; }
      | ","                                             { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANY, $1 }; }
      ;
anyarr: anyp                                            { $$ = $1; }
      ;
anyp: "class"                                           { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANY, $1 }; }
    | "delete"                                          { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANY, $1 }; }
    | number                                            { $$ = $1; }
    | string                                            { $$ = $1; }
    | ident                                             { $$ = $1; }
    | "["                                               { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANY, $1 }; }
    | "]"                                               { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANY, $1 }; }
    | ":"                                               { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANY, $1 }; }
    | "="                                               { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANY, $1 }; }
    | ANY                                               { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANY, $1 }; }
    ;
anyarray: anyarr                                        { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANYSTRING }; $$.append($1); }
        | anyarray anyarr                               { $$ = $1; $$.append($2); }
        ;
anyvalue: anyval                                        { $$ = ::sqf::parser::config::bison::astnode{ astkind::ANYSTRING }; $$.append($1); }
        | anyvalue anyval                               { $$ = $1; $$.append($2); }
        ;


%%
#ifdef __GNUG__
    #pragma GCC diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#include "config_parser.hpp"
namespace sqf::parser::config::bison
{
    void parser::error(const location_type& loc, const std::string& msg)
    {
        actual.__log(logmessage::config::ParseError({ *loc.begin.filename, loc.begin.line, loc.begin.column }, msg));
    }
    inline parser::symbol_type yylex(::sqf::parser::config::tokenizer& tokenizer)
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
         case tokenizer::etoken::m_line: return yylex(tokenizer);
         case tokenizer::etoken::i_comment_line: return yylex(tokenizer);
         case tokenizer::etoken::i_comment_block: return yylex(tokenizer);
         case tokenizer::etoken::i_whitespace: return yylex(tokenizer);
         
         case tokenizer::etoken::t_class: return parser::make_CLASS(token, loc);
         case tokenizer::etoken::t_delete: return parser::make_DELETE(token, loc);

         case tokenizer::etoken::s_curlyo: return parser::make_CURLYO(token, loc);
         case tokenizer::etoken::s_curlyc: return parser::make_CURLYC(token, loc);
         case tokenizer::etoken::s_edgeo: return parser::make_SQUAREO(token, loc);
         case tokenizer::etoken::s_edgec: return parser::make_SQUAREC(token, loc);
         case tokenizer::etoken::s_colon: return parser::make_COLON(token, loc);
         case tokenizer::etoken::s_semicolon: return parser::make_SEMICOLON(token, loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(token, loc);
         case tokenizer::etoken::t_ident: return parser::make_IDENT(token, loc);

         case tokenizer::etoken::t_string_double: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_string_single: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_number: return parser::make_NUMBER(token, loc);
         case tokenizer::etoken::t_hexadecimal: return parser::make_HEXNUMBER(token, loc);
         case tokenizer::etoken::t_plus_equal: return parser::make_PLUSEQUAL(token, loc);
         case tokenizer::etoken::s_equal: return parser::make_EQUAL(token, loc);
         case tokenizer::etoken::any: return parser::make_ANY(token, loc);
         default:
             return parser::make_ANY(token, loc);
         }
     }
}
