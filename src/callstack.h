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
	public:
		enum nextinstres
		{
			NA,
			// Tells that there is no work left to do and nothing has changed
			done,
			// Tells that there may be work left to do and m_current_instruction
			// got changed. (can be changed by calling current_instruction(...))
			updated,
			// Unused
			suspend,
			// Same as done, but also will drop the callstack above 
			exitwith
		};
	private:
		// A std::queue containing the instructions this callstack withholds
		std::queue<std::shared_ptr<sqf::instruction>> m_instruction_queue;

		// The namespace this callstack is using for eg. resolving global variables
		std::shared_ptr<sqf::sqfnamespace> m_used_namepsace;

		// Contains the values that currently are hold for eg. other instructions.
		std::vector<sqf::value> m_value_stack;

		// The current instruction.
		std::shared_ptr<sqf::instruction> m_current_instruction;
		
		// The current nextinstres
		nextinstres m_current_nextinstres;
	protected:

		// Performs the next operation
		virtual nextinstres do_next(sqf::virtualmachine* vm)
		{
			if (m_instruction_queue.empty())
				return done;
			current_instruction(m_instruction_queue.front());
			m_instruction_queue.pop();
			return updated;
		}

		// Sets the current instruction
		void current_instruction(std::shared_ptr<sqf::instruction> inst)
		{
			m_current_instruction = std::move(inst);
		}

	public:
		// Creates a new callstack with provided namespace
		callstack(
			std::shared_ptr<sqf::sqfnamespace> ns
		) : m_used_namepsace(std::move(ns)),
			m_current_nextinstres(NA)
		{
		}

		std::queue<std::shared_ptr<sqf::instruction>>& instruction_queue() { return m_instruction_queue; }

		// Returns the current instruction
		std::shared_ptr<sqf::instruction> current_instruction() const {
			return m_current_instruction;
		}

		// Returns the previos result
		nextinstres previous_nextresult() const {
			return m_current_nextinstres;
		}

		// Moves to the next instruction
		nextinstres next(sqf::virtualmachine* vm)
		{
			m_current_nextinstres = do_next(vm);
			return m_current_nextinstres;
		}

		// Adds an instruction to the this callstack.
		void push_back(std::shared_ptr<sqf::instruction> value)
		{
			m_instruction_queue.emplace(std::move(value));
		}

		// Adds a value onto the valuestack
		void push_back(value val)
		{
			m_value_stack.push_back(std::move(val));
		}

		// Returns the namespace this callstack is using
		// for eg. global variables
		std::shared_ptr<sqf::sqfnamespace> get_namespace() const
		{
			return m_used_namepsace;
		}
		// Sets the namespace this callstack uses for
		// eg. global variables
		void set_namespace(std::shared_ptr<sqf::sqfnamespace> ns)
		{
			m_used_namepsace = std::move(ns);
		}

		// Returns the instruction stacksize
		size_t size_instructions() const
		{
			return m_instruction_queue.size();
		}

		// Wether or not, this callstack supports recovering
		// from VM errors.
		virtual bool can_recover()
		{
			return false;
		}

		// The callstack name.
		virtual std::string get_name()
		{
			return "callstack";
		}

		// Attempts to receive a value from the 
		// value-stack.
		value pop_back_value(bool &success)
		{
			if (m_value_stack.empty())
			{
				success = false;
				return {};
			}
			success = true;
			auto val = m_value_stack.back();
			m_value_stack.pop_back();
			return val;
		}

		// Returns the top value from the value-stack without
		// removing it.
		value peek_value()
		{
			if (m_value_stack.empty())
				return {};
			return m_value_stack.back();
		}
		// Removes ALL values from the value-stack.
		void drop_values()
		{
			m_value_stack.clear();
		}
		// Removes ALL instructions from this callstack
		void drop_instructions()
		{
			while (!m_instruction_queue.empty())
			{
				m_instruction_queue.pop();
			}
		}
	};
}
