%{                                                            /* -*- C++ -*- */

#include "parser.hh"
#include "scanner.hh"
#include "driver.hh"

/*  Defines some macros to update locations */


#define STEP()                                      \
  do {                                              \
    driver.location_->step ();                      \
  } while (0)

#define COL(Col)				                    \
  driver.location_->columns (Col)

#define LINE(Line)				                    \
  do{						                        \
    driver.location_->lines (Line);		            \
 } while (0)

#define YY_USER_ACTION				                \
  COL(yyleng);


typedef parse::Parser::token token;
typedef parse::Parser::token_type token_type;

#define yyterminate() return token::TOK_EOF

%}

%option debug
%option c++
%option noyywrap
%option never-interactive
%option yylineno
%option nounput
%option batch
%option prefix="parse"

/*
%option stack
*/

/* Abbreviations.  */

/* blank               [ \t]+   */
/* eol                 [\n\r]+  */
number_science      [-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?
number_hex          0[xX][a-zA-Z0-9]+
number_binary       0[bB][01_]+
string              \"(([^\"]|\\\")*[^\\])?\"
identifier          [a-zA-Z_][a-zA-Z0-9_]*

%s CCOMMENT

%% /*** Regular Expressions Part ***/

 /* The rules.  */
%{
  STEP();
%}

 /*** BEGIN - Change the lexer rules below ***/
 /*** BEGIN - Change the lexer rules below ***/
 /*** BEGIN - Change the lexer rules below ***/

"//".*                                 { STEP(); }                        // eat line comment
"/*"                                   { BEGIN(CCOMMENT); STEP(); }       // start comment
<CCOMMENT>{
    "*/"                               { BEGIN(INITIAL); STEP(); }        // end comment
    "*"                                { STEP(); }                        // eat the lone star
    \n                                 { LINE(yyleng); } // eat newline
    [^*\n]+                            { STEP(); }                        // eat comment in chunks 
}
"string"        { return token::STRING; }
"return"        { return token::RETURN; }
"throw"         { return token::THROW; }
"let"           { return token::LET; }
"be"            { return token::BE; }
"function"      { return token::FUNCTION; }
"if"            { return token::IF; }
"else"          { return token::ELSE; }
"from"          { return token::FROM; }
"to"            { return token::TO; }
"step"          { return token::STEP; }
"while"         { return token::WHILE; }
"do"            { return token::DO; }
"try"           { return token::TRY; }
"catch"         { return token::CATCH; }
"switch"        { return token::SWITCH; }
"case"          { return token::CASE; }
"default"       { return token::DEFAULT; }
"nil"           { return token::NIL; }
"true"          { return token::TRUE; }
"false"         { return token::FALSE; }
"for"           { return token::FOR; }
"private"       { return token::PRIVATE; }

"{"             { return token::CURLYO; }
"}"             { return token::CURLYC; }
"("             { return token::ROUNDO; }
")"             { return token::ROUNDC; }
"["             { return token::SQUAREO; }
"]"             { return token::SQUAREC; }
";"             { return token::SEMICOLON; }
","             { return token::COMMA; }
"<="            { return token::LTEQUAL; }
"<"             { return token::LT; }
">="            { return token::GTEQUAL; }
">"             { return token::GT; }
"==="           { return token::EQUALEQUALEQUAL; }
"=="            { return token::EQUALEQUAL; }
"!=="           { return token::EXCLAMATIONMARKEQUALEQUAL; }
"!="            { return token::EXCLAMATIONMARKEQUAL; }
"!"             { return token::EXCLAMATIONMARK; }
"="             { return token::EQUAL; }
"&&"            { return token::ANDAND; }
"/"             { return token::SLASH; }
"*"             { return token::STAR; }
"+"             { return token::PLUS; }
"-"             { return token::MINUS; }
"%"             { return token::PERCENT; }
"?"             { return token::QUESTIONMARK; }
"||"            { return token::VLINEVLINE; }
"."             { return token::DOT; }
":"             { return token::COLON; }
number_science  { yylval->stringVal = std::string(yytext, yyleng); return token::NUMBER; } 
string          { yylval->stringVal = std::string(yytext, yyleng); return token::STRINGVAL; }
identifier      { yylval->stringVal = std::string(yytext, yyleng); return token::IDENT; }


\n              { LINE(yyleng); STEP(); } // eat newline
[ \t\r]+        { STEP(); } // eat whitespace
.               { return static_cast<token_type>(*yytext); } // pass all other characters up to bison



 /*** END - Change the lexer rules above ***/
 /*** END - Change the lexer rules above ***/
 /*** END - Change the lexer rules above ***/

%% /*** Additional Code ***/


/*

   CUSTOM C++ CODE

*/

namespace parse
{

    Scanner::Scanner()
    : parseFlexLexer()
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

#ifdef yylex
# undef yylex
#endif

int parseFlexLexer::yylex()
{
  std::cerr << "call parseFlexLexer::yylex()!" << std::endl;
  return 0;
}
