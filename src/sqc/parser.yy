/* $Id: parser.yy 48 2009-09-05 08:07:10Z tb $ -*- mode: c++ -*- */
/** \file parser.yy Contains the example Bison parser source */

%{ /*** C/C++ Declarations ***/

#include <stdio.h>
#include <string>
#include <vector>

#include "expression.h"

%}

/*** yacc/bison Declarations ***/

/* Require bison 2.3 or later */
%require "2.3"

/* add debug output code to generated parser. disable this for release
 * versions. */
%debug

/* start symbol is named "start" */
%start start

/* write out a header file containing the token defines */
%defines

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
%name-prefix="example"

/* set the parser's class identifier */
%define "parser_class_name" "Parser"

/* keep track of the current position within the input */
%locations
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &driver.streamname;
};

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class Driver& driver }

/* verbose error messages */
%error-verbose

 /*** BEGIN - Change the grammar's tokens below ***/
 /*** BEGIN - Change the grammar's tokens below ***/
 /*** BEGIN - Change the grammar's tokens below ***/

%union {
    int  			integerVal;
    double 			doubleVal;
    std::string*		stringVal;
    class CalcNode*		calcnode;
}

%token              END 0    "end of file"
%token              EOL      "end of line"

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

%token NUMBER <stringVal>        "number"
%token IDENT <stringVal>         "ident"
%token STRING <stringVal>        "string"
/*
%token SLASHEQUAL                "/="
%token STAREQUAL                 "*="
%token MINUSEQUAL                "-="
%token PLUSEQUAL                 "+="
%token PERCENTEQUAL              "%="
%token HEXNUMBER                 "number_hex"
%token BINARYNUMBER              "number_bin"
*/

%type <stringVal>	constant variable
%type <stringVal>	atomexpr powexpr unaryexpr mulexpr addexpr expr

%destructor { delete $$; } NUMBER IDENT STRING
%destructor { delete $$; } constant variable
%destructor { delete $$; } atomexpr powexpr unaryexpr mulexpr addexpr expr

 /*** END - Change the grammar's tokens above ***/
 /*** END - Change the grammar's tokens above ***/
 /*** END - Change the grammar's tokens above ***/

%{

#include "driver.h"
#include "scanner.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex

%}

%% /*** Grammar Rules ***/

/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
/*** BEGIN - Change the grammar rules below ***/
start: %empty
     | statements
     ;

statements: statement
          | statement statements
          ;

statement: vardecl ";"
         | funcdecl
         | if
         | for
         | while
         | trycatch
         | switch
         | assignment
         | exp01 ";"
         | ";"
		 | error
         ;
assignment: "ident" "=" exp01
          ;

vardecl: "let" "ident" "=" exp01
       | "let" "ident" "be" exp01
	   | "private" "ident" "=" exp01
       | "private" "ident" "be" exp01
       ;

funcdecl: "function" "ident" funchead codeblock
        ;

function: "function" funchead codeblock
        ;

funchead: "(" ")"
        | "(" arglist ")"
        ;

arglist: "ident"
       | "ident" ","
       | "ident" "," arglist
       ;

codeblock: statement ";"
         | "{" "}"
         | "{" statements "}"
         ;

if: "if" "(" exp01 ")" codeblock
  | "if" "(" exp01 ")" codeblock "else" codeblock
  ;

for: "for" "(" exp01 ";" exp01 ";" exp01 ")" codeblock
   | "for" ident "from" "number" "to" "number" codeblock
   | "for" ident "from" "number" "to" "number" "step" "number" codeblock
   | "for" "(" ident : "ident" ")" codeblock
   ;

while: "while" "(" exp01 ")" codeblock
     | "do" codeblock "while" "(" exp01 ")"
     ;

trycatch: "try" codeblock "catch" "(" "ident" ")" codeblock
     ;

switch: "switch" "(" exp01 ")" "{" caselist "}"
     ;

caselist: case
        | case caselist
        ;

case: "case" exp01 ":" codeblock
    | "case" exp01 ":"
    | "default" ":" codeblock
    | "default" ":"
    ;

exp01: exp02
     | exp02 "?" exp01 ":" exp01
     ;
exp02: exp03
     | exp03 "||" exp01
     ;
exp03: exp04
     | exp04 "&&" exp01
     ;
exp04: exp05
     | exp05 "===" exp01
     | exp05 "!==" exp01
     | exp05 "==" exp01
     | exp05 "!=" exp01
     ;
exp05: exp06
     | exp06 "<"  exp01
     | exp06 "<=" exp01
     | exp06 ">"  exp01
     | exp06 ">=" exp01
     ;
exp06: exp07
     | exp07 "+" exp01
     | exp07 "-" exp01
     ;
exp07: exp08
     | exp08 "*" exp01
     | exp08 "/" exp01
     | exp08 "%" exp01
     ;
exp08: expp
     | "!" expp
     ;

expp: "return" exp01
    | "return"
    | "throw" exp01
    | "(" exp01 ")" "." ident "(" explist ")"
    | "(" exp01 ")"
    | ident "(" explist ")"
    | "nil"
    | value "." ident "(" explist ")"
    | value
    ;
value: function
     | "string"
     | array
     | "number"
     | "true"
     | "false"
     | ident
     ;
array: "[" "]"
     | "[" explist "]"
     ;
explist: exp01
       | exp01 ","
       | exp01 "," explist
       ;
/*
constant : INTEGER
           {
	       $$ = new CNConstant($1);
	   }
         | DOUBLE
           {
	       $$ = new CNConstant($1);
	   }

variable : STRING
           {
	       if (!driver.calc.existsVariable(*$1)) {
		   error(yyloc, std::string("Unknown variable \"") + *$1 + "\"");
		   delete $1;
		   YYERROR;
	       }
	       else {
		   $$ = new CNConstant( driver.calc.getVariable(*$1) );
		   delete $1;
	       }
	   }

atomexpr : constant
           {
	       $$ = $1;
	   }
         | variable
           {
	       $$ = $1;
	   }
         | '(' expr ')'
           {
	       $$ = $2;
	   }

powexpr	: atomexpr
          {
	      $$ = $1;
	  }
        | atomexpr '^' powexpr
          {
	      $$ = new CNPower($1, $3);
	  }

unaryexpr : powexpr
            {
		$$ = $1;
	    }
          | '+' powexpr
            {
		$$ = $2;
	    }
          | '-' powexpr
            {
		$$ = new CNNegate($2);
	    }

mulexpr : unaryexpr
          {
	      $$ = $1;
	  }
        | mulexpr '*' unaryexpr
          {
	      $$ = new CNMultiply($1, $3);
	  }
        | mulexpr '/' unaryexpr
          {
	      $$ = new CNDivide($1, $3);
	  }
        | mulexpr '%' unaryexpr
          {
	      $$ = new CNModulo($1, $3);
	  }

addexpr : mulexpr
          {
	      $$ = $1;
	  }
        | addexpr '+' mulexpr
          {
	      $$ = new CNAdd($1, $3);
	  }
        | addexpr '-' mulexpr
          {
	      $$ = new CNSubtract($1, $3);
	  }

expr	: addexpr
          {
	      $$ = $1;
	  }

assignment : STRING '=' expr
             {
		 driver.calc.variables[*$1] = $3->evaluate();
		 std::cout << "Setting variable " << *$1
			   << " = " << driver.calc.variables[*$1] << "\n";
		 delete $1;
		 delete $3;
	     }

start	: %empty
        | start ';'
        | start EOL
	| start assignment ';'
	| start assignment EOL
	| start assignment END
        | start expr ';'
          {
	      driver.calc.expressions.push_back($2);
	  }
        | start expr EOL
          {
	      driver.calc.expressions.push_back($2);
	  }
        | start expr END
          {
	      driver.calc.expressions.push_back($2);
	  }
	  */

 /*** END - Change the grammar rules above ***/
 /*** END - Change the grammar rules above ***/
 /*** END - Change the grammar rules above ***/

%% /*** Additional Code ***/

void example::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}
