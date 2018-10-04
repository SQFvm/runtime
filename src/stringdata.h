#pragma once
#include <string>
#include <memory>
#include "data.h"
#include "string_op.h"

namespace sqf
{
	class stringdata : public data
	{
	private:
		std::string mvalue;
	public:
		stringdata() : mvalue(std::string()) {}
		stringdata(std::string s) : mvalue(s[0] != '"' && s[0] != '\'' ? s : parse_from_sqf(s)) {}
		stringdata(std::string s, bool parse) : mvalue(parse ? parse_from_sqf(s) : s) {}
		virtual std::string tosqf() const { return parse_to_sqf(mvalue); }
		operator const char*() const { return mvalue.c_str(); }
		operator std::string() const { return mvalue; }

		static std::string parse_from_sqf(std::string);
		static std::string parse_to_sqf(std::string);
		size_t length() { return mvalue.size(); }
		virtual bool equals(std::shared_ptr<data> d) const { return 0 == str_cmpi(mvalue.c_str(), -1, std::dynamic_pointer_cast<stringdata>(d)->mvalue.c_str(), -1); }
	};
}