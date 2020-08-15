%{     /* PARSER */

#include "parser.hh"
#include "scanner.hh"

#define yylex driver.scanner_->yylex
%}

%code requires
{
  #include <iostream>
  #include "driver.hh"
  #include "location.hh"
  #include "position.hh"
}

%code provides
{
  namespace parse
  {
    // Forward declaration of the Driver class
    class Driver;

    inline void
    yyerror (const char* msg)
    {
      std::cerr << msg << std::endl;
    }
  }
}



%require "2.4"
%language "C++"
%locations
%defines
%debug
%define api.namespace {parse}
%define parser_class_name {Parser}
%parse-param {Driver &driver}
%lex-param {Driver &driver}
%error-verbose

%union
{
    std::string     stringVal;
    class astnode   astnode;
}

/* Tokens */

%token RETURN                    "return"
%token THROW                     "throw"
%token LET                       "let"
%token BE                        "be"
%token FUNCTION                  "function"
%token IF                        "if"
%token ELSE                      "else"
%token FROM                      "from"
%token TO                        "to"
%token STEP                      "step"
%token WHILE                     "while"
%token DO                        "do"
%token TRY                       "try"
%token CATCH                     "catch"
%token SWITCH                    "switch"
%token CASE                      "case"
%token DEFAULT                   "default"
%token NIL                       "nil"
%token TRUE                      "true"
%token FALSE                     "false"
%token FOR                       "for"
%token PRIVATE                   "private"
%token COLON                     ":"
%token CURLYO                    "{"
%token CURLYC                    "}"
%token ROUNDO                    "("
%token ROUNDC                    ")"
%token SQUAREO                   "["
%token SQUAREC                   "]"
%token SEMICOLON                 ";"
%token COMMA                     ","
%token PLUS                      "+"
%token MINUS                     "-"
%token LTEQUAL                   "<="
%token LT                        "<"
%token GTEQUAL                   ">="
%token GT                        ">"
%token EQUALEQUALEQUAL           "==="
%token EQUALEQUAL                "=="
%token EXCLAMATIONMARKEQUALEQUAL "!=="
%token EXCLAMATIONMARKEQUAL      "!="
%token EXCLAMATIONMARK           "!"
%token EQUAL                     "="
%token ANDAND                    "&&"
%token SLASH                     "/"
%token STAR                      "*"
%token PERCENT                   "%"
%token QUESTIONMARK              "?"
%token VLINEVLINE                "||"
%token DOT                       "."

%token <stringVal> NUMBER 
%token <stringVal> IDENT  
%token <stringVal> STRING 

%type <astnode> statements statement assignment vardecl funcdecl function
%type <astnode> funchead arglist codeblock if for while trycatch switch
%type <astnode> caselist case exp01 exp02 exp03 exp04 exp05 exp06 exp07
%type <astnode> exp08 exp09 expp value array explist

/* Entry point of grammar */
%start start

%%

/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
start: %empty     
     | statements                                 { driver.push_back($1); }
     ;

statements: statement                             { $$ = $1; }
          | statement statements                  { $$ = {}; $$.append_children($1); $$.append($2); }
          ;

statement: "return" exp01 ";"                     { $$ = $2; }
         | "return" ";"                           { $$ = {}; }
         | "throw" exp01 ";"                      { $$ = $2; }
         | vardecl ";"                            { $$ = $1; }
         | funcdecl                               { $$ = $1; }
         | if                                     { $$ = $1; }
         | for                                    { $$ = $1; }
         | while                                  { $$ = $1; }
         | trycatch                               { $$ = $1; }
         | switch                                 { $$ = $1; }
         | assignment                             { $$ = $1; }
         | exp01 ";"                              { $$ = $1; }
         | ";"                                    { $$ = {}; }
		     | error                                  { $$ = {}; }
         ;

assignment: IDENT "=" exp01                       { $$ = {}; $$.append($1); $$.append($3);}
          ;

vardecl: "let" IDENT "=" exp01                    { $$ = {}; $$.append($2); $$.append($4);}
       | "let" IDENT "be" exp01                   { $$ = {}; $$.append($2); $$.append($4);}
       | "private" IDENT "=" exp01                { $$ = {}; $$.append($2); $$.append($4);}
       | "private" IDENT "be" exp01               { $$ = {}; $$.append($2); $$.append($4);}
       ;

funcdecl: "function" IDENT funchead codeblock     { $$ = {}; $$.append($2); $$.append($3); $$.append($4); }
        ;

function: "function" funchead codeblock           { $$ = {}; $$.append($2); $$.append($3); }
        ;

funchead: "(" ")"                                 { $$ = {}; }
        | "(" arglist ")"                         { $$ = $2; }
        ;

arglist: IDENT                                    { $$ = $1; }
       | IDENT ","                                { $$ = $1; }
       | IDENT "," arglist                        { $$ = {}; $$.append_children($1); $$.append($3); }
       ;

codeblock: statement ";"                          { $$ = $1; }
         | "{" "}"                                { $$ = {}; }
         | "{" statements "}"                     { $$ = $2; }
         ;

if: "if" "(" exp01 ")" codeblock                  { $$ = {}; $$.append($3); $$.append($5); }
  | "if" "(" exp01 ")" codeblock "else" codeblock { $$ = {}; $$.append($3); $$.append($5); $$.append($7); }
  ;

for: "for" IDENT "from" NUMBER "to" NUMBER codeblock                { $$ = {}; $$.append($2); $$.append($4); $$.append($6); $$.append($7); }
   | "for" IDENT "from" NUMBER "to" NUMBER "step" NUMBER codeblock  { $$ = {}; $$.append($2); $$.append($4); $$.append($6); $$.append($8); $$.append($9); }
   | "for" "(" IDENT ":" IDENT ")" codeblock                        { $$ = {}; $$.append($3); $$.append($5); $$.append($7); }
   ;

while: "while" "(" exp01 ")" codeblock                    { $$ = {}; $$.append($3); $$.append($5); }
     | "do" codeblock "while" "(" exp01 ")"               { $$ = {}; $$.append($2); $$.append($5); }
     ;

trycatch: "try" codeblock "catch" "(" IDENT ")" codeblock { $$ = {}; $$.append($2); $$.append($5); $$.append($7); }
        ;

switch: "switch" "(" exp01 ")" "{" caselist "}"           { $$ = {}; $$.append($3); $$.append($6); }
      ;

caselist: case                        { $$ = $1; }
        | caselist case               { $$ = {}; $$.append_children($1); $$.append($2); }
        ;

case: "case" exp01 ":" codeblock      { $$ = {}; $$.append($2); $$.append($4); }
    | "case" exp01 ":"                { $$ = {}; $$.append($2); }
    | "default" ":" codeblock         { $$ = {}; $$.append($3); }
    | "default" ":"                   { $$ = {}; }
    ;

exp01: exp02                          { $$ = $1; }
     | exp02 "?" exp01 ":" exp01      { $$ = {}; $$.append($1); $$.append($3); $$.append($5); }
     ;
exp02: exp03                          { $$ = $1; }
     | exp03 "||" exp01               { $$ = {}; $$.append($1); $$.append($3); }
     ;
exp03: exp04                          { $$ = $1; }
     | exp04 "&&" exp01               { $$ = {}; $$.append($1); $$.append($3); }
     ;
exp04: exp05                          { $$ = $1; }
     | exp05 "===" exp01              { $$ = {}; $$.append($1); $$.append($3); }
     | exp05 "!==" exp01              { $$ = {}; $$.append($1); $$.append($3); }
     | exp05 "==" exp01               { $$ = {}; $$.append($1); $$.append($3); }
     | exp05 "!=" exp01               { $$ = {}; $$.append($1); $$.append($3); }
     ;
exp05: exp06                          { $$ = $1; }
     | exp06 "<"  exp01               { $$ = {}; $$.append($1); $$.append($3); }
     | exp06 "<=" exp01               { $$ = {}; $$.append($1); $$.append($3); }
     | exp06 ">"  exp01               { $$ = {}; $$.append($1); $$.append($3); }
     | exp06 ">=" exp01               { $$ = {}; $$.append($1); $$.append($3); }
     ;
exp06: exp07                          { $$ = $1; }
     | exp07 "+" exp01                { $$ = {}; $$.append($1); $$.append($3); }
     | exp07 "-" exp01                { $$ = {}; $$.append($1); $$.append($3); }
     ;
exp07: exp08                          { $$ = $1; }
     | exp08 "*" exp01                { $$ = {}; $$.append($1); $$.append($3); }
     | exp08 "/" exp01                { $$ = {}; $$.append($1); $$.append($3); }
     | exp08 "%" exp01                { $$ = {}; $$.append($1); $$.append($3); }
     ;
exp08: exp09                          { $$ = $1; }
     | "!" exp09                      { $$ = $2; }
     ;
exp09: expp                           { $$ = $1; }
     | expp "." IDENT "(" explist ")" { $$ = {}; $$.append($1); $$.append($3); $$.append($5); }
     ;

expp: "(" exp01 ")"                   { $$ = $2; }
    | IDENT "(" explist ")"           { $$ = {}; $$.append($1); $$.append($3); }
    | value                           { $$ = $1; }
    ;
value: function                       { $$ = $1; }
     | STRING                         { $$ = $1; }
     | array                          { $$ = $1; }
     | NUMBER                         { $$ = $1; }
     | "true"                         { $$ = {}; }
     | "false"                        { $$ = {}; }
     | "nil"                          { $$ = {}; }
     | IDENT                          { $$ = $1; }
     ;
array: "[" "]"                        { $$ = {}; }
     | "[" explist "]"                { $$ = $2; }
     ;
explist: exp01                        { $$ = $1; }
       | exp01 ","                    { $$ = $1; }
       | explist "," exp01            { $$ = {}; $$.append_children($1); $$.append($3); }
       ;

%%

namespace parse
{
    void Parser::error(const location&, const std::string& m)
    {
        std::cerr << *driver.location_ << ": " << m << std::endl;
        driver.error_ = (driver.error_ == 127 ? 127 : driver.error_ + 1);
    }
}
