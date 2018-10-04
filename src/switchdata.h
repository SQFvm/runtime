#pragma once
#include <string>
#include <memory>
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
		switchdata(std::shared_ptr<value> val) : mvalue(val), mflag(false), mexecuted(false) {}
		virtual std::string tosqf() const;
		void flag(bool f) { mflag = f; }
		bool flag() { return mflag; }
		void executed(bool f) { mexecuted = f; }
		bool executed() { return mexecuted; }
		std::shared_ptr<value> val() { return mvalue; }
		void defaultexec(std::shared_ptr<codedata> exec) { mdefaultexec = exec; }
		std::shared_ptr<codedata> defaultexec() { return mdefaultexec; }
		virtual bool equals(std::shared_ptr<data> d) const { return mvalue == std::dynamic_pointer_cast<switchdata>(d)->mvalue; }
	};
}