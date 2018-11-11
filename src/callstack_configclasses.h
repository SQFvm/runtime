#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class codedata;
	class configdata;
	class callstack_configclasses : public callstack
	{
	private:
		size_t mcurindex;
		bool mend;
		std::vector<std::shared_ptr<value>> moutarr;
		std::shared_ptr<codedata> mcond;
		std::shared_ptr<configdata> mdata;
	public:
		callstack_configclasses(
			std::shared_ptr<sqf::sqfnamespace> ns,
			std::shared_ptr<configdata> data,
			std::shared_ptr<codedata> cond
		)
		: callstack(ns),
			mcurindex(0),
			mdata(data),
			mcond(std::move(cond))
		{}
		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;
	};
}