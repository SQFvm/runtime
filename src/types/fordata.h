#pragma once
#include <string>
#include <memory>
#include <utility>
#include "data.h"

namespace sqf
{
	class fordata : public data
	{
	private:
		std::string mvar;
		double mfrom;
		double mto;
		double mstep;
	public:
		fordata(std::string var) : mvar(std::move(var)), mfrom(0), mto(0), mstep(1) {}
		std::string tosqf() const override { return "ForType <invisible>"; }
		std::string variable() const { return mvar; }
		double from() const { return mfrom; }
		double to() const { return mto; }
		double step() const { return mstep; }
		void from(double d) { mfrom = d; }
		void to(double d) { mto = d; }
		void step(double d) { mstep = d; }
		bool equals(std::shared_ptr<data> d) const override { return false; }
        sqf::type dtype() const override { return sqf::type::FOR; };
	};
}