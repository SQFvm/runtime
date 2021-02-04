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
#line 100 "parser.y" // lalr1.cc:413

    
    #include "../../runtime/runtime.h"
     namespace sqf::parser::config::bison
     {
          // Return the next token.
          parser::symbol_type yylex (::sqf::parser::config::tokenizer&);
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
namespace sqf { namespace parser { namespace config { namespace bison  {
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
  parser::parser (::sqf::parser::config::tokenizer &tokenizer_yyarg, ::sqf::parser::config::bison::astnode& result_yyarg, ::sqf::parser::config::parser& actual_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      tokenizer (tokenizer_yyarg),
      result (result_yyarg),
      actual (actual_yyarg)
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
      case 25: // topstatements
      case 26: // topstatement
      case 27: // statements
      case 28: // statement
      case 29: // classdef
      case 30: // deleteclass
      case 31: // classbody
      case 32: // field
      case 33: // ident
      case 34: // string
      case 35: // number
      case 36: // array
      case 37: // arrayvalue
      case 38: // arrayvaluelist
      case 39: // anyval
      case 40: // anyarr
      case 41: // anyp
      case 42: // anyarray
      case 43: // anyvalue
        value.move< ::sqf::parser::config::bison::astnode > (that.value);
        break;

      case 4: // "delete"
      case 5: // "class"
      case 6: // "{"
      case 7: // "}"
      case 8: // "("
      case 9: // ")"
      case 10: // "["
      case 11: // "]"
      case 12: // ":"
      case 13: // ";"
      case 14: // ","
      case 15: // "+="
      case 16: // "="
      case 17: // IDENT
      case 18: // NUMBER
      case 19: // HEXNUMBER
      case 20: // STRING
      case 21: // ANY
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
      case 25: // topstatements
      case 26: // topstatement
      case 27: // statements
      case 28: // statement
      case 29: // classdef
      case 30: // deleteclass
      case 31: // classbody
      case 32: // field
      case 33: // ident
      case 34: // string
      case 35: // number
      case 36: // array
      case 37: // arrayvalue
      case 38: // arrayvaluelist
      case 39: // anyval
      case 40: // anyarr
      case 41: // anyp
      case 42: // anyarray
      case 43: // anyvalue
        value.copy< ::sqf::parser::config::bison::astnode > (that.value);
        break;

      case 4: // "delete"
      case 5: // "class"
      case 6: // "{"
      case 7: // "}"
      case 8: // "("
      case 9: // ")"
      case 10: // "["
      case 11: // "]"
      case 12: // ":"
      case 13: // ";"
      case 14: // ","
      case 15: // "+="
      case 16: // "="
      case 17: // IDENT
      case 18: // NUMBER
      case 19: // HEXNUMBER
      case 20: // STRING
      case 21: // ANY
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
      case 25: // topstatements
      case 26: // topstatement
      case 27: // statements
      case 28: // statement
      case 29: // classdef
      case 30: // deleteclass
      case 31: // classbody
      case 32: // field
      case 33: // ident
      case 34: // string
      case 35: // number
      case 36: // array
      case 37: // arrayvalue
      case 38: // arrayvaluelist
      case 39: // anyval
      case 40: // anyarr
      case 41: // anyp
      case 42: // anyarray
      case 43: // anyvalue
        yylhs.value.build< ::sqf::parser::config::bison::astnode > ();
        break;

      case 4: // "delete"
      case 5: // "class"
      case 6: // "{"
      case 7: // "}"
      case 8: // "("
      case 9: // ")"
      case 10: // "["
      case 11: // "]"
      case 12: // ":"
      case 13: // ";"
      case 14: // ","
      case 15: // "+="
      case 16: // "="
      case 17: // IDENT
      case 18: // NUMBER
      case 19: // HEXNUMBER
      case 20: // STRING
      case 21: // ANY
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
#line 157 "parser.y" // lalr1.cc:859
    { result = ::sqf::parser::config::bison::astnode{}; }
#line 671 "parser.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 158 "parser.y" // lalr1.cc:859
    { result = ::sqf::parser::config::bison::astnode{}; result.append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 677 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 159 "parser.y" // lalr1.cc:859
    { result = ::sqf::parser::config::bison::astnode{}; }
#line 683 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 160 "parser.y" // lalr1.cc:859
    { result = ::sqf::parser::config::bison::astnode{}; result.append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 689 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 165 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::STATEMENTS }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 695 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 166 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 701 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 167 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[2].value.as< ::sqf::parser::config::bison::astnode > (); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 707 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 169 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 713 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 170 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 719 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 172 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::STATEMENTS }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 725 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 173 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 731 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 174 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[2].value.as< ::sqf::parser::config::bison::astnode > (); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 737 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 176 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 743 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 177 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 749 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 178 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 755 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 181 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::CLASS_DEF, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 761 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 182 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::CLASS_DEF_EXT, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::config::bison::astnode > ()); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 767 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 183 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::CLASS, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[1].value.as< ::sqf::parser::config::bison::astnode > ()); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 773 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 184 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::CLASS_EXT, yystack_[4].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[3].value.as< ::sqf::parser::config::bison::astnode > ()); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[1].value.as< ::sqf::parser::config::bison::astnode > ()); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 779 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 186 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::DELETE_CLASS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 785 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 189 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::STATEMENTS }; }
#line 791 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 190 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 797 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 193 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::FIELD, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::config::bison::astnode > ()); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 803 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 194 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::FIELD, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::config::bison::astnode > ()); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 809 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 195 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::FIELD, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[2].value.as< ::sqf::parser::config::bison::astnode > ()); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 815 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 196 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::FIELD_ARRAY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[4].value.as< ::sqf::parser::config::bison::astnode > ()); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 821 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 197 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::FIELD_ARRAY_APPEND, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[4].value.as< ::sqf::parser::config::bison::astnode > ()); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 827 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 200 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::IDENT, yystack_[0].value.as< tokenizer::token > () }; }
#line 833 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 202 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 839 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 204 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::NUMBER_DECIMAL, yystack_[0].value.as< tokenizer::token > () }; }
#line 845 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 205 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::NUMBER_HEXADECIMAL, yystack_[0].value.as< tokenizer::token > () }; }
#line 851 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 207 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ARRAY }; }
#line 857 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 208 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 863 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 210 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 869 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 211 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 875 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 212 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 881 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 213 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 887 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 215 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ARRAY }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 893 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 216 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[2].value.as< ::sqf::parser::config::bison::astnode > (); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 899 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 218 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 905 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 219 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 911 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 220 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 917 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 221 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 923 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 222 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 929 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 223 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 935 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 224 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 941 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 226 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[0].value.as< ::sqf::parser::config::bison::astnode > (); }
#line 947 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 227 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 953 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 228 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 959 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 229 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 965 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 231 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 971 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 232 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 977 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 233 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 983 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 234 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 989 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 235 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 995 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 236 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 1001 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 237 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 1007 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 238 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANY, yystack_[0].value.as< tokenizer::token > () }; }
#line 1013 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 240 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANYSTRING }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 1019 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 241 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::config::bison::astnode > (); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 1025 "parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 243 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = ::sqf::parser::config::bison::astnode{ astkind::ANYSTRING }; yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 1031 "parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 244 "parser.y" // lalr1.cc:859
    { yylhs.value.as< ::sqf::parser::config::bison::astnode > () = yystack_[1].value.as< ::sqf::parser::config::bison::astnode > (); yylhs.value.as< ::sqf::parser::config::bison::astnode > ().append(yystack_[0].value.as< ::sqf::parser::config::bison::astnode > ()); }
#line 1037 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1041 "parser.tab.cc" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -34;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
       4,   -34,   -10,   -10,   -34,    10,    -2,    -1,   -34,   -34,
     -34,   -34,   -34,    67,   -34,   -34,    -1,    -2,   102,   -10,
     -34,   -34,   -34,    68,   -34,   -34,   -34,   -34,    45,    27,
     -34,   107,    32,    66,   -34,   -34,    75,   -34,   -34,   -34,
     -34,   -34,   -34,   -34,   -34,   -34,   -34,   -34,   -34,   -34,
     -34,     9,    51,    51,   -34,   -34,   -34,   -34,   -34,   -34,
     -34,   -34,    30,   -34,   -34,   -34,   -34,   -34,   -34,   -34,
      49,   -34,    84,   -34,    48,   -34,   -34,   -34,   -34,   -34,
     -34
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,     2,     0,     0,     6,     0,     4,     3,     8,    11,
      12,    31,    23,    19,     1,     7,     5,     9,     0,     0,
      21,    10,    24,     0,    13,    16,    18,    17,     0,    20,
      25,    14,     0,     0,    22,    15,     0,    55,    54,    57,
      58,    59,    60,    56,    33,    34,    32,    61,    26,    27,
      64,    28,     0,     0,    47,    48,    49,    44,    45,    46,
      65,    43,     0,    30,    29,    35,    37,    38,    39,    41,
       0,    62,    40,    36,     0,    51,    52,    53,    63,    50,
      42
  };

  const signed char
  parser::yypgoto_[] =
  {
     -34,   -34,     1,    56,    21,   -34,    41,    13,    14,    69,
     -34,     3,    47,    64,    40,     0,   -34,   -34,   -34,   -33,
     -34,   -34
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     5,    17,     7,     8,    23,    24,     9,    10,    20,
      27,    28,    66,    67,    68,    69,    70,    60,    78,    71,
      72,    51
  };

  const unsigned char
  parser::yytable_[] =
  {
      50,     6,     2,     3,     1,    12,    13,    11,     2,     3,
      14,    15,     4,    37,    38,    54,    55,     4,    61,    39,
      40,    41,    29,    56,    31,    42,    43,    57,    58,    59,
      47,    25,    26,    18,    37,    38,    62,    65,    21,    79,
      39,    40,    41,    36,    25,    26,    42,    43,    44,    45,
      46,    47,    37,    38,    62,    32,    73,    62,    39,    40,
      41,    33,    16,    74,    42,    43,    44,    45,    46,    47,
      37,    38,    35,    18,    80,    30,    39,    40,    41,    19,
      48,     4,    42,    43,    44,    45,    46,    47,    37,    38,
      52,    53,    63,    64,    39,    40,    41,    49,    34,     0,
      42,    43,    75,    76,    77,    47,     2,     3,     0,    22,
       0,     2,     3,     0,     0,     0,     0,     0,     0,    11,
      15,     0,     0,     0,    11
  };

  const signed char
  parser::yycheck_[] =
  {
      33,     0,     4,     5,     0,     2,     3,    17,     4,     5,
       0,    13,    13,     4,     5,     6,     7,    13,    51,    10,
      11,    12,    19,    14,    23,    16,    17,    18,    19,    20,
      21,    18,    18,     6,     4,     5,     6,     7,    17,    72,
      10,    11,    12,    11,    31,    31,    16,    17,    18,    19,
      20,    21,     4,     5,     6,    10,     7,     6,    10,    11,
      12,    16,     6,    14,    16,    17,    18,    19,    20,    21,
       4,     5,    31,     6,    74,     7,    10,    11,    12,    12,
      33,    13,    16,    17,    18,    19,    20,    21,     4,     5,
      15,    16,    52,    53,    10,    11,    12,    33,    29,    -1,
      16,    17,    18,    19,    20,    21,     4,     5,    -1,     7,
      -1,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      13,    -1,    -1,    -1,    17
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     0,     4,     5,    13,    23,    24,    25,    26,    29,
      30,    17,    33,    33,     0,    13,    25,    24,     6,    12,
      31,    26,     7,    27,    28,    29,    30,    32,    33,    33,
       7,    24,    10,    16,    31,    28,    11,     4,     5,    10,
      11,    12,    16,    17,    18,    19,    20,    21,    34,    35,
      41,    43,    15,    16,     6,     7,    14,    18,    19,    20,
      39,    41,     6,    36,    36,     7,    34,    35,    36,    37,
      38,    41,    42,     7,    14,    18,    19,    20,    40,    41,
      37
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    22,    23,    23,    23,    23,    24,    24,    25,    25,
      25,    26,    26,    27,    27,    27,    28,    28,    28,    29,
      29,    29,    29,    30,    31,    31,    32,    32,    32,    32,
      32,    33,    34,    35,    35,    36,    36,    37,    37,    37,
      37,    38,    38,    39,    39,    39,    39,    39,    39,    39,
      40,    40,    40,    40,    41,    41,    41,    41,    41,    41,
      41,    41,    42,    42,    43,    43
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     1,     2,     1,     2,
       3,     1,     1,     1,     2,     3,     1,     1,     1,     2,
       4,     3,     5,     2,     2,     3,     3,     3,     3,     5,
       5,     1,     1,     1,     1,     2,     3,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "END_OF_FILE", "error", "$undefined", "INVALID", "\"delete\"",
  "\"class\"", "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"[\"", "\"]\"",
  "\":\"", "\";\"", "\",\"", "\"+=\"", "\"=\"", "IDENT", "NUMBER",
  "HEXNUMBER", "STRING", "ANY", "$accept", "start", "separators",
  "topstatements", "topstatement", "statements", "statement", "classdef",
  "deleteclass", "classbody", "field", "ident", "string", "number",
  "array", "arrayvalue", "arrayvaluelist", "anyval", "anyarr", "anyp",
  "anyarray", "anyvalue", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,   157,   157,   158,   159,   160,   162,   163,   165,   166,
     167,   169,   170,   172,   173,   174,   176,   177,   178,   181,
     182,   183,   184,   186,   189,   190,   193,   194,   195,   196,
     197,   200,   202,   204,   205,   207,   208,   210,   211,   212,
     213,   215,   216,   218,   219,   220,   221,   222,   223,   224,
     226,   227,   228,   229,   231,   232,   233,   234,   235,   236,
     237,   238,   240,   241,   243,   244
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
} } } } //  ::sqf::parser::config::bison 
#line 1480 "parser.tab.cc" // lalr1.cc:1167
#line 248 "parser.y" // lalr1.cc:1168

#ifdef __GNUG__
    #pragma GCC diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#include "config_parser.hpp"
namespace sqf::parser::config::bison
{
    void parser::error(const location_type& loc, const std::string& msg)
    {
        actual.__log(logmessage::config::ParseError({ *loc.begin.filename, loc.begin.line, loc.begin.column }, msg));
    }
    inline parser::symbol_type yylex(::sqf::parser::config::tokenizer& tokenizer)
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
         case tokenizer::etoken::m_line: return yylex(tokenizer);
         case tokenizer::etoken::i_comment_line: return yylex(tokenizer);
         case tokenizer::etoken::i_comment_block: return yylex(tokenizer);
         case tokenizer::etoken::i_whitespace: return yylex(tokenizer);
         
         case tokenizer::etoken::t_class: return parser::make_CLASS(token, loc);
         case tokenizer::etoken::t_delete: return parser::make_DELETE(token, loc);

         case tokenizer::etoken::s_curlyo: return parser::make_CURLYO(token, loc);
         case tokenizer::etoken::s_curlyc: return parser::make_CURLYC(token, loc);
         case tokenizer::etoken::s_edgeo: return parser::make_SQUAREO(token, loc);
         case tokenizer::etoken::s_edgec: return parser::make_SQUAREC(token, loc);
         case tokenizer::etoken::s_colon: return parser::make_COLON(token, loc);
         case tokenizer::etoken::s_semicolon: return parser::make_SEMICOLON(token, loc);
         case tokenizer::etoken::s_comma: return parser::make_COMMA(token, loc);
         case tokenizer::etoken::t_ident: return parser::make_IDENT(token, loc);

         case tokenizer::etoken::t_string_double: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_string_single: return parser::make_STRING(token, loc);
         case tokenizer::etoken::t_number: return parser::make_NUMBER(token, loc);
         case tokenizer::etoken::t_hexadecimal: return parser::make_HEXNUMBER(token, loc);
         case tokenizer::etoken::t_plus_equal: return parser::make_PLUSEQUAL(token, loc);
         case tokenizer::etoken::s_equal: return parser::make_EQUAL(token, loc);
         default:
             return parser::make_ANY(token, loc);
         }
     }
}
