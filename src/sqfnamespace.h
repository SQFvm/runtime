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
		inline sqfnamespace(std::string name) { mname = name; }
		inline virtual std::string tosqf() const { return mname; }
		inline std::string get_name() { return mname; }
		virtual bool equals(std::shared_ptr<data> d) const { return this == d.get(); }
	};
}