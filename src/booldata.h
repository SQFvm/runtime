#pragma once
#include <string>
#include <memory>

#include "data.h"
namespace sqf
{
	class booldata : public data
	{
	private:
		bool mflag;
	public:
		booldata() : mflag(false) {}
		booldata(bool flag) : mflag(flag) {}
		std::string tosqf() const override { return std::string(mflag ? "true" : "false"); }
		operator bool() const { return mflag; }
		bool equals(std::shared_ptr<data> d) const override { return mflag == std::dynamic_pointer_cast<booldata>(d)->mflag; }
	};
	typedef std::shared_ptr<booldata> bool_s;
	typedef std::weak_ptr<booldata> bool_w;
	typedef std::unique_ptr<booldata> bool_u;
}