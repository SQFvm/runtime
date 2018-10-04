#pragma once
#include <string>
#include <memory>
#include "data.h"

namespace sqf
{
	class scalardata : public data
	{
	private:
		float mvalue;
		static int decimals;
	public:
		scalardata() : mvalue(0) {}
		scalardata(char num) : mvalue((float)num) {}
		scalardata(short num) : mvalue((float)num) {}
		scalardata(int num) : mvalue((float)num) {}
		scalardata(long num) : mvalue((float)num) {}
		scalardata(float num) : mvalue((float)num) {}
		scalardata(double num) : mvalue((float)num) {}
		scalardata(size_t num) : mvalue((float)num) {}
		virtual std::string tosqf() const;
		operator char() const { return (char)mvalue; }
		operator short() const { return (short)mvalue; }
		operator int() const { return (int)mvalue; }
		operator long() const { return (long)mvalue; }
		operator float() const { return (float)mvalue; }
		operator size_t() const { return (size_t)mvalue; }
		virtual bool equals(std::shared_ptr<data> d) const { return mvalue == std::dynamic_pointer_cast<scalardata>(d)->mvalue; }

		static void setdecimals(int val) { decimals = val; }
	};
}