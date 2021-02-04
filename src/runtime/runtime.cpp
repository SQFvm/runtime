#include "runtime.h"
#include "diagnostics/stacktrace.h"
#include "d_array.h"
#include "d_string.h"
#include "diagnostics/d_stacktrace.h"

#include <optional>

#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
#include <iostream>
#include <iomanip>
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

static sqf::runtime::runtime::result execute_do(sqf::runtime::runtime& runtime, size_t exit_after)
{
    auto& context_active = runtime.context_active();
    auto& runtime_error = runtime.__runtime_error();
    while (true)
    {
        if (runtime.is_exit_requested())
        {
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m due to \x1B[90mruntime.is_exit_requested() == true\033[0m" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return sqf::runtime::runtime::result::ok;
        }
        if (exit_after == 0)
        {
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m due to \x1B[90mexit_after == 0\033[0m" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return sqf::runtime::runtime::result::ok;
        }
        if (context_active.suspended())
        {
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m due to \x1B[90mcontext_active.suspended() == true\033[0m" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return sqf::runtime::runtime::result::ok;
        }
        if (context_active.empty())
        {
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m due to \x1B[90mcontext_active.empty() == true\033[0m" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return sqf::runtime::runtime::result::empty;
        }
        if (runtime.runtime_state() != sqf::runtime::runtime::state::running)
        {
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m due to \x1B[90mruntime.runtime_state() != sqf::runtime::runtime::state::running\033[0m" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return sqf::runtime::runtime::result::ok;
        }


        auto frame_count = context_active.frames_size();
        auto& frame = context_active.current_frame();

        auto result = frame.next(runtime);

        if (result == sqf::runtime::frame::result::done && context_active.frames_size() == frame_count)
        { // frame is done executing. Pop it from context and rerun.

            // Pop possible return value
            auto val = context_active.pop_value();

            // Clear the value-stack part of the frame
            while (context_active.pop_value().has_value());

            // Pop the actual frame
            context_active.pop_frame();

            // Readd return value of frame if it had one
            if (val.has_value())
            { context_active.push_value(val.value()); }

            // Restart loop-run
            continue;
        }

        auto instruction = frame.current();
        if (runtime.configuration().max_runtime != std::chrono::milliseconds::zero() &&
            runtime.configuration().max_runtime + runtime.runtime_timestamp() < std::chrono::system_clock::now())
        {
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mEXIT execute_do\033[0m as max runtime (\x1B[90m" << runtime.configuration().max_runtime.count() << "ms\033[0m) was reached" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
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

#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

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
            std::optional<size_t> nil_index = {};
            for (auto it = context_active.values_begin(); context_active.values_end() != it; ++it)
            {
                auto sqf = (*it).to_string_sqf();
                if (sqf == "nil")
                {
                    if (!nil_index.has_value())
                    {
                        nil_index = { values_index };
                    }
                    values_index++;
                }
                else
                {
                    if (nil_index.has_value())
                    {
                        if (*nil_index == values_index - 1)
                        {
                            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                                "        " <<
                                "        " <<
                                "    " << "    " << std::setw(2) << (values_index - 1) << ": nil" << std::endl;
                        }
                        else
                        {
                            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                                "        " <<
                                "        " <<
                                "    " << "    " << *nil_index << "-" << (values_index - 1) << ": nil" << std::endl;
                        }
                        nil_index = {};
                    }
                    std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                        "        " <<
                        "        " <<
                        "    " << "    " << std::setw(2) << values_index++ << ": " << sqf << std::endl;
                }
            }
            if (nil_index.has_value())
            {
                if (*nil_index == values_index - 1)
                {
                    std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                        "        " <<
                        "        " <<
                        "    " << "    " << std::setw(2) << (values_index - 1) << ": nil" << std::endl;
                }
                else
                {
                    std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                        "        " <<
                        "        " <<
                        "    " << "    " << *nil_index << "-" << (values_index - 1) << ": nil" << std::endl;
                }
                nil_index = {};
            }
        }


#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE


        (*instruction)->execute(runtime);


        if (!runtime_error)
        {
            runtime.log_messages.clear();
        }
        else
        {
            auto log_messages = runtime.log_messages;
            runtime.log_messages.clear();
            // Build Stacktrace
            std::vector<sqf::runtime::frame> stacktrace_frames(context_active.frames_rbegin(), context_active.frames_rend());
            sqf::runtime::diagnostics::stacktrace stacktrace(stacktrace_frames);

            // Try to find a frame that has recover behavior for runtime error
            auto res = std::find_if(context_active.frames_rbegin(), context_active.frames_rend(),
                [](sqf::runtime::frame& frame) -> bool { return frame.can_recover_runtime_error(); });

            if (res != context_active.frames_rend())
            { // We found a recoverable frame
                stacktrace.value = std::make_shared<sqf::types::d_array>(log_messages.begin(), log_messages.end());
                // Push Stacktrace to value-stack
                context_active.push_value({ std::make_shared<sqf::types::d_stacktrace>(stacktrace) });

                // Pop all frames between result and current_frame
                size_t frames_to_pop = res - context_active.frames_rbegin();
                for (size_t i = 0; i < frames_to_pop; i++)
                {
                    context_active.pop_frame();
                }

                // Recover from exception
                context_active.current_frame().recover_runtime_error(runtime);
                runtime_error = false;
            }
            else
            { // No recover frame available, exit method
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
                std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                    "        " <<
                    "        " <<
                    "    " << "\x1B[36mEXIT execute_do\033[0m as runtime error occured" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
                runtime.__logmsg(logmessage::runtime::Stacktrace((*instruction)->diag_info(), stacktrace));
                runtime_error = false;
                return sqf::runtime::runtime::result::runtime_error;
            }
        }
    }
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
            while (!m_is_exit_requested && !m_is_halt_requested && !m_contexts.empty())
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
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mREACHED EXIT FOR\033[0m \x1B[90maction::leave_scope\033[0m" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
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
            while (!m_contexts.empty())
            {
                for (size_t i = 0; i < m_contexts.size(); i++)
                {
                    m_context_active = m_contexts[i];
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
                    if (m_is_exit_requested)
                    {
                        m_contexts.clear();
                        m_state = state::empty;
                        goto start_loop_exit;
                    }
                    perform_evaluate();
                    switch (res)
                    {
                    case sqf::runtime::runtime::result::empty:
                    {
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
                        std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                            "        " <<
                            "        " <<
                            "    " << "\x1B[36mERASE CONTEXT\033[0m \x1B[90" << (m_context_active->name().empty() ? "<unnamed>" : m_context_active->name()) << "\033[0m" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
                        auto opt_val = m_context_active->pop_value(true);
                        if (opt_val.has_value() && configuration().print_context_work_to_log_on_exit)
                        {
                            __logmsg(logmessage::runtime::ContextValuePrint(opt_val.value()));
                        }
                        m_contexts.erase(m_contexts.begin() + i);
                        if (m_contexts.empty())
                        {
                            m_context_active = {};
                            goto start_loop_exit;
                        }
                        i--;
                    } break;
                    case sqf::runtime::runtime::result::invalid:
                    case sqf::runtime::runtime::result::action_error:
                    case sqf::runtime::runtime::result::runtime_error:
                        goto start_loop_exit;
                    case sqf::runtime::runtime::result::ok: /* empty */ break;
                    }
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
            }
            if (m_is_exit_requested)
            {
                m_contexts.clear();
                m_context_active = {};
                m_state = state::empty;
            }
            m_run_atomic = false;
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mREACHED EXIT FOR\033[0m \x1B[90maction::start\033[0m" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
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
            }
            if (m_is_exit_requested)
            {
                m_contexts.clear();
                m_context_active = {};
                m_state = state::empty;
            }
            m_run_atomic = false;
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mREACHED EXIT FOR\033[0m \x1B[90maction::assembly_step\033[0m" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
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
            while (!m_is_exit_requested && !m_is_halt_requested && !m_contexts.empty())
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
            }
            if (m_is_exit_requested)
            {
                m_contexts.clear();
                m_context_active = {};
                m_state = state::empty;
            }
            m_run_atomic = false;
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mREACHED EXIT FOR\033[0m \x1B[90maction::line_step\033[0m" << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
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
        auto eval_context = context_create().lock();
        frame f(default_value_scope(), opt_set.value());
        eval_context->push_frame(f);
        auto old_active = context_active_as_shared();
        m_context_active = eval_context;
        try
        {
            while (!eval_context->empty())
            {
                auto oldstate = m_state;
                if (m_state == runtime::state::empty)
                {
                    m_state = runtime::state::running;
                }
                execute_do(*this, 1);
                m_state = oldstate;
            }
        }
        catch (const std::exception& ex)
        {
            m_evaluate_halt = false;
        }
        m_context_active = old_active;
        if (m_runtime_error)
        {
            m_evaluate_halt = false;
            m_runtime_error = false;
            success = false;
            return {};
        }
        else
        {
            auto val = eval_context->pop_value(true);
            m_evaluate_halt = false;
            success = true;
            if (val.has_value())
            {
                return val.value();
            }
            else
            {
                return {};
            }
        }
    }
    else
    {
        m_evaluate_halt = false;
        success = false;
        return {};
    }
}