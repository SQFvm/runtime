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
#line 9 "parser.y" // lalr1.cc:397

    #include "tokenizer.h"
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
#line 106 "parser.y" // lalr1.cc:413

     namespace sqf::sqc::bison
     {
          // Return the next token.
          parser::symbol_type yylex (sqf::sqc::tokenizer&);
     }

#line 69 "parser.tab.cc" // lalr1.cc:413


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

#line 8 "parser.y" // lalr1.cc:479
namespace  sqf { namespace sqc { namespace bison  {
#line 155 "parser.tab.cc" // lalr1.cc:479

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
  parser::parser (sqf::sqc::tokenizer &tokenizer_yyarg, sqf::sqc::bison::astnode& result_yyarg, sqf::sqc::parser& actual_yyarg, std::string fpath_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      tokenizer (tokenizer_yyarg),
      result (result_yyarg),
      actual (actual_yyarg),
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
      case 58: // statements
      case 59: // statement
      case 60: // assignment
      case 61: // vardecl
      case 62: // funcdecl
      case 63: // function
      case 64: // funchead
      case 65: // arglist
      case 66: // codeblock
      case 67: // if
      case 68: // for
      case 69: // while
      case 70: // trycatch
      case 71: // switch
      case 72: // caselist
      case 73: // case
      case 74: // exp01
      case 75: // exp02
      case 76: // exp03
      case 77: // exp04
      case 78: // exp05
      case 79: // exp06
      case 80: // exp07
      case 81: // exp08
      case 82: // exp09
      case 83: // expp
      case 84: // value
      case 85: // array
      case 86: // explist
        value.move< sqf::sqc::bison::astnode > (that.value);
        break;

      case 53: // NUMBER
      case 54: // IDENT
      case 55: // STRING
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
      case 58: // statements
      case 59: // statement
      case 60: // assignment
      case 61: // vardecl
      case 62: // funcdecl
      case 63: // function
      case 64: // funchead
      case 65: // arglist
      case 66: // codeblock
      case 67: // if
      case 68: // for
      case 69: // while
      case 70: // trycatch
      case 71: // switch
      case 72: // caselist
      case 73: // case
      case 74: // exp01
      case 75: // exp02
      case 76: // exp03
      case 77: // exp04
      case 78: // exp05
      case 79: // exp06
      case 80: // exp07
      case 81: // exp08
      case 82: // exp09
      case 83: // expp
      case 84: // value
      case 85: // array
      case 86: // explist
        value.copy< sqf::sqc::bison::astnode > (that.value);
        break;

      case 53: // NUMBER
      case 54: // IDENT
      case 55: // STRING
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
            symbol_type yylookahead (yylex (tokenizer));
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
      case 58: // statements
      case 59: // statement
      case 60: // assignment
      case 61: // vardecl
      case 62: // funcdecl
      case 63: // function
      case 64: // funchead
      case 65: // arglist
      case 66: // codeblock
      case 67: // if
      case 68: // for
      case 69: // while
      case 70: // trycatch
      case 71: // switch
      case 72: // caselist
      case 73: // case
      case 74: // exp01
      case 75: // exp02
      case 76: // exp03
      case 77: // exp04
      case 78: // exp05
      case 79: // exp06
      case 80: // exp07
      case 81: // exp08
      case 82: // exp09
      case 83: // expp
      case 84: // value
      case 85: // array
      case 86: // explist
        yylhs.value.build< sqf::sqc::bison::astnode > ();
        break;

      case 53: // NUMBER
      case 54: // IDENT
      case 55: // STRING
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
  case 3:
#line 198 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 647 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 201 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 653 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 202 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 659 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 205 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 665 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 206 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN }; }
#line 671 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 207 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::THROW }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 677 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 208 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 683 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 209 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 689 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 210 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 695 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 211 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 701 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 212 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 707 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 213 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 713 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 214 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 719 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 215 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 725 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 216 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 731 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 217 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 737 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 218 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 743 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 221 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 749 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 224 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 755 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 225 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 761 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 226 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 767 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 227 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 773 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 228 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 779 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 229 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 785 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 232 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION_DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 791 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 235 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 797 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 238 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST }; }
#line 803 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 239 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 809 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 242 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 815 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 243 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 821 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 244 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 827 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 247 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 833 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 248 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK }; }
#line 839 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 249 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 845 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 252 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IF }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 851 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 253 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IFELSE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 857 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 256 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOR }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 863 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 257 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORSTEP }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[7].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 869 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 258 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOREACH }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 875 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 261 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::WHILE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 881 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 262 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DOWHILE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 887 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 265 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::TRYCATCH }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 893 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 268 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SWITCH }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 899 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 271 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 905 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 272 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 911 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 275 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 917 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 276 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 923 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 277 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE_DEFAULT }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 929 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 280 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 935 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 281 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_TERNARY }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 941 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 283 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 947 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 284 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_OR }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 953 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 286 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 959 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 287 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_AND }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 965 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 289 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 971 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 290 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUALEXACT }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 977 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 291 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUALEXACT }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 983 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 292 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUAL }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 989 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 293 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUAL }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 995 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 295 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1001 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 296 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHAN }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1007 "parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 297 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHANEQUAL }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1013 "parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 298 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHAN }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1019 "parser.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 299 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHANEQUAL }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1025 "parser.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 301 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1031 "parser.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 302 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_PLUS }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1037 "parser.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 303 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MINUS }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1043 "parser.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 305 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1049 "parser.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 306 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MULTIPLY }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1055 "parser.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 307 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_DIVIDE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1061 "parser.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 308 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_REMAINDER }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1067 "parser.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 310 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1073 "parser.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 311 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOT }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ());  }
#line 1079 "parser.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 313 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1085 "parser.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 314 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_BINARY }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1091 "parser.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 317 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 1097 "parser.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 318 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_UNARY }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1103 "parser.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 319 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }; }
#line 1109 "parser.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 320 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1115 "parser.tab.cc" // lalr1.cc:859
    break;

  case 82:
#line 322 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1121 "parser.tab.cc" // lalr1.cc:859
    break;

  case 83:
#line 323 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 1127 "parser.tab.cc" // lalr1.cc:859
    break;

  case 84:
#line 324 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1133 "parser.tab.cc" // lalr1.cc:859
    break;

  case 85:
#line 325 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 1139 "parser.tab.cc" // lalr1.cc:859
    break;

  case 86:
#line 326 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_TRUE }; }
#line 1145 "parser.tab.cc" // lalr1.cc:859
    break;

  case 87:
#line 327 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_FALSE }; }
#line 1151 "parser.tab.cc" // lalr1.cc:859
    break;

  case 88:
#line 328 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NIL }; }
#line 1157 "parser.tab.cc" // lalr1.cc:859
    break;

  case 89:
#line 330 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY }; }
#line 1163 "parser.tab.cc" // lalr1.cc:859
    break;

  case 90:
#line 331 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1169 "parser.tab.cc" // lalr1.cc:859
    break;

  case 91:
#line 333 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1175 "parser.tab.cc" // lalr1.cc:859
    break;

  case 92:
#line 334 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1181 "parser.tab.cc" // lalr1.cc:859
    break;

  case 93:
#line 335 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1187 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1191 "parser.tab.cc" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -70;

  const signed char parser::yytable_ninf_ = -50;

  const short int
  parser::yypact_[] =
  {
     186,   -70,    70,   392,   -47,   -24,   -18,    -7,   318,   318,
       1,   -70,   -70,   -70,   -19,     8,   392,   380,   -70,   420,
     -70,   -20,   -70,    53,   -70,    19,   -70,    36,   -70,   -70,
     -70,   -70,   -70,   -70,   -70,    39,    26,    37,    43,    16,
      28,    44,   -31,   -70,    41,   -70,   -70,    67,   -70,    69,
      71,    80,    -1,   -16,    67,   318,   392,   392,   362,   -70,
      88,   100,   392,    65,   110,     5,    92,   -70,    89,    95,
     -70,   392,   392,   -70,   -70,   -70,   -70,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,    73,   -70,   -70,   392,   -70,   392,   -70,
      96,    99,   318,   -70,   101,   102,   -70,   105,   106,   107,
     104,   111,   392,   392,   -70,   392,   -70,   392,   -70,   108,
     -70,   113,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,
     -70,   -70,   -70,   -70,   -70,   -70,   -70,   114,   -70,   -70,
      85,   -70,   -70,   318,   318,   -70,   392,    91,   117,   392,
     135,   -70,   -70,   -70,   -70,   392,   392,   -70,   139,   -70,
     120,   121,    90,   122,   392,   -70,   123,   318,   -70,   318,
     392,   130,   129,    90,   318,   274,   -70,   -70,   -70,   132,
     318,   -70,   -70,   -70,   392,   -70,   230,   -70,   318,   -70,
     -70
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    86,    87,     0,     0,     0,     0,    18,     0,
      85,    80,    83,     0,     3,     0,    16,     0,    10,    82,
      11,    12,    13,    14,    15,     0,    51,    53,    55,    57,
      62,    67,    70,    74,    76,    81,    84,     0,     7,    80,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    34,
       0,     0,     0,     0,     0,     0,     0,    89,    91,     0,
      75,     0,     0,     1,     5,     9,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     6,     8,     0,    23,     0,    29,
      31,     0,     0,    28,     0,     0,    35,     0,     0,     0,
       0,     0,     0,     0,    26,     0,    78,    92,    90,     0,
      20,     0,    54,    56,    58,    60,    59,    61,    64,    63,
      66,    65,    68,    69,    72,    71,    73,     0,    22,    21,
      32,    30,    27,     0,     0,    36,     0,     0,     0,     0,
       0,    25,    24,    93,    79,     0,     0,    33,    37,    42,
       0,     0,     0,     0,     0,    52,     0,     0,    43,     0,
       0,     0,     0,    46,     0,     0,    77,    38,    44,     0,
       0,    45,    47,    41,     0,    39,     0,    50,     0,    48,
      40
  };

  const short int
  parser::yypgoto_[] =
  {
     -70,   -70,   -13,     3,   -70,   -70,   -70,   -70,   109,    18,
      -3,   -70,   -70,   -70,   -70,   -70,   -14,   -70,    -2,   -70,
      82,    86,     0,    20,    29,    -6,   142,   -70,   -70,   -70,
     -69
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    23,    24,    59,    26,    27,    28,    29,    55,   101,
      60,    30,    31,    32,    33,    34,   172,   173,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      69
  };

  const short int
  parser::yytable_[] =
  {
      50,    51,   119,    25,    53,    96,    61,    52,    71,    63,
      56,   113,    74,    99,    66,    68,    90,    91,    92,    -4,
       1,    57,     2,     3,     4,    72,     5,     6,    25,    62,
      54,    97,     7,     8,     9,    64,    10,   114,   100,    11,
      12,    13,    14,    15,    98,   107,    -4,    16,   153,    17,
     115,    18,   103,    73,   104,   105,    80,    81,    82,    83,
     110,    25,    65,    19,    84,    85,    86,    87,    75,    68,
     120,    76,    20,    21,    22,   121,    77,    47,    88,    89,
     124,   125,   126,   127,   134,   135,   136,   166,    78,    79,
      11,    12,    13,    93,   138,    53,   139,    71,    16,   142,
      17,   108,    48,    94,   128,   129,   130,   131,   170,   171,
     150,   151,    95,   152,    19,    68,   109,   132,   133,   111,
     112,   116,   117,    20,    49,    22,   118,   137,   141,   140,
     143,   144,   145,   148,   146,   147,   149,   154,   155,   100,
     158,   159,   156,   162,   160,   161,   164,   163,   167,   168,
     169,   174,   176,   165,    68,   180,   181,   186,   157,   182,
     122,    70,   175,   102,   177,   123,   178,     0,   179,     0,
       0,   183,   185,     0,     0,     0,     0,   187,     0,     0,
       0,     0,   188,   189,     0,   190,    -2,     1,     0,     2,
       3,     4,     0,     5,     6,     0,     0,     0,     0,     7,
       8,     9,     0,    10,     0,     0,    11,    12,    13,    14,
      15,     0,     0,     0,    16,     0,    17,     0,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,     1,     0,     2,     3,     4,     0,     5,     6,    20,
      21,    22,     0,     7,     8,     9,     0,    10,   -49,   -49,
      11,    12,    13,    14,    15,     0,    58,   -49,    16,     0,
      17,     0,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    19,     1,     0,     2,     3,     4,
       0,     5,     6,    20,    21,    22,   184,     7,     8,     9,
       0,    10,     0,     0,    11,    12,    13,    14,    15,     0,
      58,     0,    16,     0,    17,     0,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    19,     1,
       0,     2,     3,     4,     0,     5,     6,    20,    21,    22,
       0,     7,     8,     9,     0,    10,     0,     0,    11,    12,
      13,    14,    15,     0,    58,     0,    16,     0,    17,     0,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    19,     1,     0,     2,     3,     4,     0,     5,
       6,    20,    21,    22,     0,     7,     8,     9,     0,    10,
       0,     0,    11,    12,    13,    14,    15,    47,     0,   106,
      16,     0,    17,     0,    18,     0,     0,     0,     0,    47,
      11,    12,    13,     0,     0,     0,    19,     0,    16,     0,
      17,    67,    11,    12,    13,    20,    21,    22,     0,     0,
      16,     0,    17,     0,    19,     0,     0,    47,     0,     0,
       0,     0,     0,    20,    49,    22,    19,     0,     0,     0,
      11,    12,    13,     0,     0,    20,    49,    22,    16,     0,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    20,    49,    22
  };

  const short int
  parser::yycheck_[] =
  {
       2,     3,    71,     0,    28,     6,     9,    54,    28,    28,
      28,     6,    25,    29,    16,    17,    47,    48,    49,     0,
       1,    28,     3,     4,     5,    45,     7,     8,    25,    28,
      54,    32,    13,    14,    15,    54,    17,    32,    54,    20,
      21,    22,    23,    24,    45,    58,    27,    28,   117,    30,
      45,    32,    55,     0,    56,    57,    40,    41,    42,    43,
      62,    58,    54,    44,    36,    37,    38,    39,    32,    71,
      72,    32,    53,    54,    55,    77,    50,     7,    34,    35,
      80,    81,    82,    83,    90,    91,    92,   156,    51,    46,
      20,    21,    22,    52,    96,    28,    98,    28,    28,   102,
      30,    13,    32,    32,    84,    85,    86,    87,    18,    19,
     112,   113,    32,   115,    44,   117,    16,    88,    89,    54,
      10,    29,    33,    53,    54,    55,    31,    54,    29,    33,
      29,    29,    27,    29,    28,    28,    25,    29,    25,    54,
     143,   144,    28,    26,   146,    54,    11,   149,     9,    29,
      29,    29,    29,   155,   156,    25,    27,    25,   140,   173,
      78,    19,   164,    54,   167,    79,   169,    -1,   170,    -1,
      -1,   174,   175,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,   184,   186,    -1,   188,     0,     1,    -1,     3,
       4,     5,    -1,     7,     8,    -1,    -1,    -1,    -1,    13,
      14,    15,    -1,    17,    -1,    -1,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    28,    -1,    30,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,     1,    -1,     3,     4,     5,    -1,     7,     8,    53,
      54,    55,    -1,    13,    14,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    28,    -1,
      30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,     1,    -1,     3,     4,     5,
      -1,     7,     8,    53,    54,    55,    12,    13,    14,    15,
      -1,    17,    -1,    -1,    20,    21,    22,    23,    24,    -1,
      26,    -1,    28,    -1,    30,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,     1,
      -1,     3,     4,     5,    -1,     7,     8,    53,    54,    55,
      -1,    13,    14,    15,    -1,    17,    -1,    -1,    20,    21,
      22,    23,    24,    -1,    26,    -1,    28,    -1,    30,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,     1,    -1,     3,     4,     5,    -1,     7,
       8,    53,    54,    55,    -1,    13,    14,    15,    -1,    17,
      -1,    -1,    20,    21,    22,    23,    24,     7,    -1,    27,
      28,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,     7,
      20,    21,    22,    -1,    -1,    -1,    44,    -1,    28,    -1,
      30,    31,    20,    21,    22,    53,    54,    55,    -1,    -1,
      28,    -1,    30,    -1,    44,    -1,    -1,     7,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    44,    -1,    -1,    -1,
      20,    21,    22,    -1,    -1,    53,    54,    55,    28,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     1,     3,     4,     5,     7,     8,    13,    14,    15,
      17,    20,    21,    22,    23,    24,    28,    30,    32,    44,
      53,    54,    55,    57,    58,    59,    60,    61,    62,    63,
      67,    68,    69,    70,    71,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,     7,    32,    54,
      74,    74,    54,    28,    54,    64,    28,    28,    26,    59,
      66,    66,    28,    28,    54,    54,    74,    31,    74,    86,
      82,    28,    45,     0,    58,    32,    32,    50,    51,    46,
      40,    41,    42,    43,    36,    37,    38,    39,    34,    35,
      47,    48,    49,    52,    32,    32,     6,    32,    45,    29,
      54,    65,    64,    66,    74,    74,    27,    58,    13,    16,
      74,    54,    10,     6,    32,    45,    29,    33,    31,    86,
      74,    74,    76,    77,    78,    78,    78,    78,    79,    79,
      79,    79,    80,    80,    81,    81,    81,    54,    74,    74,
      33,    29,    66,    29,    29,    27,    28,    28,    29,    25,
      74,    74,    74,    86,    29,    25,    28,    65,    66,    66,
      74,    54,    26,    74,    11,    74,    86,     9,    29,    29,
      18,    19,    72,    73,    29,    74,    29,    66,    66,    74,
      25,    27,    72,    66,    12,    66,    25,    66,    74,    66,
      66
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    56,    57,    57,    58,    58,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      60,    61,    61,    61,    61,    61,    61,    62,    63,    64,
      64,    65,    65,    65,    66,    66,    66,    67,    67,    68,
      68,    68,    69,    69,    70,    71,    72,    72,    73,    73,
      73,    74,    74,    75,    75,    76,    76,    77,    77,    77,
      77,    77,    78,    78,    78,    78,    78,    79,    79,    79,
      80,    80,    80,    80,    81,    81,    82,    82,    83,    83,
      83,    83,    84,    84,    84,    84,    84,    84,    84,    85,
      85,    86,    86,    86
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     1,     2,     3,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       3,     4,     4,     3,     4,     4,     3,     4,     3,     2,
       3,     1,     2,     3,     1,     2,     3,     5,     7,     7,
       9,     7,     5,     6,     7,     7,     1,     2,     4,     3,
       3,     1,     5,     1,     3,     1,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     1,     2,     1,     6,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       3,     1,     2,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "NA", "error", "$undefined", "\"return\"", "\"throw\"", "\"let\"",
  "\"be\"", "\"function\"", "\"if\"", "\"else\"", "\"from\"", "\"to\"",
  "\"step\"", "\"while\"", "\"do\"", "\"try\"", "\"catch\"", "\"switch\"",
  "\"case\"", "\"default\"", "\"nil\"", "\"true\"", "\"false\"", "\"for\"",
  "\"private\"", "\":\"", "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"[\"",
  "\"]\"", "\";\"", "\",\"", "\"+\"", "\"-\"", "\"<=\"", "\"<\"", "\">=\"",
  "\">\"", "\"===\"", "\"==\"", "\"!==\"", "\"!=\"", "\"!\"", "\"=\"",
  "\"&&\"", "\"/\"", "\"*\"", "\"%\"", "\"?\"", "\"||\"", "\".\"",
  "NUMBER", "IDENT", "STRING", "$accept", "start", "statements",
  "statement", "assignment", "vardecl", "funcdecl", "function", "funchead",
  "arglist", "codeblock", "if", "for", "while", "trycatch", "switch",
  "caselist", "case", "exp01", "exp02", "exp03", "exp04", "exp05", "exp06",
  "exp07", "exp08", "exp09", "expp", "value", "array", "explist", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   197,   197,   198,   201,   202,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     221,   224,   225,   226,   227,   228,   229,   232,   235,   238,
     239,   242,   243,   244,   247,   248,   249,   252,   253,   256,
     257,   258,   261,   262,   265,   268,   271,   272,   275,   276,
     277,   280,   281,   283,   284,   286,   287,   289,   290,   291,
     292,   293,   295,   296,   297,   298,   299,   301,   302,   303,
     305,   306,   307,   308,   310,   311,   313,   314,   317,   318,
     319,   320,   322,   323,   324,   325,   326,   327,   328,   330,
     331,   333,   334,   335
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


#line 8 "parser.y" // lalr1.cc:1167
} } } //  sqf::sqc::bison 
#line 1749 "parser.tab.cc" // lalr1.cc:1167
#line 338 "parser.y" // lalr1.cc:1168


#include "sqc_parser.h"
namespace sqf::sqc::bison
{
     void parser::error (const location_type& loc, const std::string& msg)
     {
          actual.__log(logmessage::sqf::ParseError({ fpath, loc.begin.line, loc.begin.column }, msg));
     }
     inline parser::symbol_type yylex (sqf::sqc::tokenizer& tokenizer)
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
         case tokenizer::etoken::invalid: return yylex(tokenizer);
         case tokenizer::etoken::m_line: return yylex(tokenizer);
         case tokenizer::etoken::i_comment_line: return yylex(tokenizer);
         case tokenizer::etoken::i_comment_block: return yylex(tokenizer);
         case tokenizer::etoken::i_whitespace: return yylex(tokenizer);

         case tokenizer::etoken::t_return: return parser::make_RETURN(loc);
         case tokenizer::etoken::t_throw: return parser::make_THROW(loc);
         case tokenizer::etoken::t_let: return parser::make_LET(loc);
         case tokenizer::etoken::t_be: return parser::make_BE(loc);
         case tokenizer::etoken::t_function: return parser::make_FUNCTION(loc);
         case tokenizer::etoken::t_if: return parser::make_IF(loc);
         case tokenizer::etoken::t_else: return parser::make_ELSE(loc);
         case tokenizer::etoken::t_from: return parser::make_FROM(loc);
         case tokenizer::etoken::t_to: return parser::make_TO(loc);
         case tokenizer::etoken::t_step: return parser::make_STEP(loc);
         case tokenizer::etoken::t_while: return parser::make_WHILE(loc);
         case tokenizer::etoken::t_do: return parser::make_DO(loc);
         case tokenizer::etoken::t_try: return parser::make_TRY(loc);
         case tokenizer::etoken::t_catch: return parser::make_CATCH(loc);
         case tokenizer::etoken::t_switch: return parser::make_SWITCH(loc);
         case tokenizer::etoken::t_case: return parser::make_CASE(loc);
         case tokenizer::etoken::t_default: return parser::make_DEFAULT(loc);
         case tokenizer::etoken::t_nil: return parser::make_NIL(loc);
         case tokenizer::etoken::t_true: return parser::make_TRUE(loc);
         case tokenizer::etoken::t_false: return parser::make_FALSE(loc);
         case tokenizer::etoken::t_for: return parser::make_FOR(loc);
         case tokenizer::etoken::t_private: return parser::make_PRIVATE(loc);

         case tokenizer::etoken::s_curlyo: return parser::make_CURLYO(loc);
         case tokenizer::etoken::s_curlyc: return parser::make_CURLYC(loc);
         case tokenizer::etoken::s_roundo: return parser::make_ROUNDO(loc);
         case tokenizer::etoken::s_roundc: return parser::make_ROUNDC(loc);
         case tokenizer::etoken::s_edgeo: return parser::make_SQUAREO(loc);
         case tokenizer::etoken::s_edgec: return parser::make_SQUAREC(loc);
         case tokenizer::etoken::s_equalequalequal: return parser::make_EQUALEQUALEQUAL(loc);
         case tokenizer::etoken::s_equalequal: return parser::make_EQUALEQUAL(loc);
         case tokenizer::etoken::s_equal: return parser::make_EQUAL(loc);
         case tokenizer::etoken::s_greaterthenequal: return parser::make_GTEQUAL(loc);
         case tokenizer::etoken::s_greaterthen: return parser::make_GT(loc);
         case tokenizer::etoken::s_lessthenequal: return parser::make_LTEQUAL(loc);
         case tokenizer::etoken::s_lessthen: return parser::make_LT(loc);
         case tokenizer::etoken::s_plus: return parser::make_PLUS(loc);
         case tokenizer::etoken::s_minus: return parser::make_MINUS(loc);
         case tokenizer::etoken::s_notequalequal: return parser::make_EXCLAMATIONMARKEQUALEQUAL(loc);
         case tokenizer::etoken::s_notequal: return parser::make_EXCLAMATIONMARKEQUAL(loc);
         case tokenizer::etoken::s_exclamationmark: return parser::make_EXCLAMATIONMARK(loc);
         case tokenizer::etoken::s_percent: return parser::make_PERCENT(loc);
         case tokenizer::etoken::s_star: return parser::make_STAR(loc);
         case tokenizer::etoken::s_slash: return parser::make_SLASH(loc);
         case tokenizer::etoken::s_andand: return parser::make_ANDAND(loc);
         case tokenizer::etoken::s_oror: return parser::make_VLINEVLINE(loc);
         case tokenizer::etoken::s_questionmark: return parser::make_QUESTIONMARK(loc);
         case tokenizer::etoken::s_colon: return parser::make_COLON(loc);
         case tokenizer::etoken::s_semicolon: return parser::make_SEMICOLON(loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(loc);
         case tokenizer::etoken::s_dot: return parser::make_DOT(loc);

         case tokenizer::etoken::t_string: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_ident: return parser::make_IDENT(token, loc);
         case tokenizer::etoken::t_number: return parser::make_NUMBER(token, loc);
         default:
             return parser::make_NA(loc);
         }
     }
}
