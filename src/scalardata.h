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
		scalardata(float num) : mvalue(static_cast<float>(num)) {}
		virtual std::string tosqf() const;
		operator float() const { return static_cast<float>(mvalue); }
		virtual bool equals(std::shared_ptr<data> d) const {
			const auto scalData = std::dynamic_pointer_cast<scalardata>(d);
			if (!scalData) return false;
			return mvalue == scalData->mvalue;
		}

		static void setdecimals(int val) { decimals = val; }
	};
}