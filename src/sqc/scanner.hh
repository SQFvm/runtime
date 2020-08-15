
#ifndef SCANPIT_HH_
# define SCANPIT_HH_

# include "parser.hh"


# ifndef YY_DECL
#  define YY_DECL parse::Parser::token_type                         \
     parse::Scanner::yylex(parse::Parser::semantic_type* yylval,    \
                              parse::Parser::location_type*,        \
                              parse::Driver& driver)
# endif


# ifndef __FLEX_LEXER_H
#  define yyFlexLexer parseFlexLexer
#  include <FlexLexer.h>
#  undef yyFlexLexer
# endif


namespace parse
{
    class Scanner : public parseFlexLexer
    {
        public:
            Scanner();

            virtual ~Scanner();

            virtual Parser::token_type yylex(
                Parser::semantic_type* yylval,
                Parser::location_type* l,
                Driver& driver);

            void set_debug(bool b);
    };
}

#endif // SCANPIT_HH_
