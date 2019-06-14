#pragma once
#include <string>
#include <memory>
#include <utility>
#include <vector>

#include "data.h"
#include "value.h"


namespace sqf
{
	class value;
	class codedata;
	class switchdata : public data
	{
	private:
		value mvalue;
		std::shared_ptr<codedata> mdefaultexec;
		bool mflag;
		bool mexecuted;
	public:
		switchdata(value val) : mvalue(std::move(val)), mflag(false), mexecuted(false) {}
		std::string tosqf() const override;
		void case_matches(bool f) { mflag = f; }
		bool case_matches() const { return mflag; }
		void executed(bool f) { mexecuted = f; }
		bool executed() const { return mexecuted; }
		value val() const { return mvalue; }
		void defaultexec(std::shared_ptr<codedata> exec) { mdefaultexec = std::move(exec); }
		std::shared_ptr<codedata> defaultexec() const { return mdefaultexec; }
		bool equals(std::shared_ptr<data> d) const override { return mvalue.equals(std::dynamic_pointer_cast<switchdata>(d)->mvalue); }
        sqf::type dtype() const override { return sqf::type::SWITCH; }
	};
}