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
        stringdata() = default;
		stringdata(std::string s) : mvalue(s) {}
		stringdata(std::string s, bool parse) : mvalue(parse ? parse_from_sqf(s) : s) {}
		std::string tosqf() const override { return parse_to_sqf(mvalue); }
		operator const char*() const { return mvalue.c_str(); }
		operator std::string() const { return mvalue; }

		static std::string parse_from_sqf(std::string_view);
		static std::string parse_to_sqf(std::string_view);
		size_t length() const { return mvalue.size(); }
		bool equals(std::shared_ptr<data> d) const override { return 0 == str_cmpi(mvalue.c_str(), -1, std::dynamic_pointer_cast<stringdata>(d)->mvalue.c_str(), -1); }
		bool equals_exact(std::shared_ptr<data> d) const override { return 0 == str_cmp(mvalue.c_str(), -1, std::dynamic_pointer_cast<stringdata>(d)->mvalue.c_str(), -1); }
        sqf::type dtype() const override { return sqf::type::STRING; }
	};
}