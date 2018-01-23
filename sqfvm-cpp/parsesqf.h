#ifndef _PARSESQF
#define _PARSESQF 1

#if !defined(_STRING) & !defined(_STRING_)
#error parsesqf requires <string> header
#endif // !_STRING
#if !defined(_VECTOR) & !defined(_VECTOR_)
#error parsesqf requires <vector> header
#endif // !_VECTOR
#if !defined(_OSTREAM) & !defined(_OSTREAM_)
#error parsesqf requires <ostream> header
#endif // !_OSTREAM
#if !defined(_ASTNODE)
#error parsesqf requires "astnode.h" header
#endif // !_ASTNODE


namespace sqf
{
	namespace parse
	{

		class helper
		{
			std::wostream* merr;
			std::wstring(*mdbgsegmentcb)(const wchar_t*, size_t, size_t);
			bool(*mcontains_nular)(std::wstring);
			bool(*mcontains_unary)(std::wstring);
			bool(*mcontains_binary)(std::wstring);
			short(*mprecedence)(std::wstring);
		public:
			helper(
				std::wostream* err,
				std::wstring(*dbgsegment)(const wchar_t*, size_t, size_t),
				bool(*contains_nular)(std::wstring),
				bool(*contains_unary)(std::wstring),
				bool(*contains_binary)(std::wstring),
				short (*precedence)(std::wstring)
			) : merr(err), mdbgsegmentcb(dbgsegment), mcontains_nular(contains_nular), mcontains_unary(contains_unary), mcontains_binary(contains_binary), mprecedence(precedence) {}
			std::wostream& err(void) { return *merr; }
			std::wstring dbgsegment(const wchar_t* full, size_t off, size_t length) { return mdbgsegmentcb(full, off, length); }
			bool contains_nular(std::wstring s) { return mcontains_nular(s); }
			bool contains_unary(std::wstring s) { return mcontains_unary(s); }
			bool contains_binary(std::wstring s) { return mcontains_binary(s); }
			short precedence(std::wstring s) { return mprecedence(s); }
		};


		size_t endchr(const wchar_t* code, size_t off);
		size_t identifier(const wchar_t* code, size_t off);
		size_t operator_(const wchar_t* code, size_t off);
		size_t hexadecimal(const wchar_t* code, size_t off);
		size_t scalarsub(const wchar_t* code, size_t off);
		size_t scalar(const wchar_t* code, size_t off);
		size_t anytext(const wchar_t* code, size_t off);
		bool SQF_start(helper &h, const wchar_t* code, size_t curoff);
		void SQF(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool STATEMENT_start(helper &h, const wchar_t* code, size_t curoff);
		void STATEMENT(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool ASSIGNMENT_start(helper &h, const wchar_t* code, size_t curoff);
		void ASSIGNMENT(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool BINARYEXPRESSION_start(helper &h, const wchar_t* code, size_t curoff);
		void BINARYEXPRESSION(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag, short* calleeprec = 0);
		bool BRACKETS_start(helper &h, const wchar_t* code, size_t curoff);
		void BRACKETS(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool PRIMARYEXPRESSION_start(helper &h, const wchar_t* code, size_t curoff);
		void PRIMARYEXPRESSION(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool NULAREXPRESSION_start(helper &h, const wchar_t* code, size_t curoff);
		void NULAREXPRESSION(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool UNARYEXPRESSION_start(helper &h, const wchar_t* code, size_t curoff);
		void UNARYEXPRESSION(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool NUMBER_start(helper &h, const wchar_t* code, size_t curoff);
		void NUMBER(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool VARIABLE_start(helper &h, const wchar_t* code, size_t curoff);
		void VARIABLE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool STRING_start(helper &h, const wchar_t* code, size_t curoff);
		void STRING(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool CODE_start(helper &h, const wchar_t* code, size_t curoff);
		void CODE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		bool ARRAY_start(helper &h, const wchar_t* code, size_t curoff);
		void ARRAY(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
		astnode parse_sqf(const wchar_t* codein, helper& h, bool &errflag);
		const wchar_t* astkindname(short id);
	}
}

#endif // !_PARSESQF
