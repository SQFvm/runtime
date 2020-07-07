#pragma once

#include "instruction_set.h"
#include "value_scope.h"

namespace sqf::runtime
{
	class context;
	class frame : public sqf::runtime::value_scope
	{
	public:
		class behavior
		{
		public:
			enum class result
			{
				/// <summary>
				/// Tells that nothing should happen on Frame level.
				/// </summary>
				ok,
				/// <summary>
				/// Tells that Frame should seek to start.
				/// </summary>
				seek_start,
				/// <summary>
				/// Tells that Frame should seek to end.
				/// </summary>
				seek_end
			};
			virtual result enact(sqf::runtime::context& context, const sqf::runtime::frame& frame) { return result::ok; }
			behavior() = default;
		};
		enum class result
		{
			/// <summary>
			/// Tells that the current instruction-set is done.
			/// Will always be returned once last instruction was reached.
			/// </summary>
			/// <remarks>
			/// If frame contains 3 instructions, the 3th call may return done.
			/// </remarks>
			done,
			/// <summary>
			/// Returned on success.
			/// </summary>
			ok
		};
	private:
		sqf::runtime::instruction_set m_instruction_set;
		sqf::runtime::instruction_set::reverse_iterator m_iterator;
		sqf::runtime::frame::behavior m_enter_behavior;
		sqf::runtime::frame::behavior m_exit_behavior;
	public:
		frame(sqf::runtime::frame::behavior enter_behavior, sqf::runtime::instruction_set instruction_set) : frame(enter_behavior, instruction_set, {}) {}
		frame(sqf::runtime::instruction_set instruction_set, sqf::runtime::frame::behavior exit_behavior) : frame({}, instruction_set, exit_behavior) {}
		frame(sqf::runtime::instruction_set instruction_set) : frame({}, instruction_set, {}) {}
		frame(sqf::runtime::frame::behavior enter_behavior, sqf::runtime::instruction_set instruction_set, sqf::runtime::frame::behavior exit_behavior) :
			m_instruction_set(instruction_set),
			m_iterator(instruction_set.rbegin()),
			m_enter_behavior(enter_behavior),
			m_exit_behavior(exit_behavior)
		{}

		sqf::runtime::instruction_set::reverse_iterator peek() const { bool flag; return peek(flag); }
		sqf::runtime::instruction_set::reverse_iterator peek(bool& success) const
		{
			auto it = m_iterator == m_instruction_set.rend() ? m_iterator : m_iterator + 1;
			success = it != m_instruction_set.rend();
			return it;
		}


		sqf::runtime::instruction_set::reverse_iterator current() const { m_iterator; }

		/// <summary>
		/// Moves current to next instruction.
		/// Will not trigger possible attached behaviors
		/// </summary>
		/// <returns>Enum value describing the success state of the operation.</returns>
		result next()
		{
			if (m_iterator == m_instruction_set.rend()) { return result::done; }
			return ++m_iterator == m_instruction_set.rend() ? result::done : result::ok;
		}
		/// <summary>
		/// Moves current to next instruction.
		/// </summary>
		/// <returns>Enum value describing the success state of the operation.</returns>
		result next(context& context)
		{
			if (m_iterator == m_instruction_set.rbegin()) { m_enter_behavior.enact(context, *this); }
			return next();
		}
		/// <summary>
		/// Reversed sqf::runtime::frame::next().
		/// Moves current to previous instruction.
		/// Will not trigger possible attached behaviors
		/// </summary>
		/// <returns>Enum value describing the success state of the operation.</returns>
		result previous()
		{
			if (m_iterator == m_instruction_set.rbegin()) { return result::done; }
			return --m_iterator == m_instruction_set.rbegin() ? result::done : result::ok;
		}
		/// <summary>
		/// Reversed sqf::runtime::frame::next().
		/// Moves current to previous instruction.
		/// </summary>
		/// <returns>Enum value describing the success state of the operation.</returns>
		result previous(context& context)
		{
			if (m_iterator == m_instruction_set.rbegin()) { m_enter_behavior.enact(context, *this); }
			return previous();
		}

	};
}