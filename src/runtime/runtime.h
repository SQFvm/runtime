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
			/// Allows to set a maximum to VM runtime
			/// Ignored if 0.
			/// </summary>
			std::chrono::milliseconds max_runtime;

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
		sqf::runtime::diagnostics::breakpoint m_last_breakpoint_hit;
	public:
		std::vector<sqf::runtime::context>::iterator active_context() const { return m_active_context; };
		const std::vector<sqf::runtime::diagnostics::breakpoint>& breakpoints() const { return m_breakpoints; }
		std::vector<sqf::runtime::diagnostics::breakpoint>& breakpoints() { return m_breakpoints; }

		void breakpoint_hit(sqf::runtime::diagnostics::breakpoint breakpoint) { m_last_breakpoint_hit = breakpoint; m_is_halt_requested = true; }

#pragma endregion

	private:
		configuration m_configuration;
		std::chrono::system_clock::time_point m_runtime_timestamp;
		bool m_runtime_error;

		std::vector<sqf::runtime::context> m_contexts;
		std::vector<sqf::runtime::context>::iterator m_active_context;

		std::chrono::system_clock::time_point m_created_timestamp;
		std::chrono::system_clock::time_point m_current_time;


	public:
		runtime(Logger& logger, configuration config) :
			CanLog(logger),
			m_state(state::halted),
			m_exit_code(0),
			m_is_exit_requested(false),
			m_is_halt_requested(false),
			m_last_breakpoint_hit(-1, {}),
			m_configuration(config),
			m_runtime_error(false)
		{
		}

		void push_back(sqf::runtime::context context) { m_contexts.push_back(context); }
		sqf::runtime::runtime::result execute(sqf::runtime::runtime::action action);
		const sqf::runtime::runtime::configuration configuration() const { return m_configuration; }
		std::chrono::system_clock::time_point runtime_timestamp() { return m_runtime_timestamp; }
		void runtime_timestamp_reset() { m_runtime_timestamp = std::chrono::system_clock::now(); }




		/// <summary>
		/// Breaks encapsulation to provide access to local-logger of runtime.
		/// </summary>
		/// <remarks>
		/// Should not be used outside of non-runtime-related operations (eg. Normal program IO)
		/// and only inside of actual rutime-related operations (eg. operators)
		/// </remarks>
		/// <param name="message"></param>
		inline void __logmsg(LogMessageBase&& message)
		{
			log(message);
			if (message.getLevel() <= loglevel::error)
			{
				m_runtime_error = true;
			}
		}
		inline bool& __runtime_error() { return m_runtime_error; }
	};
}
