#pragma once
#include <string>
#include <memory>
#include "data.h"
#include <cmath>

namespace sqf
{
	class scalardata : public data
	{
	private:
		float mvalue;
		static int decimals;
	public:
		scalardata() : mvalue(0) {}
		scalardata(float num) : mvalue(static_cast<float>(num)) {}
		std::string tosqf() const override;
		operator float() const { return static_cast<float>(mvalue); }
		float as_float() const { return static_cast<float>(mvalue); }
		void set(float f) { mvalue = f; }

		bool equals(std::shared_ptr<data> d) const override {
			const auto scalData = std::dynamic_pointer_cast<scalardata>(d);
			if (!scalData) return false;
			return mvalue == scalData->mvalue;
		}
        sqf::type dtype() const override { return std::isnan(mvalue) ? sqf::type::NaN : sqf::type::SCALAR; }

		static void setdecimals(int val) { decimals = val; }
	};
}