#pragma once
#include <string>
#include <memory>
#include <utility>
#include <vector>

#include "data.h"


namespace sqf
{
	class value;
	class codedata;
	class switchdata : public data
	{
	private:
		std::shared_ptr<value> mvalue;
		std::shared_ptr<codedata> mdefaultexec;
		bool mflag;
		bool mexecuted;
	public:
		switchdata(std::shared_ptr<value> val) : mvalue(std::move(val)), mflag(false), mexecuted(false) {}
		std::string tosqf() const override;
		void flag(bool f) { mflag = f; }
		bool flag() const { return mflag; }
		void executed(bool f) { mexecuted = f; }
		bool executed() { return mexecuted; }
		std::shared_ptr<value> val() { return mvalue; }
		void defaultexec(std::shared_ptr<codedata> exec) { mdefaultexec = exec; }
		std::shared_ptr<codedata> defaultexec() { return mdefaultexec; }
		bool equals(std::shared_ptr<data> d) const override { return mvalue == std::dynamic_pointer_cast<switchdata>(d)->mvalue; }
	};
}