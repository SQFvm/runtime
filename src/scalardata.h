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
		scalardata(void) : mvalue(0) {}
		scalardata(char num) : mvalue((float)num) {}
		scalardata(short num) : mvalue((float)num) {}
		scalardata(int num) : mvalue((float)num) {}
		scalardata(long num) : mvalue((float)num) {}
		scalardata(float num) : mvalue((float)num) {}
		scalardata(double num) : mvalue((float)num) {}
		scalardata(size_t num) : mvalue((float)num) {}
		virtual std::string tosqf(void) const;
		operator char(void) const { return (char)mvalue; }
		operator short(void) const { return (short)mvalue; }
		operator int(void) const { return (int)mvalue; }
		operator long(void) const { return (long)mvalue; }
		operator float(void) const { return (float)mvalue; }
		operator size_t(void) const { return (size_t)mvalue; }
		virtual bool equals(std::shared_ptr<data> d) const { return mvalue == std::dynamic_pointer_cast<scalardata>(d)->mvalue; }

		static void setdecimals(int val) { decimals = val; }
	};
}