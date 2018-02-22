#pragma once
#include <string>
#include <memory>
#include <vector>

#include "data.h"
namespace sqf
{
	class vmstack;
	class scriptdata : public data
	{
	private:
		std::shared_ptr<sqf::vmstack> mthisstack;
	public:
		scriptdata();
		inline std::shared_ptr<sqf::vmstack> stack(void) { return mthisstack; }
		bool hasfinished(void);
		virtual std::wstring tosqf(void) const { return L"SCRIPT"; }
		virtual bool equals(std::shared_ptr<data> d) const { return this == d.get(); }
	};
}