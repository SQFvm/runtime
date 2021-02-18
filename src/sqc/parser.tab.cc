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
#line 145 "parser.y" // lalr1.cc:413

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
      case 71: // filehead
      case 72: // statements
      case 73: // statement
      case 74: // assignment
      case 75: // vardecl
      case 76: // funcdecl
      case 77: // function
      case 78: // funchead
      case 79: // arglist
      case 80: // argitem
      case 81: // if
      case 82: // for
      case 83: // while
      case 84: // trycatch
      case 85: // switch
      case 86: // caselist
      case 87: // case
      case 88: // exp01
      case 89: // exp02
      case 90: // exp03
      case 91: // exp04
      case 92: // exp05
      case 93: // exp06
      case 94: // exp07
      case 95: // exp08
      case 96: // exp09
      case 97: // arrget
      case 98: // call
      case 99: // objget
      case 100: // expp
      case 101: // obj
      case 102: // obj_item
      case 103: // obj_items
      case 104: // codeblock
      case 105: // value
      case 106: // array
      case 107: // explist
      case 108: // format_string
      case 109: // format_string_match
        value.move< sqf::sqc::bison::astnode > (that.value);
        break;

      case 38: // "be"
      case 39: // "="
      case 40: // "&&"
      case 41: // "/"
      case 42: // "/="
      case 43: // "*"
      case 44: // "*="
      case 45: // "%"
      case 46: // "||"
      case 47: // ":"
      case 48: // "+"
      case 49: // "++"
      case 50: // "+="
      case 51: // "-"
      case 52: // "--"
      case 53: // "-="
      case 54: // "<="
      case 55: // "<"
      case 56: // ">="
      case 57: // ">"
      case 58: // "==="
      case 59: // "=="
      case 60: // "!=="
      case 61: // "!="
      case 62: // "!"
      case 63: // NUMBER
      case 64: // IDENT
      case 65: // STRING
      case 66: // FORMAT_STRING_START
      case 67: // FORMAT_STRING_CONTINUE
      case 68: // FORMAT_STRING_FINAL
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
      case 71: // filehead
      case 72: // statements
      case 73: // statement
      case 74: // assignment
      case 75: // vardecl
      case 76: // funcdecl
      case 77: // function
      case 78: // funchead
      case 79: // arglist
      case 80: // argitem
      case 81: // if
      case 82: // for
      case 83: // while
      case 84: // trycatch
      case 85: // switch
      case 86: // caselist
      case 87: // case
      case 88: // exp01
      case 89: // exp02
      case 90: // exp03
      case 91: // exp04
      case 92: // exp05
      case 93: // exp06
      case 94: // exp07
      case 95: // exp08
      case 96: // exp09
      case 97: // arrget
      case 98: // call
      case 99: // objget
      case 100: // expp
      case 101: // obj
      case 102: // obj_item
      case 103: // obj_items
      case 104: // codeblock
      case 105: // value
      case 106: // array
      case 107: // explist
      case 108: // format_string
      case 109: // format_string_match
        value.copy< sqf::sqc::bison::astnode > (that.value);
        break;

      case 38: // "be"
      case 39: // "="
      case 40: // "&&"
      case 41: // "/"
      case 42: // "/="
      case 43: // "*"
      case 44: // "*="
      case 45: // "%"
      case 46: // "||"
      case 47: // ":"
      case 48: // "+"
      case 49: // "++"
      case 50: // "+="
      case 51: // "-"
      case 52: // "--"
      case 53: // "-="
      case 54: // "<="
      case 55: // "<"
      case 56: // ">="
      case 57: // ">"
      case 58: // "==="
      case 59: // "=="
      case 60: // "!=="
      case 61: // "!="
      case 62: // "!"
      case 63: // NUMBER
      case 64: // IDENT
      case 65: // STRING
      case 66: // FORMAT_STRING_START
      case 67: // FORMAT_STRING_CONTINUE
      case 68: // FORMAT_STRING_FINAL
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
      case 71: // filehead
      case 72: // statements
      case 73: // statement
      case 74: // assignment
      case 75: // vardecl
      case 76: // funcdecl
      case 77: // function
      case 78: // funchead
      case 79: // arglist
      case 80: // argitem
      case 81: // if
      case 82: // for
      case 83: // while
      case 84: // trycatch
      case 85: // switch
      case 86: // caselist
      case 87: // case
      case 88: // exp01
      case 89: // exp02
      case 90: // exp03
      case 91: // exp04
      case 92: // exp05
      case 93: // exp06
      case 94: // exp07
      case 95: // exp08
      case 96: // exp09
      case 97: // arrget
      case 98: // call
      case 99: // objget
      case 100: // expp
      case 101: // obj
      case 102: // obj_item
      case 103: // obj_items
      case 104: // codeblock
      case 105: // value
      case 106: // array
      case 107: // explist
      case 108: // format_string
      case 109: // format_string_match
        yylhs.value.build< sqf::sqc::bison::astnode > ();
        break;

      case 38: // "be"
      case 39: // "="
      case 40: // "&&"
      case 41: // "/"
      case 42: // "/="
      case 43: // "*"
      case 44: // "*="
      case 45: // "%"
      case 46: // "||"
      case 47: // ":"
      case 48: // "+"
      case 49: // "++"
      case 50: // "+="
      case 51: // "-"
      case 52: // "--"
      case 53: // "-="
      case 54: // "<="
      case 55: // "<"
      case 56: // ">="
      case 57: // ">"
      case 58: // "==="
      case 59: // "=="
      case 60: // "!=="
      case 61: // "!="
      case 62: // "!"
      case 63: // NUMBER
      case 64: // IDENT
      case 65: // STRING
      case 66: // FORMAT_STRING_START
      case 67: // FORMAT_STRING_CONTINUE
      case 68: // FORMAT_STRING_FINAL
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
#line 251 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); result.append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 761 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 252 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 767 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 255 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 773 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 258 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::STATEMENTS, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 779 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 259 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 785 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 262 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 791 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 263 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; }
#line 797 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 264 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::THROW, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 803 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 265 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 809 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 266 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 815 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 267 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 821 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 268 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 827 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 269 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 833 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 270 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 839 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 271 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 845 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 272 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 851 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 273 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 857 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 274 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 863 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 275 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 869 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 278 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 875 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 279 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 881 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 280 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 887 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 281 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_STAR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 893 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 282 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_SLASH, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 899 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 283 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 905 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 284 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 911 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 285 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 917 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 286 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_STAR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 923 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 287 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_SLASH, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 929 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 288 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_SET, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 935 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 289 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_SET_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 941 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 290 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_SET_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 947 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 291 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_SET_STAR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 953 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 292 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_SET_SLASH, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 959 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 295 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 965 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 296 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 971 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 297 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 977 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 298 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 983 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 299 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 989 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 300 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 995 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 303 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1001 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 304 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FINAL_FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1007 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 307 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1013 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 310 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; }
#line 1019 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 311 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1025 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 314 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1031 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 315 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1037 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 316 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1043 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 318 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM, yystack_[0].value.as< tokenizer::token > () }; }
#line 1049 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 319 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_DEFAULT, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1055 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 320 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_TYPE, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 1061 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 321 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_TYPE_DEFAULT, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1067 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 322 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_EMPLACE, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 1073 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 325 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IF, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1079 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 326 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IFELSE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1085 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 329 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOR, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1091 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 330 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORSTEP, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[7].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1097 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 331 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOREACH, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1103 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 334 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::WHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1109 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 335 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DOWHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1115 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 338 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::TRYCATCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1121 "parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 341 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SWITCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1127 "parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 344 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1133 "parser.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 345 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1139 "parser.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 348 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1145 "parser.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 349 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1151 "parser.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 350 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE_DEFAULT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1157 "parser.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 352 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1163 "parser.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 353 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_TERNARY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1169 "parser.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 355 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1175 "parser.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 356 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_OR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1181 "parser.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 358 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1187 "parser.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 359 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_AND, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1193 "parser.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 361 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1199 "parser.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 362 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1205 "parser.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 363 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1211 "parser.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 364 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1217 "parser.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 365 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1223 "parser.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 367 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1229 "parser.tab.cc" // lalr1.cc:859
    break;

  case 82:
#line 368 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1235 "parser.tab.cc" // lalr1.cc:859
    break;

  case 83:
#line 369 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1241 "parser.tab.cc" // lalr1.cc:859
    break;

  case 84:
#line 370 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1247 "parser.tab.cc" // lalr1.cc:859
    break;

  case 85:
#line 371 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1253 "parser.tab.cc" // lalr1.cc:859
    break;

  case 86:
#line 373 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1259 "parser.tab.cc" // lalr1.cc:859
    break;

  case 87:
#line 374 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1265 "parser.tab.cc" // lalr1.cc:859
    break;

  case 88:
#line 375 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1271 "parser.tab.cc" // lalr1.cc:859
    break;

  case 89:
#line 377 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1277 "parser.tab.cc" // lalr1.cc:859
    break;

  case 90:
#line 378 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MULTIPLY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1283 "parser.tab.cc" // lalr1.cc:859
    break;

  case 91:
#line 379 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_DIVIDE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1289 "parser.tab.cc" // lalr1.cc:859
    break;

  case 92:
#line 380 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_REMAINDER, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1295 "parser.tab.cc" // lalr1.cc:859
    break;

  case 93:
#line 382 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1301 "parser.tab.cc" // lalr1.cc:859
    break;

  case 94:
#line 383 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ());  }
#line 1307 "parser.tab.cc" // lalr1.cc:859
    break;

  case 95:
#line 385 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1313 "parser.tab.cc" // lalr1.cc:859
    break;

  case 96:
#line 386 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1319 "parser.tab.cc" // lalr1.cc:859
    break;

  case 97:
#line 387 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1325 "parser.tab.cc" // lalr1.cc:859
    break;

  case 98:
#line 388 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1331 "parser.tab.cc" // lalr1.cc:859
    break;

  case 99:
#line 390 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_GET, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1337 "parser.tab.cc" // lalr1.cc:859
    break;

  case 100:
#line 392 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_CALL, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1343 "parser.tab.cc" // lalr1.cc:859
    break;

  case 101:
#line 393 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_CALL, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); }
#line 1349 "parser.tab.cc" // lalr1.cc:859
    break;

  case 102:
#line 395 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS_GET, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 1355 "parser.tab.cc" // lalr1.cc:859
    break;

  case 103:
#line 397 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_UNARY, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); }
#line 1361 "parser.tab.cc" // lalr1.cc:859
    break;

  case 104:
#line 398 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_UNARY, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1367 "parser.tab.cc" // lalr1.cc:859
    break;

  case 105:
#line 399 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::INC_PRE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }); }
#line 1373 "parser.tab.cc" // lalr1.cc:859
    break;

  case 106:
#line 400 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DEC_PRE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }); }
#line 1379 "parser.tab.cc" // lalr1.cc:859
    break;

  case 107:
#line 401 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::INC_POST, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[1].value.as< tokenizer::token > () }); }
#line 1385 "parser.tab.cc" // lalr1.cc:859
    break;

  case 108:
#line 402 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DEC_POST, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[1].value.as< tokenizer::token > () }); }
#line 1391 "parser.tab.cc" // lalr1.cc:859
    break;

  case 109:
#line 403 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 1397 "parser.tab.cc" // lalr1.cc:859
    break;

  case 110:
#line 404 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }; }
#line 1403 "parser.tab.cc" // lalr1.cc:859
    break;

  case 111:
#line 405 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1409 "parser.tab.cc" // lalr1.cc:859
    break;

  case 112:
#line 407 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT, tokenizer.create_token() }; }
#line 1415 "parser.tab.cc" // lalr1.cc:859
    break;

  case 113:
#line 408 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1421 "parser.tab.cc" // lalr1.cc:859
    break;

  case 114:
#line 409 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); }
#line 1427 "parser.tab.cc" // lalr1.cc:859
    break;

  case 115:
#line 411 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT_ITEM, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1433 "parser.tab.cc" // lalr1.cc:859
    break;

  case 116:
#line 413 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT_ITEMS, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1439 "parser.tab.cc" // lalr1.cc:859
    break;

  case 117:
#line 414 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[2].value.as< sqf::sqc::bison::astnode > (); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1445 "parser.tab.cc" // lalr1.cc:859
    break;

  case 118:
#line 416 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1451 "parser.tab.cc" // lalr1.cc:859
    break;

  case 119:
#line 418 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1457 "parser.tab.cc" // lalr1.cc:859
    break;

  case 120:
#line 420 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1463 "parser.tab.cc" // lalr1.cc:859
    break;

  case 121:
#line 421 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1469 "parser.tab.cc" // lalr1.cc:859
    break;

  case 122:
#line 422 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1475 "parser.tab.cc" // lalr1.cc:859
    break;

  case 123:
#line 423 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1481 "parser.tab.cc" // lalr1.cc:859
    break;

  case 124:
#line 424 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 1487 "parser.tab.cc" // lalr1.cc:859
    break;

  case 125:
#line 425 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 1493 "parser.tab.cc" // lalr1.cc:859
    break;

  case 126:
#line 426 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_TRUE, tokenizer.create_token() }; }
#line 1499 "parser.tab.cc" // lalr1.cc:859
    break;

  case 127:
#line 427 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_FALSE, tokenizer.create_token() }; }
#line 1505 "parser.tab.cc" // lalr1.cc:859
    break;

  case 128:
#line 428 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() }; }
#line 1511 "parser.tab.cc" // lalr1.cc:859
    break;

  case 129:
#line 429 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_THIS, tokenizer.create_token() }; }
#line 1517 "parser.tab.cc" // lalr1.cc:859
    break;

  case 130:
#line 431 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; }
#line 1523 "parser.tab.cc" // lalr1.cc:859
    break;

  case 131:
#line 432 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1529 "parser.tab.cc" // lalr1.cc:859
    break;

  case 132:
#line 433 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); }
#line 1535 "parser.tab.cc" // lalr1.cc:859
    break;

  case 133:
#line 435 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1541 "parser.tab.cc" // lalr1.cc:859
    break;

  case 134:
#line 436 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1547 "parser.tab.cc" // lalr1.cc:859
    break;

  case 135:
#line 438 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SVAL_FORMAT_STRING }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ());}
#line 1553 "parser.tab.cc" // lalr1.cc:859
    break;

  case 136:
#line 439 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SVAL_FORMAT_STRING }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1559 "parser.tab.cc" // lalr1.cc:859
    break;

  case 137:
#line 441 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() }); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1565 "parser.tab.cc" // lalr1.cc:859
    break;

  case 138:
#line 442 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1571 "parser.tab.cc" // lalr1.cc:859
    break;

  case 139:
#line 443 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() });yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ());  }
#line 1577 "parser.tab.cc" // lalr1.cc:859
    break;

  case 140:
#line 444 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 1583 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1587 "parser.tab.cc" // lalr1.cc:859
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
      35,   -98,   334,  1000,   -57,   -15,     7,   -98,   -11,    -7,
     760,   760,    -2,   -98,   -98,   -98,    56,    19,    63,   -17,
    1000,   872,   -98,    44,    48,  1000,   -98,   335,   -98,   169,
     129,   826,   117,   -98,    96,    97,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   100,   -28,    74,   126,   148,    20,    49,
     -98,   -12,   114,   -98,   121,   -98,   -98,   -98,   -98,   -98,
      56,   -98,    36,   101,   -98,   -98,   104,    57,    -9,    56,
     760,    82,    88,   146,  1000,   562,   -98,   135,   151,  1000,
     -98,    64,  1000,   -98,   123,   -98,    -8,   141,   -98,   -98,
      45,   -98,   -98,   -98,   918,  1000,  1000,  1000,   -98,  1000,
     -98,  1000,   169,   -98,    43,   -98,   -98,   430,   -98,   -98,
     -98,   -98,  1000,  1000,  1000,  1000,  1000,  1000,  1000,  1000,
    1000,  1000,  1000,  1000,  1000,  1000,  1000,  1000,  1000,   109,
    1000,  1000,  1000,  1000,  1000,  1000,  1000,  1000,  1000,  1000,
     -98,   -98,  1000,  1000,   -98,    25,   144,   142,   760,   -98,
      56,   147,  1000,   157,   261,   628,   165,   166,   167,  1000,
    1000,   175,  1000,   -98,   -16,   -98,   -98,   936,   -98,    -6,
     -98,   -98,   -98,   -98,   -98,   -98,   169,   -98,   153,    74,
     126,   148,   148,   148,   148,    20,    20,    20,    20,    49,
      49,   -98,   -98,   -98,   180,   185,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,  1000,   154,
     183,   -98,   161,   -98,   760,  1000,   212,   760,   -98,  1000,
     163,   201,   -98,   -98,   760,   -98,   -98,   -98,   -98,   -98,
     -98,  1000,   -98,  1000,   -98,   982,   -98,   -98,  1000,   -98,
     -98,   211,  1000,   231,   213,   214,    95,   -98,   -98,   -98,
      -5,   -98,   760,   694,   760,   -98,   760,  1000,   199,    53,
     -98,   -98,   -98,  1000,   -98,   -98,   -98,   200,   760,   -98,
     -98,   760,   496,   -98,   -98,   -98
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,    21,     0,     0,     0,     0,     0,   127,     0,     0,
       0,     0,     0,   128,   129,   126,     0,     0,     0,     0,
       0,     0,    20,     0,     0,     0,   125,   110,   124,   135,
       0,     0,     0,     6,     0,     0,    12,   120,    13,    14,
      15,    16,    17,     0,    70,    72,    74,    76,    81,    86,
      89,    93,    96,    98,    97,    95,   123,   111,   121,   122,
       0,     9,   110,     0,    96,    97,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,   118,     0,     0,     0,
       5,    42,     0,   112,     0,   116,     0,     0,   130,   133,
       0,   105,   106,    94,     0,     0,     0,     0,   107,     0,
     108,     0,     0,   139,     0,   136,     1,     0,     7,    18,
      11,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       8,    10,     0,     0,    46,    51,     0,    48,     0,    45,
       0,     0,     0,     0,   110,     0,     0,     0,     0,     0,
       0,     0,     0,   113,     0,   109,   131,     0,   103,     0,
      22,    26,    25,    23,    24,   137,     0,   140,     0,    73,
      75,    77,    79,    78,    80,    83,    82,    85,    84,    87,
      88,    91,    90,    92,     0,   102,    27,    31,    30,    28,
      29,    32,    36,    35,    33,    34,    38,    37,     0,     0,
      53,    47,    49,    43,     0,     0,     0,     0,   119,     0,
       0,     0,    41,    40,     0,   115,   114,   117,   132,   134,
     104,     0,   138,     0,    99,     0,    52,    55,     0,    50,
      44,     0,     0,    56,     0,     0,     0,    61,    71,   101,
       0,    54,     0,     0,     0,    62,     0,     0,     0,     0,
      65,   100,    60,     0,    58,    57,    63,     0,     0,    64,
      66,     0,     0,    69,    59,    67
  };

  const short int
  parser::yypgoto_[] =
  {
     -98,   -98,   -98,   -23,     2,   -98,   -98,   -98,   -98,   -13,
      37,   -98,   -98,   -98,   -98,   -98,   -98,   -98,     4,   187,
     -98,   137,   134,    94,   119,    -4,   -21,   -98,     0,   -98,
       6,   -98,   -98,    87,   -98,    80,   -98,   -98,   -93,   -98,
     -97
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    30,    31,    32,    76,    34,    35,    36,    37,    70,
     146,   147,    38,    39,    40,    41,    42,   259,   260,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    64,    53,
      65,    55,    56,    85,    86,    77,    57,    58,    90,    59,
     105
  };

  const short int
  parser::yytable_[] =
  {
      52,   169,    33,    80,    93,   175,    54,    67,   107,   112,
      52,    52,    83,   226,    71,    68,    54,    54,   113,    72,
     128,   163,   144,    74,   129,   230,   261,   164,    79,   231,
     231,    52,    52,    33,   108,    -2,     1,    54,    54,     2,
       3,     4,     5,     6,     7,     8,     9,    84,    84,    69,
      10,    11,   155,    73,    12,   145,   148,    13,    14,    15,
      16,    17,    18,    19,   208,    20,    94,    21,   123,    22,
      52,   124,   209,   257,   258,    52,    54,    33,   166,   232,
     167,    54,   269,    81,    23,    98,    68,    24,   100,   210,
     125,    78,   126,    82,   127,   142,   143,    25,    26,    27,
      28,    29,   159,   160,   191,   192,   193,    52,    91,   108,
     176,   177,    92,    54,   114,   257,   258,    -4,     1,   189,
     190,     2,     3,     4,     5,     6,     7,     8,     9,   106,
     109,   110,    10,    11,   111,   140,    12,   214,   141,    13,
      14,    15,   250,    17,    18,    19,   150,    20,    52,    21,
     149,    22,   151,   130,    54,    52,   131,   108,   132,   152,
     135,    54,   156,   136,   133,   137,    23,   134,   157,    24,
     162,   138,   165,   195,   139,   211,    60,   212,     7,    25,
      26,    27,    28,    29,   115,   116,   117,   118,   217,    63,
      66,    13,    14,    15,   215,   219,   220,    19,   221,    20,
     233,    21,   119,   120,   121,   122,   224,    87,    89,   181,
     182,   183,   184,   234,    52,   235,   104,    52,    23,   237,
      54,    24,   238,    54,    52,   145,   242,   245,   213,   246,
      54,    25,    26,    62,    28,    29,   102,   103,   185,   186,
     187,   188,   252,   254,   255,   256,   268,   272,   180,   239,
     179,   227,    52,    52,    52,     0,    52,     0,    54,    54,
      54,   153,    54,   270,     0,     0,   158,     0,    52,   161,
       0,    52,    52,     0,    54,     0,     0,    54,    54,     0,
       0,    89,   170,   171,   172,     0,   173,     0,   174,   104,
       0,    94,     0,     0,   240,     0,     0,   243,     0,   178,
      95,     0,     0,    96,   247,    97,     0,     0,   162,     0,
      98,    99,     0,   100,   101,   194,     0,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,     0,     0,   206,
     207,     0,   262,   264,   265,     0,   266,     0,     0,   216,
       0,    60,     0,     7,     0,     0,   222,   223,   273,   225,
       0,   274,   275,     0,   229,     0,    13,    14,    15,     0,
       0,     0,    19,   104,    20,    94,    21,     0,    61,     0,
       0,     0,     0,     0,    95,     0,     0,    96,     0,    97,
       0,     0,     0,    23,    98,    99,    24,   100,   101,     0,
       0,     0,     0,     0,     0,   236,    25,    26,    62,    28,
      29,     0,   241,     0,     0,     0,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,     0,
     248,     0,    89,     0,     0,   251,     0,     0,     0,   253,
      -3,     1,     0,     0,     2,     3,     4,     5,     6,     7,
       8,     9,     0,     0,   267,    10,    11,     0,     0,    12,
     271,     0,    13,    14,    15,     0,    17,    18,    19,     0,
      20,     0,    21,     0,    22,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    27,    28,    29,     1,     0,     0,
       2,     3,     4,     5,     6,     7,     8,     9,     0,     0,
       0,    10,    11,     0,     0,    12,   -68,   -68,    13,    14,
      15,     0,    17,    18,    75,   -68,    20,     0,    21,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,    26,
      27,    28,    29,     1,     0,     0,     2,     3,     4,     5,
       6,     7,     8,     9,     0,     0,     0,    10,    11,     0,
       0,    12,     0,     0,    13,    14,    15,     0,    17,    18,
      19,    83,    20,     0,    21,     0,    22,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    23,     0,     0,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,    26,   154,    28,    29,     1,
       0,     0,     2,     3,     4,     5,     6,     7,     8,     9,
       0,     0,     0,    10,    11,     0,     0,    12,     0,     0,
      13,    14,    15,     0,    17,    18,    19,   218,    20,     0,
      21,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,     0,     0,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,    26,    27,    28,    29,     1,     0,     0,     2,     3,
       4,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,     0,   263,    12,     0,     0,    13,    14,    15,     0,
      17,    18,    75,     0,    20,     0,    21,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,     0,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,    26,    27,    28,
      29,     1,     0,     0,     2,     3,     4,     5,     6,     7,
       8,     9,     0,     0,     0,    10,    11,     0,     0,    12,
       0,     0,    13,    14,    15,     0,    17,    18,    75,     0,
      20,     0,    21,     0,    22,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    27,    28,    29,     1,     0,     0,
       2,     3,     4,     5,     6,     7,     8,     9,     0,     0,
       0,    10,    11,     0,     0,    12,     0,     0,    13,    14,
      15,     0,    17,    18,    19,     0,    20,     0,    21,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,    24,    60,
       0,     7,     0,     0,     0,     0,     0,     0,    25,    26,
      27,    28,    29,     0,    13,    14,    15,     0,     0,     0,
      19,     0,    20,     0,    21,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    23,     0,     0,    24,    60,     0,     7,     0,     0,
       0,     0,     0,     0,    25,    26,    62,    28,    29,     0,
      13,    14,    15,    60,     0,     7,    19,     0,    20,   168,
      21,     0,     0,     0,     0,     0,     0,     0,    13,    14,
      15,     0,     0,     0,    19,     0,    20,    23,    21,   228,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,    26,    62,    28,    29,    23,     0,     0,    24,    60,
       0,     7,     0,     0,     0,     0,     0,     0,    25,    26,
      62,    28,    29,     0,    13,    14,    15,    60,     0,     7,
      19,     0,    20,   249,    21,     0,     0,     0,     0,     0,
       0,     0,    13,    14,    15,     0,     0,     0,    19,     0,
      20,    23,    21,     0,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,    26,    62,    28,    29,    23,
       0,     0,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    62,    28,    29
  };

  const short int
  parser::yycheck_[] =
  {
       0,    94,     0,    16,    25,   102,     0,    64,    31,    37,
      10,    11,    29,    29,     7,    30,    10,    11,    46,    30,
      32,    29,    31,    30,    36,    31,    31,    35,    30,    35,
      35,    31,    32,    31,    32,     0,     1,    31,    32,     4,
       5,     6,     7,     8,     9,    10,    11,    64,    64,    64,
      15,    16,    75,    64,    19,    64,    69,    22,    23,    24,
      25,    26,    27,    28,    39,    30,    30,    32,    48,    34,
      70,    51,    47,    20,    21,    75,    70,    75,    33,   176,
      35,    75,    29,    64,    49,    49,    30,    52,    52,    64,
      41,    11,    43,    30,    45,    38,    39,    62,    63,    64,
      65,    66,    38,    39,   125,   126,   127,   107,    64,   107,
      67,    68,    64,   107,    40,    20,    21,     0,     1,   123,
     124,     4,     5,     6,     7,     8,     9,    10,    11,     0,
      34,    34,    15,    16,    34,    34,    19,   150,    34,    22,
      23,    24,   235,    26,    27,    28,    64,    30,   148,    32,
      70,    34,    64,    39,   148,   155,    42,   155,    44,    13,
      39,   155,    27,    42,    50,    44,    49,    53,    17,    52,
      47,    50,    31,    64,    53,    31,     7,    35,     9,    62,
      63,    64,    65,    66,    58,    59,    60,    61,    31,     2,
       3,    22,    23,    24,    47,    30,    30,    28,    31,    30,
      47,    32,    54,    55,    56,    57,    31,    20,    21,   115,
     116,   117,   118,    33,   214,    30,    29,   217,    49,    65,
     214,    52,    39,   217,   224,    64,    14,    64,   148,    28,
     224,    62,    63,    64,    65,    66,    67,    68,   119,   120,
     121,   122,    31,    12,    31,    31,    47,    47,   114,   212,
     113,   164,   252,   253,   254,    -1,   256,    -1,   252,   253,
     254,    74,   256,   259,    -1,    -1,    79,    -1,   268,    82,
      -1,   271,   272,    -1,   268,    -1,    -1,   271,   272,    -1,
      -1,    94,    95,    96,    97,    -1,    99,    -1,   101,   102,
      -1,    30,    -1,    -1,   214,    -1,    -1,   217,    -1,   112,
      39,    -1,    -1,    42,   224,    44,    -1,    -1,    47,    -1,
      49,    50,    -1,    52,    53,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,    -1,   142,
     143,    -1,   252,   253,   254,    -1,   256,    -1,    -1,   152,
      -1,     7,    -1,     9,    -1,    -1,   159,   160,   268,   162,
      -1,   271,   272,    -1,   167,    -1,    22,    23,    24,    -1,
      -1,    -1,    28,   176,    30,    30,    32,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    42,    -1,    44,
      -1,    -1,    -1,    49,    49,    50,    52,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,   208,    62,    63,    64,    65,
      66,    -1,   215,    -1,    -1,    -1,   219,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,    -1,
     233,    -1,   235,    -1,    -1,   238,    -1,    -1,    -1,   242,
       0,     1,    -1,    -1,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,   257,    15,    16,    -1,    -1,    19,
     263,    -1,    22,    23,    24,    -1,    26,    27,    28,    -1,
      30,    -1,    32,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,     1,    -1,    -1,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    -1,    26,    27,    28,    29,    30,    -1,    32,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,     1,    -1,    -1,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    -1,
      -1,    19,    -1,    -1,    22,    23,    24,    -1,    26,    27,
      28,    29,    30,    -1,    32,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,     1,
      -1,    -1,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,
      22,    23,    24,    -1,    26,    27,    28,    29,    30,    -1,
      32,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,     1,    -1,    -1,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    -1,    18,    19,    -1,    -1,    22,    23,    24,    -1,
      26,    27,    28,    -1,    30,    -1,    32,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,
      66,     1,    -1,    -1,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    -1,    -1,    19,
      -1,    -1,    22,    23,    24,    -1,    26,    27,    28,    -1,
      30,    -1,    32,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,     1,    -1,    -1,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,    23,
      24,    -1,    26,    27,    28,    -1,    30,    -1,    32,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    52,     7,
      -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    -1,    22,    23,    24,    -1,    -1,    -1,
      28,    -1,    30,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    52,     7,    -1,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    -1,
      22,    23,    24,     7,    -1,     9,    28,    -1,    30,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    -1,    -1,    -1,    28,    -1,    30,    49,    32,    33,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    49,    -1,    -1,    52,     7,
      -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    -1,    22,    23,    24,     7,    -1,     9,
      28,    -1,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    24,    -1,    -1,    -1,    28,    -1,
      30,    49,    32,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     1,     4,     5,     6,     7,     8,     9,    10,    11,
      15,    16,    19,    22,    23,    24,    25,    26,    27,    28,
      30,    32,    34,    49,    52,    62,    63,    64,    65,    66,
      70,    71,    72,    73,    74,    75,    76,    77,    81,    82,
      83,    84,    85,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   105,   106,   108,
       7,    34,    64,    88,    97,    99,    88,    64,    30,    64,
      78,     7,    30,    64,    30,    28,    73,   104,   104,    30,
      78,    64,    30,    29,    64,   102,   103,    88,    33,    88,
     107,    64,    64,    95,    30,    39,    42,    44,    49,    50,
      52,    53,    67,    68,    88,   109,     0,    72,    73,    34,
      34,    34,    37,    46,    40,    58,    59,    60,    61,    54,
      55,    56,    57,    48,    51,    41,    43,    45,    32,    36,
      39,    42,    44,    50,    53,    39,    42,    44,    50,    53,
      34,    34,    38,    39,    31,    64,    79,    80,    78,   104,
      64,    64,    13,    88,    64,    72,    27,    17,    88,    38,
      39,    88,    47,    29,    35,    31,    33,    35,    31,   107,
      88,    88,    88,    88,    88,   109,    67,    68,    88,    90,
      91,    92,    92,    92,    92,    93,    93,    93,    93,    94,
      94,    95,    95,    95,    88,    64,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    39,    47,
      64,    31,    35,   104,    78,    47,    88,    31,    29,    30,
      30,    31,    88,    88,    31,    88,    29,   102,    33,    88,
      31,    35,   109,    47,    33,    30,    88,    65,    39,    79,
     104,    88,    14,   104,    88,    64,    28,   104,    88,    31,
     107,    88,    31,    88,    12,    31,    31,    20,    21,    86,
      87,    31,   104,    18,   104,   104,   104,    88,    47,    29,
      87,    88,    47,   104,   104,   104
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    69,    70,    70,    70,    71,    72,    72,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    75,    75,    75,
      75,    75,    75,    76,    76,    77,    78,    78,    79,    79,
      79,    80,    80,    80,    80,    80,    81,    81,    82,    82,
      82,    83,    83,    84,    85,    86,    86,    87,    87,    87,
      88,    88,    89,    89,    90,    90,    91,    91,    91,    91,
      91,    92,    92,    92,    92,    92,    93,    93,    93,    94,
      94,    94,    94,    95,    95,    96,    96,    96,    96,    97,
      98,    98,    99,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   101,   101,   101,   102,   103,   103,   104,   104,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     106,   106,   106,   107,   107,   108,   108,   109,   109,   109,
     109
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
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     2,     1,     1,     1,     1,     4,
       6,     5,     3,     3,     4,     2,     2,     2,     2,     3,
       1,     1,     2,     3,     4,     3,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     4,     1,     3,     1,     2,     2,     3,     1,
       2
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
  "\".\"", "\"?\"", "\"be\"", "\"=\"", "\"&&\"", "\"/\"", "\"/=\"",
  "\"*\"", "\"*=\"", "\"%\"", "\"||\"", "\":\"", "\"+\"", "\"++\"",
  "\"+=\"", "\"-\"", "\"--\"", "\"-=\"", "\"<=\"", "\"<\"", "\">=\"",
  "\">\"", "\"===\"", "\"==\"", "\"!==\"", "\"!=\"", "\"!\"", "NUMBER",
  "IDENT", "STRING", "FORMAT_STRING_START", "FORMAT_STRING_CONTINUE",
  "FORMAT_STRING_FINAL", "$accept", "start", "filehead", "statements",
  "statement", "assignment", "vardecl", "funcdecl", "function", "funchead",
  "arglist", "argitem", "if", "for", "while", "trycatch", "switch",
  "caselist", "case", "exp01", "exp02", "exp03", "exp04", "exp05", "exp06",
  "exp07", "exp08", "exp09", "arrget", "call", "objget", "expp", "obj",
  "obj_item", "obj_items", "codeblock", "value", "array", "explist",
  "format_string", "format_string_match", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   250,   250,   251,   252,   255,   258,   259,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   295,   296,   297,
     298,   299,   300,   303,   304,   307,   310,   311,   314,   315,
     316,   318,   319,   320,   321,   322,   325,   326,   329,   330,
     331,   334,   335,   338,   341,   344,   345,   348,   349,   350,
     352,   353,   355,   356,   358,   359,   361,   362,   363,   364,
     365,   367,   368,   369,   370,   371,   373,   374,   375,   377,
     378,   379,   380,   382,   383,   385,   386,   387,   388,   390,
     392,   393,   395,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   407,   408,   409,   411,   413,   414,   416,   418,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     431,   432,   433,   435,   436,   438,   439,   441,   442,   443,
     444
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
#line 2309 "parser.tab.cc" // lalr1.cc:1167
#line 446 "parser.y" // lalr1.cc:1168

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
         case tokenizer::etoken::s_colon: return parser::make_COLON(token, loc);
         case tokenizer::etoken::s_semicolon: return parser::make_SEMICOLON(loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(loc);
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
