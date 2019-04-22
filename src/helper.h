#pragma once
#include <string>
#include <vector>
#include <ostream>
#include <iosfwd>
#include "astnode.h"

namespace sqf
{
	namespace parse
	{

		class helper
		{
			std::basic_ostream<char, std::char_traits<char>>* merr;
			std::string(*mdbgsegmentcb)(const char*, size_t, size_t);
			bool(*mcontains_nular)(std::string_view);
			bool(*mcontains_unary)(std::string_view);
			//precedence 0 is being used as placeholder for `any`
			bool(*mcontains_binary)(std::string_view, short);
			short(*mprecedence)(std::string_view);
			bool merr_hasdata = false;
		public:
			helper(
				std::basic_ostream<char, std::char_traits<char>>* err,
				std::string(*dbgsegment)(const char*, size_t, size_t),
				bool(*contains_nular)(std::string_view),
				bool(*contains_unary)(std::string_view),
				bool(*contains_binary)(std::string_view, short),
				short(*precedence)(std::string_view)
			) : merr(err), mdbgsegmentcb(dbgsegment), mcontains_nular(contains_nular), mcontains_unary(contains_unary), mcontains_binary(contains_binary), mprecedence(precedence) {}
			std::basic_ostream<char, std::char_traits<char>>& err() { merr_hasdata = true; return *merr; }
			bool err_hasdata() { return merr_hasdata; }
			std::string dbgsegment(const char* full, size_t off, size_t length) { return mdbgsegmentcb(full, off, length); }
			bool contains_nular(std::string_view s) { return mcontains_nular(s); }
			bool contains_unary(std::string_view s) { return mcontains_unary(s); }
			bool contains_binary(std::string_view s, short p) { return mcontains_binary(s, p); }
			short precedence(std::string_view s) { return mprecedence(s); }
		};
	}
}