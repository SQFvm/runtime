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
#line 86 "parser.y" // lalr1.cc:413

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
  parser::parser (sqf::sqc::tokenizer &tokenizer_yyarg, sqf::sqc::bison::astnode result_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      tokenizer (tokenizer_yyarg),
      result (result_yyarg)
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
#line 175 "parser.y" // lalr1.cc:859
    { result = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 645 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 178 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 651 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 179 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 657 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 182 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::RETURN }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 663 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 183 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::RETURN }; }
#line 669 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 184 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::THROW }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 675 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 185 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 681 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 186 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 687 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 187 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 693 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 188 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 699 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 189 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 705 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 190 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 711 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 191 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 717 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 192 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 723 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 193 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 729 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 194 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; }
#line 735 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 195 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; }
#line 741 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 198 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::ASSIGNMENT }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 747 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 201 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 753 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 202 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 759 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 203 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::FORWARD_DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 765 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 204 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 771 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 205 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 777 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 206 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::FORWARD_DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 783 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 209 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::FUNCTION_DECLARATION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 789 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 212 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::FUNCTION }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 795 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 215 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::ARGLIST }; }
#line 801 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 216 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::ARGLIST }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 807 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 219 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 813 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 220 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 819 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 221 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 825 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 224 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::CODEBLOCK }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 831 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 225 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::CODEBLOCK }; }
#line 837 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 226 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::CODEBLOCK }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 843 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 229 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::IF }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 849 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 230 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::IFELSE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 855 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 233 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::FOR }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 861 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 234 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::FORSTEP }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[7].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 867 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 235 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::FOREACH }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 873 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 238 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::WHILE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 879 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 239 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::DOWHILE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 885 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 242 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::TRYCATCH }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 891 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 245 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::SWITCH }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 897 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 248 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 903 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 249 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 909 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 252 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::CASE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 915 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 253 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::CASE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 921 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 254 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::CASE_DEFAULT }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 927 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 257 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 933 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 258 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_TERNARY }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 939 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 260 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 945 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 261 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_OR }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 951 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 263 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 957 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 264 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_AND }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 963 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 266 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 969 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 267 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_EQUALEXACT }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 975 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 268 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_NOTEQUALEXACT }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 981 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 269 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_EQUAL }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 987 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 270 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_NOTEQUAL }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 993 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 272 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 999 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 273 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_LESSTHAN }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1005 "parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 274 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_LESSTHANEQUAL }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1011 "parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 275 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_GREATERTHAN }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1017 "parser.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 276 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_GREATERTHANEQUAL }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1023 "parser.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 278 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1029 "parser.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 279 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_PLUS }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1035 "parser.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 280 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_MINUS }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1041 "parser.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 282 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1047 "parser.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 283 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_MULTIPLY }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1053 "parser.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 284 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_DIVIDE }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1059 "parser.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 285 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_REMAINDER }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1065 "parser.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 287 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1071 "parser.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 288 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_NOT }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ());  }
#line 1077 "parser.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 290 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1083 "parser.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 291 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_BINARY }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1089 "parser.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 294 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 1095 "parser.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 295 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::OP_UNARY }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1101 "parser.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 296 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1107 "parser.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 298 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1113 "parser.tab.cc" // lalr1.cc:859
    break;

  case 82:
#line 299 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::VAL_STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 1119 "parser.tab.cc" // lalr1.cc:859
    break;

  case 83:
#line 300 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1125 "parser.tab.cc" // lalr1.cc:859
    break;

  case 84:
#line 301 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::VAL_NUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 1131 "parser.tab.cc" // lalr1.cc:859
    break;

  case 85:
#line 302 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::VAL_TRUE }; }
#line 1137 "parser.tab.cc" // lalr1.cc:859
    break;

  case 86:
#line 303 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::VAL_FALSE }; }
#line 1143 "parser.tab.cc" // lalr1.cc:859
    break;

  case 87:
#line 304 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::VAL_NIL }; }
#line 1149 "parser.tab.cc" // lalr1.cc:859
    break;

  case 88:
#line 305 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }; }
#line 1155 "parser.tab.cc" // lalr1.cc:859
    break;

  case 89:
#line 307 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::VAL_ARRAY }; }
#line 1161 "parser.tab.cc" // lalr1.cc:859
    break;

  case 90:
#line 308 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = { astkind::VAL_ARRAY }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1167 "parser.tab.cc" // lalr1.cc:859
    break;

  case 91:
#line 310 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1173 "parser.tab.cc" // lalr1.cc:859
    break;

  case 92:
#line 311 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1179 "parser.tab.cc" // lalr1.cc:859
    break;

  case 93:
#line 312 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = {}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1185 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1189 "parser.tab.cc" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -63;

  const signed char parser::yytable_ninf_ = -50;

  const short int
  parser::yypact_[] =
  {
     187,   -63,    85,   381,   -52,   -11,   -17,   -15,   319,   319,
     -12,   -63,   -63,   -63,    -1,   -47,   381,   103,   -63,   392,
     -63,   -16,   -63,    20,   -63,    18,   -63,    -8,   -63,   -63,
     -63,   -63,   -63,   -63,   -63,    12,     1,    40,    28,    23,
      64,    33,    10,   -63,    41,   -63,   -63,    76,   -63,    80,
      77,    86,     2,   -24,    76,   319,   381,   381,   363,    87,
     107,   105,   381,    68,   116,     4,    98,   -63,    95,    99,
     -63,   381,   381,   -63,   -63,   -63,   -63,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,    78,   -63,   -63,   381,   -63,   381,   -63,
     102,   108,   319,   -63,   114,   115,   -63,   109,   -63,   118,
     121,   122,   125,   381,   381,   -63,   381,   -63,   381,   -63,
     123,   -63,   128,   -63,   -63,   -63,   -63,   -63,   -63,   -63,
     -63,   -63,   -63,   -63,   -63,   -63,   -63,   -63,   131,   -63,
     -63,   106,   -63,   -63,   319,   319,   -63,   381,   110,   135,
     381,   151,   -63,   -63,   -63,   -63,   381,   381,   -63,   157,
     -63,   139,   141,    79,   142,   381,   -63,   145,   319,   -63,
     319,   381,   150,   149,    79,   319,   275,   -63,   -63,   -63,
     152,   319,   -63,   -63,   -63,   381,   -63,   231,   -63,   319,
     -63,   -63
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    85,    86,     0,     0,     0,     0,    18,     0,
      84,    88,    82,     0,     3,     0,    16,     0,    10,    81,
      11,    12,    13,    14,    15,     0,    51,    53,    55,    57,
      62,    67,    70,    74,    76,    80,    83,     0,     7,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    91,     0,
      75,     0,     0,     1,     5,     9,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     6,     8,     0,    23,     0,    29,
      31,     0,     0,    28,     0,     0,    35,     0,    34,     0,
       0,     0,     0,     0,     0,    26,     0,    78,    92,    90,
       0,    20,     0,    54,    56,    58,    60,    59,    61,    64,
      63,    66,    65,    68,    69,    72,    71,    73,     0,    22,
      21,    32,    30,    27,     0,     0,    36,     0,     0,     0,
       0,     0,    25,    24,    93,    79,     0,     0,    33,    37,
      42,     0,     0,     0,     0,     0,    52,     0,     0,    43,
       0,     0,     0,     0,    46,     0,     0,    77,    38,    44,
       0,     0,    45,    47,    41,     0,    39,     0,    50,     0,
      48,    40
  };

  const short int
  parser::yypgoto_[] =
  {
     -63,   -63,   -21,     3,   -63,   -63,   -63,   -63,   126,    38,
      -3,   -63,   -63,   -63,   -63,   -63,     7,   -63,    -2,   -63,
     -63,   -63,   -63,   -63,   -63,   -63,   163,   -63,   -63,   -63,
     -62
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    23,    24,    59,    26,    27,    28,    29,    55,   101,
      60,    30,    31,    32,    33,    34,   173,   174,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      69
  };

  const short int
  parser::yytable_[] =
  {
      50,    51,    52,    25,    74,    99,    61,    65,    96,   120,
     114,    56,    71,    57,    66,    68,    62,    53,    -4,     1,
      73,     2,     3,     4,    75,     5,     6,    63,    25,    72,
     100,     7,     8,     9,    97,    10,   115,   107,    11,    12,
      13,    14,    15,    54,    76,    -4,    16,    98,    17,   116,
      18,    77,   103,    64,   104,   105,   154,    90,    91,    92,
     111,    25,    19,    80,    81,    82,    83,    88,    89,    68,
     121,    20,    21,    22,    79,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    78,    47,    93,   139,   167,   140,   171,   172,   143,
      84,    85,    86,    87,    53,    11,    12,    13,    71,    94,
      47,   151,   152,    16,   153,    17,    68,    48,    95,   108,
     109,   110,   112,    11,    12,    13,   113,   117,   118,    19,
     119,    16,   138,    17,    67,   141,   146,   142,    20,    49,
      22,   159,   160,   144,   145,   161,   147,    19,   164,   148,
     150,   149,   155,   156,   166,    68,    20,    49,    22,   157,
     100,   163,   165,   176,   162,   178,   168,   179,   169,   180,
     170,   175,   184,   186,   177,   181,   182,   187,   188,   158,
     102,   183,    70,   189,   190,     0,   191,    -2,     1,     0,
       2,     3,     4,     0,     5,     6,     0,     0,     0,     0,
       7,     8,     9,     0,    10,     0,     0,    11,    12,    13,
      14,    15,     0,     0,     0,    16,     0,    17,     0,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    19,     1,     0,     2,     3,     4,     0,     5,     6,
      20,    21,    22,     0,     7,     8,     9,     0,    10,   -49,
     -49,    11,    12,    13,    14,    15,     0,    58,   -49,    16,
       0,    17,     0,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    19,     1,     0,     2,     3,
       4,     0,     5,     6,    20,    21,    22,   185,     7,     8,
       9,     0,    10,     0,     0,    11,    12,    13,    14,    15,
       0,    58,     0,    16,     0,    17,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
       1,     0,     2,     3,     4,     0,     5,     6,    20,    21,
      22,     0,     7,     8,     9,     0,    10,     0,     0,    11,
      12,    13,    14,    15,     0,    58,     0,    16,     0,    17,
       0,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    19,     1,     0,     2,     3,     4,     0,
       5,     6,    20,    21,    22,     0,     7,     8,     9,     0,
      10,     0,     0,    11,    12,    13,    14,    15,    47,     0,
     106,    16,     0,    17,     0,    18,     0,     0,     0,    47,
       0,    11,    12,    13,     0,     0,     0,    19,     0,    16,
       0,    17,    11,    12,    13,     0,    20,    21,    22,     0,
      16,     0,    17,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,     0,    20,    49,    22,     0,     0,     0,
       0,     0,     0,     0,     0,    20,    49,    22
  };

  const short int
  parser::yycheck_[] =
  {
       2,     3,    54,     0,    25,    29,     9,    54,     6,    71,
       6,    28,    28,    28,    16,    17,    28,    28,     0,     1,
       0,     3,     4,     5,    32,     7,     8,    28,    25,    45,
      54,    13,    14,    15,    32,    17,    32,    58,    20,    21,
      22,    23,    24,    54,    32,    27,    28,    45,    30,    45,
      32,    50,    55,    54,    56,    57,   118,    47,    48,    49,
      62,    58,    44,    40,    41,    42,    43,    34,    35,    71,
      72,    53,    54,    55,    46,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    51,     7,    52,    96,   157,    98,    18,    19,   102,
      36,    37,    38,    39,    28,    20,    21,    22,    28,    32,
       7,   113,   114,    28,   116,    30,   118,    32,    32,    32,
      13,    16,    54,    20,    21,    22,    10,    29,    33,    44,
      31,    28,    54,    30,    31,    33,    27,    29,    53,    54,
      55,   144,   145,    29,    29,   147,    28,    44,   150,    28,
      25,    29,    29,    25,   156,   157,    53,    54,    55,    28,
      54,    26,    11,   165,    54,   168,     9,   170,    29,   171,
      29,    29,   175,   176,    29,    25,    27,    25,   181,   141,
      54,   174,    19,   185,   187,    -1,   189,     0,     1,    -1,
       3,     4,     5,    -1,     7,     8,    -1,    -1,    -1,    -1,
      13,    14,    15,    -1,    17,    -1,    -1,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    28,    -1,    30,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,     1,    -1,     3,     4,     5,    -1,     7,     8,
      53,    54,    55,    -1,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    28,
      -1,    30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,     1,    -1,     3,     4,
       5,    -1,     7,     8,    53,    54,    55,    12,    13,    14,
      15,    -1,    17,    -1,    -1,    20,    21,    22,    23,    24,
      -1,    26,    -1,    28,    -1,    30,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
       1,    -1,     3,     4,     5,    -1,     7,     8,    53,    54,
      55,    -1,    13,    14,    15,    -1,    17,    -1,    -1,    20,
      21,    22,    23,    24,    -1,    26,    -1,    28,    -1,    30,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,     1,    -1,     3,     4,     5,    -1,
       7,     8,    53,    54,    55,    -1,    13,    14,    15,    -1,
      17,    -1,    -1,    20,    21,    22,    23,    24,     7,    -1,
      27,    28,    -1,    30,    -1,    32,    -1,    -1,    -1,     7,
      -1,    20,    21,    22,    -1,    -1,    -1,    44,    -1,    28,
      -1,    30,    20,    21,    22,    -1,    53,    54,    55,    -1,
      28,    -1,    30,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55
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
      54,    65,    64,    66,    74,    74,    27,    58,    32,    13,
      16,    74,    54,    10,     6,    32,    45,    29,    33,    31,
      86,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    54,    74,
      74,    33,    29,    66,    29,    29,    27,    28,    28,    29,
      25,    74,    74,    74,    86,    29,    25,    28,    65,    66,
      66,    74,    54,    26,    74,    11,    74,    86,     9,    29,
      29,    18,    19,    72,    73,    29,    74,    29,    66,    66,
      74,    25,    27,    72,    66,    12,    66,    25,    66,    74,
      66,    66
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
      83,    84,    84,    84,    84,    84,    84,    84,    84,    85,
      85,    86,    86,    86
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     1,     2,     3,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       3,     4,     4,     3,     4,     4,     3,     4,     3,     2,
       3,     1,     2,     3,     2,     2,     3,     5,     7,     7,
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
       0,   174,   174,   175,   178,   179,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     198,   201,   202,   203,   204,   205,   206,   209,   212,   215,
     216,   219,   220,   221,   224,   225,   226,   229,   230,   233,
     234,   235,   238,   239,   242,   245,   248,   249,   252,   253,
     254,   257,   258,   260,   261,   263,   264,   266,   267,   268,
     269,   270,   272,   273,   274,   275,   276,   278,   279,   280,
     282,   283,   284,   285,   287,   288,   290,   291,   294,   295,
     296,   298,   299,   300,   301,   302,   303,   304,   305,   307,
     308,   310,   311,   312
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
#line 1741 "parser.tab.cc" // lalr1.cc:1167
#line 315 "parser.y" // lalr1.cc:1168


namespace sqf::sqc::bison
{
     inline parser::symbol_type yylex (sqf::sqc::bison::tokenizer& tokenizer)
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
