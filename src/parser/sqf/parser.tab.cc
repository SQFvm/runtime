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

    #ifdef __GNUG__
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wall"
    #endif
    #ifdef _MSC_VER
        #pragma warning(push, 0)
    #endif
    #include "tokenizer.hpp"
    #include <string>
    #include <vector>
    #include <algorithm>

#line 48 "parser.tab.cc" // lalr1.cc:397


// First part of user declarations.

#line 53 "parser.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.tab.hh"

// User implementation prologue.

#line 67 "parser.tab.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 109 "parser.y" // lalr1.cc:413

    
    #include "../../runtime/runtime.h"
     namespace sqf::parser::sqf::bison
     {
          // Return the next token.
          parser::symbol_type yylex (::sqf::runtime::runtime &runtime, ::sqf::parser::sqf::tokenizer&);
     }

#line 79 "parser.tab.cc" // lalr1.cc:413


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
#line 165 "parser.tab.cc" // lalr1.cc:479

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
  parser::parser (::sqf::parser::sqf::tokenizer &tokenizer_yyarg, ::sqf::parser::sqf::bison::astnode& result_yyarg, ::sqf::parser::sqf::parser& actual_yyarg, ::sqf::runtime::runtime &runtime_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      tokenizer (tokenizer_yyarg),
      result (result_yyarg),
      actual (actual_yyarg),
      runtime (runtime_yyarg)
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
      case 65: // statements
      case 66: // statement
      case 69: // value
      case 70: // exp_list
      case 71: // code
      case 72: // array
      case 73: // assignment
      case 74: // expression
      case 75: // exp0
      case 76: // exp1
      case 77: // exp2
      case 78: // exp3
      case 79: // exp4
      case 80: // exp5
      case 81: // exp6
      case 82: // exp7
      case 83: // exp8
      case 84: // exp9
      case 85: // expu
        value.move< ::sqf::parser::sqf::bison::astnode > (that.value);
        break;

      case 4: // "false"
      case 5: // "true"
      case 6: // "private"
      case 7: // "{"
      case 8: // "}"
      case 9: // "("
      case 10: // ")"
      case 11: // "["
      case 12: // "]"
      case 13: // ";"
      case 14: // ","
      case 15: // "="
      case 16: // OPERATOR_B_0
      case 17: // OPERATOR_B_1
      case 18: // OPERATOR_B_2
      case 19: // OPERATOR_B_3
      case 20: // OPERATOR_B_4
      case 21: // OPERATOR_B_5
      case 22: // OPERATOR_B_6
      case 23: // OPERATOR_B_7
      case 24: // OPERATOR_B_8
      case 25: // OPERATOR_B_9
      case 26: // OPERATOR_BU_0
      case 27: // OPERATOR_BU_1
      case 28: // OPERATOR_BU_2
      case 29: // OPERATOR_BU_3
      case 30: // OPERATOR_BU_4
      case 31: // OPERATOR_BU_5
      case 32: // OPERATOR_BU_6
      case 33: // OPERATOR_BU_7
      case 34: // OPERATOR_BU_8
      case 35: // OPERATOR_BU_9
      case 36: // OPERATOR_BN_0
      case 37: // OPERATOR_BN_1
      case 38: // OPERATOR_BN_2
      case 39: // OPERATOR_BN_3
      case 40: // OPERATOR_BN_4
      case 41: // OPERATOR_BN_5
      case 42: // OPERATOR_BN_6
      case 43: // OPERATOR_BN_7
      case 44: // OPERATOR_BN_8
      case 45: // OPERATOR_BN_9
      case 46: // OPERATOR_BUN_0
      case 47: // OPERATOR_BUN_1
      case 48: // OPERATOR_BUN_2
      case 49: // OPERATOR_BUN_3
      case 50: // OPERATOR_BUN_4
      case 51: // OPERATOR_BUN_5
      case 52: // OPERATOR_BUN_6
      case 53: // OPERATOR_BUN_7
      case 54: // OPERATOR_BUN_8
      case 55: // OPERATOR_BUN_9
      case 56: // OPERATOR_U
      case 57: // OPERATOR_N
      case 58: // OPERATOR_UN
      case 59: // IDENT
      case 60: // NUMBER
      case 61: // HEXNUMBER
      case 62: // STRING
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
      case 65: // statements
      case 66: // statement
      case 69: // value
      case 70: // exp_list
      case 71: // code
      case 72: // array
      case 73: // assignment
      case 74: // expression
      case 75: // exp0
      case 76: // exp1
      case 77: // exp2
      case 78: // exp3
      case 79: // exp4
      case 80: // exp5
      case 81: // exp6
      case 82: // exp7
      case 83: // exp8
      case 84: // exp9
      case 85: // expu
        value.copy< ::sqf::parser::sqf::bison::astnode > (that.value);
        break;

      case 4: // "false"
      case 5: // "true"
      case 6: // "private"
      case 7: // "{"
      case 8: // "}"
      case 9: // "("
      case 10: // ")"
      case 11: // "["
      case 12: // "]"
      case 13: // ";"
      case 14: // ","
      case 15: // "="
      case 16: // OPERATOR_B_0
      case 17: // OPERATOR_B_1
      case 18: // OPERATOR_B_2
      case 19: // OPERATOR_B_3
      case 20: // OPERATOR_B_4
      case 21: // OPERATOR_B_5
      case 22: // OPERATOR_B_6
      case 23: // OPERATOR_B_7
      case 24: // OPERATOR_B_8
      case 25: // OPERATOR_B_9
      case 26: // OPERATOR_BU_0
      case 27: // OPERATOR_BU_1
      case 28: // OPERATOR_BU_2
      case 29: // OPERATOR_BU_3
      case 30: // OPERATOR_BU_4
      case 31: // OPERATOR_BU_5
      case 32: // OPERATOR_BU_6
      case 33: // OPERATOR_BU_7
      case 34: // OPERATOR_BU_8
      case 35: // OPERATOR_BU_9
      case 36: // OPERATOR_BN_0
      case 37: // OPERATOR_BN_1
      case 38: // OPERATOR_BN_2
      case 39: // OPERATOR_BN_3
      case 40: // OPERATOR_BN_4
      case 41: // OPERATOR_BN_5
      case 42: // OPERATOR_BN_6
      case 43: // OPERATOR_BN_7
      case 44: // OPERATOR_BN_8
      case 45: // OPERATOR_BN_9
      case 46: // OPERATOR_BUN_0
      case 47: // OPERATOR_BUN_1
      case 48: // OPERATOR_BUN_2
      case 49: // OPERATOR_BUN_3
      case 50: // OPERATOR_BUN_4
      case 51: // OPERATOR_BUN_5
      case 52: // OPERATOR_BUN_6
      case 53: // OPERATOR_BUN_7
      case 54: // OPERATOR_BUN_8
      case 55: // OPERATOR_BUN_9
      case 56: // OPERATOR_U
      case 57: // OPERATOR_N
      case 58: // OPERATOR_UN
      case 59: // IDENT
      case 60: // NUMBER
      case 61: // HEXNUMBER
      case 62: // STRING
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
      case 65: // statements
      case 66: // statement
      case 69: // value
      case 70: // exp_list
      case 71: // code
      case 72: // array
      case 73: // assignment
      case 74: // expression
      case 75: // exp0
      case 76: // exp1
      case 77: // exp2
      case 78: // exp3
      case 79: // exp4
      case 80: // exp5
      case 81: // exp6
      case 82: // exp7
      case 83: // exp8
      case 84: // exp9
      case 85: // expu
        yylhs.value.build< ::sqf::parser::sqf::bison::astnode > ();
        break;

      case 4: // "false"
      case 5: // "true"
      case 6: // "private"
      case 7: // "{"
      case 8: // "}"
      case 9: // "("
      case 10: // ")"
      case 11: // "["
      case 12: // "]"
      case 13: // ";"
      case 14: // ","
      case 15: // "="
      case 16: // OPERATOR_B_0
      case 17: // OPERATOR_B_1
      case 18: // OPERATOR_B_2
      case 19: // OPERATOR_B_3
      case 20: // OPERATOR_B_4
      case 21: // OPERATOR_B_5
      case 22: // OPERATOR_B_6
      case 23: // OPERATOR_B_7
      case 24: // OPERATOR_B_8
      case 25: // OPERATOR_B_9
      case 26: // OPERATOR_BU_0
      case 27: // OPERATOR_BU_1
      case 28: // OPERATOR_BU_2
      case 29: // OPERATOR_BU_3
      case 30: // OPERATOR_BU_4
      case 31: // OPERATOR_BU_5
      case 32: // OPERATOR_BU_6
      case 33: // OPERATOR_BU_7
      case 34: // OPERATOR_BU_8
      case 35: // OPERATOR_BU_9
      case 36: // OPERATOR_BN_0
      case 37: // OPERATOR_BN_1
      case 38: // OPERATOR_BN_2
      case 39: // OPERATOR_BN_3
      case 40: // OPERATOR_BN_4
      case 41: // OPERATOR_BN_5
      case 42: // OPERATOR_BN_6
      case 43: // OPERATOR_BN_7
      case 44: // OPERATOR_BN_8
      case 45: // OPERATOR_BN_9
      case 46: // OPERATOR_BUN_0
      case 47: // OPERATOR_BUN_1
      case 48: // OPERATOR_BUN_2
      case 49: // OPERATOR_BUN_3
      case 50: // OPERATOR_BUN_4
      case 51: // OPERATOR_BUN_5
      case 52: // OPERATOR_BUN_6
      case 53: // OPERATOR_BUN_7
      case 54: // OPERATOR_BUN_8
      case 55: // OPERATOR_BUN_9
      case 56: // OPERATOR_U
      case 57: // OPERATOR_N
      case 58: // OPERATOR_UN
      case 59: // IDENT
      case 60: // NUMBER
      case 61: // HEXNUMBER
      case 62: // STRING
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
  case 2:
#line 209 "parser.y" // lalr1.cc:859
    { result = ::sqf::parser::sqf::bison::astnode{}; }
#line 795 "parser.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 210 "parser.y" // lalr1.cc:859
    { result = ::sqf::parser::sqf::bison::astnode{}; result.append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 801 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 211 "parser.y" // lalr1.cc:859
    { result = ::sqf::parser::sqf::bison::astnode{}; }
#line 807 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 212 "parser.y" // lalr1.cc:859
    { result = ::sqf::parser::sqf::bison::astnode{}; result.append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 813 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 214 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::STATEMENTS }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 819 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 215 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 825 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 216 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > (); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 831 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 218 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 837 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 219 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 843 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 228 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 849 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 229 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 855 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 230 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 861 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 231 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 867 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 232 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 873 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 233 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 879 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 234 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 885 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 235 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 891 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 236 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 897 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 237 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 903 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 238 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 909 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 239 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 915 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 240 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 921 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 241 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 927 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 242 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 933 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 243 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 939 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 244 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 945 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 245 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 951 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 246 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 957 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 247 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 963 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 248 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 969 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 249 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }
#line 975 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 250 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::IDENT, yystack_[0].value.as< tokenizer::token > () }; }
#line 981 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 251 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::NUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 987 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 252 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::HEXNUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 993 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 253 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::BOOLEAN_TRUE, yystack_[0].value.as< tokenizer::token > () }; }
#line 999 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 254 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::BOOLEAN_FALSE, yystack_[0].value.as< tokenizer::token > () }; }
#line 1005 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 255 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1011 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 256 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1017 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 258 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPRESSION_LIST }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1023 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 259 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > (); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1029 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 261 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[1].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1035 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 262 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[1].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1041 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 263 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, yystack_[2].value.as< tokenizer::token > () }; }
#line 1047 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 264 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, yystack_[1].value.as< tokenizer::token > () }; }
#line 1053 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 266 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::ARRAY, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append_children(yystack_[1].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1059 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 267 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::ARRAY, yystack_[1].value.as< tokenizer::token > () }; }
#line 1065 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 269 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::ASSIGNMENT_LOCAL, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1071 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 270 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::ASSIGNMENT, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1077 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 272 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1083 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 274 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1089 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 275 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1095 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 276 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1101 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 277 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1107 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 278 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1113 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 280 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1119 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 281 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1125 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 282 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1131 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 283 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1137 "parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 284 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1143 "parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 286 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1149 "parser.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 287 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1155 "parser.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 288 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1161 "parser.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 289 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1167 "parser.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 290 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1173 "parser.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 292 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1179 "parser.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 293 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1185 "parser.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 294 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1191 "parser.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 295 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1197 "parser.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 296 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1203 "parser.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 298 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1209 "parser.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 299 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1215 "parser.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 300 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1221 "parser.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 301 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1227 "parser.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 302 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1233 "parser.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 304 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1239 "parser.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 305 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1245 "parser.tab.cc" // lalr1.cc:859
    break;

  case 82:
#line 306 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1251 "parser.tab.cc" // lalr1.cc:859
    break;

  case 83:
#line 307 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1257 "parser.tab.cc" // lalr1.cc:859
    break;

  case 84:
#line 308 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1263 "parser.tab.cc" // lalr1.cc:859
    break;

  case 85:
#line 310 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1269 "parser.tab.cc" // lalr1.cc:859
    break;

  case 86:
#line 311 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1275 "parser.tab.cc" // lalr1.cc:859
    break;

  case 87:
#line 312 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1281 "parser.tab.cc" // lalr1.cc:859
    break;

  case 88:
#line 313 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1287 "parser.tab.cc" // lalr1.cc:859
    break;

  case 89:
#line 314 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1293 "parser.tab.cc" // lalr1.cc:859
    break;

  case 90:
#line 316 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1299 "parser.tab.cc" // lalr1.cc:859
    break;

  case 91:
#line 317 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1305 "parser.tab.cc" // lalr1.cc:859
    break;

  case 92:
#line 318 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1311 "parser.tab.cc" // lalr1.cc:859
    break;

  case 93:
#line 319 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1317 "parser.tab.cc" // lalr1.cc:859
    break;

  case 94:
#line 320 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1323 "parser.tab.cc" // lalr1.cc:859
    break;

  case 95:
#line 322 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1329 "parser.tab.cc" // lalr1.cc:859
    break;

  case 96:
#line 323 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1335 "parser.tab.cc" // lalr1.cc:859
    break;

  case 97:
#line 324 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1341 "parser.tab.cc" // lalr1.cc:859
    break;

  case 98:
#line 325 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1347 "parser.tab.cc" // lalr1.cc:859
    break;

  case 99:
#line 326 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1353 "parser.tab.cc" // lalr1.cc:859
    break;

  case 100:
#line 328 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1359 "parser.tab.cc" // lalr1.cc:859
    break;

  case 101:
#line 329 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1365 "parser.tab.cc" // lalr1.cc:859
    break;

  case 102:
#line 330 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1371 "parser.tab.cc" // lalr1.cc:859
    break;

  case 103:
#line 331 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1377 "parser.tab.cc" // lalr1.cc:859
    break;

  case 104:
#line 332 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1383 "parser.tab.cc" // lalr1.cc:859
    break;

  case 105:
#line 334 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1389 "parser.tab.cc" // lalr1.cc:859
    break;

  case 106:
#line 335 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1395 "parser.tab.cc" // lalr1.cc:859
    break;

  case 107:
#line 336 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1401 "parser.tab.cc" // lalr1.cc:859
    break;

  case 108:
#line 337 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1407 "parser.tab.cc" // lalr1.cc:859
    break;

  case 109:
#line 338 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1413 "parser.tab.cc" // lalr1.cc:859
    break;

  case 110:
#line 339 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1419 "parser.tab.cc" // lalr1.cc:859
    break;

  case 111:
#line 340 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1425 "parser.tab.cc" // lalr1.cc:859
    break;

  case 112:
#line 341 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1431 "parser.tab.cc" // lalr1.cc:859
    break;

  case 113:
#line 342 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1437 "parser.tab.cc" // lalr1.cc:859
    break;

  case 114:
#line 343 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1443 "parser.tab.cc" // lalr1.cc:859
    break;

  case 115:
#line 344 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1449 "parser.tab.cc" // lalr1.cc:859
    break;

  case 116:
#line 345 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1455 "parser.tab.cc" // lalr1.cc:859
    break;

  case 117:
#line 346 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1461 "parser.tab.cc" // lalr1.cc:859
    break;

  case 118:
#line 347 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1467 "parser.tab.cc" // lalr1.cc:859
    break;

  case 119:
#line 348 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1473 "parser.tab.cc" // lalr1.cc:859
    break;

  case 120:
#line 349 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1479 "parser.tab.cc" // lalr1.cc:859
    break;

  case 121:
#line 350 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1485 "parser.tab.cc" // lalr1.cc:859
    break;

  case 122:
#line 351 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1491 "parser.tab.cc" // lalr1.cc:859
    break;

  case 123:
#line 352 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1497 "parser.tab.cc" // lalr1.cc:859
    break;

  case 124:
#line 353 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1503 "parser.tab.cc" // lalr1.cc:859
    break;

  case 125:
#line 354 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1509 "parser.tab.cc" // lalr1.cc:859
    break;

  case 126:
#line 355 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1515 "parser.tab.cc" // lalr1.cc:859
    break;

  case 127:
#line 356 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }
#line 1521 "parser.tab.cc" // lalr1.cc:859
    break;

  case 128:
#line 357 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1527 "parser.tab.cc" // lalr1.cc:859
    break;

  case 129:
#line 358 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }
#line 1533 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1537 "parser.tab.cc" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -102;

  const signed char parser::yytable_ninf_ = -1;

  const short int
  parser::yypact_[] =
  {
      71,  -102,  -102,  -102,   379,   143,   438,   320,  -102,  -102,
     438,   438,   438,   438,   438,   438,   438,   438,   438,   438,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,
     438,   438,   438,   438,   438,   438,   438,   438,   438,   438,
     438,  -102,   438,     5,  -102,  -102,  -102,    50,    33,  -102,
    -102,   261,  -102,  -102,  -102,  -102,  -102,    26,    27,    25,
      30,   115,   480,   481,   479,   482,   483,  -102,   438,    42,
    -102,  -102,    11,   202,  -102,    41,  -102,     9,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,
    -102,   438,  -102,   261,    33,  -102,   438,   438,   438,   438,
     438,   438,   438,   438,   438,   438,   438,   438,   438,   438,
     438,   438,   438,   438,   438,   438,   438,   438,   438,   438,
     438,   438,   438,   438,   438,   438,   438,   438,   438,   438,
     438,   438,   438,   438,   438,   438,   438,  -102,  -102,    14,
    -102,  -102,   438,  -102,  -102,    27,    27,    27,    27,    25,
      25,    25,    25,    30,    30,    30,    30,   115,   115,   115,
     115,   480,   480,   480,   480,   481,   481,   481,   481,   479,
     479,   479,   479,   482,   482,   482,   482,   483,   483,   483,
     483,  -102,  -102,  -102,  -102,  -102,  -102,  -102
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,     2,    41,    40,     0,     0,     0,     0,    11,    12,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
       0,    16,     0,    37,    38,    39,    15,     0,     3,     6,
      13,     4,   129,    42,    43,     9,    10,    54,    55,    60,
      65,    70,    75,    80,    85,    90,    95,   100,     0,    37,
     105,    49,     0,     0,    37,     0,    51,     0,    44,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   106,
     107,     0,     1,     7,     5,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    48,     0,
     128,    50,     0,    53,     8,    56,    58,    59,    57,    61,
      62,    63,    64,    66,    67,    68,    69,    71,    72,    73,
      74,    76,    77,    78,    79,    81,    82,    83,    84,    86,
      87,    88,    89,    91,    92,    93,    94,    96,    97,    98,
      99,   101,   102,   103,   104,    52,    47,    45
  };

  const short int
  parser::yypgoto_[] =
  {
    -102,  -102,    -3,   -48,   -47,    40,  -102,  -102,  -102,  -102,
    -102,    -6,  -102,  -101,   -24,   -23,    18,    36,    91,    92,
     142,   144,    -1
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    47,    48,    49,    50,   103,    52,    77,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67
  };

  const unsigned char
  parser::yytable_[] =
  {
      75,    78,    72,    70,   105,   155,   156,   157,   158,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,   147,
     101,   151,   196,   152,     8,     9,   105,     8,     9,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
      51,   100,   106,   114,   110,    73,     8,     9,   104,   118,
     102,   150,   107,   115,   111,   154,   105,   146,     0,   119,
       0,     0,   108,   116,   112,     0,     0,    70,     0,   120,
     149,     1,   109,   117,   113,     2,     3,     4,     5,   121,
       6,     0,     7,     0,     8,     9,   159,   160,   161,   162,
       0,   163,   164,   165,   166,   153,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,     0,   122,   167,   168,   169,   170,
     195,   191,   192,   193,   194,   123,   197,     2,     3,     4,
       5,    71,     6,     0,     7,   124,     8,     9,   171,   172,
     173,   174,     0,     0,     0,   125,     0,     0,     0,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,     2,     3,     4,     5,
     148,     6,     0,     7,     0,     8,     9,   175,   176,   177,
     178,     0,   179,   180,   181,   182,     0,     0,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,     2,     3,     4,     5,     0,
       6,     0,     7,     0,     8,     9,   183,   184,   185,   186,
       0,     0,   187,   188,   189,   190,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,     2,     3,    68,     5,     0,     6,
       0,     7,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    74,
      44,    45,    46,     2,     3,    68,     5,     0,     6,     0,
       7,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    69,    44,
      45,    46,     2,     3,    68,     5,     0,     6,     0,     7,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    74,    44,    45,
      46,   126,   134,   130,     0,     0,   138,     0,   142,     0,
       0,   127,   135,   131,     0,     0,   139,     0,   143,     0,
       0,   128,   136,   132,     0,     0,   140,     0,   144,     0,
       0,   129,   137,   133,     0,     0,   141,     0,   145
  };

  const short int
  parser::yycheck_[] =
  {
       6,     7,     5,     4,    51,   106,   107,   108,   109,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,     8,
      15,    12,     8,    14,    13,    14,    73,    13,    14,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,    42,    16,    18,    17,     5,    13,    14,    51,    19,
       0,    10,    26,    28,    27,   103,   103,    15,    -1,    29,
      -1,    -1,    36,    38,    37,    -1,    -1,    68,    -1,    39,
      73,     0,    46,    48,    47,     4,     5,     6,     7,    49,
       9,    -1,    11,    -1,    13,    14,   110,   111,   112,   113,
      -1,   114,   115,   116,   117,   101,    -1,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    20,   118,   119,   120,   121,
     146,   142,   143,   144,   145,    30,   152,     4,     5,     6,
       7,     8,     9,    -1,    11,    40,    13,    14,   122,   123,
     124,   125,    -1,    -1,    -1,    50,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    13,    14,   126,   127,   128,
     129,    -1,   130,   131,   132,   133,    -1,    -1,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,     4,     5,     6,     7,    -1,
       9,    -1,    11,    -1,    13,    14,   134,   135,   136,   137,
      -1,    -1,   138,   139,   140,   141,    -1,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,     4,     5,     6,     7,    -1,     9,
      -1,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,     4,     5,     6,     7,    -1,     9,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,     4,     5,     6,     7,    -1,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    21,    23,    22,    -1,    -1,    24,    -1,    25,    -1,
      -1,    31,    33,    32,    -1,    -1,    34,    -1,    35,    -1,
      -1,    41,    43,    42,    -1,    -1,    44,    -1,    45,    -1,
      -1,    51,    53,    52,    -1,    -1,    54,    -1,    55
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     0,     4,     5,     6,     7,     9,    11,    13,    14,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    64,    65,    66,
      67,    68,    69,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,     6,    59,
      85,     8,    65,    68,    59,    74,    12,    70,    74,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    15,     0,    68,    65,    67,    16,    26,    36,    46,
      17,    27,    37,    47,    18,    28,    38,    48,    19,    29,
      39,    49,    20,    30,    40,    50,    21,    31,    41,    51,
      22,    32,    42,    52,    23,    33,    43,    53,    24,    34,
      44,    54,    25,    35,    45,    55,    15,     8,     8,    65,
      10,    12,    14,    74,    66,    76,    76,    76,    76,    77,
      77,    77,    77,    78,    78,    78,    78,    79,    79,    79,
      79,    80,    80,    80,    80,    81,    81,    81,    81,    82,
      82,    82,    82,    83,    83,    83,    83,    84,    84,    84,
      84,    85,    85,    85,    85,    74,     8,    74
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    63,    64,    64,    64,    64,    65,    65,    65,    66,
      66,    67,    67,    68,    68,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    70,    70,    71,    71,    71,    71,
      72,    72,    73,    73,    74,    75,    75,    75,    75,    75,
      76,    76,    76,    76,    76,    77,    77,    77,    77,    77,
      78,    78,    78,    78,    78,    79,    79,    79,    79,    79,
      80,    80,    80,    80,    80,    81,    81,    81,    81,    81,
      82,    82,    82,    82,    82,    83,    83,    83,    83,    83,
      84,    84,    84,    84,    84,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     1,     2,     3,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     4,     3,     2,
       3,     2,     4,     3,     1,     1,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "END_OF_FILE", "error", "$undefined", "INVALID", "\"false\"",
  "\"true\"", "\"private\"", "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"[\"",
  "\"]\"", "\";\"", "\",\"", "\"=\"", "OPERATOR_B_0", "OPERATOR_B_1",
  "OPERATOR_B_2", "OPERATOR_B_3", "OPERATOR_B_4", "OPERATOR_B_5",
  "OPERATOR_B_6", "OPERATOR_B_7", "OPERATOR_B_8", "OPERATOR_B_9",
  "OPERATOR_BU_0", "OPERATOR_BU_1", "OPERATOR_BU_2", "OPERATOR_BU_3",
  "OPERATOR_BU_4", "OPERATOR_BU_5", "OPERATOR_BU_6", "OPERATOR_BU_7",
  "OPERATOR_BU_8", "OPERATOR_BU_9", "OPERATOR_BN_0", "OPERATOR_BN_1",
  "OPERATOR_BN_2", "OPERATOR_BN_3", "OPERATOR_BN_4", "OPERATOR_BN_5",
  "OPERATOR_BN_6", "OPERATOR_BN_7", "OPERATOR_BN_8", "OPERATOR_BN_9",
  "OPERATOR_BUN_0", "OPERATOR_BUN_1", "OPERATOR_BUN_2", "OPERATOR_BUN_3",
  "OPERATOR_BUN_4", "OPERATOR_BUN_5", "OPERATOR_BUN_6", "OPERATOR_BUN_7",
  "OPERATOR_BUN_8", "OPERATOR_BUN_9", "OPERATOR_U", "OPERATOR_N",
  "OPERATOR_UN", "IDENT", "NUMBER", "HEXNUMBER", "STRING", "$accept",
  "start", "statements", "statement", "separator", "separators", "value",
  "exp_list", "code", "array", "assignment", "expression", "exp0", "exp1",
  "exp2", "exp3", "exp4", "exp5", "exp6", "exp7", "exp8", "exp9", "expu", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   209,   209,   210,   211,   212,   214,   215,   216,   218,
     219,   221,   222,   224,   225,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   258,   259,   261,   262,   263,   264,
     266,   267,   269,   270,   272,   274,   275,   276,   277,   278,
     280,   281,   282,   283,   284,   286,   287,   288,   289,   290,
     292,   293,   294,   295,   296,   298,   299,   300,   301,   302,
     304,   305,   306,   307,   308,   310,   311,   312,   313,   314,
     316,   317,   318,   319,   320,   322,   323,   324,   325,   326,
     328,   329,   330,   331,   332,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358
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
#line 2118 "parser.tab.cc" // lalr1.cc:1167
#line 362 "parser.y" // lalr1.cc:1168

#ifdef __GNUG__
    #pragma GCC diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#include "sqf_parser.hpp"
namespace sqf::parser::sqf::bison
{
    void parser::error (const location_type& loc, const std::string& msg)
    {
        actual.__log(logmessage::sqf::ParseError({ *loc.begin.filename, loc.begin.line, loc.begin.column }, msg));
    }
    inline parser::symbol_type yylex (::sqf::runtime::runtime& runtime, ::sqf::parser::sqf::tokenizer& tokenizer)
    {
         auto token = tokenizer.next();
         parser::location_type loc;
         loc.begin.line = token.line;
         loc.begin.column = token.column;
         loc.end.line = token.line;
         loc.end.column = token.column + token.contents.length();
         loc.begin.filename = loc.end.filename = token.path;

         switch (token.type)
         {
         case tokenizer::etoken::eof: return parser::make_END_OF_FILE(loc);
         case tokenizer::etoken::invalid: return parser::make_INVALID(loc);
         case tokenizer::etoken::m_line: return yylex(runtime, tokenizer);
         case tokenizer::etoken::i_comment_line: return yylex(runtime, tokenizer);
         case tokenizer::etoken::i_comment_block: return yylex(runtime, tokenizer);
         case tokenizer::etoken::i_whitespace: return yylex(runtime, tokenizer);
         
         case tokenizer::etoken::t_false: return parser::make_FALSE(token, loc);
         case tokenizer::etoken::t_private: return parser::make_PRIVATE(token, loc);
         case tokenizer::etoken::t_true: return parser::make_TRUE(token, loc);

         case tokenizer::etoken::s_curlyo: return parser::make_CURLYO(token, loc);
         case tokenizer::etoken::s_curlyc: return parser::make_CURLYC(token, loc);
         case tokenizer::etoken::s_roundo: return parser::make_ROUNDO(token, loc);
         case tokenizer::etoken::s_roundc: return parser::make_ROUNDC(token, loc);
         case tokenizer::etoken::s_edgeo: return parser::make_SQUAREO(token, loc);
         case tokenizer::etoken::s_edgec: return parser::make_SQUAREC(token, loc);
         case tokenizer::etoken::s_semicolon: return parser::make_SEMICOLON(token, loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(token, loc);

         case tokenizer::etoken::t_operator:
         case tokenizer::etoken::t_ident:
         {
             auto key = std::string(token.contents.begin(), token.contents.end());
             short precedence = 0;
             bool binary = false;
             bool unary = false;
             bool nular = false;
             std::transform(key.begin(), key.end(), key.begin(), [](char c) -> char { return (char)std::tolower(c); });
             if (runtime.sqfop_exists_binary(key))
             {
                auto bres = runtime.sqfop_binary_by_name(key);
                precedence = bres.begin()->get().precedence();
                binary = true;
             }
             if (runtime.sqfop_exists_unary(key))
             {
                 unary = true;
             }
             if (runtime.sqfop_exists(::sqf::runtime::sqfop_nular::key{key}))
             {
                 nular = true;
             }
             if (binary && !unary && !nular)
             {
                 
                switch (precedence)
                {
                case 1:  return parser::make_OPERATOR_B_0(token, loc);
                case 2:  return parser::make_OPERATOR_B_1(token, loc);
                case 3:  return parser::make_OPERATOR_B_2(token, loc);
                case 4:  return parser::make_OPERATOR_B_3(token, loc);
                case 5:  return parser::make_OPERATOR_B_4(token, loc);
                case 6:  return parser::make_OPERATOR_B_5(token, loc);
                case 7:  return parser::make_OPERATOR_B_6(token, loc);
                case 8:  return parser::make_OPERATOR_B_7(token, loc);
                case 9:  return parser::make_OPERATOR_B_8(token, loc);
                case 10: return parser::make_OPERATOR_B_9(token, loc);
                }
             }
             else if (binary && !unary && nular)
             {
                switch (precedence)
                {
                case 1:  return parser::make_OPERATOR_BN_0(token, loc);
                case 2:  return parser::make_OPERATOR_BN_1(token, loc);
                case 3:  return parser::make_OPERATOR_BN_2(token, loc);
                case 4:  return parser::make_OPERATOR_BN_3(token, loc);
                case 5:  return parser::make_OPERATOR_BN_4(token, loc);
                case 6:  return parser::make_OPERATOR_BN_5(token, loc);
                case 7:  return parser::make_OPERATOR_BN_6(token, loc);
                case 8:  return parser::make_OPERATOR_BN_7(token, loc);
                case 9:  return parser::make_OPERATOR_BN_8(token, loc);
                case 10: return parser::make_OPERATOR_BN_9(token, loc);
                }
             }
             else if (binary && unary && !nular)
             {
                switch (precedence)
                {
                case 1:  return parser::make_OPERATOR_BU_0(token, loc);
                case 2:  return parser::make_OPERATOR_BU_1(token, loc);
                case 3:  return parser::make_OPERATOR_BU_2(token, loc);
                case 4:  return parser::make_OPERATOR_BU_3(token, loc);
                case 5:  return parser::make_OPERATOR_BU_4(token, loc);
                case 6:  return parser::make_OPERATOR_BU_5(token, loc);
                case 7:  return parser::make_OPERATOR_BU_6(token, loc);
                case 8:  return parser::make_OPERATOR_BU_7(token, loc);
                case 9:  return parser::make_OPERATOR_BU_8(token, loc);
                case 10: return parser::make_OPERATOR_BU_9(token, loc);
                }
             }
             else if (binary && unary && nular)
             {
                switch (precedence)
                {
                case 1:  return parser::make_OPERATOR_BUN_0(token, loc);
                case 2:  return parser::make_OPERATOR_BUN_1(token, loc);
                case 3:  return parser::make_OPERATOR_BUN_2(token, loc);
                case 4:  return parser::make_OPERATOR_BUN_3(token, loc);
                case 5:  return parser::make_OPERATOR_BUN_4(token, loc);
                case 6:  return parser::make_OPERATOR_BUN_5(token, loc);
                case 7:  return parser::make_OPERATOR_BUN_6(token, loc);
                case 8:  return parser::make_OPERATOR_BUN_7(token, loc);
                case 9:  return parser::make_OPERATOR_BUN_8(token, loc);
                case 10: return parser::make_OPERATOR_BUN_9(token, loc);
                }
             }
             else if (!binary && !unary && nular)
             {
                 return parser::make_OPERATOR_N(token, loc);
             }
             else if (!binary && unary && !nular)
             {
                 return parser::make_OPERATOR_U(token, loc);
             }
             else if (!binary && unary && nular)
             {
                 return parser::make_OPERATOR_UN(token, loc);
             }
             
             return token.type == tokenizer::etoken::t_ident ? parser::make_IDENT(token, loc) : parser::make_INVALID(loc);
         }

         case tokenizer::etoken::t_string_double: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_string_single: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_number: return parser::make_NUMBER(token, loc);
         case tokenizer::etoken::t_hexadecimal: return parser::make_HEXNUMBER(token, loc);
         case tokenizer::etoken::s_equal: return parser::make_EQUAL(token, loc);
         default:
             return parser::make_INVALID(loc);
         }
     }
}
