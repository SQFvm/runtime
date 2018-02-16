#pragma once
#include <string>
#include <vector>
#include <ostream>
#include "astnode.h"

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
				short(*precedence)(std::wstring)
			) : merr(err), mdbgsegmentcb(dbgsegment), mcontains_nular(contains_nular), mcontains_unary(contains_unary), mcontains_binary(contains_binary), mprecedence(precedence) {}
			std::wostream& err(void) { return *merr; }
			std::wstring dbgsegment(const wchar_t* full, size_t off, size_t length) { return mdbgsegmentcb(full, off, length); }
			bool contains_nular(std::wstring s) { return mcontains_nular(s); }
			bool contains_unary(std::wstring s) { return mcontains_unary(s); }
			bool contains_binary(std::wstring s) { return mcontains_binary(s); }
			short precedence(std::wstring s) { return mprecedence(s); }
		};
	}
}