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
#line 154 "parser.y" // lalr1.cc:413

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
      case 74: // filehead
      case 75: // statements
      case 76: // statement
      case 77: // assignment
      case 78: // vardecl
      case 79: // funcdecl
      case 80: // function
      case 81: // funchead
      case 82: // arglist
      case 83: // argitem
      case 84: // if
      case 85: // for
      case 86: // while
      case 87: // trycatch
      case 88: // switch
      case 89: // caselist
      case 90: // case
      case 91: // exp01
      case 92: // exp02
      case 93: // exp03
      case 94: // exp04
      case 95: // exp05
      case 96: // exp06
      case 97: // exp07
      case 98: // exp08
      case 99: // exp09
      case 100: // arrget
      case 101: // call
      case 102: // objget
      case 103: // expp
      case 104: // obj
      case 105: // obj_item
      case 106: // obj_items
      case 107: // codeblock
      case 108: // value
      case 109: // array
      case 110: // explist
      case 111: // format_string
      case 112: // format_string_match
        value.move< sqf::sqc::bison::astnode > (that.value);
        break;

      case 40: // "be"
      case 41: // "="
      case 42: // "&&"
      case 43: // "/"
      case 44: // "/="
      case 45: // "*"
      case 46: // "*="
      case 47: // "%"
      case 48: // "||"
      case 49: // ":"
      case 50: // "+"
      case 51: // "++"
      case 52: // "+="
      case 53: // "-"
      case 54: // "--"
      case 55: // "-="
      case 56: // "<="
      case 57: // "<"
      case 58: // ">="
      case 59: // ">>"
      case 60: // ">"
      case 61: // "==="
      case 62: // "=="
      case 63: // "!=="
      case 64: // "!="
      case 65: // "!"
      case 66: // NUMBER
      case 67: // IDENT
      case 68: // STRING
      case 69: // FORMAT_STRING_START
      case 70: // FORMAT_STRING_CONTINUE
      case 71: // FORMAT_STRING_FINAL
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
      case 74: // filehead
      case 75: // statements
      case 76: // statement
      case 77: // assignment
      case 78: // vardecl
      case 79: // funcdecl
      case 80: // function
      case 81: // funchead
      case 82: // arglist
      case 83: // argitem
      case 84: // if
      case 85: // for
      case 86: // while
      case 87: // trycatch
      case 88: // switch
      case 89: // caselist
      case 90: // case
      case 91: // exp01
      case 92: // exp02
      case 93: // exp03
      case 94: // exp04
      case 95: // exp05
      case 96: // exp06
      case 97: // exp07
      case 98: // exp08
      case 99: // exp09
      case 100: // arrget
      case 101: // call
      case 102: // objget
      case 103: // expp
      case 104: // obj
      case 105: // obj_item
      case 106: // obj_items
      case 107: // codeblock
      case 108: // value
      case 109: // array
      case 110: // explist
      case 111: // format_string
      case 112: // format_string_match
        value.copy< sqf::sqc::bison::astnode > (that.value);
        break;

      case 40: // "be"
      case 41: // "="
      case 42: // "&&"
      case 43: // "/"
      case 44: // "/="
      case 45: // "*"
      case 46: // "*="
      case 47: // "%"
      case 48: // "||"
      case 49: // ":"
      case 50: // "+"
      case 51: // "++"
      case 52: // "+="
      case 53: // "-"
      case 54: // "--"
      case 55: // "-="
      case 56: // "<="
      case 57: // "<"
      case 58: // ">="
      case 59: // ">>"
      case 60: // ">"
      case 61: // "==="
      case 62: // "=="
      case 63: // "!=="
      case 64: // "!="
      case 65: // "!"
      case 66: // NUMBER
      case 67: // IDENT
      case 68: // STRING
      case 69: // FORMAT_STRING_START
      case 70: // FORMAT_STRING_CONTINUE
      case 71: // FORMAT_STRING_FINAL
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
      case 74: // filehead
      case 75: // statements
      case 76: // statement
      case 77: // assignment
      case 78: // vardecl
      case 79: // funcdecl
      case 80: // function
      case 81: // funchead
      case 82: // arglist
      case 83: // argitem
      case 84: // if
      case 85: // for
      case 86: // while
      case 87: // trycatch
      case 88: // switch
      case 89: // caselist
      case 90: // case
      case 91: // exp01
      case 92: // exp02
      case 93: // exp03
      case 94: // exp04
      case 95: // exp05
      case 96: // exp06
      case 97: // exp07
      case 98: // exp08
      case 99: // exp09
      case 100: // arrget
      case 101: // call
      case 102: // objget
      case 103: // expp
      case 104: // obj
      case 105: // obj_item
      case 106: // obj_items
      case 107: // codeblock
      case 108: // value
      case 109: // array
      case 110: // explist
      case 111: // format_string
      case 112: // format_string_match
        yylhs.value.build< sqf::sqc::bison::astnode > ();
        break;

      case 40: // "be"
      case 41: // "="
      case 42: // "&&"
      case 43: // "/"
      case 44: // "/="
      case 45: // "*"
      case 46: // "*="
      case 47: // "%"
      case 48: // "||"
      case 49: // ":"
      case 50: // "+"
      case 51: // "++"
      case 52: // "+="
      case 53: // "-"
      case 54: // "--"
      case 55: // "-="
      case 56: // "<="
      case 57: // "<"
      case 58: // ">="
      case 59: // ">>"
      case 60: // ">"
      case 61: // "==="
      case 62: // "=="
      case 63: // "!=="
      case 64: // "!="
      case 65: // "!"
      case 66: // NUMBER
      case 67: // IDENT
      case 68: // STRING
      case 69: // FORMAT_STRING_START
      case 70: // FORMAT_STRING_CONTINUE
      case 71: // FORMAT_STRING_FINAL
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
#line 263 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); result.append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 764 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 264 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 770 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 267 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 776 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 270 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::STATEMENTS, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 782 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 271 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 788 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 274 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 794 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 275 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; }
#line 800 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 276 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::THROW, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 806 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 277 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 812 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 278 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 818 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 279 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 824 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 280 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 830 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 281 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 836 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 282 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 842 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 283 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 848 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 284 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 854 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 285 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 860 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 286 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 866 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 287 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 872 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 290 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 878 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 291 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 884 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 292 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 890 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 293 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_STAR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 896 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 294 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_SLASH, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 902 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 295 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 908 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 296 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 914 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 297 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 920 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 298 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_STAR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 926 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 299 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_SLASH, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 932 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 300 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_SET, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 938 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 301 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_SET_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 944 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 302 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_SET_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 950 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 303 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_SET_STAR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 956 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 304 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_SET_SLASH, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 962 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 307 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 968 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 308 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 974 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 309 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 980 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 310 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 986 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 311 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 992 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 312 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 998 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 315 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1004 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 316 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FINAL_FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1010 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 319 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1016 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 322 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; }
#line 1022 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 323 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1028 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 326 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1034 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 327 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1040 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 328 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1046 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 330 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM, yystack_[0].value.as< tokenizer::token > () }; }
#line 1052 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 331 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_DEFAULT, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1058 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 332 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_TYPE, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 1064 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 333 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_TYPE_DEFAULT, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1070 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 334 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_EMPLACE, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 1076 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 337 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IF, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1082 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 338 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IFELSE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1088 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 341 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOR, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1094 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 342 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORSTEP, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[7].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1100 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 343 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOREACH, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1106 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 346 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::WHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1112 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 347 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DOWHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1118 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 350 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::TRYCATCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1124 "parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 353 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SWITCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1130 "parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 356 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1136 "parser.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 357 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1142 "parser.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 360 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1148 "parser.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 361 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1154 "parser.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 362 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE_DEFAULT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1160 "parser.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 364 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1166 "parser.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 365 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_TERNARY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1172 "parser.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 367 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1178 "parser.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 368 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_OR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1184 "parser.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 370 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1190 "parser.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 371 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_AND, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1196 "parser.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 373 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1202 "parser.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 374 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1208 "parser.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 375 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1214 "parser.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 376 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1220 "parser.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 377 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1226 "parser.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 379 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1232 "parser.tab.cc" // lalr1.cc:859
    break;

  case 82:
#line 380 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1238 "parser.tab.cc" // lalr1.cc:859
    break;

  case 83:
#line 381 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1244 "parser.tab.cc" // lalr1.cc:859
    break;

  case 84:
#line 382 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1250 "parser.tab.cc" // lalr1.cc:859
    break;

  case 85:
#line 383 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1256 "parser.tab.cc" // lalr1.cc:859
    break;

  case 86:
#line 384 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_CONFIGNAV, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1262 "parser.tab.cc" // lalr1.cc:859
    break;

  case 87:
#line 386 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1268 "parser.tab.cc" // lalr1.cc:859
    break;

  case 88:
#line 387 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1274 "parser.tab.cc" // lalr1.cc:859
    break;

  case 89:
#line 388 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1280 "parser.tab.cc" // lalr1.cc:859
    break;

  case 90:
#line 390 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1286 "parser.tab.cc" // lalr1.cc:859
    break;

  case 91:
#line 391 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MULTIPLY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1292 "parser.tab.cc" // lalr1.cc:859
    break;

  case 92:
#line 392 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_DIVIDE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1298 "parser.tab.cc" // lalr1.cc:859
    break;

  case 93:
#line 393 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_REMAINDER, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1304 "parser.tab.cc" // lalr1.cc:859
    break;

  case 94:
#line 395 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1310 "parser.tab.cc" // lalr1.cc:859
    break;

  case 95:
#line 396 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ());  }
#line 1316 "parser.tab.cc" // lalr1.cc:859
    break;

  case 96:
#line 398 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1322 "parser.tab.cc" // lalr1.cc:859
    break;

  case 97:
#line 399 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1328 "parser.tab.cc" // lalr1.cc:859
    break;

  case 98:
#line 400 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1334 "parser.tab.cc" // lalr1.cc:859
    break;

  case 99:
#line 401 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1340 "parser.tab.cc" // lalr1.cc:859
    break;

  case 100:
#line 403 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_RANGE_INDEX, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); }
#line 1346 "parser.tab.cc" // lalr1.cc:859
    break;

  case 101:
#line 404 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_RANGE_INDEX_LENGTH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1352 "parser.tab.cc" // lalr1.cc:859
    break;

  case 102:
#line 405 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_RANGE_RINDEX_LENGTH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[6].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1358 "parser.tab.cc" // lalr1.cc:859
    break;

  case 103:
#line 406 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_RANGE_INDEX_RLENGTH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[6].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1364 "parser.tab.cc" // lalr1.cc:859
    break;

  case 104:
#line 407 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_RANGE_LENGTH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1370 "parser.tab.cc" // lalr1.cc:859
    break;

  case 105:
#line 408 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_RANGE_RINDEX, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); }
#line 1376 "parser.tab.cc" // lalr1.cc:859
    break;

  case 106:
#line 409 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_RANGE_RINDEX_RLENGTH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[7].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1382 "parser.tab.cc" // lalr1.cc:859
    break;

  case 107:
#line 410 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_RANGE_RLENGTH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1388 "parser.tab.cc" // lalr1.cc:859
    break;

  case 108:
#line 411 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_GET, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1394 "parser.tab.cc" // lalr1.cc:859
    break;

  case 109:
#line 413 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_CALL, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1400 "parser.tab.cc" // lalr1.cc:859
    break;

  case 110:
#line 414 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_CALL, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); }
#line 1406 "parser.tab.cc" // lalr1.cc:859
    break;

  case 111:
#line 416 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_GET, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 1412 "parser.tab.cc" // lalr1.cc:859
    break;

  case 112:
#line 418 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_UNARY, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); }
#line 1418 "parser.tab.cc" // lalr1.cc:859
    break;

  case 113:
#line 419 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_UNARY, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1424 "parser.tab.cc" // lalr1.cc:859
    break;

  case 114:
#line 420 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::INC_PRE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }); }
#line 1430 "parser.tab.cc" // lalr1.cc:859
    break;

  case 115:
#line 421 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DEC_PRE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }); }
#line 1436 "parser.tab.cc" // lalr1.cc:859
    break;

  case 116:
#line 422 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::INC_POST, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[1].value.as< tokenizer::token > () }); }
#line 1442 "parser.tab.cc" // lalr1.cc:859
    break;

  case 117:
#line 423 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DEC_POST, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[1].value.as< tokenizer::token > () }); }
#line 1448 "parser.tab.cc" // lalr1.cc:859
    break;

  case 118:
#line 424 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 1454 "parser.tab.cc" // lalr1.cc:859
    break;

  case 119:
#line 425 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }; }
#line 1460 "parser.tab.cc" // lalr1.cc:859
    break;

  case 120:
#line 426 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1466 "parser.tab.cc" // lalr1.cc:859
    break;

  case 121:
#line 428 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT, tokenizer.create_token() }; }
#line 1472 "parser.tab.cc" // lalr1.cc:859
    break;

  case 122:
#line 429 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1478 "parser.tab.cc" // lalr1.cc:859
    break;

  case 123:
#line 430 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); }
#line 1484 "parser.tab.cc" // lalr1.cc:859
    break;

  case 124:
#line 432 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT_ITEM, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1490 "parser.tab.cc" // lalr1.cc:859
    break;

  case 125:
#line 434 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT_ITEMS, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1496 "parser.tab.cc" // lalr1.cc:859
    break;

  case 126:
#line 435 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[2].value.as< sqf::sqc::bison::astnode > (); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1502 "parser.tab.cc" // lalr1.cc:859
    break;

  case 127:
#line 437 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1508 "parser.tab.cc" // lalr1.cc:859
    break;

  case 128:
#line 439 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1514 "parser.tab.cc" // lalr1.cc:859
    break;

  case 129:
#line 441 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1520 "parser.tab.cc" // lalr1.cc:859
    break;

  case 130:
#line 442 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1526 "parser.tab.cc" // lalr1.cc:859
    break;

  case 131:
#line 443 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1532 "parser.tab.cc" // lalr1.cc:859
    break;

  case 132:
#line 444 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1538 "parser.tab.cc" // lalr1.cc:859
    break;

  case 133:
#line 445 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 1544 "parser.tab.cc" // lalr1.cc:859
    break;

  case 134:
#line 446 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 1550 "parser.tab.cc" // lalr1.cc:859
    break;

  case 135:
#line 447 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_TRUE, tokenizer.create_token() }; }
#line 1556 "parser.tab.cc" // lalr1.cc:859
    break;

  case 136:
#line 448 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_FALSE, tokenizer.create_token() }; }
#line 1562 "parser.tab.cc" // lalr1.cc:859
    break;

  case 137:
#line 449 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() }; }
#line 1568 "parser.tab.cc" // lalr1.cc:859
    break;

  case 138:
#line 450 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_THIS, tokenizer.create_token() }; }
#line 1574 "parser.tab.cc" // lalr1.cc:859
    break;

  case 139:
#line 452 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; }
#line 1580 "parser.tab.cc" // lalr1.cc:859
    break;

  case 140:
#line 453 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1586 "parser.tab.cc" // lalr1.cc:859
    break;

  case 141:
#line 454 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); }
#line 1592 "parser.tab.cc" // lalr1.cc:859
    break;

  case 142:
#line 456 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1598 "parser.tab.cc" // lalr1.cc:859
    break;

  case 143:
#line 457 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1604 "parser.tab.cc" // lalr1.cc:859
    break;

  case 144:
#line 459 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SVAL_FORMAT_STRING }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ());}
#line 1610 "parser.tab.cc" // lalr1.cc:859
    break;

  case 145:
#line 460 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SVAL_FORMAT_STRING }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1616 "parser.tab.cc" // lalr1.cc:859
    break;

  case 146:
#line 462 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() }); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1622 "parser.tab.cc" // lalr1.cc:859
    break;

  case 147:
#line 463 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1628 "parser.tab.cc" // lalr1.cc:859
    break;

  case 148:
#line 464 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() });yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ());  }
#line 1634 "parser.tab.cc" // lalr1.cc:859
    break;

  case 149:
#line 465 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 1640 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1644 "parser.tab.cc" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -98;

  const signed char parser::yytable_ninf_ = -69;

  const short int
  parser::yypact_[] =
  {
     173,   -98,  1041,  1245,   -37,   -10,    31,   -98,    -8,    15,
     856,   856,    17,   -98,   -98,   -98,    38,    10,    52,   -16,
    1245,  1061,   -98,    27,    34,  1245,   -98,    82,   -98,    20,
      84,   925,   429,   -98,    70,    80,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,    86,   -32,    83,    68,    85,    16,    74,
     -98,   -11,    21,   -98,    72,   -98,   -98,   -98,   -98,   -98,
      38,   -98,    -5,   101,   -98,   -98,   105,    58,    -7,    38,
     856,    55,    81,   150,  1245,   649,   -98,   144,   155,  1245,
     -98,   106,  1245,   -98,   126,   -98,    -1,   145,   -98,   -98,
     -21,   -98,   -98,   -98,  1109,  1245,  1245,  1245,   -98,  1245,
     -98,  1245,    20,   -98,    88,   -98,   -98,   511,   -98,   -98,
     -98,   -98,  1245,  1245,  1245,  1245,  1245,  1245,  1245,  1245,
    1245,  1245,  1245,  1245,  1245,  1245,  1245,  1245,  1245,   284,
     118,  1245,  1245,  1245,  1245,  1245,  1245,  1245,  1245,  1245,
    1245,   -98,   -98,  1245,  1245,   -98,   -26,   156,   151,   856,
     -98,    38,   142,  1245,   159,    51,   718,   163,   164,   171,
    1245,  1245,   175,  1245,   -98,   -12,   -98,   -98,  1129,   -98,
       2,   -98,   -98,   -98,   -98,   -98,   -98,    20,   -98,   161,
      83,    68,    85,    85,    85,    85,    16,    16,    16,    16,
      16,    74,    74,   -98,   -98,   -98,  1177,  1245,    60,   174,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,  1245,   140,   170,   -98,   146,   -98,   856,  1245,
     200,   856,   -98,  1245,   148,   188,   -98,   -98,   856,   -98,
     -98,   -98,   -98,   -98,   -98,  1245,   -98,  1245,  1245,   184,
     186,   -98,   973,  1225,   -98,   -98,  1245,   -98,   -98,   189,
    1245,   211,   194,   199,   141,   -98,   -98,   198,   -98,   993,
     -98,  1245,   202,   -98,    23,   -98,   856,   787,   856,   -98,
     856,  1245,   183,    43,   -98,   -98,   -98,  1245,   204,   210,
     -98,   -98,   -98,  1245,   -98,   -98,   -98,   195,   856,   -98,
     -98,   212,   -98,   -98,   856,   580,   -98,   -98,   -98,   -98
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,    21,     0,     0,     0,     0,     0,   136,     0,     0,
       0,     0,     0,   137,   138,   135,     0,     0,     0,     0,
       0,     0,    20,     0,     0,     0,   134,   119,   133,   144,
       0,     0,     0,     6,     0,     0,    12,   129,    13,    14,
      15,    16,    17,     0,    70,    72,    74,    76,    81,    87,
      90,    94,    97,    99,    98,    96,   132,   120,   130,   131,
       0,     9,   119,     0,    97,    98,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,   127,     0,     0,     0,
       5,    42,     0,   121,     0,   125,     0,     0,   139,   142,
       0,   114,   115,    95,     0,     0,     0,     0,   116,     0,
     117,     0,     0,   148,     0,   145,     1,     0,     7,    18,
      11,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     8,    10,     0,     0,    46,    51,     0,    48,     0,
      45,     0,     0,     0,     0,   119,     0,     0,     0,     0,
       0,     0,     0,     0,   122,     0,   118,   140,     0,   112,
       0,    22,    26,    25,    23,    24,   146,     0,   149,     0,
      73,    75,    77,    79,    78,    80,    83,    82,    85,    86,
      84,    88,    89,    92,    91,    93,     0,     0,     0,   111,
      27,    31,    30,    28,    29,    32,    36,    35,    33,    34,
      38,    37,     0,     0,    53,    47,    49,    43,     0,     0,
       0,     0,   128,     0,     0,     0,    41,    40,     0,   124,
     123,   126,   141,   143,   113,     0,   147,     0,     0,     0,
       0,   108,     0,     0,    52,    55,     0,    50,    44,     0,
       0,    56,     0,     0,     0,    61,    71,     0,   104,     0,
     100,     0,     0,   110,     0,    54,     0,     0,     0,    62,
       0,     0,     0,     0,    65,   107,   105,     0,     0,     0,
     101,   109,    60,     0,    58,    57,    63,     0,     0,    64,
      66,     0,   102,   103,     0,     0,    69,   106,    59,    67
  };

  const short int
  parser::yypgoto_[] =
  {
     -98,   -98,   -98,   -22,     4,   -98,   -98,   -98,   -98,   -13,
      30,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -25,   231,
     -98,   134,   135,    50,    32,    45,   -18,   -98,     0,   -98,
       8,   -98,   -98,    89,   -98,    -9,   -98,   -98,   -93,   -98,
     -97
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    30,    31,    32,    76,    34,    35,    36,    37,    70,
     147,   148,    38,    39,    40,    41,    42,   273,   274,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    64,    53,
      65,    55,    56,    85,    86,    77,    57,    58,    90,    59,
     105
  };

  const short int
  parser::yytable_[] =
  {
      52,   170,    78,    80,    33,   176,   112,    93,    54,   107,
      52,    52,   167,    83,   168,   212,   113,   230,    54,    54,
      68,   129,    72,   213,   145,    94,   130,    60,   164,     7,
      67,    52,    52,   234,   165,    33,   108,   235,    71,    54,
      54,   214,    13,    14,    15,    74,    98,    79,    19,   100,
      20,    84,    21,   156,   281,    84,   149,    69,   235,    73,
     146,   150,   131,   271,   272,   132,   124,   133,    68,   125,
      52,    23,   289,   134,    24,    52,   135,    81,    54,    33,
     236,    94,    82,    54,   106,    25,    26,    62,    28,    29,
     102,   103,    95,   241,    91,    96,   242,    97,   143,   144,
     163,    92,    98,    99,   109,   100,   101,    52,   193,   194,
     195,   108,    94,   136,   110,    54,   137,   126,   138,   127,
     111,   128,   151,    95,   139,   114,    96,   140,    97,   115,
     116,   117,   118,    98,    99,   141,   100,   101,   218,   142,
     217,   119,   120,   121,   122,   123,   160,   161,   152,    52,
     264,   186,   187,   188,   189,   190,    52,    54,   177,   178,
     108,   271,   272,   153,    54,   182,   183,   184,   185,   191,
     192,   157,   158,    -2,     1,   163,   166,     2,     3,     4,
       5,     6,     7,     8,     9,   199,   216,   215,    10,    11,
     221,   219,    12,   223,   224,    13,    14,    15,    16,    17,
      18,    19,   225,    20,   243,    21,   228,    22,   245,   248,
     237,   246,   251,   146,   250,   253,   254,   258,    52,   255,
     266,    52,   259,   268,    23,   269,    54,    24,    52,    54,
     270,   275,   288,    63,    66,   280,    54,   292,    25,    26,
      27,    28,    29,   293,   295,   297,   247,   180,   290,   181,
       0,    87,    89,     0,   231,     0,     0,   282,   284,   285,
     104,   286,     0,     0,     0,     0,    52,    52,    52,     0,
      52,     0,     0,     0,    54,    54,    54,     0,    54,   296,
       0,     0,     0,     0,     0,   298,   299,     0,    52,     0,
       0,    60,     0,     7,    52,    52,    54,     0,     0,     0,
       0,     0,    54,    54,     0,   154,    13,    14,    15,     0,
     159,     0,    19,   162,    20,     0,    21,     0,     0,     0,
     196,     0,     0,   197,     0,    89,   171,   172,   173,     0,
     174,     0,   175,   104,     0,    23,     0,     0,    24,     0,
       0,     0,     0,   179,     0,     0,     0,     0,     0,    25,
      26,    62,    28,    29,     0,     0,     0,     0,     0,     0,
     198,     0,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,     0,     0,   210,   211,     0,     0,     0,     0,
       0,     0,     0,     0,   220,     0,     0,     0,     0,     0,
       0,   226,   227,     0,   229,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,   240,    -4,
       1,     0,     0,     2,     3,     4,     5,     6,     7,     8,
       9,     0,     0,   244,    10,    11,     0,     0,    12,     0,
     249,    13,    14,    15,   252,    17,    18,    19,     0,    20,
       0,    21,     0,    22,     0,     0,   233,     0,   256,   257,
       0,     0,     0,   262,    89,     0,     0,   265,     0,     0,
      23,   267,     0,    24,     0,     0,     0,     0,     0,     0,
     278,     0,   279,     0,    25,    26,    27,    28,    29,     0,
       0,     0,   287,     0,     0,     0,     0,     0,   291,     0,
       0,    -3,     1,     0,   294,     2,     3,     4,     5,     6,
       7,     8,     9,     0,     0,     0,    10,    11,     0,     0,
      12,     0,     0,    13,    14,    15,     0,    17,    18,    19,
       0,    20,     0,    21,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,     0,     0,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,    26,    27,    28,
      29,     1,     0,     0,     2,     3,     4,     5,     6,     7,
       8,     9,     0,     0,     0,    10,    11,     0,     0,    12,
     -68,   -68,    13,    14,    15,     0,    17,    18,    75,   -68,
      20,     0,    21,     0,    22,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    23,     0,     0,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    25,    26,    27,    28,    29,
       1,     0,     0,     2,     3,     4,     5,     6,     7,     8,
       9,     0,     0,     0,    10,    11,     0,     0,    12,     0,
       0,    13,    14,    15,     0,    17,    18,    19,    83,    20,
       0,    21,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,    26,   155,    28,    29,     1,
       0,     0,     2,     3,     4,     5,     6,     7,     8,     9,
       0,     0,     0,    10,    11,     0,     0,    12,     0,     0,
      13,    14,    15,     0,    17,    18,    19,   222,    20,     0,
      21,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    25,    26,    27,    28,    29,     1,     0,
       0,     2,     3,     4,     5,     6,     7,     8,     9,     0,
       0,     0,    10,    11,     0,   283,    12,     0,     0,    13,
      14,    15,     0,    17,    18,    75,     0,    20,     0,    21,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,     0,
       0,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    27,    28,    29,     1,     0,     0,
       2,     3,     4,     5,     6,     7,     8,     9,     0,     0,
       0,    10,    11,     0,     0,    12,     0,     0,    13,    14,
      15,     0,    17,    18,    75,     0,    20,     0,    21,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,     0,     0,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,    26,    27,    28,    29,     1,     0,     0,     2,
       3,     4,     5,     6,     7,     8,     9,     0,     0,     0,
      10,    11,     0,     0,    12,     0,     0,    13,    14,    15,
       0,    17,    18,    19,     0,    20,     0,    21,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,     0,     0,    24,
      60,     0,     7,     0,     0,     0,     0,     0,     0,     0,
      25,    26,    27,    28,    29,    13,    14,    15,     0,     0,
      60,    19,     7,    20,     0,    21,   260,     0,     0,     0,
       0,     0,   261,     0,     0,    13,    14,    15,     0,     0,
       0,    19,     0,    20,    23,    21,   276,    24,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,    25,    26,
      62,    28,    29,     0,    23,     0,     0,    24,    60,     0,
       7,     0,     0,     0,     0,     0,     0,     0,    25,    26,
      62,    28,    29,    13,    14,    15,     0,     0,    60,    19,
       7,    20,     0,    21,     0,    61,     0,     0,     0,     0,
       0,     0,     0,    13,    14,    15,     0,     0,     0,    19,
       0,    20,    23,    21,    88,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,    26,    62,    28,
      29,     0,    23,     0,     0,    24,    60,     0,     7,     0,
       0,     0,     0,     0,     0,     0,    25,    26,    62,    28,
      29,    13,    14,    15,     0,     0,    60,    19,     7,    20,
     169,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    13,    14,    15,     0,     0,     0,    19,     0,    20,
      23,    21,   232,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,    26,    62,    28,    29,     0,
      23,     0,     0,    24,    60,     0,     7,     0,     0,     0,
       0,     0,     0,     0,    25,    26,    62,    28,    29,    13,
      14,    15,     0,     0,     0,    19,     0,    20,     0,    21,
       0,     0,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,     0,
       0,    24,    60,     0,     7,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    62,    28,    29,    13,    14,    15,
       0,     0,    60,    19,     7,    20,   263,    21,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    13,    14,    15,
       0,     0,     0,    19,     0,    20,    23,    21,     0,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,    26,    62,    28,    29,     0,    23,     0,     0,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,    26,    62,    28,    29
  };

  const short int
  parser::yycheck_[] =
  {
       0,    94,    11,    16,     0,   102,    38,    25,     0,    31,
      10,    11,    33,    29,    35,    41,    48,    29,    10,    11,
      30,    32,    30,    49,    31,    30,    37,     7,    29,     9,
      67,    31,    32,    31,    35,    31,    32,    35,     7,    31,
      32,    67,    22,    23,    24,    30,    51,    30,    28,    54,
      30,    67,    32,    75,    31,    67,    69,    67,    35,    67,
      67,    70,    41,    20,    21,    44,    50,    46,    30,    53,
      70,    51,    29,    52,    54,    75,    55,    67,    70,    75,
     177,    30,    30,    75,     0,    65,    66,    67,    68,    69,
      70,    71,    41,    33,    67,    44,    36,    46,    40,    41,
      49,    67,    51,    52,    34,    54,    55,   107,   126,   127,
     128,   107,    30,    41,    34,   107,    44,    43,    46,    45,
      34,    47,    67,    41,    52,    42,    44,    55,    46,    61,
      62,    63,    64,    51,    52,    34,    54,    55,   151,    34,
     149,    56,    57,    58,    59,    60,    40,    41,    67,   149,
     243,   119,   120,   121,   122,   123,   156,   149,    70,    71,
     156,    20,    21,    13,   156,   115,   116,   117,   118,   124,
     125,    27,    17,     0,     1,    49,    31,     4,     5,     6,
       7,     8,     9,    10,    11,    67,    35,    31,    15,    16,
      31,    49,    19,    30,    30,    22,    23,    24,    25,    26,
      27,    28,    31,    30,    30,    32,    31,    34,    68,   218,
      49,    41,   221,    67,    14,    67,    28,    33,   218,   228,
      31,   221,    36,    12,    51,    31,   218,    54,   228,   221,
      31,    33,    49,     2,     3,    33,   228,    33,    65,    66,
      67,    68,    69,    33,    49,    33,   216,   113,   273,   114,
      -1,    20,    21,    -1,   165,    -1,    -1,   266,   267,   268,
      29,   270,    -1,    -1,    -1,    -1,   266,   267,   268,    -1,
     270,    -1,    -1,    -1,   266,   267,   268,    -1,   270,   288,
      -1,    -1,    -1,    -1,    -1,   294,   295,    -1,   288,    -1,
      -1,     7,    -1,     9,   294,   295,   288,    -1,    -1,    -1,
      -1,    -1,   294,   295,    -1,    74,    22,    23,    24,    -1,
      79,    -1,    28,    82,    30,    -1,    32,    -1,    -1,    -1,
      36,    -1,    -1,    39,    -1,    94,    95,    96,    97,    -1,
      99,    -1,   101,   102,    -1,    51,    -1,    -1,    54,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,    -1,    -1,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,
      -1,   160,   161,    -1,   163,    -1,    -1,    -1,    -1,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,     0,
       1,    -1,    -1,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,   212,    15,    16,    -1,    -1,    19,    -1,
     219,    22,    23,    24,   223,    26,    27,    28,    -1,    30,
      -1,    32,    -1,    34,    -1,    -1,   235,    -1,   237,   238,
      -1,    -1,    -1,   242,   243,    -1,    -1,   246,    -1,    -1,
      51,   250,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
     259,    -1,   261,    -1,    65,    66,    67,    68,    69,    -1,
      -1,    -1,   271,    -1,    -1,    -1,    -1,    -1,   277,    -1,
      -1,     0,     1,    -1,   283,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    16,    -1,    -1,
      19,    -1,    -1,    22,    23,    24,    -1,    26,    27,    28,
      -1,    30,    -1,    32,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,    68,
      69,     1,    -1,    -1,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    28,    29,
      30,    -1,    32,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    67,    68,    69,
       1,    -1,    -1,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,
      -1,    22,    23,    24,    -1,    26,    27,    28,    29,    30,
      -1,    32,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    68,    69,     1,
      -1,    -1,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,
      22,    23,    24,    -1,    26,    27,    28,    29,    30,    -1,
      32,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,    68,    69,     1,    -1,
      -1,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    16,    -1,    18,    19,    -1,    -1,    22,
      23,    24,    -1,    26,    27,    28,    -1,    30,    -1,    32,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    66,    67,    68,    69,     1,    -1,    -1,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,    23,
      24,    -1,    26,    27,    28,    -1,    30,    -1,    32,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,    68,    69,     1,    -1,    -1,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    -1,    -1,    19,    -1,    -1,    22,    23,    24,
      -1,    26,    27,    28,    -1,    30,    -1,    32,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    54,
       7,    -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    66,    67,    68,    69,    22,    23,    24,    -1,    -1,
       7,    28,     9,    30,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    22,    23,    24,    -1,    -1,
      -1,    28,    -1,    30,    51,    32,    33,    54,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    65,    66,
      67,    68,    69,    -1,    51,    -1,    -1,    54,     7,    -1,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,
      67,    68,    69,    22,    23,    24,    -1,    -1,     7,    28,
       9,    30,    -1,    32,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    -1,    -1,    -1,    28,
      -1,    30,    51,    32,    33,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,    68,
      69,    -1,    51,    -1,    -1,    54,     7,    -1,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,    68,
      69,    22,    23,    24,    -1,    -1,     7,    28,     9,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    23,    24,    -1,    -1,    -1,    28,    -1,    30,
      51,    32,    33,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    68,    69,    -1,
      51,    -1,    -1,    54,     7,    -1,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    68,    69,    22,
      23,    24,    -1,    -1,    -1,    28,    -1,    30,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    54,     7,    -1,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    66,    67,    68,    69,    22,    23,    24,
      -1,    -1,     7,    28,     9,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      -1,    -1,    -1,    28,    -1,    30,    51,    32,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    66,    67,    68,    69,    -1,    51,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    66,    67,    68,    69
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     1,     4,     5,     6,     7,     8,     9,    10,    11,
      15,    16,    19,    22,    23,    24,    25,    26,    27,    28,
      30,    32,    34,    51,    54,    65,    66,    67,    68,    69,
      73,    74,    75,    76,    77,    78,    79,    80,    84,    85,
      86,    87,    88,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   108,   109,   111,
       7,    34,    67,    91,   100,   102,    91,    67,    30,    67,
      81,     7,    30,    67,    30,    28,    76,   107,   107,    30,
      81,    67,    30,    29,    67,   105,   106,    91,    33,    91,
     110,    67,    67,    98,    30,    41,    44,    46,    51,    52,
      54,    55,    70,    71,    91,   112,     0,    75,    76,    34,
      34,    34,    38,    48,    42,    61,    62,    63,    64,    56,
      57,    58,    59,    60,    50,    53,    43,    45,    47,    32,
      37,    41,    44,    46,    52,    55,    41,    44,    46,    52,
      55,    34,    34,    40,    41,    31,    67,    82,    83,    81,
     107,    67,    67,    13,    91,    67,    75,    27,    17,    91,
      40,    41,    91,    49,    29,    35,    31,    33,    35,    31,
     110,    91,    91,    91,    91,    91,   112,    70,    71,    91,
      93,    94,    95,    95,    95,    95,    96,    96,    96,    96,
      96,    97,    97,    98,    98,    98,    36,    39,    91,    67,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    41,    49,    67,    31,    35,   107,    81,    49,
      91,    31,    29,    30,    30,    31,    91,    91,    31,    91,
      29,   105,    33,    91,    31,    35,   112,    49,    39,    91,
      91,    33,    36,    30,    91,    68,    41,    82,   107,    91,
      14,   107,    91,    67,    28,   107,    91,    91,    33,    36,
      33,    39,    91,    31,   110,    91,    31,    91,    12,    31,
      31,    20,    21,    89,    90,    33,    33,    39,    91,    91,
      33,    31,   107,    18,   107,   107,   107,    91,    49,    29,
      90,    91,    33,    33,    91,    49,   107,    33,   107,   107
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    72,    73,    73,    73,    74,    75,    75,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    78,    78,    78,
      78,    78,    78,    79,    79,    80,    81,    81,    82,    82,
      82,    83,    83,    83,    83,    83,    84,    84,    85,    85,
      85,    86,    86,    87,    88,    89,    89,    90,    90,    90,
      91,    91,    92,    92,    93,    93,    94,    94,    94,    94,
      94,    95,    95,    95,    95,    95,    95,    96,    96,    96,
      97,    97,    97,    97,    98,    98,    99,    99,    99,    99,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   101,
     101,   102,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   104,   104,   104,   105,   106,   106,   107,   107,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   109,
     109,   109,   110,   110,   111,   111,   112,   112,   112,   112
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     2,     1,     2,     1,     2,     3,     2,
       3,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     2,
       4,     4,     2,     4,     5,     3,     2,     3,     1,     2,
       3,     1,     3,     2,     4,     3,     5,     7,     7,     9,
       7,     5,     6,     7,     7,     1,     2,     4,     3,     3,
       1,     5,     1,     3,     1,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     1,     2,     1,     1,     1,     1,
       5,     6,     7,     7,     5,     6,     8,     6,     4,     6,
       5,     3,     3,     4,     2,     2,     2,     2,     3,     1,
       1,     2,     3,     4,     3,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       3,     4,     1,     3,     1,     2,     2,     3,     1,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "NA", "error", "$undefined", "\"break\"", "\"return\"", "\"throw\"",
  "\"let\"", "\"function\"", "\"final\"", "\"false\"", "\"for\"", "\"if\"",
  "\"else\"", "\"from\"", "\"to\"", "\"do\"", "\"try\"", "\"catch\"",
  "\"step\"", "\"switch\"", "\"case\"", "\"default\"", "\"nil\"",
  "\"this\"", "\"true\"", "\"params\"", "\"private\"", "\"while\"",
  "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"[\"", "\"]\"", "\";\"", "\",\"",
  "\"..\"", "\".\"", "\"?\"", "\"^\"", "\"be\"", "\"=\"", "\"&&\"",
  "\"/\"", "\"/=\"", "\"*\"", "\"*=\"", "\"%\"", "\"||\"", "\":\"",
  "\"+\"", "\"++\"", "\"+=\"", "\"-\"", "\"--\"", "\"-=\"", "\"<=\"",
  "\"<\"", "\">=\"", "\">>\"", "\">\"", "\"===\"", "\"==\"", "\"!==\"",
  "\"!=\"", "\"!\"", "NUMBER", "IDENT", "STRING", "FORMAT_STRING_START",
  "FORMAT_STRING_CONTINUE", "FORMAT_STRING_FINAL", "$accept", "start",
  "filehead", "statements", "statement", "assignment", "vardecl",
  "funcdecl", "function", "funchead", "arglist", "argitem", "if", "for",
  "while", "trycatch", "switch", "caselist", "case", "exp01", "exp02",
  "exp03", "exp04", "exp05", "exp06", "exp07", "exp08", "exp09", "arrget",
  "call", "objget", "expp", "obj", "obj_item", "obj_items", "codeblock",
  "value", "array", "explist", "format_string", "format_string_match", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   262,   262,   263,   264,   267,   270,   271,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   307,   308,   309,
     310,   311,   312,   315,   316,   319,   322,   323,   326,   327,
     328,   330,   331,   332,   333,   334,   337,   338,   341,   342,
     343,   346,   347,   350,   353,   356,   357,   360,   361,   362,
     364,   365,   367,   368,   370,   371,   373,   374,   375,   376,
     377,   379,   380,   381,   382,   383,   384,   386,   387,   388,
     390,   391,   392,   393,   395,   396,   398,   399,   400,   401,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   413,
     414,   416,   418,   419,   420,   421,   422,   423,   424,   425,
     426,   428,   429,   430,   432,   434,   435,   437,   439,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   452,
     453,   454,   456,   457,   459,   460,   462,   463,   464,   465
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
#line 2422 "parser.tab.cc" // lalr1.cc:1167
#line 467 "parser.y" // lalr1.cc:1168

#ifdef __GNUG__
    #pragma GCC diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

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
         
         case tokenizer::etoken::t_be: return parser::make_BE(token, loc);
         case tokenizer::etoken::t_catch: return parser::make_CATCH(loc);
         case tokenizer::etoken::t_case: return parser::make_CASE(loc);
         case tokenizer::etoken::t_default: return parser::make_DEFAULT(loc);
         case tokenizer::etoken::t_do: return parser::make_DO(loc);
         case tokenizer::etoken::t_break: return parser::make_BREAK(loc);
         case tokenizer::etoken::t_else: return parser::make_ELSE(loc);
         case tokenizer::etoken::t_false: return parser::make_FALSE(loc);
         case tokenizer::etoken::t_for: return parser::make_FOR(loc);
         case tokenizer::etoken::t_function: return parser::make_FUNCTION(loc);
         case tokenizer::etoken::t_final: return parser::make_FINAL(loc);
         case tokenizer::etoken::t_from: return parser::make_FROM(loc);
         case tokenizer::etoken::t_if: return parser::make_IF(loc);
         case tokenizer::etoken::t_let: return parser::make_LET(loc);
         case tokenizer::etoken::t_nil: return parser::make_NIL(loc);
         case tokenizer::etoken::t_params: return parser::make_PARAMS(loc);
         case tokenizer::etoken::t_private: return parser::make_PRIVATE(loc);
         case tokenizer::etoken::t_return: return parser::make_RETURN(loc);
         case tokenizer::etoken::t_step: return parser::make_STEP(loc);
         case tokenizer::etoken::t_switch: return parser::make_SWITCH(loc);
         case tokenizer::etoken::t_throw: return parser::make_THROW(loc);
         case tokenizer::etoken::t_try: return parser::make_TRY(loc);
         case tokenizer::etoken::t_this: return parser::make_THIS(loc);
         case tokenizer::etoken::t_true: return parser::make_TRUE(loc);
         case tokenizer::etoken::t_to: return parser::make_TO(loc);
         case tokenizer::etoken::t_while: return parser::make_WHILE(loc);

         case tokenizer::etoken::s_curlyo: return parser::make_CURLYO(loc);
         case tokenizer::etoken::s_curlyc: return parser::make_CURLYC(loc);
         case tokenizer::etoken::s_roundo: return parser::make_ROUNDO(loc);
         case tokenizer::etoken::s_roundc: return parser::make_ROUNDC(loc);
         case tokenizer::etoken::s_edgeo: return parser::make_SQUAREO(loc);
         case tokenizer::etoken::s_edgec: return parser::make_SQUAREC(loc);
         case tokenizer::etoken::s_equalequalequal: return parser::make_EQUALEQUALEQUAL(token, loc);
         case tokenizer::etoken::s_equalequal: return parser::make_EQUALEQUAL(token, loc);
         case tokenizer::etoken::s_equal: return parser::make_EQUAL(token, loc);
         case tokenizer::etoken::s_greaterthenequal: return parser::make_GTEQUAL(token, loc);
         case tokenizer::etoken::s_greaterthengreaterthen: return parser::make_GTGT(token, loc);
         case tokenizer::etoken::s_greaterthen: return parser::make_GT(token, loc);
         case tokenizer::etoken::s_lessthenequal: return parser::make_LTEQUAL(token, loc);
         case tokenizer::etoken::s_lessthen: return parser::make_LT(token, loc);
         case tokenizer::etoken::s_plus: return parser::make_PLUS(token, loc);
         case tokenizer::etoken::s_plusplus: return parser::make_PLUSPLUS(token, loc);
         case tokenizer::etoken::s_plusassign: return parser::make_PLUSASSIGN(token, loc);
         case tokenizer::etoken::s_minus: return parser::make_MINUS(token, loc);
         case tokenizer::etoken::s_minusminus: return parser::make_MINUSMINUS(token, loc);
         case tokenizer::etoken::s_minusassign: return parser::make_MINUSASSIGN(token, loc);
         case tokenizer::etoken::s_notequalequal: return parser::make_EXCLAMATIONMARKEQUALEQUAL(token, loc);
         case tokenizer::etoken::s_notequal: return parser::make_EXCLAMATIONMARKEQUAL(token, loc);
         case tokenizer::etoken::s_exclamationmark: return parser::make_EXCLAMATIONMARK(token, loc);
         case tokenizer::etoken::s_percent: return parser::make_PERCENT(token, loc);
         case tokenizer::etoken::s_star: return parser::make_STAR(token, loc);
         case tokenizer::etoken::s_starassign: return parser::make_STARASSIGN(token, loc);
         case tokenizer::etoken::s_slash: return parser::make_SLASH(token, loc);
         case tokenizer::etoken::s_slashassign: return parser::make_SLASHASSIGN(token, loc);
         case tokenizer::etoken::s_andand: return parser::make_ANDAND(token, loc);
         case tokenizer::etoken::s_oror: return parser::make_VLINEVLINE(token, loc);
         case tokenizer::etoken::s_questionmark: return parser::make_QUESTIONMARK(loc);
         case tokenizer::etoken::s_circumflex: return parser::make_CIRCUMFLEX(loc);
         case tokenizer::etoken::s_colon: return parser::make_COLON(token, loc);
         case tokenizer::etoken::s_semicolon: return parser::make_SEMICOLON(loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(loc);
         case tokenizer::etoken::s_dotdot: return parser::make_DOTDOT(loc);
         case tokenizer::etoken::s_dot: return parser::make_DOT(loc);

         case tokenizer::etoken::t_string: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_ident: return parser::make_IDENT(token, loc);
         case tokenizer::etoken::t_number: return parser::make_NUMBER(token, loc);
         case tokenizer::etoken::t_formatted_string_start: return parser::make_FORMAT_STRING_START(token, loc);
         case tokenizer::etoken::t_formatted_string_continue: return parser::make_FORMAT_STRING_CONTINUE(token, loc);
         case tokenizer::etoken::t_formatted_string_final: return parser::make_FORMAT_STRING_FINAL(token, loc);
         default:
             return parser::make_NA(loc);
         }
     }
}
