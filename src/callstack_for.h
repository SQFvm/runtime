#pragma once
#include <memory>
#include "callstack.h"

namespace sqf
{
	class fordata;
	class codedata;
	class callstack_for : public callstack
	{
	private:
		std::shared_ptr<fordata> mfordata;
		std::shared_ptr<codedata> mexec;
	public:
		callstack_for(std::shared_ptr<sqf::sqfnamespace> ns, std::shared_ptr<fordata> fordata, std::shared_ptr<codedata> exec);
		std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) override;
	};
}