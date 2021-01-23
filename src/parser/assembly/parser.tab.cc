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
#line 52 "parser.y" // lalr1.cc:413

    
    #include "../../runtime/runtime.h"
     namespace sqf::parser::assembly::bison
     {
          // Return the next token.
          parser::symbol_type yylex (::sqf::runtime::runtime &runtime, ::sqf::parser::assembly::tokenizer&);
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
namespace sqf { namespace parser { namespace assembly { namespace bison  {
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
  parser::parser (::sqf::parser::assembly::tokenizer &tokenizer_yyarg, ::sqf::parser::assembly::bison::astnode& result_yyarg, ::sqf::parser::assembly::parser& actual_yyarg, ::sqf::runtime::runtime &runtime_yyarg)
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
      case 29: // statements
      case 30: // statement
      case 31: // assignto
      case 32: // assigntolocal
      case 33: // getvariable
      case 34: // callunary
      case 35: // callnular
      case 36: // callbinary
      case 37: // push
      case 38: // endstatement
      case 39: // value
      case 40: // exp_list
      case 41: // code
      case 42: // array
        value.move< ::sqf::parser::assembly::bison::astnode > (that.value);
        break;

      case 4: // "false"
      case 5: // "true"
      case 6: // "{"
      case 7: // "}"
      case 8: // "("
      case 9: // ")"
      case 10: // "["
      case 11: // "]"
      case 12: // ";"
      case 13: // ","
      case 14: // "="
      case 15: // ASSIGN_TO
      case 16: // ASSIGN_TO_LOCAL
      case 17: // GET_VARIABLE
      case 18: // CALL_UNARY
      case 19: // CALL_NULAR
      case 20: // CALL_BINARY
      case 21: // PUSH
      case 22: // END_STATEMENT
      case 23: // IDENT
      case 24: // NUMBER
      case 25: // HEXNUMBER
      case 26: // STRING
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
      case 29: // statements
      case 30: // statement
      case 31: // assignto
      case 32: // assigntolocal
      case 33: // getvariable
      case 34: // callunary
      case 35: // callnular
      case 36: // callbinary
      case 37: // push
      case 38: // endstatement
      case 39: // value
      case 40: // exp_list
      case 41: // code
      case 42: // array
        value.copy< ::sqf::parser::assembly::bison::astnode > (that.value);
        break;

      case 4: // "false"
      case 5: // "true"
      case 6: // "{"
      case 7: // "}"
      case 8: // "("
      case 9: // ")"
      case 10: // "["
      case 11: // "]"
      case 12: // ";"
      case 13: // ","
      case 14: // "="
      case 15: // ASSIGN_TO
      case 16: // ASSIGN_TO_LOCAL
      case 17: // GET_VARIABLE
      case 18: // CALL_UNARY
      case 19: // CALL_NULAR
      case 20: // CALL_BINARY
      case 21: // PUSH
      case 22: // END_STATEMENT
      case 23: // IDENT
      case 24: // NUMBER
      case 25: // HEXNUMBER
      case 26: // STRING
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
      case 29: // statements
      case 30: // statement
      case 31: // assignto
      case 32: // assigntolocal
      case 33: // getvariable
      case 34: // callunary
      case 35: // callnular
      case 36: // callbinary
      case 37: // push
      case 38: // endstatement
      case 39: // value
      case 40: // exp_list
      case 41: // code
      case 42: // array
        yylhs.value.build< ::sqf::parser::assembly::bison::astnode > ();
        break;

      case 4: // "false"
      case 5: // "true"
      case 6: // "{"
      case 7: // "}"
      case 8: // "("
      case 9: // ")"
      case 10: // "["
      case 11: // "]"
      case 12: // ";"
      case 13: // ","
      case 14: // "="
      case 15: // ASSIGN_TO
      case 16: // ASSIGN_TO_LOCAL
      case 17: // GET_VARIABLE
      case 18: // CALL_UNARY
      case 19: // CALL_NULAR
      case 20: // CALL_BINARY
      case 21: // PUSH
      case 22: // END_STATEMENT
      case 23: // IDENT
      case 24: // NUMBER
      case 25: // HEXNUMBER
      case 26: // STRING
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
#line 116 "parser.y" // lalr1.cc:859
    { result = ::sqf::parser::assembly::bison::astnode{}; }
#line 672 "parser.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 117 "parser.y" // lalr1.cc:859
    { result = ::sqf::parser::assembly::bison::astnode{}; result.append(yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > ()); }
#line 678 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 119 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::STATEMENTS }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > ()); }
#line 684 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 120 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::assembly::bison::astnode > (); yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > ()); }
#line 690 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 122 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > (); }
#line 696 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 123 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > (); }
#line 702 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 124 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > (); }
#line 708 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 125 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > (); }
#line 714 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 126 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > (); }
#line 720 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 127 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > (); }
#line 726 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 128 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > (); }
#line 732 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 129 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > (); }
#line 738 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 130 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{}; }
#line 744 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 132 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::ASSIGN_TO, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(::sqf::parser::assembly::bison::astnode{ astkind::STRING, yystack_[1].value.as< tokenizer::token > () }); }
#line 750 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 134 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::ASSIGN_TO_LOCAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(::sqf::parser::assembly::bison::astnode{ astkind::STRING, yystack_[1].value.as< tokenizer::token > () });  }
#line 756 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 136 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::GET_VARIABLE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(::sqf::parser::assembly::bison::astnode{ astkind::STRING, yystack_[1].value.as< tokenizer::token > () });  }
#line 762 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 138 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::CALL_UNARY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(::sqf::parser::assembly::bison::astnode{ astkind::IDENT, yystack_[1].value.as< tokenizer::token > () }); }
#line 768 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 140 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::CALL_NULAR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(::sqf::parser::assembly::bison::astnode{ astkind::IDENT, yystack_[1].value.as< tokenizer::token > () }); }
#line 774 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 142 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::CALL_BINARY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(::sqf::parser::assembly::bison::astnode{ astkind::IDENT, yystack_[1].value.as< tokenizer::token > () }); }
#line 780 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 144 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::PUSH, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 786 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 146 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::END_STATEMENT, yystack_[0].value.as< tokenizer::token > () }; }
#line 792 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 149 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 798 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 150 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::NUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 804 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 151 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::HEXNUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 810 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 152 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::BOOLEAN_TRUE, yystack_[0].value.as< tokenizer::token > () }; }
#line 816 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 153 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::BOOLEAN_FALSE, yystack_[0].value.as< tokenizer::token > () }; }
#line 822 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 154 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > (); }
#line 828 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 155 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > (); }
#line 834 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 157 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::EXPRESSION_LIST }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > ()); }
#line 840 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 158 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = yystack_[2].value.as< ::sqf::parser::assembly::bison::astnode > (); yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::assembly::bison::astnode > ()); }
#line 846 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 160 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{  astkind::CODE, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append(yystack_[1].value.as< ::sqf::parser::assembly::bison::astnode > ()); }
#line 852 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 161 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{  astkind::CODE, yystack_[1].value.as< tokenizer::token > () }; }
#line 858 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 163 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::ARRAY, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::assembly::bison::astnode > ().append_children(yystack_[1].value.as< ::sqf::parser::assembly::bison::astnode > ()); }
#line 864 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 164 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::assembly::bison::astnode > () = ::sqf::parser::assembly::bison::astnode{ astkind::ARRAY, yystack_[1].value.as< tokenizer::token > () }; }
#line 870 "parser.tab.cc" // lalr1.cc:859
    break;


#line 874 "parser.tab.cc" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -23;

  const signed char parser::yytable_ninf_ = -4;

  const signed char
  parser::yypact_[] =
  {
       1,   -23,   -23,   -22,   -17,   -16,    -9,    10,    11,    26,
     -23,    15,    24,   -23,   -23,   -23,   -23,   -23,   -23,   -23,
     -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,
      46,     2,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,
     -23,    68,   -23,   -23,    -8,   -23,   -23,    26,   -23
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,     2,    14,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     4,     6,     7,     8,     9,    10,    11,
      12,    13,    15,    16,    17,    18,    19,    20,    27,    26,
       0,     0,    24,    25,    23,    21,    28,    29,     1,     5,
      33,     0,    35,    30,     0,    32,    34,     0,    31
  };

  const signed char
  parser::yypgoto_[] =
  {
     -23,   -23,   -19,   -12,   -23,   -23,   -23,   -23,   -23,   -23,
     -23,   -23,     7,   -23,   -23,   -23
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    35,    44,    36,    37
  };

  const signed char
  parser::yytable_[] =
  {
      39,     1,     2,    46,    22,    47,    28,    29,    30,    23,
      24,    41,    31,    42,    25,    38,     3,     4,     5,     6,
       7,     8,     9,    10,    -3,     2,    32,    33,    34,    39,
      28,    29,    30,    26,    27,     0,    31,     0,    43,     3,
       4,     5,     6,     7,     8,     9,    10,     2,     0,     0,
      32,    33,    34,    40,    48,     0,     0,     0,     0,     0,
       0,     3,     4,     5,     6,     7,     8,     9,    10,     2,
       0,     0,     0,     0,     0,    45,     0,     0,     0,     0,
       0,     0,     0,     3,     4,     5,     6,     7,     8,     9,
      10
  };

  const signed char
  parser::yycheck_[] =
  {
      12,     0,     1,    11,    26,    13,     4,     5,     6,    26,
      26,    30,    10,    11,    23,     0,    15,    16,    17,    18,
      19,    20,    21,    22,     0,     1,    24,    25,    26,    41,
       4,     5,     6,    23,    23,    -1,    10,    -1,    31,    15,
      16,    17,    18,    19,    20,    21,    22,     1,    -1,    -1,
      24,    25,    26,     7,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    20,    21,    22,     1,
      -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    16,    17,    18,    19,    20,    21,
      22
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     0,     1,    15,    16,    17,    18,    19,    20,    21,
      22,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    26,    26,    26,    23,    23,    23,     4,     5,
       6,    10,    24,    25,    26,    39,    41,    42,     0,    30,
       7,    29,    11,    39,    40,     7,    11,    13,    39
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    27,    28,    28,    29,    29,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    39,    39,    39,    39,    39,    39,
      40,    40,    41,    41,    42,    42
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     3,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "END_OF_FILE", "error", "$undefined", "INVALID", "\"false\"",
  "\"true\"", "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"[\"", "\"]\"",
  "\";\"", "\",\"", "\"=\"", "ASSIGN_TO", "ASSIGN_TO_LOCAL",
  "GET_VARIABLE", "CALL_UNARY", "CALL_NULAR", "CALL_BINARY", "PUSH",
  "END_STATEMENT", "IDENT", "NUMBER", "HEXNUMBER", "STRING", "$accept",
  "start", "statements", "statement", "assignto", "assigntolocal",
  "getvariable", "callunary", "callnular", "callbinary", "push",
  "endstatement", "value", "exp_list", "code", "array", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,   116,   116,   117,   119,   120,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   132,   134,   136,   138,   140,
     142,   144,   146,   149,   150,   151,   152,   153,   154,   155,
     157,   158,   160,   161,   163,   164
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
} } } } //  ::sqf::parser::assembly::bison 
#line 1284 "parser.tab.cc" // lalr1.cc:1167
#line 167 "parser.y" // lalr1.cc:1168

#ifdef __GNUG__
    #pragma GCC diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#include "assembly_parser.h"
namespace sqf::parser::assembly::bison
{
    void parser::error(const location_type& loc, const std::string& msg)
    {
        actual.__log(logmessage::sqf::ParseError({ *loc.begin.filename, loc.begin.line, loc.begin.column }, msg));
    }
    inline parser::symbol_type yylex (::sqf::runtime::runtime& runtime, ::sqf::parser::assembly::tokenizer& tokenizer)
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
         case tokenizer::etoken::t_true: return parser::make_TRUE(token, loc);
         case tokenizer::etoken::t_assign_to: return parser::make_ASSIGN_TO(token, loc);
         case tokenizer::etoken::t_assign_to_local: return parser::make_ASSIGN_TO_LOCAL(token, loc);
         case tokenizer::etoken::t_get_variable: return parser::make_GET_VARIABLE(token, loc);
         case tokenizer::etoken::t_call_unary: return parser::make_CALL_UNARY(token, loc);
         case tokenizer::etoken::t_call_nular: return parser::make_CALL_NULAR(token, loc);
         case tokenizer::etoken::t_call_binary: return parser::make_CALL_BINARY(token, loc);
         case tokenizer::etoken::t_push: return parser::make_PUSH(token, loc);
         case tokenizer::etoken::t_end_statement: return parser::make_END_STATEMENT(token, loc);

         case tokenizer::etoken::s_curlyo: return parser::make_CURLYO(token, loc);
         case tokenizer::etoken::s_curlyc: return parser::make_CURLYC(token, loc);
         case tokenizer::etoken::s_roundo: return parser::make_ROUNDO(token, loc);
         case tokenizer::etoken::s_roundc: return parser::make_ROUNDC(token, loc);
         case tokenizer::etoken::s_edgeo: return parser::make_SQUAREO(token, loc);
         case tokenizer::etoken::s_edgec: return parser::make_SQUAREC(token, loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(token, loc);

         case tokenizer::etoken::t_operator:
         case tokenizer::etoken::t_ident: return parser::make_IDENT(token, loc);

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
