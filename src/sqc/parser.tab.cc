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
#line 132 "parser.y" // lalr1.cc:413

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
      case 70: // filehead
      case 71: // statements
      case 72: // statement
      case 73: // assignment
      case 74: // vardecl
      case 75: // funcdecl
      case 76: // function
      case 77: // funchead
      case 78: // arglist
      case 79: // argitem
      case 80: // codeblock
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
      case 98: // expp
      case 99: // obj
      case 100: // obj_item
      case 101: // obj_items
      case 102: // value
      case 103: // array
      case 104: // explist
      case 105: // format_string
      case 106: // format_string_match
        value.move< sqf::sqc::bison::astnode > (that.value);
        break;

      case 37: // "be"
      case 38: // "="
      case 39: // "&&"
      case 40: // "/"
      case 41: // "/="
      case 42: // "*"
      case 43: // "*="
      case 44: // "%"
      case 45: // "||"
      case 46: // ":"
      case 47: // "+"
      case 48: // "++"
      case 49: // "+="
      case 50: // "-"
      case 51: // "--"
      case 52: // "-="
      case 53: // "<="
      case 54: // "<"
      case 55: // ">="
      case 56: // ">"
      case 57: // "==="
      case 58: // "=="
      case 59: // "!=="
      case 60: // "!="
      case 61: // "!"
      case 62: // NUMBER
      case 63: // IDENT
      case 64: // STRING
      case 65: // FORMAT_STRING_START
      case 66: // FORMAT_STRING_CONTINUE
      case 67: // FORMAT_STRING_FINAL
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
      case 70: // filehead
      case 71: // statements
      case 72: // statement
      case 73: // assignment
      case 74: // vardecl
      case 75: // funcdecl
      case 76: // function
      case 77: // funchead
      case 78: // arglist
      case 79: // argitem
      case 80: // codeblock
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
      case 98: // expp
      case 99: // obj
      case 100: // obj_item
      case 101: // obj_items
      case 102: // value
      case 103: // array
      case 104: // explist
      case 105: // format_string
      case 106: // format_string_match
        value.copy< sqf::sqc::bison::astnode > (that.value);
        break;

      case 37: // "be"
      case 38: // "="
      case 39: // "&&"
      case 40: // "/"
      case 41: // "/="
      case 42: // "*"
      case 43: // "*="
      case 44: // "%"
      case 45: // "||"
      case 46: // ":"
      case 47: // "+"
      case 48: // "++"
      case 49: // "+="
      case 50: // "-"
      case 51: // "--"
      case 52: // "-="
      case 53: // "<="
      case 54: // "<"
      case 55: // ">="
      case 56: // ">"
      case 57: // "==="
      case 58: // "=="
      case 59: // "!=="
      case 60: // "!="
      case 61: // "!"
      case 62: // NUMBER
      case 63: // IDENT
      case 64: // STRING
      case 65: // FORMAT_STRING_START
      case 66: // FORMAT_STRING_CONTINUE
      case 67: // FORMAT_STRING_FINAL
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
      case 70: // filehead
      case 71: // statements
      case 72: // statement
      case 73: // assignment
      case 74: // vardecl
      case 75: // funcdecl
      case 76: // function
      case 77: // funchead
      case 78: // arglist
      case 79: // argitem
      case 80: // codeblock
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
      case 98: // expp
      case 99: // obj
      case 100: // obj_item
      case 101: // obj_items
      case 102: // value
      case 103: // array
      case 104: // explist
      case 105: // format_string
      case 106: // format_string_match
        yylhs.value.build< sqf::sqc::bison::astnode > ();
        break;

      case 37: // "be"
      case 38: // "="
      case 39: // "&&"
      case 40: // "/"
      case 41: // "/="
      case 42: // "*"
      case 43: // "*="
      case 44: // "%"
      case 45: // "||"
      case 46: // ":"
      case 47: // "+"
      case 48: // "++"
      case 49: // "+="
      case 50: // "-"
      case 51: // "--"
      case 52: // "-="
      case 53: // "<="
      case 54: // "<"
      case 55: // ">="
      case 56: // ">"
      case 57: // "==="
      case 58: // "=="
      case 59: // "!=="
      case 60: // "!="
      case 61: // "!"
      case 62: // NUMBER
      case 63: // IDENT
      case 64: // STRING
      case 65: // FORMAT_STRING_START
      case 66: // FORMAT_STRING_CONTINUE
      case 67: // FORMAT_STRING_FINAL
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
#line 236 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); result.append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 755 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 237 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 761 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 240 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 767 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 243 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::STATEMENTS, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 773 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 244 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 779 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 247 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 785 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 248 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; }
#line 791 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 249 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::THROW, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 797 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 250 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 803 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 251 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 809 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 252 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 815 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 253 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 821 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 254 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 827 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 255 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 833 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 256 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 839 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 257 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 845 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 258 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 851 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 259 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 857 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 260 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 863 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 263 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 869 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 264 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 875 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 265 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 881 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 266 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 887 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 267 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_STAR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 893 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 268 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT_SLASH, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 899 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 269 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 905 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 270 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 911 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 271 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 917 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 272 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_STAR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 923 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 273 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET_SLASH, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 929 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 276 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 935 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 277 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 941 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 278 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 947 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 279 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 953 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 280 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 959 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 281 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 965 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 284 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 971 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 285 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FINAL_FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 977 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 288 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 983 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 291 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; }
#line 989 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 292 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 995 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 295 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1001 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 296 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1007 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 297 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1013 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 299 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM, yystack_[0].value.as< tokenizer::token > () }; }
#line 1019 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 300 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_DEFAULT, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1025 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 301 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_TYPE, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 1031 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 302 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_TYPE_DEFAULT, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1037 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 303 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_EMPLACE, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 1043 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 305 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1049 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 306 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; }
#line 1055 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 307 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1061 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 310 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IF, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1067 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 311 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IFELSE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1073 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 314 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOR, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1079 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 315 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORSTEP, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[7].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1085 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 316 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOREACH, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1091 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 319 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::WHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1097 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 320 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DOWHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1103 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 323 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::TRYCATCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1109 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 326 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SWITCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1115 "parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 329 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1121 "parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 330 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1127 "parser.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 333 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1133 "parser.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 334 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1139 "parser.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 335 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE_DEFAULT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1145 "parser.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 338 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1151 "parser.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 339 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_TERNARY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1157 "parser.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 341 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1163 "parser.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 342 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_OR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1169 "parser.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 344 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1175 "parser.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 345 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_AND, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1181 "parser.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 347 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1187 "parser.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 348 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1193 "parser.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 349 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1199 "parser.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 350 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1205 "parser.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 351 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1211 "parser.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 353 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1217 "parser.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 354 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1223 "parser.tab.cc" // lalr1.cc:859
    break;

  case 82:
#line 355 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1229 "parser.tab.cc" // lalr1.cc:859
    break;

  case 83:
#line 356 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1235 "parser.tab.cc" // lalr1.cc:859
    break;

  case 84:
#line 357 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1241 "parser.tab.cc" // lalr1.cc:859
    break;

  case 85:
#line 359 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1247 "parser.tab.cc" // lalr1.cc:859
    break;

  case 86:
#line 360 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1253 "parser.tab.cc" // lalr1.cc:859
    break;

  case 87:
#line 361 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1259 "parser.tab.cc" // lalr1.cc:859
    break;

  case 88:
#line 363 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1265 "parser.tab.cc" // lalr1.cc:859
    break;

  case 89:
#line 364 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MULTIPLY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1271 "parser.tab.cc" // lalr1.cc:859
    break;

  case 90:
#line 365 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_DIVIDE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1277 "parser.tab.cc" // lalr1.cc:859
    break;

  case 91:
#line 366 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_REMAINDER, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1283 "parser.tab.cc" // lalr1.cc:859
    break;

  case 92:
#line 368 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1289 "parser.tab.cc" // lalr1.cc:859
    break;

  case 93:
#line 369 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ());  }
#line 1295 "parser.tab.cc" // lalr1.cc:859
    break;

  case 94:
#line 371 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1301 "parser.tab.cc" // lalr1.cc:859
    break;

  case 95:
#line 372 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_CALL, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1307 "parser.tab.cc" // lalr1.cc:859
    break;

  case 96:
#line 373 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ACCESS, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 1313 "parser.tab.cc" // lalr1.cc:859
    break;

  case 97:
#line 374 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1319 "parser.tab.cc" // lalr1.cc:859
    break;

  case 98:
#line 376 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_GET, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1325 "parser.tab.cc" // lalr1.cc:859
    break;

  case 99:
#line 378 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 1331 "parser.tab.cc" // lalr1.cc:859
    break;

  case 100:
#line 379 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_UNARY, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); }
#line 1337 "parser.tab.cc" // lalr1.cc:859
    break;

  case 101:
#line 380 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_UNARY, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1343 "parser.tab.cc" // lalr1.cc:859
    break;

  case 102:
#line 381 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }; }
#line 1349 "parser.tab.cc" // lalr1.cc:859
    break;

  case 103:
#line 382 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::INC_PRE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }); }
#line 1355 "parser.tab.cc" // lalr1.cc:859
    break;

  case 104:
#line 383 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DEC_PRE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }); }
#line 1361 "parser.tab.cc" // lalr1.cc:859
    break;

  case 105:
#line 384 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::INC_POST, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[1].value.as< tokenizer::token > () }); }
#line 1367 "parser.tab.cc" // lalr1.cc:859
    break;

  case 106:
#line 385 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DEC_POST, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[1].value.as< tokenizer::token > () }); }
#line 1373 "parser.tab.cc" // lalr1.cc:859
    break;

  case 107:
#line 386 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1379 "parser.tab.cc" // lalr1.cc:859
    break;

  case 108:
#line 388 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT, tokenizer.create_token() }; }
#line 1385 "parser.tab.cc" // lalr1.cc:859
    break;

  case 109:
#line 389 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1391 "parser.tab.cc" // lalr1.cc:859
    break;

  case 110:
#line 390 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); }
#line 1397 "parser.tab.cc" // lalr1.cc:859
    break;

  case 111:
#line 392 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT_ITEM, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1403 "parser.tab.cc" // lalr1.cc:859
    break;

  case 112:
#line 394 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OBJECT_ITEMS, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1409 "parser.tab.cc" // lalr1.cc:859
    break;

  case 113:
#line 395 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[2].value.as< sqf::sqc::bison::astnode > (); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1415 "parser.tab.cc" // lalr1.cc:859
    break;

  case 114:
#line 397 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1421 "parser.tab.cc" // lalr1.cc:859
    break;

  case 115:
#line 398 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 1427 "parser.tab.cc" // lalr1.cc:859
    break;

  case 116:
#line 399 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1433 "parser.tab.cc" // lalr1.cc:859
    break;

  case 117:
#line 400 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1439 "parser.tab.cc" // lalr1.cc:859
    break;

  case 118:
#line 401 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 1445 "parser.tab.cc" // lalr1.cc:859
    break;

  case 119:
#line 402 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_TRUE, tokenizer.create_token() }; }
#line 1451 "parser.tab.cc" // lalr1.cc:859
    break;

  case 120:
#line 403 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_FALSE, tokenizer.create_token() }; }
#line 1457 "parser.tab.cc" // lalr1.cc:859
    break;

  case 121:
#line 404 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() }; }
#line 1463 "parser.tab.cc" // lalr1.cc:859
    break;

  case 122:
#line 406 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; }
#line 1469 "parser.tab.cc" // lalr1.cc:859
    break;

  case 123:
#line 407 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1475 "parser.tab.cc" // lalr1.cc:859
    break;

  case 124:
#line 409 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1481 "parser.tab.cc" // lalr1.cc:859
    break;

  case 125:
#line 410 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1487 "parser.tab.cc" // lalr1.cc:859
    break;

  case 126:
#line 411 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1493 "parser.tab.cc" // lalr1.cc:859
    break;

  case 127:
#line 413 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SVAL_FORMAT_STRING }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ());}
#line 1499 "parser.tab.cc" // lalr1.cc:859
    break;

  case 128:
#line 414 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SVAL_FORMAT_STRING }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1505 "parser.tab.cc" // lalr1.cc:859
    break;

  case 129:
#line 416 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() }); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1511 "parser.tab.cc" // lalr1.cc:859
    break;

  case 130:
#line 417 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1517 "parser.tab.cc" // lalr1.cc:859
    break;

  case 131:
#line 418 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() });yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ());  }
#line 1523 "parser.tab.cc" // lalr1.cc:859
    break;

  case 132:
#line 419 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 1529 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1533 "parser.tab.cc" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -91;

  const signed char parser::yytable_ninf_ = -68;

  const short int
  parser::yypact_[] =
  {
     272,   -91,   150,   862,     7,   -21,    44,   -91,   -15,    49,
     599,   599,    58,   -91,   -91,    63,    10,    65,   862,   799,
     -91,    37,    42,   862,   -91,   198,   -91,    53,   123,   794,
     338,   -91,    96,    97,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   106,   -18,   103,    78,    57,     6,    39,   -91,    19,
      14,   -91,   -91,   -91,   -91,    63,   -91,   -12,   110,   -91,
     111,    21,   -17,    63,   599,    83,    84,   142,   862,   664,
     -91,   130,   141,   862,   -91,    30,   862,   131,   -91,   126,
      -6,   -91,   -91,   -91,   844,   817,   862,   862,   -91,   862,
     -91,   862,    53,   -91,   -46,   -91,   -91,   404,   -91,   -91,
     -91,   -91,   862,   862,   862,   862,   862,   862,   862,   862,
     862,   862,   862,   862,   862,   862,   862,   862,   862,   100,
     862,   862,   862,   862,   862,   -91,   -91,   862,   862,   -91,
     -22,   136,   140,   599,   -91,    63,   129,   862,   146,   -91,
     729,   151,   153,   154,   862,   862,   157,   -91,   -91,   -91,
     862,   -91,    31,   -19,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,    53,   -91,   139,   103,    78,    57,    57,    57,    57,
       6,     6,     6,     6,    39,    39,   -91,   -91,   -91,   156,
     160,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   862,   127,
     152,   -91,   132,   -91,   599,   862,   179,   599,   -91,   862,
     133,   173,   -91,   -91,   599,   -91,   -91,   -91,   159,   -91,
      -9,   -91,   862,   -91,   862,   -91,   -91,   862,   -91,   -91,
     172,   862,   194,   177,   178,    17,   -91,   155,   -91,   -16,
     -91,    69,   -91,   599,   534,   599,   -91,   599,   862,   163,
      12,   -91,   -91,   -91,   -91,   -91,   -91,   862,   -91,   -91,
     -91,   164,   599,   -91,   -91,   599,   469,   -91,   -91,   -91
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,    21,     0,     0,     0,     0,     0,   120,     0,     0,
       0,     0,     0,   121,   119,     0,     0,     0,     0,     0,
      20,     0,     0,     0,   118,   102,   115,   127,     0,     0,
       0,     6,     0,     0,    12,   114,    13,    14,    15,    16,
      17,     0,    69,    71,    73,    75,    80,    85,    88,    92,
      97,    94,   107,   116,   117,     0,     9,   102,     0,    97,
       0,    35,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     5,    38,     0,     0,   122,   124,
       0,   103,   104,    93,     0,     0,     0,     0,   105,     0,
     106,     0,     0,   131,     0,   128,     1,     0,     7,    18,
      11,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     8,    10,     0,     0,    42,
      47,     0,    44,     0,    41,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,    99,   125,   123,
       0,   100,     0,     0,    22,    23,    27,    26,    24,    25,
     129,     0,   132,     0,    72,    74,    76,    78,    77,    79,
      82,    81,    84,    83,    86,    87,    90,    89,    91,     0,
      96,    28,    32,    31,    29,    30,    34,    33,     0,     0,
      49,    43,    45,    39,     0,     0,     0,     0,    54,     0,
       0,     0,    37,    36,     0,   126,   101,   108,     0,   112,
       0,   130,     0,    98,     0,    48,    51,     0,    46,    40,
       0,     0,    55,     0,     0,     0,    60,     0,   109,     0,
      70,     0,    50,     0,     0,     0,    61,     0,     0,     0,
       0,    64,   111,   110,   113,    95,    59,     0,    57,    56,
      62,     0,     0,    63,    65,     0,     0,    68,    58,    66
  };

  const signed char
  parser::yypgoto_[] =
  {
     -91,   -91,   -91,   -26,     5,   -91,   -91,   -91,   -91,   -14,
      32,   -91,    34,   -91,   -91,   -91,   -91,   -91,   -91,   -11,
       4,   -91,   119,   122,    45,    59,   -28,    -8,   -91,     0,
     -91,   -91,     1,   -91,    -4,   -91,   -80,   -91,   -90
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    28,    29,    30,    70,    32,    33,    34,    35,    64,
     131,   132,    71,    36,    37,    38,    39,    40,   240,   241,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    59,
      51,   155,   209,   210,    52,    53,    80,    54,    95
  };

  const short int
  parser::yytable_[] =
  {
      50,    74,   160,    97,   152,    31,    58,    60,    62,   207,
      50,    50,   243,   129,    66,    83,   188,    84,   102,   228,
     161,   162,    77,    79,   189,   229,   149,   103,   150,    50,
      50,    94,   238,   239,    31,    98,    88,   238,   239,    90,
     253,   190,    63,   140,   208,    72,   130,   208,    67,   133,
     118,    65,   120,   113,   119,   121,   114,   122,   127,   128,
      55,   206,     7,   123,    50,   150,   124,   144,   145,    50,
      61,   211,   138,    75,    31,    13,    14,   143,    68,   115,
     146,   116,    18,   117,    19,   174,   175,    73,    79,   154,
     156,   157,    62,   158,    76,   159,    94,    50,   134,   245,
      81,    21,    98,   150,    22,    82,   163,   176,   177,   178,
     109,   110,   111,   112,    23,    24,    57,    26,    27,    92,
      93,   194,   179,    96,   181,   182,   183,   184,   185,    99,
     100,   186,   187,    50,   231,   105,   106,   107,   108,   101,
      50,   196,   104,   125,   126,    98,   135,   136,   202,   203,
     166,   167,   168,   169,   205,   137,   141,    55,   142,     7,
     148,   147,    55,   180,     7,    94,   191,   193,   170,   171,
     172,   173,    13,    14,   192,   195,   197,    13,    14,    18,
     199,    19,   200,    56,   201,   212,    19,   204,   213,   214,
     217,   216,   215,   221,    50,   130,   224,    50,    21,   220,
     225,    22,   233,   223,    50,   227,   235,   236,   237,   252,
     256,    23,    24,    57,    26,    27,   230,    24,    79,    26,
      27,   232,   164,   242,   218,   234,   165,    84,   219,   254,
     244,   222,     0,    50,    50,    50,    85,    50,   226,    86,
       0,    87,   251,     0,     0,     0,    88,    89,     0,    90,
      91,   255,    50,     0,     0,    50,    50,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,   248,   249,
       0,   250,    -2,     1,     0,     0,     2,     3,     4,     5,
       6,     7,     8,     9,     0,     0,   257,    10,    11,   258,
     259,    12,     0,     0,    13,    14,    15,    16,    17,     0,
       0,    18,     0,    19,     0,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,    24,    25,    26,    27,    -4,     1,
       0,     0,     2,     3,     4,     5,     6,     7,     8,     9,
       0,     0,     0,    10,    11,     0,     0,    12,     0,     0,
      13,    14,     0,    16,    17,     0,     0,    18,     0,    19,
       0,    20,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,    27,    -3,     1,     0,     0,     2,     3,
       4,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,     0,     0,    12,     0,     0,    13,    14,     0,    16,
      17,     0,     0,    18,     0,    19,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    24,    25,    26,    27,
       1,     0,     0,     2,     3,     4,     5,     6,     7,     8,
       9,     0,     0,     0,    10,    11,     0,     0,    12,   -67,
     -67,    13,    14,     0,    16,    17,    69,   -67,    18,     0,
      19,     0,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,    24,    25,    26,    27,     1,     0,     0,     2,     3,
       4,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,     0,   247,    12,     0,     0,    13,    14,     0,    16,
      17,    69,     0,    18,     0,    19,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    24,    25,    26,    27,
       1,     0,     0,     2,     3,     4,     5,     6,     7,     8,
       9,     0,     0,     0,    10,    11,     0,     0,    12,     0,
       0,    13,    14,     0,    16,    17,    69,     0,    18,     0,
      19,     0,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,    24,    25,    26,    27,     1,     0,     0,     2,     3,
       4,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,     0,     0,    12,     0,     0,    13,    14,     0,    16,
      17,     0,   139,    18,     0,    19,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    24,    25,    26,    27,
       1,     0,     0,     2,     3,     4,     5,     6,     7,     8,
       9,     0,     0,     0,    10,    11,     0,     0,    12,     0,
       0,    13,    14,     0,    16,    17,     0,   198,    18,     0,
      19,     0,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,    24,    25,    26,    27,     1,     0,     0,     2,     3,
       4,     5,     6,     7,     8,     9,    55,     0,     7,    10,
      11,     0,     0,    12,     0,     0,    13,    14,     0,    16,
      17,    13,    14,    18,    55,    19,     7,    20,    18,     0,
      19,    78,     0,     0,     0,     0,     0,     0,     0,    13,
      14,     0,    21,     0,   153,    22,    18,    21,    19,     0,
      22,    55,     0,     7,     0,    23,    24,    25,    26,    27,
      23,    24,    57,    26,    27,    21,    13,    14,    22,    55,
       0,     7,     0,    18,   151,    19,     0,     0,    23,    24,
      57,    26,    27,     0,    13,    14,     0,     0,     0,     0,
       0,    18,    21,    19,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    24,    57,    26,    27,
      21,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,    24,    57,    26,    27
  };

  const short int
  parser::yycheck_[] =
  {
       0,    15,    92,    29,    84,     0,     2,     3,    29,    28,
      10,    11,    28,    30,    29,    23,    38,    29,    36,    28,
      66,    67,    18,    19,    46,    34,    32,    45,    34,    29,
      30,    27,    20,    21,    29,    30,    48,    20,    21,    51,
      28,    63,    63,    69,    63,    11,    63,    63,    63,    63,
      31,     7,    38,    47,    35,    41,    50,    43,    37,    38,
       7,    30,     9,    49,    64,    34,    52,    37,    38,    69,
      63,   161,    68,    63,    69,    22,    23,    73,    29,    40,
      76,    42,    29,    44,    31,   113,   114,    29,    84,    85,
      86,    87,    29,    89,    29,    91,    92,    97,    64,    30,
      63,    48,    97,    34,    51,    63,   102,   115,   116,   117,
      53,    54,    55,    56,    61,    62,    63,    64,    65,    66,
      67,   135,   118,     0,   120,   121,   122,   123,   124,    33,
      33,   127,   128,   133,   214,    57,    58,    59,    60,    33,
     140,   137,    39,    33,    33,   140,    63,    63,   144,   145,
     105,   106,   107,   108,   150,    13,    26,     7,    17,     9,
      34,    30,     7,    63,     9,   161,    30,   133,   109,   110,
     111,   112,    22,    23,    34,    46,    30,    22,    23,    29,
      29,    31,    29,    33,    30,    46,    31,    30,    32,    29,
      38,    64,   188,    14,   194,    63,    63,   197,    48,   195,
      27,    51,    30,   199,   204,    46,    12,    30,    30,    46,
      46,    61,    62,    63,    64,    65,   212,    62,   214,    64,
      65,   217,   103,   227,   192,   221,   104,    29,   194,   240,
     229,   197,    -1,   233,   234,   235,    38,   237,   204,    41,
      -1,    43,   238,    -1,    -1,    -1,    48,    49,    -1,    51,
      52,   247,   252,    -1,    -1,   255,   256,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   233,   234,   235,
      -1,   237,     0,     1,    -1,    -1,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,   252,    15,    16,   255,
     256,    19,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    29,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    65,     0,     1,
      -1,    -1,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,
      22,    23,    -1,    25,    26,    -1,    -1,    29,    -1,    31,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    65,     0,     1,    -1,    -1,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    -1,    -1,    19,    -1,    -1,    22,    23,    -1,    25,
      26,    -1,    -1,    29,    -1,    31,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,
       1,    -1,    -1,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    -1,    25,    26,    27,    28,    29,    -1,
      31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    65,     1,    -1,    -1,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    -1,    18,    19,    -1,    -1,    22,    23,    -1,    25,
      26,    27,    -1,    29,    -1,    31,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,
       1,    -1,    -1,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,
      -1,    22,    23,    -1,    25,    26,    27,    -1,    29,    -1,
      31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    65,     1,    -1,    -1,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    -1,    -1,    19,    -1,    -1,    22,    23,    -1,    25,
      26,    -1,    28,    29,    -1,    31,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,
       1,    -1,    -1,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,
      -1,    22,    23,    -1,    25,    26,    -1,    28,    29,    -1,
      31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    65,     1,    -1,    -1,     4,     5,
       6,     7,     8,     9,    10,    11,     7,    -1,     9,    15,
      16,    -1,    -1,    19,    -1,    -1,    22,    23,    -1,    25,
      26,    22,    23,    29,     7,    31,     9,    33,    29,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      23,    -1,    48,    -1,    27,    51,    29,    48,    31,    -1,
      51,     7,    -1,     9,    -1,    61,    62,    63,    64,    65,
      61,    62,    63,    64,    65,    48,    22,    23,    51,     7,
      -1,     9,    -1,    29,    30,    31,    -1,    -1,    61,    62,
      63,    64,    65,    -1,    22,    23,    -1,    -1,    -1,    -1,
      -1,    29,    48,    31,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,
      48,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    65
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     1,     4,     5,     6,     7,     8,     9,    10,    11,
      15,    16,    19,    22,    23,    24,    25,    26,    29,    31,
      33,    48,    51,    61,    62,    63,    64,    65,    69,    70,
      71,    72,    73,    74,    75,    76,    81,    82,    83,    84,
      85,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,   102,   103,   105,     7,    33,    63,    88,    97,
      88,    63,    29,    63,    77,     7,    29,    63,    29,    27,
      72,    80,    80,    29,    77,    63,    29,    88,    32,    88,
     104,    63,    63,    95,    29,    38,    41,    43,    48,    49,
      51,    52,    66,    67,    88,   106,     0,    71,    72,    33,
      33,    33,    36,    45,    39,    57,    58,    59,    60,    53,
      54,    55,    56,    47,    50,    40,    42,    44,    31,    35,
      38,    41,    43,    49,    52,    33,    33,    37,    38,    30,
      63,    78,    79,    77,    80,    63,    63,    13,    88,    28,
      71,    26,    17,    88,    37,    38,    88,    30,    34,    32,
      34,    30,   104,    27,    88,    99,    88,    88,    88,    88,
     106,    66,    67,    88,    90,    91,    92,    92,    92,    92,
      93,    93,    93,    93,    94,    94,    95,    95,    95,    88,
      63,    88,    88,    88,    88,    88,    88,    88,    38,    46,
      63,    30,    34,    80,    77,    46,    88,    30,    28,    29,
      29,    30,    88,    88,    30,    88,    30,    28,    63,   100,
     101,   106,    46,    32,    29,    88,    64,    38,    78,    80,
      88,    14,    80,    88,    63,    27,    80,    46,    28,    34,
      88,   104,    88,    30,    88,    12,    30,    30,    20,    21,
      86,    87,   102,    28,   100,    30,    80,    18,    80,    80,
      80,    88,    46,    28,    87,    88,    46,    80,    80,    80
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    68,    69,    69,    69,    70,    71,    71,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    74,    74,    74,    74,    74,    74,    75,
      75,    76,    77,    77,    78,    78,    78,    79,    79,    79,
      79,    79,    80,    80,    80,    81,    81,    82,    82,    82,
      83,    83,    84,    85,    86,    86,    87,    87,    87,    88,
      88,    89,    89,    90,    90,    91,    91,    91,    91,    91,
      92,    92,    92,    92,    92,    93,    93,    93,    94,    94,
      94,    94,    95,    95,    96,    96,    96,    96,    97,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    99,    99,
      99,   100,   101,   101,   102,   102,   102,   102,   102,   102,
     102,   102,   103,   103,   104,   104,   104,   105,   105,   106,
     106,   106,   106
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     2,     1,     2,     1,     2,     3,     2,
       3,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     2,     4,     4,     2,     4,
       5,     3,     2,     3,     1,     2,     3,     1,     3,     2,
       4,     3,     1,     2,     3,     5,     7,     7,     9,     7,
       5,     6,     7,     7,     1,     2,     4,     3,     3,     1,
       5,     1,     3,     1,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     2,     1,     6,     3,     1,     4,     3,
       3,     4,     1,     2,     2,     2,     2,     1,     2,     3,
       4,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     1,     2,     3,     1,     2,     2,
       3,     1,     2
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
  "\"true\"", "\"params\"", "\"private\"", "\"while\"", "\"{\"", "\"}\"",
  "\"(\"", "\")\"", "\"[\"", "\"]\"", "\";\"", "\",\"", "\".\"", "\"?\"",
  "\"be\"", "\"=\"", "\"&&\"", "\"/\"", "\"/=\"", "\"*\"", "\"*=\"",
  "\"%\"", "\"||\"", "\":\"", "\"+\"", "\"++\"", "\"+=\"", "\"-\"",
  "\"--\"", "\"-=\"", "\"<=\"", "\"<\"", "\">=\"", "\">\"", "\"===\"",
  "\"==\"", "\"!==\"", "\"!=\"", "\"!\"", "NUMBER", "IDENT", "STRING",
  "FORMAT_STRING_START", "FORMAT_STRING_CONTINUE", "FORMAT_STRING_FINAL",
  "$accept", "start", "filehead", "statements", "statement", "assignment",
  "vardecl", "funcdecl", "function", "funchead", "arglist", "argitem",
  "codeblock", "if", "for", "while", "trycatch", "switch", "caselist",
  "case", "exp01", "exp02", "exp03", "exp04", "exp05", "exp06", "exp07",
  "exp08", "exp09", "arrget", "expp", "obj", "obj_item", "obj_items",
  "value", "array", "explist", "format_string", "format_string_match", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   235,   235,   236,   237,   240,   243,   244,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   276,   277,   278,   279,   280,   281,   284,
     285,   288,   291,   292,   295,   296,   297,   299,   300,   301,
     302,   303,   305,   306,   307,   310,   311,   314,   315,   316,
     319,   320,   323,   326,   329,   330,   333,   334,   335,   338,
     339,   341,   342,   344,   345,   347,   348,   349,   350,   351,
     353,   354,   355,   356,   357,   359,   360,   361,   363,   364,
     365,   366,   368,   369,   371,   372,   373,   374,   376,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   388,   389,
     390,   392,   394,   395,   397,   398,   399,   400,   401,   402,
     403,   404,   406,   407,   409,   410,   411,   413,   414,   416,
     417,   418,   419
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
#line 2215 "parser.tab.cc" // lalr1.cc:1167
#line 421 "parser.y" // lalr1.cc:1168


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
