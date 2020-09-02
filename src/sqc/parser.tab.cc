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
#line 109 "parser.y" // lalr1.cc:413

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
      case 61: // filehead
      case 62: // statements
      case 63: // statement
      case 64: // assignment
      case 65: // vardecl
      case 66: // funcdecl
      case 67: // function
      case 68: // funchead
      case 69: // arglist
      case 70: // codeblock
      case 71: // if
      case 72: // for
      case 73: // while
      case 74: // trycatch
      case 75: // switch
      case 76: // caselist
      case 77: // case
      case 78: // exp01
      case 79: // exp02
      case 80: // exp03
      case 81: // exp04
      case 82: // exp05
      case 83: // exp06
      case 84: // exp07
      case 85: // exp08
      case 86: // exp09
      case 87: // expp
      case 88: // value
      case 89: // array
      case 90: // explist
        value.move< sqf::sqc::bison::astnode > (that.value);
        break;

      case 39: // "&&"
      case 40: // "/"
      case 41: // "*"
      case 42: // "%"
      case 43: // "||"
      case 44: // ":"
      case 45: // "+"
      case 46: // "-"
      case 47: // "<="
      case 48: // "<"
      case 49: // ">="
      case 50: // ">"
      case 51: // "==="
      case 52: // "=="
      case 53: // "!=="
      case 54: // "!="
      case 55: // "!"
      case 56: // NUMBER
      case 57: // IDENT
      case 58: // STRING
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
      case 61: // filehead
      case 62: // statements
      case 63: // statement
      case 64: // assignment
      case 65: // vardecl
      case 66: // funcdecl
      case 67: // function
      case 68: // funchead
      case 69: // arglist
      case 70: // codeblock
      case 71: // if
      case 72: // for
      case 73: // while
      case 74: // trycatch
      case 75: // switch
      case 76: // caselist
      case 77: // case
      case 78: // exp01
      case 79: // exp02
      case 80: // exp03
      case 81: // exp04
      case 82: // exp05
      case 83: // exp06
      case 84: // exp07
      case 85: // exp08
      case 86: // exp09
      case 87: // expp
      case 88: // value
      case 89: // array
      case 90: // explist
        value.copy< sqf::sqc::bison::astnode > (that.value);
        break;

      case 39: // "&&"
      case 40: // "/"
      case 41: // "*"
      case 42: // "%"
      case 43: // "||"
      case 44: // ":"
      case 45: // "+"
      case 46: // "-"
      case 47: // "<="
      case 48: // "<"
      case 49: // ">="
      case 50: // ">"
      case 51: // "==="
      case 52: // "=="
      case 53: // "!=="
      case 54: // "!="
      case 55: // "!"
      case 56: // NUMBER
      case 57: // IDENT
      case 58: // STRING
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
      case 61: // filehead
      case 62: // statements
      case 63: // statement
      case 64: // assignment
      case 65: // vardecl
      case 66: // funcdecl
      case 67: // function
      case 68: // funchead
      case 69: // arglist
      case 70: // codeblock
      case 71: // if
      case 72: // for
      case 73: // while
      case 74: // trycatch
      case 75: // switch
      case 76: // caselist
      case 77: // case
      case 78: // exp01
      case 79: // exp02
      case 80: // exp03
      case 81: // exp04
      case 82: // exp05
      case 83: // exp06
      case 84: // exp07
      case 85: // exp08
      case 86: // exp09
      case 87: // expp
      case 88: // value
      case 89: // array
      case 90: // explist
        yylhs.value.build< sqf::sqc::bison::astnode > ();
        break;

      case 39: // "&&"
      case 40: // "/"
      case 41: // "*"
      case 42: // "%"
      case 43: // "||"
      case 44: // ":"
      case 45: // "+"
      case 46: // "-"
      case 47: // "<="
      case 48: // "<"
      case 49: // ">="
      case 50: // ">"
      case 51: // "==="
      case 52: // "=="
      case 53: // "!=="
      case 54: // "!="
      case 55: // "!"
      case 56: // NUMBER
      case 57: // IDENT
      case 58: // STRING
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
#line 203 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); result.append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 701 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 204 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 707 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 207 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 713 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 210 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 719 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 211 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 725 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 214 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 731 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 215 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; }
#line 737 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 216 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::THROW, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 743 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 217 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 749 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 218 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 755 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 219 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 761 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 220 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 767 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 221 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 773 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 222 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 779 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 223 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 785 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 224 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 791 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 225 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 797 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 226 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 803 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 227 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 809 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 230 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 815 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 231 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 821 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 234 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 827 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 235 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 833 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 236 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 839 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 237 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 845 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 238 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 851 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 239 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 857 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 242 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 863 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 243 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FINAL_FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 869 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 246 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 875 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 249 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; }
#line 881 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 250 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 887 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 253 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 893 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 254 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 899 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 255 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 905 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 257 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 911 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 258 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; }
#line 917 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 259 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 923 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 262 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IF, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 929 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 263 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IFELSE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 935 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 266 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOR, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 941 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 267 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORSTEP, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[7].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 947 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 268 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOREACH, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 953 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 271 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::WHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 959 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 272 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DOWHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 965 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 275 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::TRYCATCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 971 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 278 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SWITCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 977 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 281 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 983 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 282 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 989 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 285 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 995 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 286 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1001 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 287 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE_DEFAULT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1007 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 290 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1013 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 291 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_TERNARY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1019 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 293 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1025 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 294 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_OR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1031 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 296 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1037 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 297 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_AND, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1043 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 299 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1049 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 300 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1055 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 301 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1061 "parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 302 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1067 "parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 303 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1073 "parser.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 305 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1079 "parser.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 306 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1085 "parser.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 307 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1091 "parser.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 308 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1097 "parser.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 309 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1103 "parser.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 311 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1109 "parser.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 312 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1115 "parser.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 313 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1121 "parser.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 315 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1127 "parser.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 316 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MULTIPLY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1133 "parser.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 317 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_DIVIDE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1139 "parser.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 318 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_REMAINDER, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1145 "parser.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 320 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1151 "parser.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 321 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ());  }
#line 1157 "parser.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 323 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1163 "parser.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 324 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_BINARY, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1169 "parser.tab.cc" // lalr1.cc:859
    break;

  case 82:
#line 325 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_GET, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1175 "parser.tab.cc" // lalr1.cc:859
    break;

  case 83:
#line 328 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 1181 "parser.tab.cc" // lalr1.cc:859
    break;

  case 84:
#line 329 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_UNARY, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1187 "parser.tab.cc" // lalr1.cc:859
    break;

  case 85:
#line 330 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }; }
#line 1193 "parser.tab.cc" // lalr1.cc:859
    break;

  case 86:
#line 331 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1199 "parser.tab.cc" // lalr1.cc:859
    break;

  case 87:
#line 333 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1205 "parser.tab.cc" // lalr1.cc:859
    break;

  case 88:
#line 334 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 1211 "parser.tab.cc" // lalr1.cc:859
    break;

  case 89:
#line 335 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1217 "parser.tab.cc" // lalr1.cc:859
    break;

  case 90:
#line 336 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 1223 "parser.tab.cc" // lalr1.cc:859
    break;

  case 91:
#line 337 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_TRUE, tokenizer.create_token() }; }
#line 1229 "parser.tab.cc" // lalr1.cc:859
    break;

  case 92:
#line 338 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_FALSE, tokenizer.create_token() }; }
#line 1235 "parser.tab.cc" // lalr1.cc:859
    break;

  case 93:
#line 339 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() }; }
#line 1241 "parser.tab.cc" // lalr1.cc:859
    break;

  case 94:
#line 341 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; }
#line 1247 "parser.tab.cc" // lalr1.cc:859
    break;

  case 95:
#line 342 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1253 "parser.tab.cc" // lalr1.cc:859
    break;

  case 96:
#line 344 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1259 "parser.tab.cc" // lalr1.cc:859
    break;

  case 97:
#line 345 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1265 "parser.tab.cc" // lalr1.cc:859
    break;

  case 98:
#line 346 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1271 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1275 "parser.tab.cc" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -79;

  const signed char parser::yytable_ninf_ = -54;

  const short int
  parser::yypact_[] =
  {
     196,   -79,    83,   528,   -42,   -21,    27,   -79,    -3,    37,
     429,   429,    62,   -79,   -79,    67,     0,    68,   528,   492,
     -79,    86,   -79,   -23,   -79,    63,   487,   -79,   255,    66,
      71,   -79,   -79,   -79,   -79,   -79,   -79,   -79,    48,    70,
      78,    72,   -30,    23,   -34,    44,    82,    85,   -79,   -79,
      67,   -79,    94,    95,    96,    11,   -26,    67,   429,    74,
      75,   119,   528,    32,   -79,   107,   117,   528,   -79,    17,
     528,   105,   -79,   102,   113,    82,   528,   528,   -79,   -79,
     -79,   -79,   -79,   528,   -79,   528,   528,   528,   528,   528,
     528,   528,   528,   528,   528,   528,   528,   528,   528,   528,
     528,   528,    90,   -79,   -79,   528,   528,   -79,   114,   120,
     429,   -79,    67,   106,   528,   122,   -79,   125,   127,   128,
     129,   528,   528,   130,   -79,   528,   -79,   131,   -79,   126,
     124,   -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   133,   134,   -79,   -79,
     112,   -79,   -79,   429,   528,   148,   429,   -79,   528,   115,
     142,   -79,   -79,   429,   -79,   -79,   137,   528,   -79,   528,
     -79,   -79,   149,   528,   166,   150,   151,     4,   -79,   528,
     -79,   152,   429,   371,   429,   -79,   429,   528,   140,   157,
       4,   -79,   -79,   -79,   528,   -79,   -79,   -79,   143,   429,
     -79,   -79,   429,   313,   -79,   -79,   -79
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,    21,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,    93,    91,     0,     0,     0,     0,     0,
      20,     0,    90,    85,    88,     0,     0,     4,     0,     0,
       0,    12,    87,    13,    14,    15,    16,    17,     0,    55,
      57,    59,    61,    66,    71,    74,    78,    80,    86,    89,
       0,     9,    85,     0,     0,    26,     0,     0,     0,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     5,    29,
       0,     0,    94,    96,     0,    79,     0,     0,     1,     3,
       7,    18,    11,     0,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     8,    10,     0,     0,    33,    35,     0,
       0,    32,     0,     0,     0,     0,    39,     0,     0,     0,
       0,     0,     0,     0,    83,    97,    95,     0,    22,     0,
       0,    58,    60,    62,    64,    63,    65,    68,    67,    70,
      69,    72,    73,    76,    75,    77,     0,     0,    25,    24,
      36,    34,    30,     0,     0,     0,     0,    40,     0,     0,
       0,    28,    27,     0,    98,    84,     0,     0,    82,     0,
      37,    31,     0,     0,    41,     0,     0,     0,    46,     0,
      56,     0,     0,     0,     0,    47,     0,     0,     0,     0,
      50,    23,    81,    45,     0,    43,    42,    48,     0,     0,
      49,    51,     0,     0,    54,    44,    52
  };

  const short int
  parser::yypgoto_[] =
  {
     -79,   -79,   -79,   -18,     6,   -79,   -79,   -79,   -79,   -11,
      38,    -8,   -79,   -79,   -79,   -79,   -79,    -1,   -79,    -2,
     -79,   104,   111,   -12,    33,   -78,   -70,   172,   -79,   -79,
     -79,   -74
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    25,    26,    27,    64,    29,    30,    31,    32,    58,
     109,    65,    33,    34,    35,    36,    37,   189,   190,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    74
  };

  const short int
  parser::yytable_[] =
  {
      53,    54,   127,    66,    68,   107,    28,    76,    79,    56,
      80,    96,    97,    77,   105,    55,    71,    73,   141,   142,
     121,    88,    89,    90,    91,   187,   188,    60,   143,   144,
     145,   108,    28,     1,    28,    59,    57,     2,     3,     4,
       5,     6,     7,     8,     9,   117,   110,   106,    10,    11,
     111,   164,    12,   122,    61,    13,    14,    69,    16,    17,
     115,   116,    18,    78,    19,   120,    20,    62,   123,    28,
      92,    93,    94,    95,    73,   128,   133,   134,   135,   136,
      83,   129,    84,   130,    98,    99,   100,    21,    22,    23,
      24,    50,    67,     7,    50,   181,     7,    56,    70,   146,
      81,   153,   152,   148,   149,    82,    13,    14,    85,    13,
      14,    87,   155,    18,   101,    19,    18,    51,    19,   161,
     162,    86,   102,    73,    76,   137,   138,   139,   140,   103,
     104,   112,   113,   114,   118,   119,   124,   125,    21,    22,
      52,    24,    22,    52,    24,   171,   126,   147,   174,   150,
     154,   151,   172,   156,   157,   178,   175,   158,   159,   166,
     160,   163,   165,   173,   169,   180,   168,    73,   167,   108,
     177,   183,   176,   179,   193,   195,   196,   191,   197,   184,
     182,   185,   186,   192,   199,   198,   200,   203,   170,   201,
     131,   204,   202,    75,   205,   206,    -2,     1,   132,     0,
       0,     2,     3,     4,     5,     6,     7,     8,     9,     0,
       0,     0,    10,    11,     0,     0,    12,     0,     0,    13,
      14,    15,    16,    17,     0,     0,    18,     0,    19,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,    22,    23,    24,    -6,     1,     0,     0,     0,
       2,     3,     4,     5,     6,     7,     8,     9,     0,     0,
       0,    10,    11,     0,     0,    12,     0,     0,    13,    14,
       0,    16,    17,     0,    -6,    18,     0,    19,     0,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,    22,    23,    24,     1,     0,     0,     0,     2,     3,
       4,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,     0,     0,    12,   -53,   -53,    13,    14,     0,    16,
      17,    63,   -53,    18,     0,    19,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,    22,
      23,    24,     1,     0,     0,     0,     2,     3,     4,     5,
       6,     7,     8,     9,     0,     0,     0,    10,    11,     0,
     194,    12,     0,     0,    13,    14,     0,    16,    17,    63,
       0,    18,     0,    19,     0,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,    22,    23,    24,
       1,     0,     0,     0,     2,     3,     4,     5,     6,     7,
       8,     9,     0,     0,     0,    10,    11,     0,     0,    12,
       0,     0,    13,    14,     0,    16,    17,    63,     0,    18,
       0,    19,     0,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,    22,    23,    24,     1,     0,
       0,     0,     2,     3,     4,     5,     6,     7,     8,     9,
      50,     0,     7,    10,    11,     0,     0,    12,     0,     0,
      13,    14,     0,    16,    17,    13,    14,    18,     0,    19,
       0,    20,    18,     0,    19,    72,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     7,     0,
       0,     0,    21,    22,    23,    24,     0,    21,    22,    52,
      24,    13,    14,     0,     0,     0,     0,     0,    18,     0,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,    22,    52,    24
  };

  const short int
  parser::yycheck_[] =
  {
       2,     3,    76,    11,    15,    31,     0,    30,    26,    30,
      28,    45,    46,    36,     3,    57,    18,    19,    96,    97,
       3,    51,    52,    53,    54,    21,    22,    30,    98,    99,
     100,    57,    26,     1,    28,     8,    57,     5,     6,     7,
       8,     9,    10,    11,    12,    63,    57,    36,    16,    17,
      58,   125,    20,    36,    57,    23,    24,    57,    26,    27,
      62,    29,    30,     0,    32,    67,    34,    30,    70,    63,
      47,    48,    49,    50,    76,    77,    88,    89,    90,    91,
      32,    83,    34,    85,    40,    41,    42,    55,    56,    57,
      58,     8,    30,    10,     8,   169,    10,    30,    30,   101,
      34,   112,   110,   105,   106,    34,    23,    24,    38,    23,
      24,    39,   114,    30,    32,    32,    30,    34,    32,   121,
     122,    43,    37,   125,    30,    92,    93,    94,    95,    34,
      34,    57,    57,    14,    27,    18,    31,    35,    55,    56,
      57,    58,    56,    57,    58,   153,    33,    57,   156,    35,
      44,    31,   154,    31,    29,   163,   158,    30,    30,    33,
      31,    31,    31,    15,    30,   167,    33,   169,    44,    57,
      28,   173,    57,    36,   182,   183,   184,   179,   186,    13,
      31,    31,    31,    31,    44,   187,    29,    44,   150,   190,
      86,   199,   194,    21,   202,   203,     0,     1,    87,    -1,
      -1,     5,     6,     7,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    16,    17,    -1,    -1,    20,    -1,    -1,    23,
      24,    25,    26,    27,    -1,    -1,    30,    -1,    32,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    58,     0,     1,    -1,    -1,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    16,    17,    -1,    -1,    20,    -1,    -1,    23,    24,
      -1,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    58,     1,    -1,    -1,    -1,     5,     6,
       7,     8,     9,    10,    11,    12,    -1,    -1,    -1,    16,
      17,    -1,    -1,    20,    21,    22,    23,    24,    -1,    26,
      27,    28,    29,    30,    -1,    32,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    58,     1,    -1,    -1,    -1,     5,     6,     7,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    16,    17,    -1,
      19,    20,    -1,    -1,    23,    24,    -1,    26,    27,    28,
      -1,    30,    -1,    32,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
       1,    -1,    -1,    -1,     5,     6,     7,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    16,    17,    -1,    -1,    20,
      -1,    -1,    23,    24,    -1,    26,    27,    28,    -1,    30,
      -1,    32,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,     1,    -1,
      -1,    -1,     5,     6,     7,     8,     9,    10,    11,    12,
       8,    -1,    10,    16,    17,    -1,    -1,    20,    -1,    -1,
      23,    24,    -1,    26,    27,    23,    24,    30,    -1,    32,
      -1,    34,    30,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,    55,    56,    57,    58,    -1,    55,    56,    57,
      58,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     1,     5,     6,     7,     8,     9,    10,    11,    12,
      16,    17,    20,    23,    24,    25,    26,    27,    30,    32,
      34,    55,    56,    57,    58,    60,    61,    62,    63,    64,
      65,    66,    67,    71,    72,    73,    74,    75,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
       8,    34,    57,    78,    78,    57,    30,    57,    68,     8,
      30,    57,    30,    28,    63,    70,    70,    30,    68,    57,
      30,    78,    33,    78,    90,    86,    30,    36,     0,    62,
      62,    34,    34,    32,    34,    38,    43,    39,    51,    52,
      53,    54,    47,    48,    49,    50,    45,    46,    40,    41,
      42,    32,    37,    34,    34,     3,    36,    31,    57,    69,
      68,    70,    57,    57,    14,    78,    29,    62,    27,    18,
      78,     3,    36,    78,    31,    35,    33,    90,    78,    78,
      78,    80,    81,    82,    82,    82,    82,    83,    83,    83,
      83,    84,    84,    85,    85,    85,    78,    57,    78,    78,
      35,    31,    70,    68,    44,    78,    31,    29,    30,    30,
      31,    78,    78,    31,    90,    31,    33,    44,    33,    30,
      69,    70,    78,    15,    70,    78,    57,    28,    70,    36,
      78,    90,    31,    78,    13,    31,    31,    21,    22,    76,
      77,    78,    31,    70,    19,    70,    70,    70,    78,    44,
      29,    76,    78,    44,    70,    70,    70
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    59,    60,    60,    60,    61,    62,    62,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    64,    64,    65,    65,    65,    65,    65,    65,
      66,    66,    67,    68,    68,    69,    69,    69,    70,    70,
      70,    71,    71,    72,    72,    72,    73,    73,    74,    75,
      76,    76,    77,    77,    77,    78,    78,    79,    79,    80,
      80,    81,    81,    81,    81,    81,    82,    82,    82,    82,
      82,    83,    83,    83,    84,    84,    84,    84,    85,    85,
      86,    86,    86,    87,    87,    87,    87,    88,    88,    88,
      88,    88,    88,    88,    89,    89,    90,    90,    90
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     2,     1,     2,     1,     2,     3,     2,
       3,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     3,     6,     4,     4,     2,     4,     4,     2,
       4,     5,     3,     2,     3,     1,     2,     3,     1,     2,
       3,     5,     7,     7,     9,     7,     5,     6,     7,     7,
       1,     2,     4,     3,     3,     1,     5,     1,     3,     1,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     1,     2,
       1,     6,     4,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     3,     1,     2,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "NA", "error", "$undefined", "\"be\"", "\"break\"", "\"return\"",
  "\"throw\"", "\"let\"", "\"function\"", "\"final\"", "\"false\"",
  "\"for\"", "\"if\"", "\"else\"", "\"from\"", "\"to\"", "\"do\"",
  "\"try\"", "\"catch\"", "\"step\"", "\"switch\"", "\"case\"",
  "\"default\"", "\"nil\"", "\"true\"", "\"params\"", "\"private\"",
  "\"while\"", "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"[\"", "\"]\"",
  "\";\"", "\",\"", "\"=\"", "\".\"", "\"?\"", "\"&&\"", "\"/\"", "\"*\"",
  "\"%\"", "\"||\"", "\":\"", "\"+\"", "\"-\"", "\"<=\"", "\"<\"",
  "\">=\"", "\">\"", "\"===\"", "\"==\"", "\"!==\"", "\"!=\"", "\"!\"",
  "NUMBER", "IDENT", "STRING", "$accept", "start", "filehead",
  "statements", "statement", "assignment", "vardecl", "funcdecl",
  "function", "funchead", "arglist", "codeblock", "if", "for", "while",
  "trycatch", "switch", "caselist", "case", "exp01", "exp02", "exp03",
  "exp04", "exp05", "exp06", "exp07", "exp08", "exp09", "expp", "value",
  "array", "explist", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   202,   202,   203,   204,   207,   210,   211,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   230,   231,   234,   235,   236,   237,   238,   239,
     242,   243,   246,   249,   250,   253,   254,   255,   257,   258,
     259,   262,   263,   266,   267,   268,   271,   272,   275,   278,
     281,   282,   285,   286,   287,   290,   291,   293,   294,   296,
     297,   299,   300,   301,   302,   303,   305,   306,   307,   308,
     309,   311,   312,   313,   315,   316,   317,   318,   320,   321,
     323,   324,   325,   328,   329,   330,   331,   333,   334,   335,
     336,   337,   338,   339,   341,   342,   344,   345,   346
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
#line 1860 "parser.tab.cc" // lalr1.cc:1167
#line 349 "parser.y" // lalr1.cc:1168


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
         case tokenizer::etoken::invalid: return parser::make_NA(loc);
         case tokenizer::etoken::m_line: return yylex(tokenizer);
         case tokenizer::etoken::i_comment_line: return yylex(tokenizer);
         case tokenizer::etoken::i_comment_block: return yylex(tokenizer);
         case tokenizer::etoken::i_whitespace: return yylex(tokenizer);

         case tokenizer::etoken::t_return: return parser::make_RETURN(loc);
         case tokenizer::etoken::t_throw: return parser::make_THROW(loc);
         case tokenizer::etoken::t_let: return parser::make_LET(loc);
         case tokenizer::etoken::t_be: return parser::make_BE(loc);
         case tokenizer::etoken::t_break: return parser::make_BREAK(loc);
         case tokenizer::etoken::t_function: return parser::make_FUNCTION(loc);
         case tokenizer::etoken::t_final: return parser::make_FINAL(loc);
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
         case tokenizer::etoken::s_equalequalequal: return parser::make_EQUALEQUALEQUAL(token, loc);
         case tokenizer::etoken::s_equalequal: return parser::make_EQUALEQUAL(token, loc);
         case tokenizer::etoken::s_equal: return parser::make_EQUAL(loc);
         case tokenizer::etoken::s_greaterthenequal: return parser::make_GTEQUAL(token, loc);
         case tokenizer::etoken::s_greaterthen: return parser::make_GT(token, loc);
         case tokenizer::etoken::s_lessthenequal: return parser::make_LTEQUAL(token, loc);
         case tokenizer::etoken::s_lessthen: return parser::make_LT(token, loc);
         case tokenizer::etoken::s_plus: return parser::make_PLUS(token, loc);
         case tokenizer::etoken::s_minus: return parser::make_MINUS(token, loc);
         case tokenizer::etoken::s_notequalequal: return parser::make_EXCLAMATIONMARKEQUALEQUAL(token, loc);
         case tokenizer::etoken::s_notequal: return parser::make_EXCLAMATIONMARKEQUAL(token, loc);
         case tokenizer::etoken::s_exclamationmark: return parser::make_EXCLAMATIONMARK(token, loc);
         case tokenizer::etoken::s_percent: return parser::make_PERCENT(token, loc);
         case tokenizer::etoken::s_star: return parser::make_STAR(token, loc);
         case tokenizer::etoken::s_slash: return parser::make_SLASH(token, loc);
         case tokenizer::etoken::s_andand: return parser::make_ANDAND(token, loc);
         case tokenizer::etoken::s_oror: return parser::make_VLINEVLINE(token, loc);
         case tokenizer::etoken::s_questionmark: return parser::make_QUESTIONMARK(loc);
         case tokenizer::etoken::s_colon: return parser::make_COLON(token, loc);
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
