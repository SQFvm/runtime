#pragma once
#include <string>
#include <memory>
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
		fordata(std::string var) : mvar(var), mfrom(0), mto(0), mstep(1) {}
		virtual std::string tosqf() const { return "ForType <invisible>"; }
		std::string variable() const { return mvar; }
		double from() const { return mfrom; }
		double to() const { return mto; }
		double step() const { return mstep; }
		void from(double d) { mfrom = d; }
		void to(double d) { mto = d; }
		void step(double d) { mstep = d; }
		virtual bool equals(std::shared_ptr<data> d) const { return false; }
	};
}