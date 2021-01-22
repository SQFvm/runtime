#pragma once
#include "frame.h"
#include "value.h"

#include <vector>
#include <chrono>
#include <optional>
#include <string_view>
#include <algorithm>
#include <cctype>

#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
#include <iostream>
#include <iomanip>
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE


namespace sqf::runtime
{
    class context final
    {
    private:
        std::vector<sqf::runtime::frame> m_frames;
        std::vector<sqf::runtime::value> m_values;
        bool m_can_suspend;
        bool m_suspended;
        bool m_weak_error_handling;
        std::chrono::system_clock::time_point m_wakeup_timestamp;
        std::string m_name;
        bool m_terminate;

    public:
        context() :
            m_can_suspend(false),
            m_suspended(false),
            m_weak_error_handling(false),
            m_terminate(false)
        { }


        std::string name() const { return m_name; }
        void name(std::string value) { m_name = value; }

        bool can_suspend() const { return m_can_suspend; }
        void can_suspend(bool flag) { m_can_suspend = flag; }
        bool suspended() const { return m_suspended; }
        std::chrono::system_clock::time_point wakeup_timestamp() const { return m_wakeup_timestamp; }
        template<class _Rep, class _Period>
        void suspend(std::chrono::duration<_Rep, _Period> duration)
        {
            m_wakeup_timestamp = std::chrono::system_clock::now() + duration;
            m_suspended = true;
        }
        void unsuspend() { m_suspended = false; }
        bool empty() const { return m_frames.empty(); }
        size_t frames_size() const { return m_frames.size(); }
        size_t values_size() const { return m_values.size(); }
        void clear_frames() { m_frames.clear(); }
        void clear_values(bool ingore_frame_boundaries = false)
        {
            if (ingore_frame_boundaries)
            {
                m_values.clear();
            }
            else if (!m_frames.empty())
            {
                for (size_t i = m_values.size(); i > current_frame().value_stack_pos(); i--)
                {
                    m_values.pop_back();
                }
            }
        }
        void push_frame(sqf::runtime::frame frame)
        {
            m_frames.push_back(frame);
            m_frames.back().value_stack_pos(m_values.size());
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "[FC:" << std::setw(3) << frames_size() << "]" <<
                "[VC:" << std::setw(3) << values_size() << "]" <<
                "    " << "    " << "Pushed \x1B[91mFrame\033[0m ";
            m_frames.back().dbg_str();
            std::cout << std::endl;
#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
        }
        void push_value(sqf::runtime::value value)
        {
            m_values.push_back(value);

#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "[FC:" << std::setw(3) << frames_size() << "]" <<
                "[VC:" << std::setw(3) << values_size() << "]" <<
                "    " << "    " << "Pushed Return \x1B[92mValue\033[0m " << value.to_string_sqf() << std::endl;

#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
        }
        sqf::runtime::frame&& pop_frame()
        {
            auto& frame = m_frames.back();
            m_frames.pop_back();
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

            std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                "[FC:" << std::setw(3) << frames_size() << "]" <<
                "[VC:" << std::setw(3) << values_size() << "]" <<
                "    " << "    " << "Popped \x1B[91mFrame\033[0m" << std::endl;

#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
            return std::move(frame);
        }
        std::optional<sqf::runtime::value> pop_value(bool ingore_frame_boundaries = false)
        {
            if (m_values.empty() || (!ingore_frame_boundaries && (m_frames.empty() || m_values.size() <= current_frame().value_stack_pos())))
            {
                return {};
            }
            else
            {
                auto value = m_values.back();
                m_values.pop_back();
#ifdef DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

                std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                    "[FC:" << std::setw(3) << frames_size() << "]" <<
                    "[VC:" << std::setw(3) << values_size() << "]" <<
                    "    " << "    " << "Popped \x1B[92mValue\033[0m " << value.to_string_sqf() << std::endl;

#endif // DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
                return value;
            }
        }
        sqf::runtime::value::cref peek_value() { return m_values.back(); }

        std::vector<sqf::runtime::frame>::reverse_iterator frames_rbegin() { return m_frames.rbegin(); }
        std::vector<sqf::runtime::frame>::reverse_iterator frames_rend() { return m_frames.rend(); }
        std::vector<sqf::runtime::value>::iterator values_begin() { return m_values.begin(); }
        std::vector<sqf::runtime::value>::iterator values_end() { return m_values.end(); }
        std::vector<sqf::runtime::value>::reverse_iterator values_rbegin() { return m_values.rbegin(); }
        std::vector<sqf::runtime::value>::reverse_iterator values_rend() { return m_values.rend(); }

        std::optional<sqf::runtime::value> get_variable(std::string variable_name) const
        {
            std::transform(variable_name.begin(), variable_name.end(), variable_name.begin(), [](char& c) { return (char)std::tolower((int)c); });
            for (auto rit = m_frames.rbegin(); rit != m_frames.rend(); rit++)
            {
                if (rit->contains(variable_name))
                {
                    return (*rit)[variable_name];
                }
                else if (!rit->bubble_variable())
                {
                    return {};
                }
            }
            return {};
        }

        bool weak_error_handling() const { return m_weak_error_handling; }
        void weak_error_handling(bool flag) { m_weak_error_handling = flag; }

        frame& current_frame() { return m_frames.back(); }

        bool terminate() const { return m_terminate; }
        void terminate(bool flag) { m_terminate = flag; }
    };
}
