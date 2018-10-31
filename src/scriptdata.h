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
		std::shared_ptr<sqf::vmstack> stack() { return mthisstack; }
		bool hasfinished();
		std::string tosqf() const override;
		bool equals(std::shared_ptr<data> d) const override { return this == d.get(); }
	};
}