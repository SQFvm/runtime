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


    #include "tokenizer.hpp"
    #include <string>
    #include <vector>
    #include <algorithm>




// First part of user declarations.



# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.tab.hh"

// User implementation prologue.


// Unqualified %code blocks.


    
    #include "../../runtime/runtime.h"
     namespace sqf::parser::sqf::bison
     {
          // Return the next token.
          parser::symbol_type yylex (::sqf::runtime::runtime &runtime, ::sqf::parser::sqf::tokenizer&);
     }




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


namespace sqf { namespace parser { namespace sqf { namespace bison  {


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
      case 64: // statements
      case 65: // statement
      case 68: // value
      case 69: // exp_list
      case 70: // code
      case 71: // array
      case 72: // assignment
      case 73: // expression
      case 74: // exp0
      case 75: // exp1
      case 76: // exp2
      case 77: // exp3
      case 78: // exp4
      case 79: // exp5
      case 80: // exp6
      case 81: // exp7
      case 82: // exp8
      case 83: // exp9
      case 84: // expu
        value.move< ::sqf::parser::sqf::bison::astnode > (that.value);
        break;

      case 3: // "false"
      case 4: // "true"
      case 5: // "private"
      case 6: // "{"
      case 7: // "}"
      case 8: // "("
      case 9: // ")"
      case 10: // "["
      case 11: // "]"
      case 12: // ";"
      case 13: // ","
      case 14: // "="
      case 15: // OPERATOR_B_0
      case 16: // OPERATOR_B_1
      case 17: // OPERATOR_B_2
      case 18: // OPERATOR_B_3
      case 19: // OPERATOR_B_4
      case 20: // OPERATOR_B_5
      case 21: // OPERATOR_B_6
      case 22: // OPERATOR_B_7
      case 23: // OPERATOR_B_8
      case 24: // OPERATOR_B_9
      case 25: // OPERATOR_BU_0
      case 26: // OPERATOR_BU_1
      case 27: // OPERATOR_BU_2
      case 28: // OPERATOR_BU_3
      case 29: // OPERATOR_BU_4
      case 30: // OPERATOR_BU_5
      case 31: // OPERATOR_BU_6
      case 32: // OPERATOR_BU_7
      case 33: // OPERATOR_BU_8
      case 34: // OPERATOR_BU_9
      case 35: // OPERATOR_BN_0
      case 36: // OPERATOR_BN_1
      case 37: // OPERATOR_BN_2
      case 38: // OPERATOR_BN_3
      case 39: // OPERATOR_BN_4
      case 40: // OPERATOR_BN_5
      case 41: // OPERATOR_BN_6
      case 42: // OPERATOR_BN_7
      case 43: // OPERATOR_BN_8
      case 44: // OPERATOR_BN_9
      case 45: // OPERATOR_BUN_0
      case 46: // OPERATOR_BUN_1
      case 47: // OPERATOR_BUN_2
      case 48: // OPERATOR_BUN_3
      case 49: // OPERATOR_BUN_4
      case 50: // OPERATOR_BUN_5
      case 51: // OPERATOR_BUN_6
      case 52: // OPERATOR_BUN_7
      case 53: // OPERATOR_BUN_8
      case 54: // OPERATOR_BUN_9
      case 55: // OPERATOR_U
      case 56: // OPERATOR_N
      case 57: // OPERATOR_UN
      case 58: // IDENT
      case 59: // NUMBER
      case 60: // HEXNUMBER
      case 61: // STRING
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
      case 64: // statements
      case 65: // statement
      case 68: // value
      case 69: // exp_list
      case 70: // code
      case 71: // array
      case 72: // assignment
      case 73: // expression
      case 74: // exp0
      case 75: // exp1
      case 76: // exp2
      case 77: // exp3
      case 78: // exp4
      case 79: // exp5
      case 80: // exp6
      case 81: // exp7
      case 82: // exp8
      case 83: // exp9
      case 84: // expu
        value.copy< ::sqf::parser::sqf::bison::astnode > (that.value);
        break;

      case 3: // "false"
      case 4: // "true"
      case 5: // "private"
      case 6: // "{"
      case 7: // "}"
      case 8: // "("
      case 9: // ")"
      case 10: // "["
      case 11: // "]"
      case 12: // ";"
      case 13: // ","
      case 14: // "="
      case 15: // OPERATOR_B_0
      case 16: // OPERATOR_B_1
      case 17: // OPERATOR_B_2
      case 18: // OPERATOR_B_3
      case 19: // OPERATOR_B_4
      case 20: // OPERATOR_B_5
      case 21: // OPERATOR_B_6
      case 22: // OPERATOR_B_7
      case 23: // OPERATOR_B_8
      case 24: // OPERATOR_B_9
      case 25: // OPERATOR_BU_0
      case 26: // OPERATOR_BU_1
      case 27: // OPERATOR_BU_2
      case 28: // OPERATOR_BU_3
      case 29: // OPERATOR_BU_4
      case 30: // OPERATOR_BU_5
      case 31: // OPERATOR_BU_6
      case 32: // OPERATOR_BU_7
      case 33: // OPERATOR_BU_8
      case 34: // OPERATOR_BU_9
      case 35: // OPERATOR_BN_0
      case 36: // OPERATOR_BN_1
      case 37: // OPERATOR_BN_2
      case 38: // OPERATOR_BN_3
      case 39: // OPERATOR_BN_4
      case 40: // OPERATOR_BN_5
      case 41: // OPERATOR_BN_6
      case 42: // OPERATOR_BN_7
      case 43: // OPERATOR_BN_8
      case 44: // OPERATOR_BN_9
      case 45: // OPERATOR_BUN_0
      case 46: // OPERATOR_BUN_1
      case 47: // OPERATOR_BUN_2
      case 48: // OPERATOR_BUN_3
      case 49: // OPERATOR_BUN_4
      case 50: // OPERATOR_BUN_5
      case 51: // OPERATOR_BUN_6
      case 52: // OPERATOR_BUN_7
      case 53: // OPERATOR_BUN_8
      case 54: // OPERATOR_BUN_9
      case 55: // OPERATOR_U
      case 56: // OPERATOR_N
      case 57: // OPERATOR_UN
      case 58: // IDENT
      case 59: // NUMBER
      case 60: // HEXNUMBER
      case 61: // STRING
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
      case 64: // statements
      case 65: // statement
      case 68: // value
      case 69: // exp_list
      case 70: // code
      case 71: // array
      case 72: // assignment
      case 73: // expression
      case 74: // exp0
      case 75: // exp1
      case 76: // exp2
      case 77: // exp3
      case 78: // exp4
      case 79: // exp5
      case 80: // exp6
      case 81: // exp7
      case 82: // exp8
      case 83: // exp9
      case 84: // expu
        yylhs.value.build< ::sqf::parser::sqf::bison::astnode > ();
        break;

      case 3: // "false"
      case 4: // "true"
      case 5: // "private"
      case 6: // "{"
      case 7: // "}"
      case 8: // "("
      case 9: // ")"
      case 10: // "["
      case 11: // "]"
      case 12: // ";"
      case 13: // ","
      case 14: // "="
      case 15: // OPERATOR_B_0
      case 16: // OPERATOR_B_1
      case 17: // OPERATOR_B_2
      case 18: // OPERATOR_B_3
      case 19: // OPERATOR_B_4
      case 20: // OPERATOR_B_5
      case 21: // OPERATOR_B_6
      case 22: // OPERATOR_B_7
      case 23: // OPERATOR_B_8
      case 24: // OPERATOR_B_9
      case 25: // OPERATOR_BU_0
      case 26: // OPERATOR_BU_1
      case 27: // OPERATOR_BU_2
      case 28: // OPERATOR_BU_3
      case 29: // OPERATOR_BU_4
      case 30: // OPERATOR_BU_5
      case 31: // OPERATOR_BU_6
      case 32: // OPERATOR_BU_7
      case 33: // OPERATOR_BU_8
      case 34: // OPERATOR_BU_9
      case 35: // OPERATOR_BN_0
      case 36: // OPERATOR_BN_1
      case 37: // OPERATOR_BN_2
      case 38: // OPERATOR_BN_3
      case 39: // OPERATOR_BN_4
      case 40: // OPERATOR_BN_5
      case 41: // OPERATOR_BN_6
      case 42: // OPERATOR_BN_7
      case 43: // OPERATOR_BN_8
      case 44: // OPERATOR_BN_9
      case 45: // OPERATOR_BUN_0
      case 46: // OPERATOR_BUN_1
      case 47: // OPERATOR_BUN_2
      case 48: // OPERATOR_BUN_3
      case 49: // OPERATOR_BUN_4
      case 50: // OPERATOR_BUN_5
      case 51: // OPERATOR_BUN_6
      case 52: // OPERATOR_BUN_7
      case 53: // OPERATOR_BUN_8
      case 54: // OPERATOR_BUN_9
      case 55: // OPERATOR_U
      case 56: // OPERATOR_N
      case 57: // OPERATOR_UN
      case 58: // IDENT
      case 59: // NUMBER
      case 60: // HEXNUMBER
      case 61: // STRING
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

    { result = ::sqf::parser::sqf::bison::astnode{}; }

    break;

  case 3:

    { result = ::sqf::parser::sqf::bison::astnode{}; result.append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 4:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::STATEMENTS }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 5:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 6:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > (); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 7:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 8:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 13:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::STRING, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 14:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 15:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 16:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 17:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 18:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 19:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 20:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 21:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 22:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 23:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 24:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 25:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 26:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 27:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 28:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 29:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 30:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 31:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 32:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 33:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 34:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 35:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::IDENT, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 36:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::NUMBER, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 37:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::HEXNUMBER, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 38:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::BOOLEAN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 39:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::BOOLEAN, yystack_[0].value.as< tokenizer::token > () }; }

    break;

  case 40:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 41:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 42:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPRESSION_LIST }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 43:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > (); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 44:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[1].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 45:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[1].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 46:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, yystack_[2].value.as< tokenizer::token > () }; }

    break;

  case 47:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{  astkind::CODE, yystack_[1].value.as< tokenizer::token > () }; }

    break;

  case 48:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::ARRAY, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append_children(yystack_[1].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 49:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::ARRAY, yystack_[1].value.as< tokenizer::token > () }; }

    break;

  case 50:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::ASSIGNMENT_LOCAL, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 51:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::ASSIGNMENT, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 52:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 53:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 54:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 55:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 56:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 57:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP0, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 58:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 59:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 60:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 61:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 62:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP1, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 63:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 64:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 65:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 66:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 67:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP2, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 68:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 69:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 70:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 71:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 72:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP3, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 73:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 74:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 75:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 76:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 77:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP4, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 78:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 79:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 80:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 81:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 82:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP5, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 83:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 84:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 85:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 86:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 87:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP6, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 88:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 89:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 90:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 91:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 92:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP7, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 93:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 94:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 95:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 96:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 97:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP8, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 98:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 99:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 100:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 101:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 102:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXP9, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::sqf::bison::astnode > ()); yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 103:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 104:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 105:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 106:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 107:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 108:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 109:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 110:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 111:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 112:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 113:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 114:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 115:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 116:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 117:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 118:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 119:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 120:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 121:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 122:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 123:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 124:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 125:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = ::sqf::parser::sqf::bison::astnode{ astkind::EXPU, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::sqf::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > ()); }

    break;

  case 126:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;

  case 127:

    { yylhs.value.as< ::sqf::parser::sqf::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::sqf::bison::astnode > (); }

    break;



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


  const signed char parser::yypact_ninf_ = -60;

  const signed char parser::yytable_ninf_ = -1;

  const short int
  parser::yypact_[] =
  {
     312,   -60,   -60,   371,    70,   430,   253,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   -60,   430,
      -9,   -60,   -60,   -60,    19,     9,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,    23,    24,    25,    33,   473,   474,   472,
     475,   476,   471,   430,    27,   -60,   -60,   -60,   -60,    11,
     -60,   135,   -60,    40,   -60,    -7,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   430,
     -60,   194,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   -60,   -60,    13,   -60,   -60,   -60,   430,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60
  };

  const unsigned char
  parser::yydefact_[] =
  {
       2,    39,    38,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,     0,    14,     0,
      35,    36,    37,    13,     0,     3,     4,   127,    40,    41,
       7,     8,    52,    53,    58,    63,    68,    73,    78,    83,
      88,    93,    98,     0,    35,   103,    47,     9,    10,     0,
      11,     0,    35,     0,    49,     0,    42,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   104,   105,     0,
       1,     5,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,    46,     0,    12,   126,    48,     0,
      51,     6,    54,    56,    57,    55,    59,    60,    61,    62,
      64,    65,    66,    67,    69,    70,    71,    72,    74,    75,
      76,    77,    79,    80,    81,    82,    84,    85,    86,    87,
      89,    90,    91,    92,    94,    95,    96,    97,    99,   100,
     101,   102,    50,    45,    43
  };

  const signed char
  parser::yypgoto_[] =
  {
     -60,   -60,    -2,   -44,   -54,    55,   -60,   -60,   -60,   -60,
     -60,    -5,   -59,   -53,   -46,   -30,   -29,    10,    26,    78,
      22,    74,     0
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    44,    45,    46,    70,   101,    47,    75,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62
  };

  const unsigned char
  parser::yytable_[] =
  {
      73,    76,    69,    65,   148,    99,   149,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,   146,   143,   100,
     193,    67,    68,    67,    68,    67,    68,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,   102,    98,
     106,   142,   110,   152,   153,   154,   155,   146,   103,   147,
     107,   114,   111,   156,   157,   158,   159,   151,   104,    71,
     108,   115,   112,    65,   160,   161,   162,   163,   105,   145,
     109,   116,   113,     1,     2,     3,     4,    66,     5,     0,
       6,   117,    67,    68,   164,   165,   166,   167,     0,   168,
     169,   170,   171,     0,   150,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,   172,   173,   174,   175,     0,   192,     1,     2,
       3,     4,   144,     5,   194,     6,     0,    67,    68,     0,
       0,     0,   176,   177,   178,   179,   184,   185,   186,   187,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,     1,     2,     3,
       4,     0,     5,     0,     6,     0,    67,    68,   180,   181,
     182,   183,   188,   189,   190,   191,     0,     0,     0,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,     1,     2,    63,     4,
       0,     5,     0,     6,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    72,    41,    42,    43,     1,     2,     3,     4,     0,
       5,     0,     6,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,     1,     2,    63,     4,     0,     5,
       0,     6,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    64,
      41,    42,    43,     1,     2,    63,     4,     0,     5,     0,
       6,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    72,    41,
      42,    43,   118,   126,   122,   138,     0,   130,     0,   134,
       0,     0,   119,   127,   123,   139,     0,   131,     0,   135,
       0,     0,   120,   128,   124,   140,     0,   132,     0,   136,
       0,     0,   121,   129,   125,   141,     0,   133,     0,   137
  };

  const short int
  parser::yycheck_[] =
  {
       5,     6,     4,     3,    11,    14,    13,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    71,     7,     0,
       7,    12,    13,    12,    13,    12,    13,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    15,    39,
      16,    14,    17,   102,   103,   104,   105,   101,    25,     9,
      26,    18,    27,   106,   107,   108,   109,   101,    35,     4,
      36,    28,    37,    63,   110,   111,   112,   113,    45,    71,
      46,    38,    47,     3,     4,     5,     6,     7,     8,    -1,
      10,    48,    12,    13,   114,   115,   116,   117,    -1,   118,
     119,   120,   121,    -1,    99,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,   122,   123,   124,   125,    -1,   142,     3,     4,
       5,     6,     7,     8,   149,    10,    -1,    12,    13,    -1,
      -1,    -1,   126,   127,   128,   129,   134,   135,   136,   137,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,     3,     4,     5,
       6,    -1,     8,    -1,    10,    -1,    12,    13,   130,   131,
     132,   133,   138,   139,   140,   141,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,     3,     4,     5,     6,
      -1,     8,    -1,    10,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,     3,     4,     5,     6,    -1,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,     3,     4,     5,     6,    -1,     8,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,     3,     4,     5,     6,    -1,     8,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    19,    21,    20,    24,    -1,    22,    -1,    23,
      -1,    -1,    29,    31,    30,    34,    -1,    32,    -1,    33,
      -1,    -1,    39,    41,    40,    44,    -1,    42,    -1,    43,
      -1,    -1,    49,    51,    50,    54,    -1,    52,    -1,    53
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     3,     4,     5,     6,     8,    10,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    63,    64,    65,    68,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,     5,    58,    84,     7,    12,    13,    64,
      66,    67,    58,    73,    11,    69,    73,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    14,
       0,    67,    15,    25,    35,    45,    16,    26,    36,    46,
      17,    27,    37,    47,    18,    28,    38,    48,    19,    29,
      39,    49,    20,    30,    40,    50,    21,    31,    41,    51,
      22,    32,    42,    52,    23,    33,    43,    53,    24,    34,
      44,    54,    14,     7,     7,    64,    66,     9,    11,    13,
      73,    65,    74,    74,    74,    74,    75,    75,    75,    75,
      76,    76,    76,    76,    77,    77,    77,    77,    78,    78,
      78,    78,    79,    79,    79,    79,    80,    80,    80,    80,
      81,    81,    81,    81,    82,    82,    82,    82,    83,    83,
      83,    83,    73,     7,    73
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    62,    63,    63,    64,    64,    64,    65,    65,    66,
      66,    67,    67,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    69,    69,    70,    70,    70,    70,    71,    71,
      72,    72,    73,    74,    74,    74,    74,    74,    75,    75,
      75,    75,    75,    76,    76,    76,    76,    76,    77,    77,
      77,    77,    77,    78,    78,    78,    78,    78,    79,    79,
      79,    79,    79,    80,    80,    80,    80,    80,    81,    81,
      81,    81,    81,    82,    82,    82,    82,    82,    83,    83,
      83,    83,    83,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     1,     2,     3,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     4,     3,     2,     3,     2,
       4,     3,     1,     1,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "NA", "error", "$undefined", "\"false\"", "\"true\"", "\"private\"",
  "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"[\"", "\"]\"", "\";\"", "\",\"",
  "\"=\"", "OPERATOR_B_0", "OPERATOR_B_1", "OPERATOR_B_2", "OPERATOR_B_3",
  "OPERATOR_B_4", "OPERATOR_B_5", "OPERATOR_B_6", "OPERATOR_B_7",
  "OPERATOR_B_8", "OPERATOR_B_9", "OPERATOR_BU_0", "OPERATOR_BU_1",
  "OPERATOR_BU_2", "OPERATOR_BU_3", "OPERATOR_BU_4", "OPERATOR_BU_5",
  "OPERATOR_BU_6", "OPERATOR_BU_7", "OPERATOR_BU_8", "OPERATOR_BU_9",
  "OPERATOR_BN_0", "OPERATOR_BN_1", "OPERATOR_BN_2", "OPERATOR_BN_3",
  "OPERATOR_BN_4", "OPERATOR_BN_5", "OPERATOR_BN_6", "OPERATOR_BN_7",
  "OPERATOR_BN_8", "OPERATOR_BN_9", "OPERATOR_BUN_0", "OPERATOR_BUN_1",
  "OPERATOR_BUN_2", "OPERATOR_BUN_3", "OPERATOR_BUN_4", "OPERATOR_BUN_5",
  "OPERATOR_BUN_6", "OPERATOR_BUN_7", "OPERATOR_BUN_8", "OPERATOR_BUN_9",
  "OPERATOR_U", "OPERATOR_N", "OPERATOR_UN", "IDENT", "NUMBER",
  "HEXNUMBER", "STRING", "$accept", "start", "statements", "statement",
  "separator", "separators", "value", "exp_list", "code", "array",
  "assignment", "expression", "exp0", "exp1", "exp2", "exp3", "exp4",
  "exp5", "exp6", "exp7", "exp8", "exp9", "expu", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   185,   185,   186,   188,   189,   190,   192,   193,   195,
     196,   198,   199,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   232,   233,   235,   236,   237,   238,   240,   241,
     243,   244,   246,   248,   249,   250,   251,   252,   254,   255,
     256,   257,   258,   260,   261,   262,   263,   264,   266,   267,
     268,   269,   270,   272,   273,   274,   275,   276,   278,   279,
     280,   281,   282,   284,   285,   286,   287,   288,   290,   291,
     292,   293,   294,   296,   297,   298,   299,   300,   302,   303,
     304,   305,   306,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332
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



} } } } //  ::sqf::parser::sqf::bison 




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
             
             return token.type == tokenizer::etoken::t_ident ? parser::make_IDENT(token, loc) : parser::make_NA(loc);
         }

         case tokenizer::etoken::t_string_double: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_string_single: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_number: return parser::make_NUMBER(token, loc);
         case tokenizer::etoken::t_hexadecimal: return parser::make_HEXNUMBER(token, loc);
         case tokenizer::etoken::s_equal: return parser::make_EQUAL(token, loc);
         default:
             return parser::make_NA(loc);
         }
     }
}
