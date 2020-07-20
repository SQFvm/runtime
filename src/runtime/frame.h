#pragma once

#include "instruction_set.h"
#include "value_scope.h"

namespace sqf::runtime
{
	class runtime;
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
				seek_end,
				/// <summary>
				/// Tells that the active instruction_set should be changed to the one
				/// provided by this behavior.
				/// </summary>
				exchange,
				/// <summary>
				/// Indicates a failure state of the enact method.
				/// If not used within error_behavior, then this acts like result::ok
				/// </summary>
				fail
			};
			virtual sqf::runtime::instruction_set get_instruction_set(sqf::runtime::frame& frame) { return {}; };
			virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) = 0;
			behavior() = default;
		};
		enum class result
		{
			/// <summary>
			/// Something moved wrong
			/// </summary>
			error = -1,
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
		enum seek_
		{
			end,
			start
		};
	private:
		sqf::runtime::instruction_set m_instruction_set;
		sqf::runtime::instruction_set::reverse_iterator m_iterator;
		std::shared_ptr<behavior> m_enter_behavior;
		std::shared_ptr<behavior> m_exit_behavior;
		std::shared_ptr<behavior> m_error_behavior;
		std::shared_ptr<sqf::runtime::value_scope> m_globals_value_scope;
		bool m_bubble_variable;
	public:
		frame() : frame({}, {}, {}, {}, {}) {}
		frame(std::shared_ptr<sqf::runtime::value_scope> globals_scope, std::shared_ptr<behavior> enter_behavior, sqf::runtime::instruction_set instruction_set) : frame(globals_scope, enter_behavior, instruction_set, {}, {}) {}
		frame(std::shared_ptr<sqf::runtime::value_scope> globals_scope, sqf::runtime::instruction_set instruction_set, std::shared_ptr<behavior> exit_behavior) : frame(globals_scope, {}, instruction_set, exit_behavior, {}) {}
		frame(std::shared_ptr<sqf::runtime::value_scope> globals_scope, sqf::runtime::instruction_set instruction_set) : frame(globals_scope, {}, instruction_set, {}, {}) {}
		frame(std::shared_ptr<sqf::runtime::value_scope> globals_scope,
			std::shared_ptr<behavior> enter_behavior,
			sqf::runtime::instruction_set instruction_set,
			std::shared_ptr<behavior> exit_behavior,
			std::shared_ptr<behavior> error_behavior)
			:
			m_instruction_set(instruction_set),
			m_iterator(instruction_set.rbegin()),
			m_enter_behavior(enter_behavior),
			m_exit_behavior(exit_behavior),
			m_error_behavior(error_behavior),
			m_globals_value_scope(globals_scope),
			m_bubble_variable(true)
		{}

		bool can_recover_runtime_error() { return m_error_behavior != nullptr; }
		result recover_runtime_error(runtime& runtime)
		{
			if (!m_error_behavior) { return result::error; }
			switch (m_error_behavior->enact(runtime, *this))
			{
			case behavior::result::seek_end:
				m_iterator = m_instruction_set.rend();
				return result::done;
			case behavior::result::seek_start:
				m_iterator = m_instruction_set.rbegin();
				return result::ok;
			case behavior::result::exchange:
				m_instruction_set = m_enter_behavior->get_instruction_set(*this);
				m_iterator = m_instruction_set.rbegin();
				return result::ok;
			case behavior::result::fail:
				return result::error;
			}
			return result::ok;
		}

		void seek(long target, seek_ from)
		{
			switch (from)
			{
			case sqf::runtime::frame::end:
				m_iterator = m_instruction_set.rend() + target;
				break;
			case sqf::runtime::frame::start:
				m_iterator = m_instruction_set.rbegin() + target;
				break;
			default:
				break;
			}
		}
		size_t position() const
		{
			return m_iterator - m_instruction_set.rbegin();
		}

		sqf::runtime::instruction_set::reverse_iterator peek() const { bool flag; return peek(flag); }
		sqf::runtime::instruction_set::reverse_iterator peek(bool& success) const
		{
			auto it = m_iterator == m_instruction_set.rend() ? m_iterator : m_iterator + 1;
			success = it != m_instruction_set.rend();
			return it;
		}

		bool bubble_variable() const { return m_bubble_variable; }
		void bubble_variable(bool flag) { m_bubble_variable = flag; }

		sqf::runtime::instruction_set::reverse_iterator current() const { return m_iterator; }
		std::shared_ptr<sqf::runtime::value_scope> globals_value_scope() const { return m_globals_value_scope; }
		void globals_value_scope(std::shared_ptr<sqf::runtime::value_scope> scope) { m_globals_value_scope = scope; }

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
		result next(runtime& runtime)
		{
			if (m_iterator == m_instruction_set.rbegin())
			{
				switch (m_enter_behavior->enact(runtime, *this))
				{
				case behavior::result::seek_end:
					m_iterator = m_instruction_set.rend();
					break;
				case behavior::result::seek_start:
					m_iterator = m_instruction_set.rbegin();
					break;
				case behavior::result::exchange:
					m_instruction_set = m_enter_behavior->get_instruction_set(*this);
					m_iterator = m_instruction_set.rbegin();
					break;
				}
			}
			if (m_iterator != m_instruction_set.rend())
			{
				auto res = next();
				if (m_iterator == m_instruction_set.rend())
				{
					switch (m_enter_behavior->enact(runtime, *this))
					{
					case behavior::result::seek_end:
						m_iterator = m_instruction_set.rend();
						return result::done;
					case behavior::result::seek_start:
						m_iterator = m_instruction_set.rbegin();
						return next();
					case behavior::result::exchange:
						m_instruction_set = m_enter_behavior->get_instruction_set(*this);
						m_iterator = m_instruction_set.rbegin();
						return next();
					}
				}
				return res;
			}
			else
			{
				return next();
			}
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
	};
}