#pragma once
#include <memory>
#include <queue>
#include <vector>
#include <string>
#include "varscope.h"

namespace sqf
{
	class instruction;
	class virtualmachine;
	class sqfnamespace;
	class value;


	class callstack : public varscope
	{
	private:
		std::queue<std::shared_ptr<sqf::instruction>> mstack;
		std::shared_ptr<sqf::sqfnamespace> mwith;
		std::vector<std::shared_ptr<sqf::value>> mvalstack;
	protected:
		void clear_stack() { while (!mstack.empty()) { mstack.pop(); } }
	public:
		callstack(std::shared_ptr<sqf::sqfnamespace>);
		void pushinst(std::shared_ptr<sqf::instruction> value) { mstack.push(value); }
		virtual std::shared_ptr<sqf::instruction> popinst(sqf::virtualmachine* vm) { if (mstack.empty()) return std::shared_ptr<sqf::instruction>(); auto ret = mstack.front(); mstack.pop(); return ret; }
		std::shared_ptr<sqf::instruction> peekinst() { if (mstack.empty()) return std::shared_ptr<sqf::instruction>(); auto front = mstack.front(); return front; }
		std::shared_ptr<sqf::sqfnamespace> getnamespace() const { return mwith; }
		size_t inststacksize() const { return mstack.size(); }
		void setnamespace(std::shared_ptr<sqf::sqfnamespace> ns) { mwith = ns; }

		virtual bool recover() { return false; }

		void push_back_value(std::shared_ptr<value> val) { mvalstack.push_back(val); }
		std::shared_ptr<value> pop_back_value(bool &success)
		{
			if (mvalstack.empty())
			{
				success = false;
				return std::shared_ptr<value>();
			}
			success = true;
			auto val = mvalstack.back();
			mvalstack.pop_back();
			return val;
		}
		std::shared_ptr<value> peek_value() { if (mvalstack.empty()) return std::shared_ptr<value>(); return mvalstack.back(); }
		void drop_values() { mvalstack.clear(); }
	};
}
