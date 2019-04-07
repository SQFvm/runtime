#pragma once
#include <memory>
#include <string>
#include <vector>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class configdata;
	class callstack_configproperties : public callstack
	{
	private:
		size_t mcurindex;
		bool mend;
		std::vector<std::shared_ptr<value>> moutarr;
		std::shared_ptr<codedata> mcond;
		std::shared_ptr<configdata> mdata;
		std::vector<std::string> mvisited;
		bool minclude_inherited;
	public:
		callstack_configproperties(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<configdata> data,
			std::shared_ptr<codedata> cond,
			bool include_inherited
		)
		: callstack(ns),
			mcurindex(0),
			mdata(data),
			mcond(std::move(cond)),
			minclude_inherited(include_inherited),
			mend(false)
		{}
		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;

		std::string get_name() override { return "callstack_configproperties"; }
	};
}