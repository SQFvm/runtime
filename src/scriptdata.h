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
		std::shared_ptr<sqf::vmstack> stack() const { return mthisstack; }
		bool hasfinished() const;
		std::string tosqf() const override;
		bool equals(std::shared_ptr<data> d) const override { return this == d.get(); }
        sqf::type dtype() const override { return sqf::type::SCRIPT; }
	};
}