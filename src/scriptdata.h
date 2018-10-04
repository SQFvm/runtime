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
		inline std::shared_ptr<sqf::vmstack> stack() { return mthisstack; }
		bool hasfinished();
		virtual std::string tosqf() const { return "SCRIPT"; }
		virtual bool equals(std::shared_ptr<data> d) const { return this == d.get(); }
	};
}