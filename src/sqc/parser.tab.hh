// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file parser.tab.hh
 ** Define the  sqf::sqc::bison ::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_PARSER_TAB_HH_INCLUDED
# define YY_YY_PARSER_TAB_HH_INCLUDED
// //                    "%code requires" blocks.
#line 16 "parser.y" // lalr1.cc:377

     namespace sqf::sqc
     {
          class parser;
     }
     namespace sqf::sqc::bison
     {
          enum class astkind
          {
               __TOKEN = -1,
               NA = 0,
               RETURN,
               THROW,
               ASSIGNMENT,
               DECLARATION,
               FORWARD_DECLARATION,
               FUNCTION_DECLARATION,
               FINAL_FUNCTION_DECLARATION,
               FUNCTION,
               ARGLIST,
               CODEBLOCK,
               IF,
               IFELSE,
               FOR,
               FORSTEP,
               FOREACH,
               WHILE,
               DOWHILE,
               TRYCATCH,
               SWITCH,
               CASE,
               CASE_DEFAULT,
               OP_TERNARY,
               OP_OR,
               OP_AND,
               OP_EQUALEXACT,
               OP_EQUAL,
               OP_NOTEQUALEXACT,
               OP_NOTEQUAL,
               OP_LESSTHAN,
               OP_GREATERTHAN,
               OP_LESSTHANEQUAL,
               OP_GREATERTHANEQUAL,
               OP_PLUS,
               OP_MINUS,
               OP_MULTIPLY,
               OP_DIVIDE,
               OP_REMAINDER,
               OP_NOT,
               OP_BINARY,
               OP_UNARY,
               OP_ARRAY_GET,
               OP_ARRAY_SET,
               VAL_STRING,
               VAL_ARRAY,
               VAL_NUMBER,
               VAL_TRUE,
               VAL_FALSE,
               VAL_NIL,
               GET_VARIABLE
          };
          struct astnode
          {
               sqf::sqc::tokenizer::token token;
               astkind kind;
               std::vector<astnode> children;

               astnode() : token(), kind(astkind::NA)
               {
               }
               astnode(astkind kind) : token(), kind(kind)
               {
               }
               astnode(sqf::sqc::tokenizer::token t) : token(t), kind(astkind::__TOKEN)
               {
               }
               astnode(astkind kind, sqf::sqc::tokenizer::token t) : token(t), kind(kind) {}

               void append(astnode node)
               {
                   children.push_back(node);
               }
               void append_children(const astnode& other)
               { 
                   for (auto node : other.children)
                   {
                       append(node); 
                   } 
               }
          };
     }

#line 137 "parser.tab.hh" // lalr1.cc:377


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
# include "stack.hh"
# include "location.hh"

#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

#line 8 "parser.y" // lalr1.cc:377
namespace  sqf { namespace sqc { namespace bison  {
#line 214 "parser.tab.hh" // lalr1.cc:377



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {}

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;
  };


  /// A Bison parser.
  class parser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // filehead
      // statements
      // statement
      // assignment
      // vardecl
      // funcdecl
      // function
      // funchead
      // arglist
      // codeblock
      // if
      // for
      // while
      // trycatch
      // switch
      // caselist
      // case
      // exp01
      // exp02
      // exp03
      // exp04
      // exp05
      // exp06
      // exp07
      // exp08
      // exp09
      // expp
      // value
      // array
      // explist
      char dummy1[sizeof(sqf::sqc::bison::astnode)];

      // "&&"
      // "/"
      // "*"
      // "%"
      // "||"
      // ":"
      // "+"
      // "-"
      // "<="
      // "<"
      // ">="
      // ">"
      // "==="
      // "=="
      // "!=="
      // "!="
      // "!"
      // NUMBER
      // IDENT
      // STRING
      char dummy2[sizeof(tokenizer::token)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        NA = 0,
        BE = 258,
        BREAK = 259,
        RETURN = 260,
        THROW = 261,
        LET = 262,
        FUNCTION = 263,
        FINAL = 264,
        FALSE = 265,
        FOR = 266,
        IF = 267,
        ELSE = 268,
        FROM = 269,
        TO = 270,
        DO = 271,
        TRY = 272,
        CATCH = 273,
        STEP = 274,
        SWITCH = 275,
        CASE = 276,
        DEFAULT = 277,
        NIL = 278,
        TRUE = 279,
        PARAMS = 280,
        PRIVATE = 281,
        WHILE = 282,
        CURLYO = 283,
        CURLYC = 284,
        ROUNDO = 285,
        ROUNDC = 286,
        SQUAREO = 287,
        SQUAREC = 288,
        SEMICOLON = 289,
        COMMA = 290,
        EQUAL = 291,
        DOT = 292,
        QUESTIONMARK = 293,
        ANDAND = 294,
        SLASH = 295,
        STAR = 296,
        PERCENT = 297,
        VLINEVLINE = 298,
        COLON = 299,
        PLUS = 300,
        MINUS = 301,
        LTEQUAL = 302,
        LT = 303,
        GTEQUAL = 304,
        GT = 305,
        EQUALEQUALEQUAL = 306,
        EQUALEQUAL = 307,
        EXCLAMATIONMARKEQUALEQUAL = 308,
        EXCLAMATIONMARKEQUAL = 309,
        EXCLAMATIONMARK = 310,
        NUMBER = 311,
        IDENT = 312,
        STRING = 313
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const sqf::sqc::bison::astnode v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const tokenizer::token v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      /// Destroy the symbol.
      ~basic_symbol ();

      /// Destroy contents, and record that is empty.
      void clear ();

      /// Whether empty.
      bool empty () const;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_NA (const location_type& l);

    static inline
    symbol_type
    make_BE (const location_type& l);

    static inline
    symbol_type
    make_BREAK (const location_type& l);

    static inline
    symbol_type
    make_RETURN (const location_type& l);

    static inline
    symbol_type
    make_THROW (const location_type& l);

    static inline
    symbol_type
    make_LET (const location_type& l);

    static inline
    symbol_type
    make_FUNCTION (const location_type& l);

    static inline
    symbol_type
    make_FINAL (const location_type& l);

    static inline
    symbol_type
    make_FALSE (const location_type& l);

    static inline
    symbol_type
    make_FOR (const location_type& l);

    static inline
    symbol_type
    make_IF (const location_type& l);

    static inline
    symbol_type
    make_ELSE (const location_type& l);

    static inline
    symbol_type
    make_FROM (const location_type& l);

    static inline
    symbol_type
    make_TO (const location_type& l);

    static inline
    symbol_type
    make_DO (const location_type& l);

    static inline
    symbol_type
    make_TRY (const location_type& l);

    static inline
    symbol_type
    make_CATCH (const location_type& l);

    static inline
    symbol_type
    make_STEP (const location_type& l);

    static inline
    symbol_type
    make_SWITCH (const location_type& l);

    static inline
    symbol_type
    make_CASE (const location_type& l);

    static inline
    symbol_type
    make_DEFAULT (const location_type& l);

    static inline
    symbol_type
    make_NIL (const location_type& l);

    static inline
    symbol_type
    make_TRUE (const location_type& l);

    static inline
    symbol_type
    make_PARAMS (const location_type& l);

    static inline
    symbol_type
    make_PRIVATE (const location_type& l);

    static inline
    symbol_type
    make_WHILE (const location_type& l);

    static inline
    symbol_type
    make_CURLYO (const location_type& l);

    static inline
    symbol_type
    make_CURLYC (const location_type& l);

    static inline
    symbol_type
    make_ROUNDO (const location_type& l);

    static inline
    symbol_type
    make_ROUNDC (const location_type& l);

    static inline
    symbol_type
    make_SQUAREO (const location_type& l);

    static inline
    symbol_type
    make_SQUAREC (const location_type& l);

    static inline
    symbol_type
    make_SEMICOLON (const location_type& l);

    static inline
    symbol_type
    make_COMMA (const location_type& l);

    static inline
    symbol_type
    make_EQUAL (const location_type& l);

    static inline
    symbol_type
    make_DOT (const location_type& l);

    static inline
    symbol_type
    make_QUESTIONMARK (const location_type& l);

    static inline
    symbol_type
    make_ANDAND (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_SLASH (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_STAR (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_PERCENT (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_VLINEVLINE (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_COLON (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_PLUS (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_MINUS (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_LTEQUAL (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_LT (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_GTEQUAL (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_GT (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_EQUALEQUALEQUAL (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_EQUALEQUAL (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_EXCLAMATIONMARKEQUALEQUAL (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_EXCLAMATIONMARKEQUAL (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_EXCLAMATIONMARK (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_NUMBER (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_IDENT (const tokenizer::token& v, const location_type& l);

    static inline
    symbol_type
    make_STRING (const tokenizer::token& v, const location_type& l);


    /// Build a parser object.
    parser (sqf::sqc::tokenizer &tokenizer_yyarg, sqf::sqc::bison::astnode& result_yyarg, sqf::sqc::parser& actual_yyarg, std::string fpath_yyarg);
    virtual ~parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    parser (const parser&);
    parser& operator= (const parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short int yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const short int yytable_[];

  static const short int yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 586,     ///< Last index in yytable_.
      yynnts_ = 32,  ///< Number of nonterminal symbols.
      yyfinal_ = 78, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 59  ///< Number of tokens.
    };


    // User arguments.
    sqf::sqc::tokenizer &tokenizer;
    sqf::sqc::bison::astnode& result;
    sqf::sqc::parser& actual;
    std::string fpath;
  };

  // Symbol number corresponding to token number t.
  inline
  parser::token_number_type
  parser::yytranslate_ (token_type t)
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
      55,    56,    57,    58
    };
    const unsigned int user_token_number_max_ = 313;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 61: // filehead
      case 62: // statements
      case 63: // statement
      case 64: // assignment
      case 65: // vardecl
      case 66: // funcdecl
      case 67: // function
      case 68: // funchead
      case 69: // arglist
      case 70: // codeblock
      case 71: // if
      case 72: // for
      case 73: // while
      case 74: // trycatch
      case 75: // switch
      case 76: // caselist
      case 77: // case
      case 78: // exp01
      case 79: // exp02
      case 80: // exp03
      case 81: // exp04
      case 82: // exp05
      case 83: // exp06
      case 84: // exp07
      case 85: // exp08
      case 86: // exp09
      case 87: // expp
      case 88: // value
      case 89: // array
      case 90: // explist
        value.copy< sqf::sqc::bison::astnode > (other.value);
        break;

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
        value.copy< tokenizer::token > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 61: // filehead
      case 62: // statements
      case 63: // statement
      case 64: // assignment
      case 65: // vardecl
      case 66: // funcdecl
      case 67: // function
      case 68: // funchead
      case 69: // arglist
      case 70: // codeblock
      case 71: // if
      case 72: // for
      case 73: // while
      case 74: // trycatch
      case 75: // switch
      case 76: // caselist
      case 77: // case
      case 78: // exp01
      case 79: // exp02
      case 80: // exp03
      case 81: // exp04
      case 82: // exp05
      case 83: // exp06
      case 84: // exp07
      case 85: // exp08
      case 86: // exp09
      case 87: // expp
      case 88: // value
      case 89: // array
      case 90: // explist
        value.copy< sqf::sqc::bison::astnode > (v);
        break;

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
        value.copy< tokenizer::token > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const sqf::sqc::bison::astnode v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const tokenizer::token v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 61: // filehead
      case 62: // statements
      case 63: // statement
      case 64: // assignment
      case 65: // vardecl
      case 66: // funcdecl
      case 67: // function
      case 68: // funchead
      case 69: // arglist
      case 70: // codeblock
      case 71: // if
      case 72: // for
      case 73: // while
      case 74: // trycatch
      case 75: // switch
      case 76: // caselist
      case 77: // case
      case 78: // exp01
      case 79: // exp02
      case 80: // exp03
      case 81: // exp04
      case 82: // exp05
      case 83: // exp06
      case 84: // exp07
      case 85: // exp08
      case 86: // exp09
      case 87: // expp
      case 88: // value
      case 89: // array
      case 90: // explist
        value.template destroy< sqf::sqc::bison::astnode > ();
        break;

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
        value.template destroy< tokenizer::token > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 61: // filehead
      case 62: // statements
      case 63: // statement
      case 64: // assignment
      case 65: // vardecl
      case 66: // funcdecl
      case 67: // function
      case 68: // funchead
      case 69: // arglist
      case 70: // codeblock
      case 71: // if
      case 72: // for
      case 73: // while
      case 74: // trycatch
      case 75: // switch
      case 76: // caselist
      case 77: // case
      case 78: // exp01
      case 79: // exp02
      case 80: // exp03
      case 81: // exp04
      case 82: // exp05
      case 83: // exp06
      case 84: // exp07
      case 85: // exp08
      case 86: // exp09
      case 87: // expp
      case 88: // value
      case 89: // array
      case 90: // explist
        value.move< sqf::sqc::bison::astnode > (s.value);
        break;

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
        value.move< tokenizer::token > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  parser::by_type::type_get () const
  {
    return type;
  }

  inline
  parser::token_type
  parser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  parser::symbol_type
  parser::make_NA (const location_type& l)
  {
    return symbol_type (token::NA, l);
  }

  parser::symbol_type
  parser::make_BE (const location_type& l)
  {
    return symbol_type (token::BE, l);
  }

  parser::symbol_type
  parser::make_BREAK (const location_type& l)
  {
    return symbol_type (token::BREAK, l);
  }

  parser::symbol_type
  parser::make_RETURN (const location_type& l)
  {
    return symbol_type (token::RETURN, l);
  }

  parser::symbol_type
  parser::make_THROW (const location_type& l)
  {
    return symbol_type (token::THROW, l);
  }

  parser::symbol_type
  parser::make_LET (const location_type& l)
  {
    return symbol_type (token::LET, l);
  }

  parser::symbol_type
  parser::make_FUNCTION (const location_type& l)
  {
    return symbol_type (token::FUNCTION, l);
  }

  parser::symbol_type
  parser::make_FINAL (const location_type& l)
  {
    return symbol_type (token::FINAL, l);
  }

  parser::symbol_type
  parser::make_FALSE (const location_type& l)
  {
    return symbol_type (token::FALSE, l);
  }

  parser::symbol_type
  parser::make_FOR (const location_type& l)
  {
    return symbol_type (token::FOR, l);
  }

  parser::symbol_type
  parser::make_IF (const location_type& l)
  {
    return symbol_type (token::IF, l);
  }

  parser::symbol_type
  parser::make_ELSE (const location_type& l)
  {
    return symbol_type (token::ELSE, l);
  }

  parser::symbol_type
  parser::make_FROM (const location_type& l)
  {
    return symbol_type (token::FROM, l);
  }

  parser::symbol_type
  parser::make_TO (const location_type& l)
  {
    return symbol_type (token::TO, l);
  }

  parser::symbol_type
  parser::make_DO (const location_type& l)
  {
    return symbol_type (token::DO, l);
  }

  parser::symbol_type
  parser::make_TRY (const location_type& l)
  {
    return symbol_type (token::TRY, l);
  }

  parser::symbol_type
  parser::make_CATCH (const location_type& l)
  {
    return symbol_type (token::CATCH, l);
  }

  parser::symbol_type
  parser::make_STEP (const location_type& l)
  {
    return symbol_type (token::STEP, l);
  }

  parser::symbol_type
  parser::make_SWITCH (const location_type& l)
  {
    return symbol_type (token::SWITCH, l);
  }

  parser::symbol_type
  parser::make_CASE (const location_type& l)
  {
    return symbol_type (token::CASE, l);
  }

  parser::symbol_type
  parser::make_DEFAULT (const location_type& l)
  {
    return symbol_type (token::DEFAULT, l);
  }

  parser::symbol_type
  parser::make_NIL (const location_type& l)
  {
    return symbol_type (token::NIL, l);
  }

  parser::symbol_type
  parser::make_TRUE (const location_type& l)
  {
    return symbol_type (token::TRUE, l);
  }

  parser::symbol_type
  parser::make_PARAMS (const location_type& l)
  {
    return symbol_type (token::PARAMS, l);
  }

  parser::symbol_type
  parser::make_PRIVATE (const location_type& l)
  {
    return symbol_type (token::PRIVATE, l);
  }

  parser::symbol_type
  parser::make_WHILE (const location_type& l)
  {
    return symbol_type (token::WHILE, l);
  }

  parser::symbol_type
  parser::make_CURLYO (const location_type& l)
  {
    return symbol_type (token::CURLYO, l);
  }

  parser::symbol_type
  parser::make_CURLYC (const location_type& l)
  {
    return symbol_type (token::CURLYC, l);
  }

  parser::symbol_type
  parser::make_ROUNDO (const location_type& l)
  {
    return symbol_type (token::ROUNDO, l);
  }

  parser::symbol_type
  parser::make_ROUNDC (const location_type& l)
  {
    return symbol_type (token::ROUNDC, l);
  }

  parser::symbol_type
  parser::make_SQUAREO (const location_type& l)
  {
    return symbol_type (token::SQUAREO, l);
  }

  parser::symbol_type
  parser::make_SQUAREC (const location_type& l)
  {
    return symbol_type (token::SQUAREC, l);
  }

  parser::symbol_type
  parser::make_SEMICOLON (const location_type& l)
  {
    return symbol_type (token::SEMICOLON, l);
  }

  parser::symbol_type
  parser::make_COMMA (const location_type& l)
  {
    return symbol_type (token::COMMA, l);
  }

  parser::symbol_type
  parser::make_EQUAL (const location_type& l)
  {
    return symbol_type (token::EQUAL, l);
  }

  parser::symbol_type
  parser::make_DOT (const location_type& l)
  {
    return symbol_type (token::DOT, l);
  }

  parser::symbol_type
  parser::make_QUESTIONMARK (const location_type& l)
  {
    return symbol_type (token::QUESTIONMARK, l);
  }

  parser::symbol_type
  parser::make_ANDAND (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::ANDAND, v, l);
  }

  parser::symbol_type
  parser::make_SLASH (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::SLASH, v, l);
  }

  parser::symbol_type
  parser::make_STAR (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::STAR, v, l);
  }

  parser::symbol_type
  parser::make_PERCENT (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::PERCENT, v, l);
  }

  parser::symbol_type
  parser::make_VLINEVLINE (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::VLINEVLINE, v, l);
  }

  parser::symbol_type
  parser::make_COLON (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::COLON, v, l);
  }

  parser::symbol_type
  parser::make_PLUS (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::PLUS, v, l);
  }

  parser::symbol_type
  parser::make_MINUS (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::MINUS, v, l);
  }

  parser::symbol_type
  parser::make_LTEQUAL (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::LTEQUAL, v, l);
  }

  parser::symbol_type
  parser::make_LT (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::LT, v, l);
  }

  parser::symbol_type
  parser::make_GTEQUAL (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::GTEQUAL, v, l);
  }

  parser::symbol_type
  parser::make_GT (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::GT, v, l);
  }

  parser::symbol_type
  parser::make_EQUALEQUALEQUAL (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::EQUALEQUALEQUAL, v, l);
  }

  parser::symbol_type
  parser::make_EQUALEQUAL (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::EQUALEQUAL, v, l);
  }

  parser::symbol_type
  parser::make_EXCLAMATIONMARKEQUALEQUAL (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::EXCLAMATIONMARKEQUALEQUAL, v, l);
  }

  parser::symbol_type
  parser::make_EXCLAMATIONMARKEQUAL (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::EXCLAMATIONMARKEQUAL, v, l);
  }

  parser::symbol_type
  parser::make_EXCLAMATIONMARK (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::EXCLAMATIONMARK, v, l);
  }

  parser::symbol_type
  parser::make_NUMBER (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::NUMBER, v, l);
  }

  parser::symbol_type
  parser::make_IDENT (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::IDENT, v, l);
  }

  parser::symbol_type
  parser::make_STRING (const tokenizer::token& v, const location_type& l)
  {
    return symbol_type (token::STRING, v, l);
  }


#line 8 "parser.y" // lalr1.cc:377
} } } //  sqf::sqc::bison 
#line 1854 "parser.tab.hh" // lalr1.cc:377




#endif // !YY_YY_PARSER_TAB_HH_INCLUDED
