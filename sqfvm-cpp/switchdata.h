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
		virtual std::string tosqf(void) const;
		inline void flag(bool f) { mflag = f; }
		inline bool flag(void) { return mflag; }
		inline void executed(bool f) { mexecuted = f; }
		inline bool executed(void) { return mexecuted; }
		inline std::shared_ptr<value> val(void) { return mvalue; }
		inline void defaultexec(std::shared_ptr<codedata> exec) { mdefaultexec = exec; }
		inline std::shared_ptr<codedata> defaultexec(void) { return mdefaultexec; }
		virtual bool equals(std::shared_ptr<data> d) const { return mvalue == std::dynamic_pointer_cast<switchdata>(d)->mvalue; }
	};
}