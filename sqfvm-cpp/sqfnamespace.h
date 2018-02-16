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
		std::wstring mname;
	public:
		inline sqfnamespace(std::wstring name) { mname = name; }
		inline virtual std::wstring tosqf(void) const { return mname; }
		inline std::wstring get_name(void) { return mname; }
		virtual bool equals(std::shared_ptr<data> d) const { return this == d.get(); }
	};
}