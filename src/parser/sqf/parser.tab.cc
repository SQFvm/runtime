// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.
// //                    "%code top" blocks.
#line 10 "parser.y" // lalr1.cc:397

    #include "tokenizer.hpp"
    #include <string>
    #include <vector>

#line 40 "parser.tab.cc" // lalr1.cc:397


// First part of user declarations.

#line 45 "parser.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.tab.hh"

// User implementation prologue.

#line 59 "parser.tab.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 83 "parser.y" // lalr1.cc:413

    
    #include "../../runtime/runtime.h"
     namespace sqf::parser::sqf::bison
     {
          // Return the next token.
          parser::symbol_type yylex (::sqf::runtime::runtime &runtime, ::sqf::parser::sqf::tokenizer&);
     }

#line 71 "parser.tab.cc" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 9 "parser.y" // lalr1.cc:479
namespace sqf { namespace parser { namespace sqf { namespace bison  {
#line 157 "parser.tab.cc" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (::sqf::parser::sqf::tokenizer &tokenizer_yyarg, ::sqf::parser::sqf::bison::astnode& result_yyarg, ::sqf::parser::sqf::parser& actual_yyarg, ::sqf::runtime::runtime &runtime_yyarg, std::string fpath_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      tokenizer (tokenizer_yyarg),
      result (result_yyarg),
      actual (actual_yyarg),
      runtime (runtime_yyarg),
      fpath (fpath_yyarg)
  {}

  parser::~parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  parser::symbol_number_type
  parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 15: // OPERATOR_0
      case 16: // IDENT_0
      case 17: // OPERATOR_1
      case 18: // IDENT_1
      case 19: // OPERATOR_2
      case 20: // IDENT_2
      case 21: // OPERATOR_3
      case 22: // IDENT_3
      case 23: // OPERATOR_4
      case 24: // IDENT_4
      case 25: // OPERATOR_5
      case 26: // IDENT_5
      case 27: // OPERATOR_6
      case 28: // IDENT_6
      case 29: // OPERATOR_7
      case 30: // IDENT_7
      case 31: // OPERATOR_8
      case 32: // IDENT_8
      case 33: // OPERATOR_9
      case 34: // IDENT_9
      case 35: // OPERATOR_U
      case 36: // IDENT_U
      case 37: // IDENT_N
      case 38: // IDENT
      case 39: // NUMBER
      case 40: // STRING
        value.move< tokenizer::token > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 15: // OPERATOR_0
      case 16: // IDENT_0
      case 17: // OPERATOR_1
      case 18: // IDENT_1
      case 19: // OPERATOR_2
      case 20: // IDENT_2
      case 21: // OPERATOR_3
      case 22: // IDENT_3
      case 23: // OPERATOR_4
      case 24: // IDENT_4
      case 25: // OPERATOR_5
      case 26: // IDENT_5
      case 27: // OPERATOR_6
      case 28: // IDENT_6
      case 29: // OPERATOR_7
      case 30: // IDENT_7
      case 31: // OPERATOR_8
      case 32: // IDENT_8
      case 33: // OPERATOR_9
      case 34: // IDENT_9
      case 35: // OPERATOR_U
      case 36: // IDENT_U
      case 37: // IDENT_N
      case 38: // IDENT
      case 39: // NUMBER
      case 40: // STRING
        value.copy< tokenizer::token > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (runtime, tokenizer));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 15: // OPERATOR_0
      case 16: // IDENT_0
      case 17: // OPERATOR_1
      case 18: // IDENT_1
      case 19: // OPERATOR_2
      case 20: // IDENT_2
      case 21: // OPERATOR_3
      case 22: // IDENT_3
      case 23: // OPERATOR_4
      case 24: // IDENT_4
      case 25: // OPERATOR_5
      case 26: // IDENT_5
      case 27: // OPERATOR_6
      case 28: // IDENT_6
      case 29: // OPERATOR_7
      case 30: // IDENT_7
      case 31: // OPERATOR_8
      case 32: // IDENT_8
      case 33: // OPERATOR_9
      case 34: // IDENT_9
      case 35: // OPERATOR_U
      case 36: // IDENT_U
      case 37: // IDENT_N
      case 38: // IDENT
      case 39: // NUMBER
      case 40: // STRING
        yylhs.value.build< tokenizer::token > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {

#line 621 "parser.tab.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -7;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
       7,    -7,    -7,    15,    -1,    23,    64,    23,    23,    -7,
       8,    -7,    -7,    24,    36,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    41,    54,    69,    70,    71,    72,    78,    85,
      68,    83,    23,    16,    -7,    -7,    74,    -7,    32,    -7,
      -7,     3,    -7,    -7,    23,    -7,     7,     7,    23,    23,
      23,    23,    23,    23,    23,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    23,    23,    23,    -7,
      -7,    -7,    73,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7
  };

  const unsigned char
  parser::yydefact_[] =
  {
       2,    14,    13,     0,     0,     0,     0,     0,     0,    10,
      11,    12,     9,     0,     3,     4,    60,    15,    16,     7,
       8,    25,    26,    29,    32,    35,    38,    41,    44,    47,
      50,    53,     0,    11,    58,    20,     0,    11,     0,    22,
      17,     0,    56,    57,     0,     1,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
      59,    21,     0,    24,     5,     6,    27,    28,    30,    31,
      33,    34,    36,    37,    39,    40,    42,    43,    45,    46,
      48,    49,    51,    52,    54,    55,    23,    18
  };

  const signed char
  parser::yypgoto_[] =
  {
      -7,    -7,     4,    61,    -6,    -7,    -7,    -7,    -7,    -4,
      75,    76,    66,    67,    79,    80,    81,    82,    65,    84,
      77
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    13,    14,    15,    16,    41,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31
  };

  const unsigned char
  parser::yytable_[] =
  {
      40,    38,     1,     2,     3,     4,    35,     5,    36,     6,
       1,     2,     3,     4,    71,     5,    72,     6,     1,     2,
      32,     4,    44,     5,    45,     6,     1,     2,    32,     4,
      68,     5,     0,     6,     7,     8,     9,    10,    11,    12,
      73,    70,     7,     8,     9,    10,    11,    12,    46,    47,
       7,     8,     9,    33,    11,    12,    48,    49,     7,     8,
       9,    37,    11,    12,    96,     0,    97,     1,     2,     0,
       4,    50,    51,     0,     6,    39,     1,     2,     0,     4,
      34,    69,     0,     6,    42,    43,    46,    47,    52,    53,
       0,    54,    55,     0,    56,    57,     0,    58,    59,    64,
      65,     9,    37,    11,    12,    60,    61,    74,    75,    34,
       9,    37,    11,    12,    62,    63,    66,    67,    80,    81,
       0,    82,    83,    76,    77,     0,    78,    79,     0,    92,
      93,     0,     0,     0,     0,    84,    85,     0,    86,    87,
       0,    88,    89,     0,    90,    91,     0,     0,     0,     0,
      94,    95
  };

  const signed char
  parser::yycheck_[] =
  {
       6,     5,     3,     4,     5,     6,     7,     8,     4,    10,
       3,     4,     5,     6,    11,     8,    13,    10,     3,     4,
       5,     6,    14,     8,     0,    10,     3,     4,     5,     6,
      14,     8,    -1,    10,    35,    36,    37,    38,    39,    40,
      44,     9,    35,    36,    37,    38,    39,    40,    12,    13,
      35,    36,    37,    38,    39,    40,    15,    16,    35,    36,
      37,    38,    39,    40,    68,    -1,    72,     3,     4,    -1,
       6,    17,    18,    -1,    10,    11,     3,     4,    -1,     6,
       3,     7,    -1,    10,     7,     8,    12,    13,    19,    20,
      -1,    21,    22,    -1,    23,    24,    -1,    25,    26,    31,
      32,    37,    38,    39,    40,    27,    28,    46,    47,    32,
      37,    38,    39,    40,    29,    30,    33,    34,    52,    53,
      -1,    54,    55,    48,    49,    -1,    50,    51,    -1,    64,
      65,    -1,    -1,    -1,    -1,    56,    57,    -1,    58,    59,
      -1,    60,    61,    -1,    62,    63,    -1,    -1,    -1,    -1,
      66,    67
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     3,     4,     5,     6,     8,    10,    35,    36,    37,
      38,    39,    40,    42,    43,    44,    45,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,     5,    38,    61,     7,    43,    38,    50,    11,
      45,    46,    61,    61,    14,     0,    12,    13,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    14,     7,
       9,    11,    13,    50,    44,    44,    51,    51,    52,    52,
      53,    53,    54,    54,    55,    55,    56,    56,    57,    57,
      58,    58,    59,    59,    60,    60,    50,    45
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    41,    42,    42,    43,    43,    43,    44,    44,    45,
      45,    45,    45,    45,    45,    45,    45,    46,    46,    47,
      47,    48,    48,    49,    49,    50,    51,    51,    51,    52,
      52,    52,    53,    53,    53,    54,    54,    54,    55,    55,
      55,    56,    56,    56,    57,    57,    57,    58,    58,    58,
      59,    59,    59,    60,    60,    60,    61,    61,    61,    61,
      61
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     3,     2,     4,     3,     1,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     2,     2,     2,     3,
       1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "NA", "error", "$undefined", "\"false\"", "\"true\"", "\"private\"",
  "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"[\"", "\"]\"", "\";\"", "\",\"",
  "\"=\"", "OPERATOR_0", "IDENT_0", "OPERATOR_1", "IDENT_1", "OPERATOR_2",
  "IDENT_2", "OPERATOR_3", "IDENT_3", "OPERATOR_4", "IDENT_4",
  "OPERATOR_5", "IDENT_5", "OPERATOR_6", "IDENT_6", "OPERATOR_7",
  "IDENT_7", "OPERATOR_8", "IDENT_8", "OPERATOR_9", "IDENT_9",
  "OPERATOR_U", "IDENT_U", "IDENT_N", "IDENT", "NUMBER", "STRING",
  "$accept", "start", "statements", "statement", "value", "value_list",
  "code", "array", "assignment", "expression", "exp0", "exp1", "exp2",
  "exp3", "exp4", "exp5", "exp6", "exp7", "exp8", "exp9", "expu", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,   161,   161,   162,   164,   165,   166,   168,   169,   172,
     173,   174,   175,   176,   177,   178,   179,   181,   182,   184,
     185,   187,   188,   190,   191,   193,   195,   196,   197,   199,
     200,   201,   203,   204,   205,   207,   208,   209,   211,   212,
     213,   215,   216,   217,   219,   220,   221,   223,   224,   225,
     227,   228,   229,   231,   232,   233,   235,   236,   237,   238,
     239
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 9 "parser.y" // lalr1.cc:1167
} } } } //  ::sqf::parser::sqf::bison 
#line 1071 "parser.tab.cc" // lalr1.cc:1167
#line 243 "parser.y" // lalr1.cc:1168


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
         
         case tokenizer::etoken::t_false: return parser::make_FALSE(loc);
         case tokenizer::etoken::t_private: return parser::make_PRIVATE(loc);
         case tokenizer::etoken::t_true: return parser::make_TRUE(loc);

         case tokenizer::etoken::s_curlyo: return parser::make_CURLYO(loc);
         case tokenizer::etoken::s_curlyc: return parser::make_CURLYC(loc);
         case tokenizer::etoken::s_roundo: return parser::make_ROUNDO(loc);
         case tokenizer::etoken::s_roundc: return parser::make_ROUNDC(loc);
         case tokenizer::etoken::s_edgeo: return parser::make_SQUAREO(loc);
         case tokenizer::etoken::s_edgec: return parser::make_SQUAREC(loc);
         case tokenizer::etoken::s_semicolon: return parser::make_SEMICOLON(loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(loc);

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
         case tokenizer::etoken::t_hexadecimal: return parser::make_NUMBER(token, loc);
         default:
             return parser::make_NA(loc);
         }
     }
}
