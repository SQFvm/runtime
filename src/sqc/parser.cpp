#include "parser.h"

/*
SQC = %empty
    | STATEMENTS
    ;

STATEMENTS = STATEMENT
           | STATEMENT STATEMENTS
           ;

STATEMENT = VARDECL ";"
          | FUNCDECL
          | IF
          | FOR
          | WHILE
          | TRYCATCH
          | SWITCH
          | EXP0 ";"
          | ASSIGNMENT
          | ";"
          ;
ASSIGNMENT = ident "=" EXP0
           ;

VARDECL = let ident "=" EXP0
        | let ident be EXP0
        ;

FUNCDECL = function ident FUNCHEAD CODEBLOCK
         ;

FUNCTION = function FUNCHEAD CODEBLOCK
         ;

FUNCHEAD = "(" ")"
         | "(" ARGLIST ")"
         ;

ARGLIST = ident
        | ident "," ARGLIST
        ;

CODEBLOCK = STATEMENT ";"
          | "{" "}"
          | "{" STATEMENTS "}"
          ;

IF = if "(" EXP01 ")" CODEBLOCK
   | if "(" EXP01 ")" CODEBLOCK else "(" EXP01 ")" CODEBLOCK
   ;

FOR = for "(" EXP01 ";" EXP01 ";" EXP01 ")" CODEBLOCK
    | for ident from number to number CODEBLOCK
    | for "(" ident : ident ")" CODEBLOCK
    ;

WHILE = "while" "(" EXP01 ")" CODEBLOCK
      | "do" CODEBLOCK "while" "(" EXP01 ")"
      ;

TRYCATCH = try CODEBLOCK catch "(" ident ")" CODEBLOCK
         ;

SWITCH = switch "(" EXP01 ")" "{" CASELIST "}"
       ;

CASELIST = CASE
         | CASE CASELIST
         ;

CASE = case EXP01 ":" CODEBLOCK
     | case EXP01 ":"
     | default ":" CODEBLOCK
     | default ":"
     ;

EXP01: EXP02
     | EXP02 "?" EXP01 ":" EXP01
     ;
EXP02: EXP03
     | EXP03 "||" EXP01
     ;
EXP03: EXP04
     | EXP04 "&&" EXP01
     ;
EXP04: EXP05
     | EXP05 "==" EXP01
     | EXP05 "!=" EXP01
     ;
EXP05: EXP06
     | EXP06 "<"  EXP01
     | EXP06 "<=" EXP01
     | EXP06 ">"  EXP01
     | EXP06 ">=" EXP01
     ;
EXP06: EXP07
     | EXP07 "+" EXP01
     | EXP07 "-" EXP01
     ;
EXP07: EXP08
     | EXP08 "*" EXP01
     | EXP08 "/" EXP01
     | EXP08 "%" EXP01
     ;
EXP08: EXPP
     | "!" EXPP
     ;

EXPP = return EXP01
     | return
     | throw EXP01
     | "(" EXP01 ")" "." ident "(" EXPLIST ")"
     | "(" EXP01 ")"
     | ident "(" EXPLIST ")"
     | null
     | nil
     | VALUE "." ident "(" EXPLIST ")"
     | VALUE
     ;
VALUE = FUNCTION
      | string
      | ARRAY
      | number
      | true
      | false
      | ident
      ;
ARRAY = "[" "]"
      | "[" EXPLIST "]"
      ;
EXPLIST = EXP01
        | EXP01 "," EXPLIST
        ;
*/

bool sqf::sqc::parser::check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file)
{
    return false;
}

std::optional<::sqf::runtime::instruction_set> sqf::sqc::parser::parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file)
{
    return std::optional<::sqf::runtime::instruction_set>();
}
