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
		scalardata(char num) : mvalue(static_cast<float>(num)) {}
		scalardata(short num) : mvalue(static_cast<float>(num)) {}
		scalardata(int num) : mvalue(static_cast<float>(num)) {}
		scalardata(long num) : mvalue(static_cast<float>(num)) {}
		scalardata(float num) : mvalue(static_cast<float>(num)) {}
		scalardata(double num) : mvalue(static_cast<float>(num)) {}
		scalardata(size_t num) : mvalue(static_cast<float>(num)) {}
		virtual std::string tosqf() const;
		operator char() const { return static_cast<char>(mvalue); }
		operator short() const { return static_cast<short>(mvalue); }
		operator int() const { return static_cast<int>(mvalue); }
		operator long() const { return static_cast<long>(mvalue); }
		operator float() const { return static_cast<float>(mvalue); }
		operator size_t() const { return static_cast<size_t>(mvalue); }
		virtual bool equals(std::shared_ptr<data> d) const {
			const auto scalData = std::dynamic_pointer_cast<scalardata>(d);
			if (!scalData) return false;
			return mvalue == scalData->mvalue;
		}

		static void setdecimals(int val) { decimals = val; }
	};
}