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
#line 114 "parser.y" // lalr1.cc:413

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
      case 64: // filehead
      case 65: // statements
      case 66: // statement
      case 67: // assignment
      case 68: // vardecl
      case 69: // funcdecl
      case 70: // function
      case 71: // funchead
      case 72: // arglist
      case 73: // argitem
      case 74: // codeblock
      case 75: // if
      case 76: // for
      case 77: // while
      case 78: // trycatch
      case 79: // switch
      case 80: // caselist
      case 81: // case
      case 82: // exp01
      case 83: // exp02
      case 84: // exp03
      case 85: // exp04
      case 86: // exp05
      case 87: // exp06
      case 88: // exp07
      case 89: // exp08
      case 90: // exp09
      case 91: // arrget
      case 92: // expp
      case 93: // value
      case 94: // array
      case 95: // explist
      case 96: // format_string
      case 97: // format_string_match
        value.move< sqf::sqc::bison::astnode > (that.value);
        break;

      case 37: // "be"
      case 38: // "="
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
      case 59: // FORMAT_STRING_START
      case 60: // FORMAT_STRING_CONTINUE
      case 61: // FORMAT_STRING_FINAL
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
      case 64: // filehead
      case 65: // statements
      case 66: // statement
      case 67: // assignment
      case 68: // vardecl
      case 69: // funcdecl
      case 70: // function
      case 71: // funchead
      case 72: // arglist
      case 73: // argitem
      case 74: // codeblock
      case 75: // if
      case 76: // for
      case 77: // while
      case 78: // trycatch
      case 79: // switch
      case 80: // caselist
      case 81: // case
      case 82: // exp01
      case 83: // exp02
      case 84: // exp03
      case 85: // exp04
      case 86: // exp05
      case 87: // exp06
      case 88: // exp07
      case 89: // exp08
      case 90: // exp09
      case 91: // arrget
      case 92: // expp
      case 93: // value
      case 94: // array
      case 95: // explist
      case 96: // format_string
      case 97: // format_string_match
        value.copy< sqf::sqc::bison::astnode > (that.value);
        break;

      case 37: // "be"
      case 38: // "="
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
      case 59: // FORMAT_STRING_START
      case 60: // FORMAT_STRING_CONTINUE
      case 61: // FORMAT_STRING_FINAL
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
      case 64: // filehead
      case 65: // statements
      case 66: // statement
      case 67: // assignment
      case 68: // vardecl
      case 69: // funcdecl
      case 70: // function
      case 71: // funchead
      case 72: // arglist
      case 73: // argitem
      case 74: // codeblock
      case 75: // if
      case 76: // for
      case 77: // while
      case 78: // trycatch
      case 79: // switch
      case 80: // caselist
      case 81: // case
      case 82: // exp01
      case 83: // exp02
      case 84: // exp03
      case 85: // exp04
      case 86: // exp05
      case 87: // exp06
      case 88: // exp07
      case 89: // exp08
      case 90: // exp09
      case 91: // arrget
      case 92: // expp
      case 93: // value
      case 94: // array
      case 95: // explist
      case 96: // format_string
      case 97: // format_string_match
        yylhs.value.build< sqf::sqc::bison::astnode > ();
        break;

      case 37: // "be"
      case 38: // "="
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
      case 59: // FORMAT_STRING_START
      case 60: // FORMAT_STRING_CONTINUE
      case 61: // FORMAT_STRING_FINAL
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
#line 212 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); result.append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 728 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 213 "parser.y" // lalr1.cc:859
    { result = sqf::sqc::bison::astnode{}; result.append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 734 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 216 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 740 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 219 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 746 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 220 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 752 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 223 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 758 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 224 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::RETURN, tokenizer.create_token() }; }
#line 764 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 225 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::THROW, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 770 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 226 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 776 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 227 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 782 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 228 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 788 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 229 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 794 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 230 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 800 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 231 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 806 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 232 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 812 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 233 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 818 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 234 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 824 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 235 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 830 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 236 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; }
#line 836 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 239 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ASSIGNMENT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 842 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 240 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_SET, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 848 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 243 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 854 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 244 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 860 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 245 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 866 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 246 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 872 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 247 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DECLARATION, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 878 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 248 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORWARD_DECLARATION, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 884 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 251 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 890 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 252 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FINAL_FUNCTION_DECLARATION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 896 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 255 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FUNCTION, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 902 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 258 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; }
#line 908 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 259 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGLIST, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 914 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 262 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 920 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 263 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 926 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 264 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 932 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 266 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM, yystack_[0].value.as< tokenizer::token > () }; }
#line 938 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 267 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_DEFAULT, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 944 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 268 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_TYPE, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); }
#line 950 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 269 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::ARGITEM_TYPE_DEFAULT, yystack_[2].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 956 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 271 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 962 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 272 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; }
#line 968 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 273 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CODEBLOCK, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 974 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 276 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IF, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 980 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 277 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::IFELSE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 986 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 280 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOR, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 992 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 281 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FORSTEP, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[7].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 998 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 282 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::FOREACH, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1004 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 285 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::WHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1010 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 286 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::DOWHILE, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1016 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 289 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::TRYCATCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1022 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 292 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SWITCH, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1028 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 295 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1034 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 296 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1040 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 299 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1046 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 300 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE, yystack_[0].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1052 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 301 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::CASE_DEFAULT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1058 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 304 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1064 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 305 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_TERNARY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[4].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1070 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 307 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1076 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 308 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_OR, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1082 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 310 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1088 "parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 311 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_AND, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1094 "parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 313 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1100 "parser.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 314 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1106 "parser.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 315 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUALEXACT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1112 "parser.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 316 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_EQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1118 "parser.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 317 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOTEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1124 "parser.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 319 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1130 "parser.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 320 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1136 "parser.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 321 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_LESSTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1142 "parser.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 322 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHAN, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1148 "parser.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 323 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_GREATERTHANEQUAL, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1154 "parser.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 325 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1160 "parser.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 326 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_PLUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1166 "parser.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 327 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MINUS, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1172 "parser.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 329 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1178 "parser.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 330 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_MULTIPLY, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1184 "parser.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 331 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_DIVIDE, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1190 "parser.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 332 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_REMAINDER, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1196 "parser.tab.cc" // lalr1.cc:859
    break;

  case 82:
#line 334 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1202 "parser.tab.cc" // lalr1.cc:859
    break;

  case 83:
#line 335 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_NOT, yystack_[1].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ());  }
#line 1208 "parser.tab.cc" // lalr1.cc:859
    break;

  case 84:
#line 337 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1214 "parser.tab.cc" // lalr1.cc:859
    break;

  case 85:
#line 338 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_BINARY, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[5].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1220 "parser.tab.cc" // lalr1.cc:859
    break;

  case 86:
#line 339 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1226 "parser.tab.cc" // lalr1.cc:859
    break;

  case 87:
#line 341 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_ARRAY_GET, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1232 "parser.tab.cc" // lalr1.cc:859
    break;

  case 88:
#line 343 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[1].value.as< sqf::sqc::bison::astnode > (); }
#line 1238 "parser.tab.cc" // lalr1.cc:859
    break;

  case 89:
#line 344 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::OP_UNARY, yystack_[3].value.as< tokenizer::token > () }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[3].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1244 "parser.tab.cc" // lalr1.cc:859
    break;

  case 90:
#line 345 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::GET_VARIABLE, yystack_[0].value.as< tokenizer::token > () }; }
#line 1250 "parser.tab.cc" // lalr1.cc:859
    break;

  case 91:
#line 346 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1256 "parser.tab.cc" // lalr1.cc:859
    break;

  case 92:
#line 348 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1262 "parser.tab.cc" // lalr1.cc:859
    break;

  case 93:
#line 349 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_STRING, yystack_[0].value.as< tokenizer::token > () }; }
#line 1268 "parser.tab.cc" // lalr1.cc:859
    break;

  case 94:
#line 350 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1274 "parser.tab.cc" // lalr1.cc:859
    break;

  case 95:
#line 351 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = yystack_[0].value.as< sqf::sqc::bison::astnode > (); }
#line 1280 "parser.tab.cc" // lalr1.cc:859
    break;

  case 96:
#line 352 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NUMBER, yystack_[0].value.as< tokenizer::token > () }; }
#line 1286 "parser.tab.cc" // lalr1.cc:859
    break;

  case 97:
#line 353 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_TRUE, tokenizer.create_token() }; }
#line 1292 "parser.tab.cc" // lalr1.cc:859
    break;

  case 98:
#line 354 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_FALSE, tokenizer.create_token() }; }
#line 1298 "parser.tab.cc" // lalr1.cc:859
    break;

  case 99:
#line 355 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() }; }
#line 1304 "parser.tab.cc" // lalr1.cc:859
    break;

  case 100:
#line 357 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; }
#line 1310 "parser.tab.cc" // lalr1.cc:859
    break;

  case 101:
#line 358 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::VAL_ARRAY, tokenizer.create_token() }; yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1316 "parser.tab.cc" // lalr1.cc:859
    break;

  case 102:
#line 360 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1322 "parser.tab.cc" // lalr1.cc:859
    break;

  case 103:
#line 361 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); }
#line 1328 "parser.tab.cc" // lalr1.cc:859
    break;

  case 104:
#line 362 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1334 "parser.tab.cc" // lalr1.cc:859
    break;

  case 105:
#line 364 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SVAL_FORMAT_STRING }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ());}
#line 1340 "parser.tab.cc" // lalr1.cc:859
    break;

  case 106:
#line 365 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{ astkind::SVAL_FORMAT_STRING }; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1346 "parser.tab.cc" // lalr1.cc:859
    break;

  case 107:
#line 367 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() }); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1352 "parser.tab.cc" // lalr1.cc:859
    break;

  case 108:
#line 368 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[2].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< tokenizer::token > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append_children(yystack_[0].value.as< sqf::sqc::bison::astnode > ()); }
#line 1358 "parser.tab.cc" // lalr1.cc:859
    break;

  case 109:
#line 369 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(sqf::sqc::bison::astnode{ astkind::VAL_NIL, tokenizer.create_token() });yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ());  }
#line 1364 "parser.tab.cc" // lalr1.cc:859
    break;

  case 110:
#line 370 "parser.y" // lalr1.cc:859
    { yylhs.value.as< sqf::sqc::bison::astnode > () = sqf::sqc::bison::astnode{}; yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[1].value.as< sqf::sqc::bison::astnode > ()); yylhs.value.as< sqf::sqc::bison::astnode > ().append(yystack_[0].value.as< tokenizer::token > ()); }
#line 1370 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1374 "parser.tab.cc" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -78;

  const signed char parser::yytable_ninf_ = -58;

  const short int
  parser::yypact_[] =
  {
     215,   -78,    91,   593,   -31,   -21,    23,   -78,   -20,     6,
     452,   452,     9,   -78,   -78,    17,   -25,    27,   593,   575,
     -78,   122,   -78,   -13,   -78,    32,    68,   570,   -78,   275,
      37,    53,   -78,   -78,   -78,   -78,   -78,   -78,   -78,    66,
      58,    62,    69,   -34,    -5,   -22,    18,    78,    77,    83,
     -78,   -78,   -78,    17,   -78,    92,    90,   -78,    94,    28,
     -17,    17,   452,    71,    73,   112,   593,   511,   -78,   106,
     118,   593,   -78,    35,   593,   108,   -78,   102,   107,    78,
     593,   593,    32,   -78,    15,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   593,   593,   593,   593,   593,   593,   593,   593,
     593,   593,   593,   593,   593,   593,   593,   593,   593,   593,
      84,   -78,   -78,   593,   593,   -78,   -23,   110,   109,   452,
     -78,    17,   111,   593,   124,   -78,   128,   123,   130,   127,
     593,   593,   132,   -78,   593,   -78,   133,   -78,   -78,    32,
     -78,   116,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   134,   -78,   135,
     -78,   -78,   593,   131,   -78,   115,   -78,   452,   593,   159,
     452,   -78,   593,   117,   149,   -78,   -78,   452,   -78,   -78,
     -78,   593,   -78,   593,   -78,   593,   -78,   -78,   152,   593,
     171,   155,   157,    61,   -78,   -78,   164,   -78,   452,   393,
     452,   -78,   452,   593,   151,   168,    61,   -78,   -78,   593,
     -78,   -78,   -78,   153,   452,   -78,   -78,   452,   334,   -78,
     -78,   -78
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,    21,     0,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,    99,    97,     0,     0,     0,     0,     0,
      20,     0,    96,    90,    93,   105,     0,     0,     4,     0,
       0,     0,    12,    92,    13,    14,    15,    16,    17,     0,
      59,    61,    63,    65,    70,    75,    78,    82,    86,    84,
      91,    94,    95,     0,     9,    90,     0,    86,     0,    26,
       0,     0,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     5,    29,     0,     0,   100,   102,     0,    83,
       0,     0,     0,   109,     0,   106,     1,     3,     7,    18,
      11,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     8,    10,     0,     0,    33,    38,     0,    35,     0,
      32,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,     0,    88,   103,   101,     0,    22,   107,     0,
     110,     0,    62,    64,    66,    68,    67,    69,    72,    71,
      74,    73,    76,    77,    80,    79,    81,     0,    23,     0,
      25,    24,     0,    40,    34,    36,    30,     0,     0,     0,
       0,    44,     0,     0,     0,    28,    27,     0,   104,    89,
     108,     0,    87,     0,    39,     0,    37,    31,     0,     0,
      45,     0,     0,     0,    50,    60,     0,    41,     0,     0,
       0,    51,     0,     0,     0,     0,    54,    85,    49,     0,
      47,    46,    52,     0,     0,    53,    55,     0,     0,    58,
      48,    56
  };

  const short int
  parser::yypgoto_[] =
  {
     -78,   -78,   -78,   -15,     4,   -78,   -78,   -78,   -78,   -14,
      36,   -78,    -9,   -78,   -78,   -78,   -78,   -78,    -3,   -78,
       3,   -78,   114,   119,   -47,     2,    -7,   -27,   183,     0,
     -78,   -78,   -78,   -77,   -78,   -75
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    26,    27,    28,    68,    30,    31,    32,    33,    62,
     117,   118,    69,    34,    35,    36,    37,    38,   205,   206,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    57,
      49,    50,    51,    78,    52,    85
  };

  const short int
  parser::yytable_[] =
  {
      48,    72,    70,   136,    29,    56,    58,   138,    60,    64,
      48,    48,    87,   115,    88,   162,    80,    95,    96,    97,
      98,    75,    77,   103,   104,    81,    59,    48,    84,    48,
      63,    29,    73,    29,   163,    66,    61,    65,    71,    53,
     116,     7,    99,   100,   101,   102,    60,   119,   144,   145,
     146,   147,   126,   120,    13,    14,    74,   178,   105,   106,
     107,    18,    48,    19,   180,   113,   114,    48,    86,   124,
      89,    29,   130,   131,   129,   139,   140,   132,   154,   155,
     156,   203,   204,    77,   137,    84,    90,    21,    22,    55,
      24,    25,    82,    83,    92,   141,   152,   153,    53,    91,
       7,   148,   149,   150,   151,    93,   196,   167,    94,   108,
     166,   157,   158,    13,    14,   109,   160,   161,   110,    48,
      18,    80,    19,   111,    54,   123,   169,   112,   121,    53,
     122,     7,   127,   175,   176,   128,   134,    77,   133,   135,
     164,   159,    84,   165,    13,    14,    21,    22,    55,    24,
      25,    18,   172,    19,   170,   168,   171,   174,   187,   173,
     181,   190,   177,   179,   183,   184,   182,    48,   194,   185,
      48,   188,   116,   189,   192,   191,   193,    48,    22,    55,
      24,    25,   198,   200,   195,   201,    77,   202,   197,   208,
     210,   211,   199,   212,   207,   214,   215,   218,    48,    48,
      48,   186,    48,   216,    79,   219,   213,   142,   220,   221,
       0,     0,   217,   143,    48,    -2,     1,    48,    48,     2,
       3,     4,     5,     6,     7,     8,     9,     0,     0,     0,
      10,    11,     0,     0,    12,     0,     0,    13,    14,    15,
      16,    17,     0,     0,    18,     0,    19,     0,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,    22,    23,    24,    25,    -6,     1,     0,     0,     2,
       3,     4,     5,     6,     7,     8,     9,     0,     0,     0,
      10,    11,     0,     0,    12,     0,     0,    13,    14,     0,
      16,    17,     0,    -6,    18,     0,    19,     0,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,    22,    23,    24,    25,     1,     0,     0,     2,     3,
       4,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,     0,     0,    12,   -57,   -57,    13,    14,     0,    16,
      17,    67,   -57,    18,     0,    19,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
      22,    23,    24,    25,     1,     0,     0,     2,     3,     4,
       5,     6,     7,     8,     9,     0,     0,     0,    10,    11,
       0,   209,    12,     0,     0,    13,    14,     0,    16,    17,
      67,     0,    18,     0,    19,     0,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,    22,
      23,    24,    25,     1,     0,     0,     2,     3,     4,     5,
       6,     7,     8,     9,     0,     0,     0,    10,    11,     0,
       0,    12,     0,     0,    13,    14,     0,    16,    17,    67,
       0,    18,     0,    19,     0,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,    22,    23,
      24,    25,     1,     0,     0,     2,     3,     4,     5,     6,
       7,     8,     9,     0,     0,     0,    10,    11,     0,     0,
      12,     0,     0,    13,    14,     0,    16,    17,     0,   125,
      18,     0,    19,     0,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,    22,    23,    24,
      25,     1,     0,     0,     2,     3,     4,     5,     6,     7,
       8,     9,    53,     0,     7,    10,    11,     0,     0,    12,
       0,     0,    13,    14,     0,    16,    17,    13,    14,    18,
      53,    19,     7,    20,    18,     0,    19,    76,     0,     0,
       0,     0,     0,     0,     0,    13,    14,     0,     0,     0,
       0,     0,    18,     0,    19,    21,    22,    23,    24,    25,
      21,    22,    55,    24,    25,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,    22,
      55,    24,    25
  };

  const short int
  parser::yycheck_[] =
  {
       0,    15,    11,    80,     0,     2,     3,    82,    29,    29,
      10,    11,    27,    30,    29,    38,    29,    51,    52,    53,
      54,    18,    19,    45,    46,    38,    57,    27,    25,    29,
       7,    27,    57,    29,    57,    29,    57,    57,    29,     7,
      57,     9,    47,    48,    49,    50,    29,    61,    95,    96,
      97,    98,    67,    62,    22,    23,    29,   134,    40,    41,
      42,    29,    62,    31,   139,    37,    38,    67,     0,    66,
      33,    67,    37,    38,    71,    60,    61,    74,   105,   106,
     107,    20,    21,    80,    81,    82,    33,    55,    56,    57,
      58,    59,    60,    61,    36,    92,   103,   104,     7,    33,
       9,    99,   100,   101,   102,    43,   183,   121,    39,    31,
     119,   108,   109,    22,    23,    38,   113,   114,    35,   119,
      29,    29,    31,    33,    33,    13,   123,    33,    57,     7,
      57,     9,    26,   130,   131,    17,    34,   134,    30,    32,
      30,    57,   139,    34,    22,    23,    55,    56,    57,    58,
      59,    29,    29,    31,    30,    44,    28,    30,   167,    29,
      44,   170,    30,    30,    29,   162,    32,   167,   177,    38,
     170,   168,    57,    14,    57,   172,    27,   177,    56,    57,
      58,    59,    30,    12,   181,    30,   183,    30,   185,   198,
     199,   200,   189,   202,    30,    44,    28,    44,   198,   199,
     200,   165,   202,   206,    21,   214,   203,    93,   217,   218,
      -1,    -1,   209,    94,   214,     0,     1,   217,   218,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    -1,    -1,    19,    -1,    -1,    22,    23,    24,
      25,    26,    -1,    -1,    29,    -1,    31,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    58,    59,     0,     1,    -1,    -1,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    -1,    -1,    19,    -1,    -1,    22,    23,    -1,
      25,    26,    -1,    28,    29,    -1,    31,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    58,    59,     1,    -1,    -1,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    -1,    25,
      26,    27,    28,    29,    -1,    31,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    58,    59,     1,    -1,    -1,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    16,
      -1,    18,    19,    -1,    -1,    22,    23,    -1,    25,    26,
      27,    -1,    29,    -1,    31,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    58,    59,     1,    -1,    -1,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    -1,
      -1,    19,    -1,    -1,    22,    23,    -1,    25,    26,    27,
      -1,    29,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    59,     1,    -1,    -1,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    16,    -1,    -1,
      19,    -1,    -1,    22,    23,    -1,    25,    26,    -1,    28,
      29,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      59,     1,    -1,    -1,     4,     5,     6,     7,     8,     9,
      10,    11,     7,    -1,     9,    15,    16,    -1,    -1,    19,
      -1,    -1,    22,    23,    -1,    25,    26,    22,    23,    29,
       7,    31,     9,    33,    29,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    31,    55,    56,    57,    58,    59,
      55,    56,    57,    58,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    58,    59
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     1,     4,     5,     6,     7,     8,     9,    10,    11,
      15,    16,    19,    22,    23,    24,    25,    26,    29,    31,
      33,    55,    56,    57,    58,    59,    63,    64,    65,    66,
      67,    68,    69,    70,    75,    76,    77,    78,    79,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    96,     7,    33,    57,    82,    91,    82,    57,
      29,    57,    71,     7,    29,    57,    29,    27,    66,    74,
      74,    29,    71,    57,    29,    82,    32,    82,    95,    90,
      29,    38,    60,    61,    82,    97,     0,    65,    65,    33,
      33,    33,    36,    43,    39,    51,    52,    53,    54,    47,
      48,    49,    50,    45,    46,    40,    41,    42,    31,    38,
      35,    33,    33,    37,    38,    30,    57,    72,    73,    71,
      74,    57,    57,    13,    82,    28,    65,    26,    17,    82,
      37,    38,    82,    30,    34,    32,    95,    82,    97,    60,
      61,    82,    84,    85,    86,    86,    86,    86,    87,    87,
      87,    87,    88,    88,    89,    89,    89,    82,    82,    57,
      82,    82,    38,    57,    30,    34,    74,    71,    44,    82,
      30,    28,    29,    29,    30,    82,    82,    30,    95,    30,
      97,    44,    32,    29,    82,    38,    72,    74,    82,    14,
      74,    82,    57,    27,    74,    82,    95,    82,    30,    82,
      12,    30,    30,    20,    21,    80,    81,    30,    74,    18,
      74,    74,    74,    82,    44,    28,    80,    82,    44,    74,
      74,    74
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    62,    63,    63,    63,    64,    65,    65,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    67,    67,    68,    68,    68,    68,    68,    68,
      69,    69,    70,    71,    71,    72,    72,    72,    73,    73,
      73,    73,    74,    74,    74,    75,    75,    76,    76,    76,
      77,    77,    78,    79,    80,    80,    81,    81,    81,    82,
      82,    83,    83,    84,    84,    85,    85,    85,    85,    85,
      86,    86,    86,    86,    86,    87,    87,    87,    88,    88,
      88,    88,    89,    89,    90,    90,    90,    91,    92,    92,
      92,    92,    93,    93,    93,    93,    93,    93,    93,    93,
      94,    94,    95,    95,    95,    96,    96,    97,    97,    97,
      97
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     2,     1,     2,     1,     2,     3,     2,
       3,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     3,     3,     4,     4,     2,     4,     4,     2,
       4,     5,     3,     2,     3,     1,     2,     3,     1,     3,
       2,     4,     1,     2,     3,     5,     7,     7,     9,     7,
       5,     6,     7,     7,     1,     2,     4,     3,     3,     1,
       5,     1,     3,     1,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     2,     1,     6,     1,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     1,     2,     3,     1,     2,     2,     3,     1,
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
  "\"true\"", "\"params\"", "\"private\"", "\"while\"", "\"{\"", "\"}\"",
  "\"(\"", "\")\"", "\"[\"", "\"]\"", "\";\"", "\",\"", "\".\"", "\"?\"",
  "\"be\"", "\"=\"", "\"&&\"", "\"/\"", "\"*\"", "\"%\"", "\"||\"",
  "\":\"", "\"+\"", "\"-\"", "\"<=\"", "\"<\"", "\">=\"", "\">\"",
  "\"===\"", "\"==\"", "\"!==\"", "\"!=\"", "\"!\"", "NUMBER", "IDENT",
  "STRING", "FORMAT_STRING_START", "FORMAT_STRING_CONTINUE",
  "FORMAT_STRING_FINAL", "$accept", "start", "filehead", "statements",
  "statement", "assignment", "vardecl", "funcdecl", "function", "funchead",
  "arglist", "argitem", "codeblock", "if", "for", "while", "trycatch",
  "switch", "caselist", "case", "exp01", "exp02", "exp03", "exp04",
  "exp05", "exp06", "exp07", "exp08", "exp09", "arrget", "expp", "value",
  "array", "explist", "format_string", "format_string_match", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   211,   211,   212,   213,   216,   219,   220,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   239,   240,   243,   244,   245,   246,   247,   248,
     251,   252,   255,   258,   259,   262,   263,   264,   266,   267,
     268,   269,   271,   272,   273,   276,   277,   280,   281,   282,
     285,   286,   289,   292,   295,   296,   299,   300,   301,   304,
     305,   307,   308,   310,   311,   313,   314,   315,   316,   317,
     319,   320,   321,   322,   323,   325,   326,   327,   329,   330,
     331,   332,   334,   335,   337,   338,   339,   341,   343,   344,
     345,   346,   348,   349,   350,   351,   352,   353,   354,   355,
     357,   358,   360,   361,   362,   364,   365,   367,   368,   369,
     370
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
#line 1986 "parser.tab.cc" // lalr1.cc:1167
#line 372 "parser.y" // lalr1.cc:1168


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
         case tokenizer::etoken::t_be: return parser::make_BE(token, loc);
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
         case tokenizer::etoken::s_equal: return parser::make_EQUAL(token, loc);
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
         case tokenizer::etoken::t_formatted_string_start: return parser::make_FORMAT_STRING_START(token, loc);
         case tokenizer::etoken::t_formatted_string_continue: return parser::make_FORMAT_STRING_CONTINUE(token, loc);
         case tokenizer::etoken::t_formatted_string_final: return parser::make_FORMAT_STRING_FINAL(token, loc);
         default:
             return parser::make_NA(loc);
         }
     }
}
