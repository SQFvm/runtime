#pragma once
#include "logging.h"
#include "diagnostics/breakpoint.h"
#include "context.h"

#include <chrono>
#include <atomic>
#include <vector>

namespace sqf::runtime
{
	class runtime : public CanLog
	{
	public:
		enum class action
		{
			start,
			stop,
			abort,
			assembly_step,
			line_step,
			leave_scope,
			// Special execaction to reset the run_atomic flag in case of
			// exception being thrown by the virtualmachine execution method.
			reset_run_atomic
		};
		enum class result
		{
			invalid = -2,
			empty = -1,
			ok,
			action_error,
			runtime_error
		};
		enum class state
		{
			empty,
			halted,
			running,
			halted_error,
			evaluating
		};

		struct configuration
		{
			/// <summary>
			/// Allows to set an upper-limit to the VM instructions that may be exeucted.
			/// Ignored if 0.
			/// </summary>
			unsigned long long max_instructions;

			/// <summary>
			/// If true, all sleeps are ignored.
			/// </summary>
			bool disable_sleep;

			/// <summary>
			/// If true, instructs operators to
			/// error on unexisting classnames.
			/// If false, they shall only warn and return
			/// a dummy/default value for what they would
			/// have read from config normally.
			/// </summary>
			bool enable_classname_check;

			/// <summary>
			/// Wether networking is disabled.
			/// </summary>
			bool disable_networking;


			configuration() = default;
		};

#pragma region Runtime state handling

	private:
		bool m_is_halt_requested;
		bool m_is_exit_requested;
		state m_state;
		int m_exit_code;
		std::atomic<bool> m_run_atomic;

	public:
		bool is_exit_requested() const { return m_is_exit_requested; }
		void exit(int exit_code) { m_exit_code = exit_code; m_is_exit_requested = true; }
		state runtime_state() const { return m_state; }

#pragma endregion
#pragma region Runtime Diagnostics

	private:
		std::vector<sqf::runtime::diagnostics::breakpoint> m_breakpoints;
		int m_last_breakpoint_line_hit;
		unsigned long long m_instructions_count;
	public:
		std::vector<sqf::runtime::context>::iterator active_context() const { return m_active_context; };


#pragma endregion

	private:
		configuration m_configuration;
		std::vector<sqf::runtime::context> m_contexts;
		std::vector<sqf::runtime::context>::iterator m_active_context;

		std::chrono::system_clock::time_point m_created_timestamp;
		std::chrono::system_clock::time_point m_current_time;


	public:
		runtime() = default;

		void push_back(sqf::runtime::context context) { m_contexts.push_back(context); }
		sqf::runtime::runtime::result execute(sqf::runtime::runtime::action action);



	};
}
