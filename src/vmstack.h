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
		std::vector<std::shared_ptr<sqf::callstack>> m_stacks;
		std::chrono::system_clock::time_point m_wakeup_stamp;
		std::string m_script_name;
		sqf::value m_last_value;
		bool m_is_scheduled;
		bool m_is_asleep;
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

		vmstack() :
			m_script_name(""),
			m_is_scheduled(false),
			m_is_asleep(false),
			m_terminate(false)
		{
		}
		vmstack(bool isscheduled) :
			m_script_name(""),
			m_is_scheduled(isscheduled),
			m_is_asleep(false),
			m_terminate(false)
		{
		}

		void push_back(sqf::virtualmachine* vm, std::shared_ptr<instruction> inst);
		const std::string& script_name() const { return m_script_name; }
		void script_name(std::string val) { if (m_script_name.empty()) { m_script_name = std::move(val); } }
		bool terminate() { return m_terminate; }
		void terminate(bool flag) { m_terminate = flag; }
		std::shared_ptr<instruction> pop_back_instruction(sqf::virtualmachine* vm)
		{
			if (m_stacks.empty())
				return std::shared_ptr<sqf::instruction>();
			
			switch (m_stacks.back()->previous_nextresult())
			{
			case callstack::exitwith:
				drop_callstack();
				// fallthrough
			case callstack::done:
				drop_callstack();
				break;
			}

			if (m_stacks.empty())
				return std::shared_ptr<sqf::instruction>();

			auto res = m_stacks.back()->next(vm);
			if (res == callstack::done || res == callstack::exitwith)
			{
				return pop_back_instruction(vm);
			}
			return m_stacks.back()->current_instruction();
		}
		void push_back(std::shared_ptr<sqf::callstack> cs) { m_stacks.emplace_back(std::move(cs)); m_last_value = {}; }

		/// Will only be set when all stacks have been emptied.
		/// Contains the value returned by the last callstack if available.
		sqf::value last_value() { return m_last_value; }

		/// Drops the top-most callstack and puts the last value
		/// from its value stack onto the lower callstack.
		void drop_callstack()
		{
			if (!m_stacks.empty())
			{
				bool success = false;
				auto value = m_stacks.back()->pop_back_value(success);
				m_stacks.pop_back();
				if (success)
				{
					if (m_stacks.empty())
					{
						m_last_value = value;
					}
					else
					{
						m_stacks.back()->push_back(value);
					}
				}
			}
		}
		void drop_callstack_by_scopename(std::string name, bool include = true)
		{
            for (int i = static_cast<int>(m_stacks.size()) - 1; i >= 0; i--)
			{
				auto stack = m_stacks[i];
				if (str_cmpi(stack->get_scopename().c_str(), -1, name.c_str(), -1) == 0)
				{
					i = static_cast<int>(m_stacks.size()) - i;
					if (include)
					{
						i++;
					}
					for (; i > 0; i--)
					{
						m_stacks.pop_back();
					}
					break;
				}
			}
		}
		void drop_callstack_by_callstackname(std::string name, bool include = true)
		{
			for (int i = static_cast<int>(m_stacks.size()) - 1; i >= 0; i--)
			{
				auto stack = m_stacks[i];
				if (str_cmpi(stack->get_name().c_str(), -1, name.c_str(), -1) == 0)
				{
					i = static_cast<int>(m_stacks.size()) - i;
					if (include)
					{
						i++;
					}
					for (; i > 0; i--)
					{
						m_stacks.pop_back();
					}
					break;
				}
			}
		}


		std::vector<std::shared_ptr<sqf::callstack>>::reverse_iterator stacks_begin() { return m_stacks.rbegin(); }
		std::vector<std::shared_ptr<sqf::callstack>>::reverse_iterator stacks_end() { return m_stacks.rend(); }
		std::shared_ptr<sqf::callstack> stacks_top() { return m_stacks.back(); }

		void push_back(value val)
		{
			m_stacks.back()->push_back(std::move(val));
		}
		value pop_back_value(bool &success)
		{
			if (m_stacks.empty())
			{
				success = false;
				return {};
			}
			return m_stacks.back()->pop_back_value(success);
		}
		value peek_value()
		{
			if (m_stacks.empty())
			{
				return {};
			}
			return m_stacks.back()->peek_value();
		}

		value get_variable(std::string_view varname) { bool flag = false; return get_variable(varname, flag); }
		value get_variable(std::string_view varname, bool& ref_success);
		bool empty() const { return m_stacks.empty(); }
		bool scheduled() const { return m_is_scheduled; }
		bool asleep() const { return m_is_asleep; }
		void wakeup() { m_is_asleep = false; }
		std::chrono::system_clock::time_point get_wakeupstamp() const { return m_wakeup_stamp; }
		void sleep(std::chrono::milliseconds ms);
	};
}
