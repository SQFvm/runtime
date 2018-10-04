#pragma once
#include <memory>
#include <queue>
#include <string>
#include "varscope.h"

namespace sqf
{
	class instruction;
	class virtualmachine;
	class sqfnamespace;


	class callstack : public varscope
	{
	private:
		std::queue<std::shared_ptr<sqf::instruction>> mstack;
		std::shared_ptr<sqf::sqfnamespace> mwith;
	public:
		callstack(std::shared_ptr<sqf::sqfnamespace>);
		void pushinst(std::shared_ptr<sqf::instruction> value) { mstack.push(value); }
		virtual std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) { if (mstack.empty()) return std::shared_ptr<sqf::instruction>(); auto ret = mstack.front(); mstack.pop(); return ret; }
		std::shared_ptr<sqf::instruction> peekinst() { if (mstack.empty()) return std::shared_ptr<sqf::instruction>(); auto front = mstack.front(); return front; }
		std::shared_ptr<sqf::sqfnamespace> getnamespace() const { return mwith; }
		size_t inststacksize() const { return mstack.size(); }
		void setnamespace(std::shared_ptr<sqf::sqfnamespace> ns) { mwith = ns; }
	};
}
