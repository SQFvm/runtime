/* $Id: scanner.ll 44 2008-10-23 09:03:19Z tb $ -*- mode: c++ -*- */
/** \file scanner.ll Define the example Flex lexical scanner */

%{ /*** C/C++ Declarations ***/

#include <string>

#include "scanner.h"

/* import the parser's token type into a local typedef */
typedef example::Parser::token token;
typedef example::Parser::token_type token_type;

/* By default yylex returns int, we use token_type. Unfortunately yyterminate
 * by default returns 0, which is not of token_type. */
#define yyterminate() return token::END

/* This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. */
#define YY_NO_UNISTD_H

%}

/*** Flex Declarations and Options ***/

/* enable c++ scanner class generation */
%option c++

/* change the name of the scanner class. results in "ExampleFlexLexer" */
%option prefix="Example"

/* the manual says "somewhat more optimized" */
%option batch

/* enable scanner to generate debug output. disable this for release
 * versions. */
%option debug

/* no support for include files is planned */
%option yywrap nounput 

/* enables the use of start condition stacks */
%option stack

/* The following paragraph suffices to track locations accurately. Each time
 * yylex is invoked, the begin position is moved onto the end position. */
%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}


%s CCOMMENT

%% /*** Regular Expressions Part ***/

 /* code to place at the beginning of yylex() */
%{
    // reset location
    yylloc->step();
%}

 /*** BEGIN - Change the lexer rules below ***/
 /*** BEGIN - Change the lexer rules below ***/
 /*** BEGIN - Change the lexer rules below ***/

"//".*                                 { yylloc->step(); }                        // eat line comment
"/*"                                   { BEGIN(CCOMMENT); yylloc->step(); }       // start comment
<CCOMMENT>{
    "*/"                               { BEGIN(INITIAL); yylloc->step(); }        // end comment
    "*"                                { yylloc->step(); }                        // eat the lone star
    \n                                 { yylloc->lines(yyleng); yylloc->step(); } // eat newline
    [^*\n]+                            { yylloc->step(); }                        // eat comment in chunks 
}
"string"   { return token::STRING; }
"return"   { return token::RETURN; }
"throw"    { return token::THROW; }
"let"      { return token::LET; }
"be"       { return token::BE; }
"function" { return token::FUNCTION; }
"if"       { return token::IF; }
"else"     { return token::ELSE; }
"from"     { return token::FROM; }
"to"       { return token::TO; }
"step"     { return token::STEP; }
"while"    { return token::WHILE; }
"do"       { return token::DO; }
"try"      { return token::TRY; }
"catch"    { return token::CATCH; }
"switch"   { return token::SWITCH; }
"case"     { return token::CASE; }
"default"  { return token::DEFAULT; }
"nil"      { return token::NIL; }
"true"     { return token::TRUE; }
"false"    { return token::FALSE; }
"for"      { return token::FOR; }
"private"  { return token::PRIVATE; }

"{"        { return token::CURLYO; }
"}"        { return token::CURLYC; }
"("        { return token::ROUNDO; }
")"        { return token::ROUNDC; }
"["        { return token::SQUAREO; }
"]"        { return token::SQUAREC; }
";"        { return token::SEMICOLON; }
","        { return token::COMMA; }
"<="       { return token::LTEQUAL; }
"<"        { return token::LT; }
">="       { return token::GTEQUAL; }
">"        { return token::GT; }
"==="      { return token::EQUALEQUALEQUAL; }
"=="       { return token::EQUALEQUAL; }
"!=="      { return token::EXCLAMATIONMARKEQUALEQUAL; }
"!="       { return token::EXCLAMATIONMARKEQUAL; }
"!"        { return token::EXCLAMATIONMARK; }
"="        { return token::EQUAL; }
"&&"       { return token::ANDAND; }
/* "/="       { return token::SLASHEQUAL; } */
"/"        { return token::SLASH; }
/* "*="       { return token::STAREQUAL; } */
"*"        { return token::STAR; }
/* "+="       { return token::PLUSEQUAL; } */
"+"        { return token::PLUS; }
/* "-="       { return token::MINUSEQUAL; } */
"-"        { return token::MINUS; }
/* "%="       { return token::PERCENTEQUAL; } */
"%"        { return token::PERCENT; }
"?"        { return token::QUESTIONMARK; }
"||"       { return token::VLINEVLINE; }
"."        { return token::DOT; }
":"        { return token::COLON; }
[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)? { yylval->stringVal = new std::string(yytext, yyleng); return token::NUMBER; }
/*
0[xX][a-zA-Z0-9]+                      { yylval->stringVal = new std::string(yytext, yyleng); return token::HEXNUMBER; }
0[bB][01_]+                            { yylval->stringVal = new std::string(yytext, yyleng); return token::BINARYNUMBER; }
*/
\"(([^\"]|\\\")*[^\\])?\"              { yylval->stringVal = new std::string(yytext, yyleng); return token::STRINGVAL; }
[a-zA-Z_][a-zA-Z0-9_]*                 { yylval->stringVal = new std::string(yytext, yyleng); return token::IDENT; }

/* gobble up end-of-lines */
\n {
    yylloc->lines(yyleng); yylloc->step();
    return token::EOL;
}

/* gobble up white-spaces */
[ \t\r]+ {
    yylloc->step();
}

 /* pass all other characters up to bison */
. {
    return static_cast<token_type>(*yytext);
}



 /*** END - Change the lexer rules above ***/
 /*** END - Change the lexer rules above ***/
 /*** END - Change the lexer rules above ***/

%% /*** Additional Code ***/

namespace example {

Scanner::Scanner(std::istream* in,
		 std::ostream* out)
    : ExampleFlexLexer(in, out)
{
}

Scanner::~Scanner()
{
}

void Scanner::set_debug(bool b)
{
    yy_flex_debug = b;
}

}

/* This implementation of ExampleFlexLexer::yylex() is required to fill the
 * vtable of the class ExampleFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */

#ifdef yylex
#undef yylex
#endif

int ExampleFlexLexer::yylex()
{
    std::cerr << "in ExampleFlexLexer::yylex() !" << std::endl;
    return 0;
}

/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */

int ExampleFlexLexer::yywrap()
{
    return 1;
}
