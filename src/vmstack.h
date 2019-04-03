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
		bool misscheduled;
		std::chrono::system_clock::time_point mwakeupstamp;
		bool misasleep;
		std::string mscriptname;
		std::shared_ptr<sqf::value> mlast_value;
	public:
		vmstack() : misscheduled(false), misasleep(false) {}
		vmstack(bool isscheduled) : misscheduled(isscheduled), misasleep(false) {}
		void pushinst(sqf::virtualmachine* vm, std::shared_ptr<instruction> inst);
		std::string script_name() { return mscriptname; }
		void script_name(std::string val) { if (mscriptname.empty()) { mscriptname = val; } }
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
		void pushcallstack(std::shared_ptr<sqf::callstack> cs) { mstacks.push_back(cs); mlast_value = std::shared_ptr<sqf::value>(); }

		/// Will only be set when all stacks have been emptied.
		/// Contains the value returned by the last callstack if available.
		std::shared_ptr<sqf::value> last_value() { return mlast_value; }

		/// Drops the top-most callstack and puts the last value
		/// from its value stack onto the lower callstack.
		void dropcallstack()
		{
			if (!mstacks.empty())
			{
				bool success = false;
				auto value = mstacks.back()->pop_back_value(success);
				mstacks.pop_back();
				if (success)
				{
					if (mstacks.empty())
					{
						mlast_value = value;
					}
					else
					{
						mstacks.back()->push_back_value(value);
					}
				}
			}
		}
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

		void pushval(std::shared_ptr<value> val)
		{
			mstacks.back()->push_back_value(val);
		}
		std::shared_ptr<value> popval(bool &success)
		{
			if (mstacks.empty())
			{
				success = false;
				return std::shared_ptr<value>();
			}
			return mstacks.back()->pop_back_value(success);
		}
		std::shared_ptr<value> peekval()
		{
			if (mstacks.empty())
			{
				return std::shared_ptr<value>();
			}
			return mstacks.back()->peek_value();
		}

		std::shared_ptr<value> getlocalvar(std::string varname);
		bool isempty() { return mstacks.size() == 0; }
		bool isscheduled() { return misscheduled; }
		bool isasleep() { return misasleep; }
		void wakeup() { misasleep = false; }
		std::chrono::system_clock::time_point get_wakeupstamp() { return mwakeupstamp; }
		void sleep(std::chrono::milliseconds ms);
	};
}
