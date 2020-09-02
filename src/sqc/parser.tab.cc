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
#line 108 "parser.y" // lalr1.cc:413

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

      case 36: // "&&"
      case 37: // "/"
      case 38: // "*"
      case 39: // "%"
      case 40: // "||"
      case 41: // ":"
      case 42: // "+"
      case 43: // "-"
      case 44: // "<="
      case 45: // "<"
      case 46: // ">="
      case 47: // ">"
      case 48: // "==="
      case 49: // "=="
      case 50: // "!=="
      case 51: // "!="
      case 52: // "!"
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

      case 36: // "&&"
      case 37: // "/"
      case 38: // "*"
      case 39: // "%"
      case 40: // "||"
      case 41: // ":"
      case 42: // "+"
      case 43: // "-"
      case 44: // "<="
      case 45: // "<"
      case 46: // ">="
      case 47: // ">"
      case 48: // "==="
      case 49: // "=="
      case 50: // "!=="
      case 51: // "!="
      case 52: // "!"
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

      case 36: // "&&"
      case 37: // "/"
      case 38: // "*"
      case 39: // "%"
      case 40: // "||"
      case 41: // ":"
      case 42: // "+"
      case 43: // "-"
      case 44: // "<="
      case 45: // "<"
      case 46: // ">="
      case 47: // ">"
      case 48: // "==="
      case 49: // "=="
      case 50: // "!=="
      case 51: // "!="
      case 52: // "!"
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
#line 199 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 698 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 202 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 704 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 203 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 710 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 206 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 716 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 207 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; }
#line 722 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 208 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::THROW, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 728 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 209 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 734 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 210 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 740 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 211 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 746 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 212 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 752 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 213 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 758 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 214 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 764 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 215 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 770 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 216 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 776 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 217 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 782 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 218 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 788 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 219 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 794 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 222 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 800 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 223 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 806 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 226 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 812 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 227 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 818 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 228 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 824 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 229 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 830 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 230 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 836 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 231 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 842 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 234 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 848 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 237 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 854 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 240 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; }
#line 860 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 241 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 866 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 244 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 872 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 245 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 878 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 246 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 884 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 248 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 890 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 249 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; }
#line 896 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 250 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 902 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 253 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IF, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 908 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 254 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IFELSE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 914 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 257 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOR, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 920 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 258 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORSTEP, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[7].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 926 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 259 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOREACH, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 932 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 262 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::WHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 938 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 263 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DOWHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 944 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 266 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::TRYCATCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 950 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 269 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SWITCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 956 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 272 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 962 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 273 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 968 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 276 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 974 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 277 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 980 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 278 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE_DEFAULT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 986 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 281 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 992 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 282 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_TERNARY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 998 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 284 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1004 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 285 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_OR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1010 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 287 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1016 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 288 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_AND, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1022 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 290 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1028 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 291 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1034 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 292 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1040 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 293 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1046 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 294 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1052 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 296 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1058 "parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 297 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1064 "parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 298 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1070 "parser.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 299 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1076 "parser.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 300 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1082 "parser.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 302 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1088 "parser.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 303 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1094 "parser.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 304 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1100 "parser.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 306 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1106 "parser.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 307 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MULTIPLY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1112 "parser.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 308 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_DIVIDE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1118 "parser.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 309 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_REMAINDER, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1124 "parser.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 311 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1130 "parser.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 312 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ());  }
#line 1136 "parser.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 314 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1142 "parser.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 315 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_BINARY, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1148 "parser.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 316 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_GET, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1154 "parser.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 319 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 1160 "parser.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 320 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_UNARY, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1166 "parser.tab.cc" // lalr1.cc:859
    break;

  case 82:
#line 321 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }; }
#line 1172 "parser.tab.cc" // lalr1.cc:859
    break;

  case 83:
#line 322 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1178 "parser.tab.cc" // lalr1.cc:859
    break;

  case 84:
#line 324 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1184 "parser.tab.cc" // lalr1.cc:859
    break;

  case 85:
#line 325 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 1190 "parser.tab.cc" // lalr1.cc:859
    break;

  case 86:
#line 326 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1196 "parser.tab.cc" // lalr1.cc:859
    break;

  case 87:
#line 327 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 1202 "parser.tab.cc" // lalr1.cc:859
    break;

  case 88:
#line 328 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_TRUE, tokenizer.create_token() }; }
#line 1208 "parser.tab.cc" // lalr1.cc:859
    break;

  case 89:
#line 329 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_FALSE, tokenizer.create_token() }; }
#line 1214 "parser.tab.cc" // lalr1.cc:859
    break;

  case 90:
#line 330 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() }; }
#line 1220 "parser.tab.cc" // lalr1.cc:859
    break;

  case 91:
#line 332 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; }
#line 1226 "parser.tab.cc" // lalr1.cc:859
    break;

  case 92:
#line 333 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1232 "parser.tab.cc" // lalr1.cc:859
    break;

  case 93:
#line 335 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1238 "parser.tab.cc" // lalr1.cc:859
    break;

  case 94:
#line 336 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1244 "parser.tab.cc" // lalr1.cc:859
    break;

  case 95:
#line 337 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1250 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1254 "parser.tab.cc" // lalr1.cc:859
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

  const signed char parser::yytable_ninf_ = -51;

  const short int
  parser::yypact_[] =
  {
     193,   -70,    99,    82,   -42,   -19,    -6,    24,   334,   334,
      26,   -70,   -70,   -70,   -16,   -29,    82,   111,   -70,   389,
     -70,   -20,   -70,    30,   -70,    19,    16,    45,   -70,   -70,
     -70,   -70,   -70,   -70,   -70,   -13,    53,    50,    55,    14,
      34,   -33,    20,    67,    65,   -70,   -70,    73,   -70,    78,
      76,    77,    11,   -23,    73,   334,    82,    82,   377,   -70,
      97,   101,    82,    61,   117,    23,   114,   -70,   107,   115,
      67,    82,    82,   -70,   -70,   -70,   -70,    82,   -70,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    75,   -70,   -70,    82,
      82,   -70,   116,   118,   334,   -70,   121,   127,   -70,   130,
     131,   133,   134,   120,    82,    82,    82,   -70,    82,   -70,
     141,   -70,   143,   135,   -70,   -70,   -70,   -70,   -70,   -70,
     -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   145,
     144,   -70,   -70,   125,   -70,   -70,   334,   334,   -70,    82,
     126,   156,    82,   171,   -70,   -70,   -70,   -70,   150,    82,
     -70,    82,   -70,   176,   -70,   158,   159,    68,   160,    82,
      82,   -70,   163,   334,   -70,   334,    82,   154,   173,    68,
     334,   291,   -70,   -70,   -70,   -70,   161,   334,   -70,   -70,
     -70,    82,   -70,   236,   -70,   334,   -70,   -70
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    88,    89,     0,     0,     0,     0,    18,     0,
      87,    82,    85,     0,     3,     0,     0,     0,    10,    84,
      11,    12,    13,    14,    15,     0,    52,    54,    56,    58,
      63,    68,    71,    75,    77,    83,    86,     0,     7,    82,
       0,     0,    24,     0,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,     0,    27,     0,    91,    93,     0,
      76,     0,     0,     1,     5,    16,     9,     0,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     6,     8,     0,
       0,    30,    32,     0,     0,    29,     0,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,    80,    94,    92,
       0,    20,     0,     0,    55,    57,    59,    61,    60,    62,
      65,    64,    67,    66,    69,    70,    73,    72,    74,     0,
       0,    23,    22,    33,    31,    28,     0,     0,    37,     0,
       0,     0,     0,     0,    26,    25,    95,    81,     0,     0,
      79,     0,    34,    38,    43,     0,     0,     0,     0,     0,
       0,    53,     0,     0,    44,     0,     0,     0,     0,    47,
       0,     0,    21,    78,    39,    45,     0,     0,    46,    48,
      42,     0,    40,     0,    51,     0,    49,    41
  };

  const short int
  parser::yypgoto_[] =
  {
     -70,   -70,   -21,     3,   -70,   -70,   -70,   -70,   149,    62,
      -3,   -70,   -70,   -70,   -70,   -70,    25,   -70,    -2,   -70,
     129,   137,     0,    36,     4,   -26,   192,   -70,   -70,   -70,
     -69
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    23,    24,    59,    26,    27,    28,    29,    55,   103,
      60,    30,    31,    32,    33,    34,   178,   179,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      69
  };

  const short int
  parser::yytable_[] =
  {
      50,    51,   120,    25,    74,   101,    61,    71,    53,    90,
      91,    63,    52,    72,    66,    68,    77,    99,    78,    -4,
       1,    56,     2,     3,     4,    65,     5,     6,    25,   115,
      73,   102,     7,     8,     9,    54,    10,   109,    64,    11,
      12,    13,    14,    15,   100,    -4,    16,    75,    17,   156,
      18,    57,   105,    62,   106,   107,   116,    92,    93,    94,
     112,    25,    82,    83,    84,    85,   136,   137,   138,    68,
     121,    19,    20,    21,    22,   122,    76,   123,    86,    87,
      88,    89,   126,   127,   128,   129,   176,   177,    79,    47,
      80,    81,   172,   139,   134,   135,    95,   141,   142,    96,
      53,   145,    11,    12,    13,    71,    47,    97,    98,    16,
     110,    17,   153,   154,   155,   113,    68,   111,    47,    11,
      12,    13,   130,   131,   132,   133,    16,   114,    17,   140,
      48,    11,    12,    13,    19,    20,    49,    22,    16,   118,
      17,    67,   117,   163,   164,   119,   144,   165,   143,   146,
     168,    19,    20,    49,    22,   147,   148,   171,   149,    68,
     150,   152,   151,    19,    20,    49,    22,   181,   182,   157,
     184,   161,   185,   158,   186,   160,   159,   190,   192,   102,
     166,   167,   169,   170,   194,   173,   174,   175,   180,   195,
     196,   183,   197,    -2,     1,   187,     2,     3,     4,   188,
       5,     6,   193,   104,   189,   162,     7,     8,     9,   124,
      10,    70,     0,    11,    12,    13,    14,    15,   125,     0,
      16,     0,    17,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     0,     2,
       3,     4,     0,     5,     6,    19,    20,    21,    22,     7,
       8,     9,     0,    10,   -50,   -50,    11,    12,    13,    14,
      15,    58,   -50,    16,     0,    17,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    19,    20,
      21,    22,     1,     0,     2,     3,     4,     0,     5,     6,
       0,     0,     0,   191,     7,     8,     9,     0,    10,     0,
       0,    11,    12,    13,    14,    15,    58,     0,    16,     0,
      17,     0,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     1,     0,     2,     3,     4,
       0,     5,     6,    19,    20,    21,    22,     7,     8,     9,
       0,    10,     0,     0,    11,    12,    13,    14,    15,    58,
       0,    16,     0,    17,     0,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     0,
       2,     3,     4,     0,     5,     6,    19,    20,    21,    22,
       7,     8,     9,     0,    10,     0,    47,    11,    12,    13,
      14,    15,     0,   108,    16,     0,    17,     0,    18,    11,
      12,    13,     0,     0,     0,     0,    16,     0,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
      20,    21,    22,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,    49,    22
  };

  const short int
  parser::yycheck_[] =
  {
       2,     3,    71,     0,    25,    28,     9,    27,    27,    42,
      43,    27,    54,    33,    16,    17,    29,     6,    31,     0,
       1,    27,     3,     4,     5,    54,     7,     8,    25,     6,
       0,    54,    13,    14,    15,    54,    17,    58,    54,    20,
      21,    22,    23,    24,    33,    26,    27,    31,    29,   118,
      31,    27,    55,    27,    56,    57,    33,    37,    38,    39,
      62,    58,    48,    49,    50,    51,    92,    93,    94,    71,
      72,    52,    53,    54,    55,    77,    31,    79,    44,    45,
      46,    47,    82,    83,    84,    85,    18,    19,    35,     7,
      40,    36,   161,    95,    90,    91,    29,    99,   100,    34,
      27,   104,    20,    21,    22,    27,     7,    31,    31,    27,
      13,    29,   114,   115,   116,    54,   118,    16,     7,    20,
      21,    22,    86,    87,    88,    89,    27,    10,    29,    54,
      31,    20,    21,    22,    52,    53,    54,    55,    27,    32,
      29,    30,    28,   146,   147,    30,    28,   149,    32,    28,
     152,    52,    53,    54,    55,    28,    26,   159,    27,   161,
      27,    41,    28,    52,    53,    54,    55,   169,   170,    28,
     173,    27,   175,    30,   176,    30,    41,   180,   181,    54,
      54,    25,    11,    33,   187,     9,    28,    28,    28,   191,
     193,    28,   195,     0,     1,    41,     3,     4,     5,    26,
       7,     8,    41,    54,   179,   143,    13,    14,    15,    80,
      17,    19,    -1,    20,    21,    22,    23,    24,    81,    -1,
      27,    -1,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
       4,     5,    -1,     7,     8,    52,    53,    54,    55,    13,
      14,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    53,
      54,    55,     1,    -1,     3,     4,     5,    -1,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    -1,    17,    -1,
      -1,    20,    21,    22,    23,    24,    25,    -1,    27,    -1,
      29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,
      -1,     7,     8,    52,    53,    54,    55,    13,    14,    15,
      -1,    17,    -1,    -1,    20,    21,    22,    23,    24,    25,
      -1,    27,    -1,    29,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,    -1,     7,     8,    52,    53,    54,    55,
      13,    14,    15,    -1,    17,    -1,     7,    20,    21,    22,
      23,    24,    -1,    26,    27,    -1,    29,    -1,    31,    20,
      21,    22,    -1,    -1,    -1,    -1,    27,    -1,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      53,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    54,    55
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     1,     3,     4,     5,     7,     8,    13,    14,    15,
      17,    20,    21,    22,    23,    24,    27,    29,    31,    52,
      53,    54,    55,    57,    58,    59,    60,    61,    62,    63,
      67,    68,    69,    70,    71,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,     7,    31,    54,
      74,    74,    54,    27,    54,    64,    27,    27,    25,    59,
      66,    66,    27,    27,    54,    54,    74,    30,    74,    86,
      82,    27,    33,     0,    58,    31,    31,    29,    31,    35,
      40,    36,    48,    49,    50,    51,    44,    45,    46,    47,
      42,    43,    37,    38,    39,    29,    34,    31,    31,     6,
      33,    28,    54,    65,    64,    66,    74,    74,    26,    58,
      13,    16,    74,    54,    10,     6,    33,    28,    32,    30,
      86,    74,    74,    74,    76,    77,    78,    78,    78,    78,
      79,    79,    79,    79,    80,    80,    81,    81,    81,    74,
      54,    74,    74,    32,    28,    66,    28,    28,    26,    27,
      27,    28,    41,    74,    74,    74,    86,    28,    30,    41,
      30,    27,    65,    66,    66,    74,    54,    25,    74,    11,
      33,    74,    86,     9,    28,    28,    18,    19,    72,    73,
      28,    74,    74,    28,    66,    66,    74,    41,    26,    72,
      66,    12,    66,    41,    66,    74,    66,    66
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    56,    57,    57,    58,    58,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      60,    60,    61,    61,    61,    61,    61,    61,    62,    63,
      64,    64,    65,    65,    65,    66,    66,    66,    67,    67,
      68,    68,    68,    69,    69,    70,    71,    72,    72,    73,
      73,    73,    74,    74,    75,    75,    76,    76,    77,    77,
      77,    77,    77,    78,    78,    78,    78,    78,    79,    79,
      79,    80,    80,    80,    80,    81,    81,    82,    82,    82,
      83,    83,    83,    83,    84,    84,    84,    84,    84,    84,
      84,    85,    85,    86,    86,    86
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     1,     2,     3,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       3,     6,     4,     4,     2,     4,     4,     2,     4,     3,
       2,     3,     1,     2,     3,     1,     2,     3,     5,     7,
       7,     9,     7,     5,     6,     7,     7,     1,     2,     4,
       3,     3,     1,     5,     1,     3,     1,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     2,     1,     6,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     1,     2,     3
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
  "\"private\"", "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"[\"", "\"]\"",
  "\";\"", "\",\"", "\"=\"", "\".\"", "\"?\"", "\"&&\"", "\"/\"", "\"*\"",
  "\"%\"", "\"||\"", "\":\"", "\"+\"", "\"-\"", "\"<=\"", "\"<\"",
  "\">=\"", "\">\"", "\"===\"", "\"==\"", "\"!==\"", "\"!=\"", "\"!\"",
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
       0,   198,   198,   199,   202,   203,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     222,   223,   226,   227,   228,   229,   230,   231,   234,   237,
     240,   241,   244,   245,   246,   248,   249,   250,   253,   254,
     257,   258,   259,   262,   263,   266,   269,   272,   273,   276,
     277,   278,   281,   282,   284,   285,   287,   288,   290,   291,
     292,   293,   294,   296,   297,   298,   299,   300,   302,   303,
     304,   306,   307,   308,   309,   311,   312,   314,   315,   316,
     319,   320,   321,   322,   324,   325,   326,   327,   328,   329,
     330,   332,   333,   335,   336,   337
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
#line 1806 "parser.tab.cc" // lalr1.cc:1167
#line 340 "parser.y" // lalr1.cc:1168


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
