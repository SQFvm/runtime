#pragma once
#include <vector>
#include <memory>
#include <string>

#include "callstack.h"
#include "string_op.h"
#include <chrono>

namespace sqf
{
	class value;
	class instruction;
	class virtualmachine;
	class vmstack
	{
	private:
		std::vector<std::shared_ptr<sqf::callstack>> mstacks;
		std::vector<std::shared_ptr<sqf::value>> mvalstack;
		bool misscheduled;
		std::chrono::system_clock::time_point mwakeupstamp;
		bool misasleep;
	public:
		vmstack() : misscheduled(false), misasleep(false) {}
		vmstack(bool isscheduled) : misscheduled(isscheduled), misasleep(false) {}
		void pushinst(sqf::virtualmachine* vm, std::shared_ptr<instruction> inst);
		std::shared_ptr<instruction> popinst(sqf::virtualmachine* vm)
		{
			if (mstacks.empty())
				return std::shared_ptr<sqf::instruction>();
			auto ret = mstacks.back()->popinst(vm);
			if (!ret.get())
			{
				dropcallstack();
				return popinst(vm);
			}
			return ret;
		}
		std::shared_ptr<instruction> peekinst() { if (mstacks.empty()) return std::shared_ptr<sqf::instruction>(); return mstacks.back()->peekinst(); }
		void pushcallstack(std::shared_ptr<sqf::callstack> cs) { mstacks.push_back(cs); }
		void dropcallstack() { if(!mstacks.empty()) mstacks.pop_back(); }
		void dropcallstack(std::string name, bool include = true)
		{
			int i;
			for (i = (int)mstacks.size() - 1; i >= 0; i--)
			{
				auto stack = mstacks[i];
				if (str_cmpi(stack->getscopename().c_str(), -1, name.c_str(), -1) == 0)
				{
					i = (int)mstacks.size() - i;
					if (include)
					{
						i++;
					}
					for (; i > 0; i--)
					{
						mstacks.pop_back();
					}
					break;
				}
			}
		}


		std::vector<std::shared_ptr<sqf::callstack>>::reverse_iterator stacks_begin() { return mstacks.rbegin(); }
		std::vector<std::shared_ptr<sqf::callstack>>::reverse_iterator stacks_end() { return mstacks.rend(); }
		std::shared_ptr<sqf::callstack> stacks_top() { return mstacks.back(); }

		void pushval(std::shared_ptr<value> val) { mvalstack.push_back(val); }
		std::shared_ptr<value> popval(bool &success) { if (mvalstack.empty()) { success = false; return std::shared_ptr<value>(); } success = true; auto val = mvalstack.back(); mvalstack.pop_back(); return val; }
		std::shared_ptr<value> peekval() { if (mvalstack.empty()) return std::shared_ptr<value>(); return mvalstack.back(); }
		void dropvals() { mvalstack.clear(); }
		std::shared_ptr<value> getlocalvar(std::string varname);
		bool isempty() { return mstacks.size() == 0; }
		bool isscheduled() { return misscheduled; }
		bool isasleep() { return misasleep; }
		void wakeup() { misasleep = false; }
		std::chrono::system_clock::time_point get_wakeupstamp() { return mwakeupstamp; }
		void sleep(std::chrono::milliseconds ms);
	};
}
