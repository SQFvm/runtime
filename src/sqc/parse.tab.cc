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


// First part of user declarations.
#line 1 "parse.yy" // lalr1.cc:404
     /* PARSER */

#include "parser.hh"
#include "scanner.hh"

#define yylex driver.scanner_->yylex

#line 44 "parse.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parse.tab.hh"

// User implementation prologue.

#line 58 "parse.tab.cc" // lalr1.cc:412


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

#line 39 "parse.yy" // lalr1.cc:479
namespace parse {
#line 144 "parse.tab.cc" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
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
  Parser::Parser (Driver &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  Parser::~Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  Parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  Parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  Parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  Parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  Parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Parser::symbol_number_type
  Parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
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
  Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::parse ()
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
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, driver));
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
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

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
#line 122 "parse.yy" // lalr1.cc:859
    { driver.push_back((yystack_[0].value.astnode)); }
#line 624 "parse.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 125 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 630 "parse.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 126 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append_children((yystack_[1].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 636 "parse.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 129 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.astnode); }
#line 642 "parse.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 130 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; }
#line 648 "parse.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 131 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.astnode); }
#line 654 "parse.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 132 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.astnode); }
#line 660 "parse.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 133 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 666 "parse.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 134 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 672 "parse.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 135 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 678 "parse.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 136 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 684 "parse.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 137 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 690 "parse.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 138 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 696 "parse.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 139 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 702 "parse.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 140 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.astnode); }
#line 708 "parse.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 141 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; }
#line 714 "parse.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 142 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; }
#line 720 "parse.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 145 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.stringVal)); (yylhs.value.astnode).append((yystack_[0].value.astnode));}
#line 726 "parse.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 148 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.stringVal)); (yylhs.value.astnode).append((yystack_[0].value.astnode));}
#line 732 "parse.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 149 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.stringVal)); (yylhs.value.astnode).append((yystack_[0].value.astnode));}
#line 738 "parse.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 150 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.stringVal)); (yylhs.value.astnode).append((yystack_[0].value.astnode));}
#line 744 "parse.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 151 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.stringVal)); (yylhs.value.astnode).append((yystack_[0].value.astnode));}
#line 750 "parse.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 154 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.stringVal)); (yylhs.value.astnode).append((yystack_[1].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 756 "parse.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 157 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[1].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 762 "parse.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 160 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; }
#line 768 "parse.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 161 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.astnode); }
#line 774 "parse.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 164 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.stringVal); }
#line 780 "parse.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 165 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.stringVal); }
#line 786 "parse.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 166 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append_children((yystack_[2].value.stringVal)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 792 "parse.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 169 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.astnode); }
#line 798 "parse.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 170 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; }
#line 804 "parse.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 171 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.astnode); }
#line 810 "parse.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 174 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 816 "parse.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 175 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[4].value.astnode)); (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 822 "parse.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 178 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[5].value.stringVal)); (yylhs.value.astnode).append((yystack_[3].value.stringVal)); (yylhs.value.astnode).append((yystack_[1].value.stringVal)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 828 "parse.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 179 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[7].value.stringVal)); (yylhs.value.astnode).append((yystack_[5].value.stringVal)); (yylhs.value.astnode).append((yystack_[3].value.stringVal)); (yylhs.value.astnode).append((yystack_[1].value.stringVal)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 834 "parse.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 180 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[4].value.stringVal)); (yylhs.value.astnode).append((yystack_[2].value.stringVal)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 840 "parse.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 183 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 846 "parse.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 184 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[4].value.astnode)); (yylhs.value.astnode).append((yystack_[1].value.astnode)); }
#line 852 "parse.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 187 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[5].value.astnode)); (yylhs.value.astnode).append((yystack_[2].value.stringVal)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 858 "parse.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 190 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[4].value.astnode)); (yylhs.value.astnode).append((yystack_[1].value.astnode)); }
#line 864 "parse.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 193 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 870 "parse.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 194 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append_children((yystack_[1].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 876 "parse.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 197 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 882 "parse.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 198 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[1].value.astnode)); }
#line 888 "parse.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 199 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 894 "parse.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 200 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; }
#line 900 "parse.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 203 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 906 "parse.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 204 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[4].value.astnode)); (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 912 "parse.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 206 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 918 "parse.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 207 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 924 "parse.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 209 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 930 "parse.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 210 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 936 "parse.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 212 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 942 "parse.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 213 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 948 "parse.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 214 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 954 "parse.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 215 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 960 "parse.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 216 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 966 "parse.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 218 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 972 "parse.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 219 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 978 "parse.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 220 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 984 "parse.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 221 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 990 "parse.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 222 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 996 "parse.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 224 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 1002 "parse.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 225 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 1008 "parse.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 226 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 1014 "parse.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 228 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 1020 "parse.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 229 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 1026 "parse.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 230 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 1032 "parse.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 231 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 1038 "parse.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 233 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 1044 "parse.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 234 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 1050 "parse.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 236 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 1056 "parse.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 237 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[5].value.astnode)); (yylhs.value.astnode).append((yystack_[3].value.stringVal)); (yylhs.value.astnode).append((yystack_[1].value.astnode)); }
#line 1062 "parse.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 240 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.astnode); }
#line 1068 "parse.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 241 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append((yystack_[3].value.stringVal)); (yylhs.value.astnode).append((yystack_[1].value.astnode)); }
#line 1074 "parse.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 242 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 1080 "parse.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 244 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 1086 "parse.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 245 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.stringVal); }
#line 1092 "parse.tab.cc" // lalr1.cc:859
    break;

  case 82:
#line 246 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 1098 "parse.tab.cc" // lalr1.cc:859
    break;

  case 83:
#line 247 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.stringVal); }
#line 1104 "parse.tab.cc" // lalr1.cc:859
    break;

  case 84:
#line 248 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; }
#line 1110 "parse.tab.cc" // lalr1.cc:859
    break;

  case 85:
#line 249 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; }
#line 1116 "parse.tab.cc" // lalr1.cc:859
    break;

  case 86:
#line 250 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; }
#line 1122 "parse.tab.cc" // lalr1.cc:859
    break;

  case 87:
#line 251 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.stringVal); }
#line 1128 "parse.tab.cc" // lalr1.cc:859
    break;

  case 88:
#line 253 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; }
#line 1134 "parse.tab.cc" // lalr1.cc:859
    break;

  case 89:
#line 254 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.astnode); }
#line 1140 "parse.tab.cc" // lalr1.cc:859
    break;

  case 90:
#line 256 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[0].value.astnode); }
#line 1146 "parse.tab.cc" // lalr1.cc:859
    break;

  case 91:
#line 257 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = (yystack_[1].value.astnode); }
#line 1152 "parse.tab.cc" // lalr1.cc:859
    break;

  case 92:
#line 258 "parse.yy" // lalr1.cc:859
    { (yylhs.value.astnode) = {}; (yylhs.value.astnode).append_children((yystack_[2].value.astnode)); (yylhs.value.astnode).append((yystack_[0].value.astnode)); }
#line 1158 "parse.tab.cc" // lalr1.cc:859
    break;


#line 1162 "parse.tab.cc" // lalr1.cc:859
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
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
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


  const signed char Parser::yypact_ninf_ = -64;

  const signed char Parser::yytable_ninf_ = -50;

  const short int
  Parser::yypact_[] =
  {
     180,   -64,    94,   430,   -47,   -23,   -14,     2,   356,   356,
      10,   -64,   -64,   -64,   -15,   -37,   430,   418,   -64,   458,
     -64,   -19,   -64,    49,   -64,    20,   -64,    21,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,    40,    44,    56,    62,    26,
      61,    71,    55,   -64,    57,   -64,   -64,    82,   -64,    83,
      86,    87,     0,     3,    82,   356,   430,   430,   400,    88,
     108,   107,   430,    73,   115,     6,    99,   -64,    96,    32,
     -64,   430,   430,   -64,   -64,   -64,   -64,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,    76,   -64,   -64,   430,   430,   -64,    98,
     103,   356,   -64,   104,   110,   -64,   109,   -64,   112,   113,
     114,   117,    84,   430,   430,   -64,   -64,   -64,   430,   -11,
     -64,   119,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   118,   -64,   -64,
      97,   -64,   -64,   356,   356,   -64,   430,   102,   124,   105,
     141,   -64,   -64,   -64,   -64,   430,   430,   -64,   144,   -64,
     128,   132,    41,   133,   111,   -64,    25,   356,   -64,   356,
     430,   138,    -8,   -64,   356,   312,   -64,   -64,   -64,   142,
     224,   -64,   -64,   -64,   120,   -64,   268,   -64,   356,   -64,
     -64
  };

  const unsigned char
  Parser::yydefact_[] =
  {
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    84,    85,     0,     0,     0,     0,    18,     0,
      83,    87,    81,     0,     3,     0,    16,     0,    10,    80,
      11,    12,    13,    14,    15,     0,    50,    52,    54,    56,
      61,    66,    69,    73,    75,    79,    82,     0,     7,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    90,     0,
      74,     0,     0,     1,     5,     9,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     6,     8,     0,     0,    27,    29,
       0,     0,    26,     0,     0,    33,     0,    32,     0,     0,
       0,     0,     0,     0,     0,    77,    91,    89,     0,     0,
      20,     0,    53,    55,    57,    59,    58,    60,    63,    62,
      65,    64,    67,    68,    71,    70,    72,     0,    22,    21,
      30,    28,    25,     0,     0,    34,     0,     0,     0,     0,
       0,    24,    23,    92,    78,     0,     0,    31,    35,    40,
       0,     0,     0,     0,     0,    51,     0,     0,    41,     0,
       0,     0,     0,    44,     0,     0,    76,    36,    42,     0,
       0,    43,    45,    39,     0,    37,     0,    48,     0,    46,
      38
  };

  const short int
  Parser::yypgoto_[] =
  {
     -64,   -64,   -22,     4,   -64,   -64,   -64,   -64,   116,    28,
      -9,   -64,   -64,   -64,   -64,   -64,   -64,    14,    -1,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   153,   -64,   -64,   -64,
     -63
  };

  const short int
  Parser::yydefgoto_[] =
  {
      -1,    23,    24,    59,    26,    27,    28,    29,    55,   100,
      60,    30,    31,    32,    33,    34,   172,   173,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      69
  };

  const short int
  Parser::yytable_[] =
  {
      61,    50,    51,    74,    25,    53,    96,    52,   119,    71,
     170,   171,   113,    63,    56,    66,    68,    65,   154,   181,
      -4,     1,   118,     2,     3,     4,    72,     5,     6,    25,
      57,    54,    98,     7,     8,     9,   106,    10,    62,    64,
      11,    12,    13,    14,    15,    97,   102,    -4,    16,    73,
      17,   114,    18,    75,   176,   103,   104,    99,   118,   170,
     171,   110,    25,   117,    19,   118,    80,    81,    82,    83,
      68,   120,    76,    20,    21,    22,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   142,   166,    77,   138,   139,    84,    85,    86,
      87,    47,    90,    91,    92,    88,    89,    78,    79,    93,
      53,    71,   151,   152,    11,    12,    13,   153,    94,    95,
     107,   108,    16,   109,    17,   112,    48,   111,   115,   116,
     137,   140,   141,   143,   158,   159,   145,   150,    19,   144,
     146,   147,   149,   148,   155,   160,   156,    20,    49,    22,
     162,    99,   164,   167,   165,    68,   161,   168,   177,   163,
     178,   169,   174,   180,   175,   183,   185,   186,   157,   179,
     101,   187,    70,   188,     0,     0,     0,   189,     0,   190,
      -2,     1,     0,     2,     3,     4,   182,     5,     6,     0,
       0,     0,     0,     7,     8,     9,     0,    10,     0,     0,
      11,    12,    13,    14,    15,     0,     0,     0,    16,     0,
      17,     0,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    19,     1,     0,     2,     3,     4,
       0,     5,     6,    20,    21,    22,     0,     7,     8,     9,
       0,    10,   -49,   -49,    11,    12,    13,    14,    15,     0,
      58,   -49,    16,     0,    17,     0,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    19,     1,
       0,     2,     3,     4,     0,     5,     6,    20,    21,    22,
       0,     7,     8,     9,     0,    10,   -47,   -47,    11,    12,
      13,    14,    15,     0,    58,   -47,    16,     0,    17,     0,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    19,     1,     0,     2,     3,     4,     0,     5,
       6,    20,    21,    22,   184,     7,     8,     9,     0,    10,
       0,     0,    11,    12,    13,    14,    15,     0,    58,     0,
      16,     0,    17,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,     1,     0,     2,
       3,     4,     0,     5,     6,    20,    21,    22,     0,     7,
       8,     9,     0,    10,     0,     0,    11,    12,    13,    14,
      15,     0,    58,     0,    16,     0,    17,     0,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,     1,     0,     2,     3,     4,     0,     5,     6,    20,
      21,    22,     0,     7,     8,     9,     0,    10,     0,     0,
      11,    12,    13,    14,    15,    47,     0,   105,    16,     0,
      17,     0,    18,     0,     0,     0,     0,    47,    11,    12,
      13,     0,     0,     0,    19,     0,    16,     0,    17,    67,
      11,    12,    13,    20,    21,    22,     0,     0,    16,     0,
      17,     0,    19,     0,     0,    47,     0,     0,     0,     0,
       0,    20,    49,    22,    19,     0,     0,     0,    11,    12,
      13,     0,     0,    20,    49,    22,    16,     0,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    20,    49,    22
  };

  const short int
  Parser::yycheck_[] =
  {
       9,     2,     3,    25,     0,    28,     6,    54,    71,    28,
      18,    19,     6,    28,    28,    16,    17,    54,    29,    27,
       0,     1,    33,     3,     4,     5,    45,     7,     8,    25,
      28,    54,    29,    13,    14,    15,    58,    17,    28,    54,
      20,    21,    22,    23,    24,    45,    55,    27,    28,     0,
      30,    45,    32,    32,    29,    56,    57,    54,    33,    18,
      19,    62,    58,    31,    44,    33,    40,    41,    42,    43,
      71,    72,    32,    53,    54,    55,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,   101,   156,    50,    96,    97,    36,    37,    38,
      39,     7,    47,    48,    49,    34,    35,    51,    46,    52,
      28,    28,   113,   114,    20,    21,    22,   118,    32,    32,
      32,    13,    28,    16,    30,    10,    32,    54,    29,    33,
      54,    33,    29,    29,   143,   144,    27,    53,    44,    29,
      28,    28,    25,    29,    25,   146,    28,    53,    54,    55,
      26,    54,    11,     9,   155,   156,    54,    29,   167,    54,
     169,    29,    29,    25,    53,   174,   175,    25,   140,   170,
      54,   180,    19,    53,    -1,    -1,    -1,   186,    -1,   188,
       0,     1,    -1,     3,     4,     5,   172,     7,     8,    -1,
      -1,    -1,    -1,    13,    14,    15,    -1,    17,    -1,    -1,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    28,    -1,
      30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,     1,    -1,     3,     4,     5,
      -1,     7,     8,    53,    54,    55,    -1,    13,    14,    15,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      26,    27,    28,    -1,    30,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,     1,
      -1,     3,     4,     5,    -1,     7,     8,    53,    54,    55,
      -1,    13,    14,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    28,    -1,    30,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,     1,    -1,     3,     4,     5,    -1,     7,
       8,    53,    54,    55,    12,    13,    14,    15,    -1,    17,
      -1,    -1,    20,    21,    22,    23,    24,    -1,    26,    -1,
      28,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,     1,    -1,     3,
       4,     5,    -1,     7,     8,    53,    54,    55,    -1,    13,
      14,    15,    -1,    17,    -1,    -1,    20,    21,    22,    23,
      24,    -1,    26,    -1,    28,    -1,    30,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,     1,    -1,     3,     4,     5,    -1,     7,     8,    53,
      54,    55,    -1,    13,    14,    15,    -1,    17,    -1,    -1,
      20,    21,    22,    23,    24,     7,    -1,    27,    28,    -1,
      30,    -1,    32,    -1,    -1,    -1,    -1,     7,    20,    21,
      22,    -1,    -1,    -1,    44,    -1,    28,    -1,    30,    31,
      20,    21,    22,    53,    54,    55,    -1,    -1,    28,    -1,
      30,    -1,    44,    -1,    -1,     7,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    44,    -1,    -1,    -1,    20,    21,
      22,    -1,    -1,    53,    54,    55,    28,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,     1,     3,     4,     5,     7,     8,    13,    14,    15,
      17,    20,    21,    22,    23,    24,    28,    30,    32,    44,
      53,    54,    55,    57,    58,    59,    60,    61,    62,    63,
      67,    68,    69,    70,    71,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,     7,    32,    54,
      74,    74,    54,    28,    54,    64,    28,    28,    26,    59,
      66,    66,    28,    28,    54,    54,    74,    31,    74,    86,
      82,    28,    45,     0,    58,    32,    32,    50,    51,    46,
      40,    41,    42,    43,    36,    37,    38,    39,    34,    35,
      47,    48,    49,    52,    32,    32,     6,    45,    29,    54,
      65,    64,    66,    74,    74,    27,    58,    32,    13,    16,
      74,    54,    10,     6,    45,    29,    33,    31,    33,    86,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    54,    74,    74,
      33,    29,    66,    29,    29,    27,    28,    28,    29,    25,
      53,    74,    74,    74,    29,    25,    28,    65,    66,    66,
      74,    54,    26,    54,    11,    74,    86,     9,    29,    29,
      18,    19,    72,    73,    29,    53,    29,    66,    66,    74,
      25,    27,    73,    66,    12,    66,    25,    66,    53,    66,
      66
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    56,    57,    57,    58,    58,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      60,    61,    61,    61,    61,    62,    63,    64,    64,    65,
      65,    65,    66,    66,    66,    67,    67,    68,    68,    68,
      69,    69,    70,    71,    72,    72,    73,    73,    73,    73,
      74,    74,    75,    75,    76,    76,    77,    77,    77,    77,
      77,    78,    78,    78,    78,    78,    79,    79,    79,    80,
      80,    80,    80,    81,    81,    82,    82,    83,    83,    83,
      84,    84,    84,    84,    84,    84,    84,    84,    85,    85,
      86,    86,    86
  };

  const unsigned char
  Parser::yyr2_[] =
  {
       0,     2,     0,     1,     1,     2,     3,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       3,     4,     4,     4,     4,     4,     3,     2,     3,     1,
       2,     3,     2,     2,     3,     5,     7,     7,     9,     7,
       5,     6,     7,     7,     1,     2,     4,     3,     3,     2,
       1,     5,     1,     3,     1,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     2,     1,     6,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     3,
       1,     2,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "$end", "error", "$undefined", "\"return\"", "\"throw\"", "\"let\"",
  "\"be\"", "\"function\"", "\"if\"", "\"else\"", "\"from\"", "\"to\"",
  "\"step\"", "\"while\"", "\"do\"", "\"try\"", "\"catch\"", "\"switch\"",
  "\"case\"", "\"default\"", "\"nil\"", "\"true\"", "\"false\"", "\"for\"",
  "\"private\"", "\":\"", "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"[\"",
  "\"]\"", "\";\"", "\",\"", "\"+\"", "\"-\"", "\"<=\"", "\"<\"", "\">=\"",
  "\">\"", "\"===\"", "\"==\"", "\"!==\"", "\"!=\"", "\"!\"", "\"=\"",
  "\"&&\"", "\"/\"", "\"*\"", "\"%\"", "\"?\"", "\"||\"", "\".\"",
  "NUMBER", "IDENT", "STRING", "$accept", "start", "statements",
  "statement", "assignment", "vardecl", "funcdecl", "function", "funchead",
  "arglist", "codeblock", "if", "for", "while", "trycatch", "switch",
  "caselist", "case", "exp01", "exp02", "exp03", "exp04", "exp05", "exp06",
  "exp07", "exp08", "exp09", "expp", "value", "array", "explist", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  Parser::yyrline_[] =
  {
       0,   121,   121,   122,   125,   126,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     145,   148,   149,   150,   151,   154,   157,   160,   161,   164,
     165,   166,   169,   170,   171,   174,   175,   178,   179,   180,
     183,   184,   187,   190,   193,   194,   197,   198,   199,   200,
     203,   204,   206,   207,   209,   210,   212,   213,   214,   215,
     216,   218,   219,   220,   221,   222,   224,   225,   226,   228,
     229,   230,   231,   233,   234,   236,   237,   240,   241,   242,
     244,   245,   246,   247,   248,   249,   250,   251,   253,   254,
     256,   257,   258
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
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
  Parser::yy_reduce_print_ (int yyrule)
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

  // Symbol number corresponding to token number t.
  inline
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
    };
    const unsigned int user_token_number_max_ = 310;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 39 "parse.yy" // lalr1.cc:1167
} // parse
#line 1780 "parse.tab.cc" // lalr1.cc:1167
#line 261 "parse.yy" // lalr1.cc:1168


namespace parse
{
    void Parser::error(const location&, const std::string& m)
    {
        std::cerr << *driver.location_ << ": " << m << std::endl;
        driver.error_ = (driver.error_ == 127 ? 127 : driver.error_ + 1);
    }
}
