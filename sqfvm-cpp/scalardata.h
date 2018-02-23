#pragma once
#include <string>
#include <memory>
#include "data.h"

namespace sqf
{
	class scalardata : public data
	{
	private:
		double mvalue;
	public:
		scalardata(void) : mvalue(0) {}
		scalardata(char num) : mvalue((double)num) {}
		scalardata(short num) : mvalue((double)num) {}
		scalardata(int num) : mvalue((double)num) {}
		scalardata(long num) : mvalue((double)num) {}
		scalardata(float num) : mvalue((double)num) {}
		scalardata(double num) : mvalue((double)num) {}
		scalardata(size_t num) : mvalue((double)num) {}
		virtual std::wstring tosqf(void) const { return std::to_wstring(mvalue); }
		operator char(void) const { return (char)mvalue; }
		operator short(void) const { return (short)mvalue; }
		operator int(void) const { return (int)mvalue; }
		operator long(void) const { return (long)mvalue; }
		operator float(void) const { return (float)mvalue; }
		operator double(void) const { return (double)mvalue; }
		operator size_t(void) const { return (size_t)mvalue; }
		virtual bool equals(std::shared_ptr<data> d) const { return mvalue == std::dynamic_pointer_cast<scalardata>(d)->mvalue; }
	};
}