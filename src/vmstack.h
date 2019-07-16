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
		sqf::value mlast_value;
		bool m_terminate;
	public:
		struct stackdump
		{
			std::shared_ptr<sqf::sqfnamespace> namespace_used;
			size_t line;
			size_t column;
			std::string file;
			std::string dbginf;
			std::string callstack_name;
			std::string scope_name;
		};
		// Creates a stackdump from current top-callstack to the target callstack.
		// If no target is provided, whole callstack will be dumped.
		std::vector<sqf::vmstack::stackdump> dump_callstack_diff(std::shared_ptr<sqf::callstack> target);
		vmstack() : misscheduled(false), misasleep(false), m_terminate(false) {}
		vmstack(bool isscheduled) : misscheduled(isscheduled), misasleep(false) {}
		void pushinst(sqf::virtualmachine* vm, std::shared_ptr<instruction> inst);
		const std::string& script_name() const { return mscriptname; }
		void script_name(std::string val) { if (mscriptname.empty()) { mscriptname = std::move(val); } }
		bool terminate() { return m_terminate; }
		void terminate(bool flag) { m_terminate = flag; }
		std::shared_ptr<instruction> popinst(sqf::virtualmachine* vm)
		{
			if (mstacks.empty())
				return std::shared_ptr<sqf::instruction>();
			
			switch (mstacks.back()->previous_nextresult())
			{
			case callstack::exitwith:
				drop_callstack();
				// fallthrough
			case callstack::done:
				drop_callstack();
				break;
			}

			if (mstacks.empty())
				return std::shared_ptr<sqf::instruction>();

			auto res = mstacks.back()->next(vm);
			if (res == callstack::done || res == callstack::exitwith)
			{
				return popinst(vm);
			}
			return mstacks.back()->current_instruction();
		}
		void pushcallstack(std::shared_ptr<sqf::callstack> cs) { mstacks.emplace_back(std::move(cs)); mlast_value = {}; }

		/// Will only be set when all stacks have been emptied.
		/// Contains the value returned by the last callstack if available.
		sqf::value last_value() { return mlast_value; }

		/// Drops the top-most callstack and puts the last value
		/// from its value stack onto the lower callstack.
		void drop_callstack()
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
						mstacks.back()->push_back(value);
					}
				}
			}
		}
		void drop_callstack_by_scopename(std::string name, bool include = true)
		{
			for (int i = static_cast<int>(mstacks.size()) - 1; i >= 0; i--)
			{
				auto stack = mstacks[i];
				if (str_cmpi(stack->get_scopename().c_str(), -1, name.c_str(), -1) == 0)
				{
					i = static_cast<int>(mstacks.size()) - i;
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
		void drop_callstack_by_callstackname(std::string name, bool include = true)
		{
			for (int i = static_cast<int>(mstacks.size()) - 1; i >= 0; i--)
			{
				auto stack = mstacks[i];
				if (str_cmpi(stack->get_name().c_str(), -1, name.c_str(), -1) == 0)
				{
					i = static_cast<int>(mstacks.size()) - i;
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

		void pushval(value val)
		{
			mstacks.back()->push_back(std::move(val));
		}
		value popval(bool &success)
		{
			if (mstacks.empty())
			{
				success = false;
				return {};
			}
			return mstacks.back()->pop_back_value(success);
		}
		value peekval()
		{
			if (mstacks.empty())
			{
				return {};
			}
			return mstacks.back()->peek_value();
		}

		value getlocalvar(std::string_view varname);
		bool isempty() const { return mstacks.empty(); }
		bool isscheduled() const { return misscheduled; }
		bool isasleep() const { return misasleep; }
		void wakeup() { misasleep = false; }
		std::chrono::system_clock::time_point get_wakeupstamp() const { return mwakeupstamp; }
		void sleep(std::chrono::milliseconds ms);
	};
}
