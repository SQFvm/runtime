#include "runtime.h"
#include "diagnostics/stacktrace.h"
#include "diagnostics/d_stacktrace.h"

#include <optional>

#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
#include <iostream>
#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

static sqf::runtime::runtime::result execute_do(sqf::runtime::runtime& runtime, size_t exit_after)
{
    auto& context_active = runtime.context_active();
    auto& runtime_error = runtime.__runtime_error();
    while (true)
    {
        if (runtime.is_exit_requested())
        {
#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m due to \x1B[90mruntime.is_exit_requested() == true\033[0m" << std::endl;
#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return sqf::runtime::runtime::result::ok;
        }
        if (exit_after == 0)
        {
#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m due to \x1B[90mexit_after == 0\033[0m" << std::endl;
#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return sqf::runtime::runtime::result::ok;
        }
        if (context_active.suspended())
        {
#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m due to \x1B[90context_active.suspended() == true\033[0m" << std::endl;
#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return sqf::runtime::runtime::result::ok;
        }
        if (context_active.empty())
        {
#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m due to \x1B[90context_active.empty() == true\033[0m" << std::endl;
#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return sqf::runtime::runtime::result::ok;
        }
        if (runtime.runtime_state() != sqf::runtime::runtime::state::running)
        {
#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m due to \x1B[90runtime.runtime_state() != sqf::runtime::runtime::state::running\033[0m" << std::endl;
#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return sqf::runtime::runtime::result::ok;
        }


        auto frame_count = context_active.frames_size();
        auto& frame = context_active.current_frame();

        auto result = frame.next(runtime);

        if (result == sqf::runtime::frame::result::done && context_active.frames_size() == frame_count)
        { // frame is done executing. Pop it from context and rerun.
            context_active.pop_frame();
            continue;
        }

        auto instruction = frame.current();
        if (runtime.configuration().max_runtime != std::chrono::milliseconds::zero() &&
            runtime.configuration().max_runtime + runtime.runtime_timestamp() < std::chrono::system_clock::now())
        {
            runtime.__logmsg(logmessage::runtime::MaximumRuntimeReached((*instruction)->diag_info(), runtime.configuration().max_runtime));
            runtime.exit(0);
            return sqf::runtime::runtime::result::ok;
        }

        // Check if breakpoint was hit
        {
            auto dinf = (*instruction)->diag_info();
            for (const auto& breakpoint : runtime.breakpoints())
            {
                if (breakpoint.is_enabled() && breakpoint.line() == dinf.line && breakpoint.file() == dinf.path.physical)
                {
                    runtime.breakpoint_hit(breakpoint);
                    context_active.current_frame().previous(); // Unput instruction
                    return sqf::runtime::runtime::result::ok;
                }
            }
        }

        if (exit_after > 0)
        {
            exit_after--;
        }

#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

        {
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "[FC:" << std::setw(3) << context_active.frames_size() << "]" <<
                "[VC:" << std::setw(3) << context_active.values_size() << "]" <<
                "    " << (*instruction)->to_string() << std::endl;
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "    " << LogLocationInfo((*frame.current())->diag_info()).format() << std::endl;

            size_t values_index = 0;
            for (auto& it = context_active.values_begin(); context_active.values_end() != it; ++it)
            {
                std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                    "        " <<
                    "        " <<
                    "    " << "    " << std::setw(2) << values_index++ << ": " << (*it).to_string_sqf() << std::endl;
            }
        }


#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE


        (*instruction)->execute(runtime);

        if (runtime_error)
        {
            runtime_error = false;
            // Build Stacktrace
            std::vector<sqf::runtime::frame> stacktrace_frames(context_active.frames_rbegin(), context_active.frames_rend());
            sqf::runtime::diagnostics::stacktrace stacktrace(stacktrace_frames);

            // Try to find a frame that has recover behavior for runtime error
            auto res = std::find_if(context_active.frames_rbegin(), context_active.frames_rend(),
                [](sqf::runtime::frame& frame) -> bool { return frame.can_recover_runtime_error(); });

            if (res != context_active.frames_rend())
            { // We found a recoverable frame

                // Push Stacktrace to value-stack
                context_active.push_value({ std::make_shared<sqf::types::d_stacktrace>(stacktrace) });

                // Pop all frames between result and current_frame
                size_t frames_to_pop = res - context_active.frames_rbegin();
                for (size_t i = 0; i < frames_to_pop; i++)
                {
                    context_active.pop_frame();
                }

                // Recover from exception
                res->recover_runtime_error(runtime);
            }
            else
            { // No recover frame available, exit method

                runtime.__logmsg(logmessage::runtime::Stacktrace((*instruction)->diag_info(), stacktrace));
                return sqf::runtime::runtime::result::runtime_error;
            }
        }
    }
    return sqf::runtime::runtime::result::ok;
}

sqf::runtime::runtime::result sqf::runtime::runtime::execute(sqf::runtime::runtime::action action)
{
    sqf::runtime::runtime::result res = result::invalid;
    bool expected = false;
    switch (action)
    {
    case action::leave_scope:
        if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
        {
            m_is_exit_requested = false;
            m_is_halt_requested = false;
            auto scopeNum = m_context_active->frames_size() - 1;
            m_state = state::running;
            while (!m_is_exit_requested && !m_is_halt_requested && !m_contexts.size() == 0)
            {
                res = execute_do(*this, 1);
                perform_evaluate();
                if (res != result::ok)
                {
                    break;
                }
                if (m_context_active->frames_size() <= scopeNum)
                {
                    break;
                }
            }
            switch (res)
            {
            case sqf::runtime::runtime::result::empty:
                m_state = state::empty;
                break;
            case sqf::runtime::runtime::result::ok:
                m_state = state::halted;
                break;
            case sqf::runtime::runtime::result::invalid:
            case sqf::runtime::runtime::result::action_error:
            case sqf::runtime::runtime::result::runtime_error:
                m_state = state::halted_error;
                break;
            default:
                break;
            }
            if (m_is_exit_requested)
            {
                m_contexts.clear();
                m_state = state::empty;
            }
            m_run_atomic = false;
        }
        else
        {
            res = result::action_error;
        }
        break;
    case action::start:
        if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
        {
            m_is_exit_requested = false;
            m_is_halt_requested = false;
            m_state = state::running;
            for (auto iterator = m_contexts.begin(); iterator != m_contexts.end(); iterator++)
            {
                m_context_active = *iterator;
                if (m_context_active->suspended())
                {
                    if (m_context_active->wakeup_timestamp() <= std::chrono::system_clock::now())
                    {
                        m_context_active->unsuspend();
                        res = execute_do(*this, 150);
                    }
                    else
                    {
                        res = result::ok;
                    }
                }
                else
                {
                    res = execute_do(*this, 150);
                }
                perform_evaluate();
                switch (res)
                {
                case sqf::runtime::runtime::result::empty:
                    m_contexts.erase(iterator);
                    if (m_contexts.empty())
                    {
                        m_context_active = {};
                        break;
                    }
                    iterator = m_contexts.begin();
                    break;
                case sqf::runtime::runtime::result::invalid:
                case sqf::runtime::runtime::result::action_error:
                case sqf::runtime::runtime::result::runtime_error:
                    goto start_loop_exit;
                }
            }
        start_loop_exit:

            switch (res)
            {
            case sqf::runtime::runtime::result::empty:
                m_state = state::empty;
                break;
            case sqf::runtime::runtime::result::ok:
                m_state = state::halted;
                break;
            case sqf::runtime::runtime::result::invalid:
            case sqf::runtime::runtime::result::action_error:
            case sqf::runtime::runtime::result::runtime_error:
                m_state = state::halted_error;
                break;
            default:
                break;
            }
            if (m_is_exit_requested)
            {
                m_contexts.clear();
                m_context_active = {};
                m_state = state::empty;
            }
            m_run_atomic = false;
        }
        else
        {
            res = result::action_error;
        }
        break;
    case action::assembly_step:
        if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
        {
            m_is_exit_requested = false;
            m_is_halt_requested = false;
            m_state = state::running;
            res = execute_do(*this, 1);
            switch (res)
            {
            case sqf::runtime::runtime::result::empty:
                m_state = state::empty;
                break;
            case sqf::runtime::runtime::result::ok:
                m_state = state::halted;
                break;
            case sqf::runtime::runtime::result::invalid:
            case sqf::runtime::runtime::result::action_error:
            case sqf::runtime::runtime::result::runtime_error:
                m_state = state::halted_error;
                break;
            default:
                break;
            }
            if (m_is_exit_requested)
            {
                m_contexts.clear();
                m_context_active = {};
                m_state = state::empty;
            }
            m_run_atomic = false;
        }
        else
        {
            res = result::action_error;
        }
        break;
    case action::line_step:
        if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
        {
            m_is_exit_requested = false;
            m_is_halt_requested = false;
            bool success;
            m_state = state::running;
            std::optional<diagnostics::diag_info> dinf;
            while (!m_is_exit_requested && !m_is_halt_requested && !m_contexts.size() == 0)
            {
                if (!dinf.has_value())
                {
                    auto next_inst = m_context_active->current_frame().peek(success);
                    if (success)
                    {
                        dinf = { (*next_inst)->diag_info() };
                    }
                }

                res = execute_do(*this, 1);
                perform_evaluate();
                if (res != result::ok)
                {
                    break;
                }
                if (dinf.has_value())
                {
                    auto next_inst = m_context_active->current_frame().peek(success);
                    if (success && dinf.value() != (*next_inst)->diag_info())
                    {
                        break;
                    }
                }
            }
            switch (res)
            {
            case sqf::runtime::runtime::result::empty:
                m_state = state::empty;
                break;
            case sqf::runtime::runtime::result::ok:
                m_state = state::halted;
                break;
            case sqf::runtime::runtime::result::invalid:
            case sqf::runtime::runtime::result::action_error:
            case sqf::runtime::runtime::result::runtime_error:
                m_state = state::halted_error;
                break;
            default:
                break;
            }
            if (m_is_exit_requested)
            {
                m_contexts.clear();
                m_context_active = {};
                m_state = state::empty;
            }
            m_run_atomic = false;
        }
        else
        {
            res = result::action_error;
        }
        break;


    case action::stop:
        if (m_state != state::running)
        {
            res = result::action_error;
        }
        else if (!m_run_atomic)
        {
            res = result::action_error;
        }
        else
        {
            m_is_exit_requested = true;
            res = result::ok;
        }
        break;
    case action::abort:
        if (m_state == state::running)
        {
            if (!m_run_atomic)
            {
                res = result::action_error;
            }
            else
            {
                m_is_exit_requested = true;
                res = result::ok;
            }
        }
        else if (m_state == state::halted_error || m_state == state::halted)
        {
            if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
            {
                m_contexts.clear();
                m_context_active = {};
                m_state = state::empty;
                res = result::ok;
                m_run_atomic = false;
            }
            else
            {
                res = result::action_error;
            }
        }
        else
        {
            res = result::action_error;
        }
        break;
    case action::reset_run_atomic:
        m_run_atomic = false;
        break;
    default:
        res = result::action_error;
        break;
    }
    return res;
}

::sqf::runtime::value sqf::runtime::runtime::evaluate_expression(std::string view, bool& success, bool request_halt)
{
    while (m_evaluate_halt);
    m_evaluate_halt = true;
    if (request_halt)
    {
        while (m_state == state::running);
    }
    auto& sqf_parser = parser_sqf();
    auto opt_set = sqf_parser.parse(*this, view, { std::string("__evaluate_expression__.sqf"), {} });
    if (opt_set.has_value())
    {
        frame f(default_value_scope(), opt_set.value());
        std::vector<sqf::runtime::value> values(context_active().values_rbegin(), context_active().values_rend());
        auto frames = context_active().frames_size();
        context_active().push_frame(f);
        context_active().clear_values();
        try
        {
            while (frames < context_active().frames_size())
            {
                execute_do(*this, 1);
            }
        }
        catch (const std::exception& ex)
        {
            m_evaluate_halt = false;
        }
        if (m_runtime_error)
        {
            m_evaluate_halt = false;
            m_runtime_error = false;
            success = false;
            context_active().clear_values();
            for (auto val : values)
            {
                context_active().push_value(val);
            }
            return {};
        }
        else
        {
            auto val = context_active().pop_value();
            context_active().clear_values();
            for (auto it : values)
            {
                context_active().push_value(it);
            }
            m_evaluate_halt = false;
            success = true;
            return val.value();
        }
    }
    else
    {
        m_evaluate_halt = false;
        success = false;
        return {};
    }
}