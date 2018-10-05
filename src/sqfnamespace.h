#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "data.h"
#include "varscope.h"

namespace sqf
{
	class sqfnamespace : public data, public varscope
	{
	private:
		std::string mname;
	public:
		sqfnamespace(std::string name) { mname = name; }
		std::string tosqf() const override { return mname; }
		std::string get_name() const { return mname; }
		bool equals(std::shared_ptr<data> d) const override { return this == d.get(); }
	};
}