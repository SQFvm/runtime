#pragma once

#include "instruction_set.h"
#include "value_scope.h"

#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
#include <iostream>
#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE


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
        enum class seekpos
        {
            end,
            start
        };
    private:
        sqf::runtime::instruction_set m_instruction_set;
        // Use index over iterator due to iterator invalidation problems
        size_t m_position;
        std::shared_ptr<behavior> m_exit_behavior;
        std::shared_ptr<behavior> m_error_behavior;
        std::shared_ptr<sqf::runtime::value_scope> m_globals_value_scope;
        bool m_bubble_variable;
        bool m_started;
    public:
        static const size_t position_invalid = ~(size_t)0;
        frame() :
            frame({}, {}, {}, {}) {}
        frame(std::shared_ptr<sqf::runtime::value_scope> globals_scope, sqf::runtime::instruction_set instruction_set, std::shared_ptr<behavior> exit_behavior) :
            frame(globals_scope, instruction_set, exit_behavior, {}) {}
        frame(std::shared_ptr<sqf::runtime::value_scope> globals_scope, sqf::runtime::instruction_set instruction_set) :
            frame(globals_scope, instruction_set, {}, {}) {}
        frame(std::shared_ptr<sqf::runtime::value_scope> globals_scope,
            sqf::runtime::instruction_set instruction_set,
            std::shared_ptr<behavior> exit_behavior,
            std::shared_ptr<behavior> error_behavior)
            :
            m_instruction_set(instruction_set),
            m_position(position_invalid),
            m_exit_behavior(exit_behavior),
            m_error_behavior(error_behavior),
            m_globals_value_scope(globals_scope),
            m_bubble_variable(true),
            m_started(false)
        {}

#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

        void dbg_str()
        {
            std::cout << "{";

            for (auto& it : m_instruction_set)
            {
                std::cout << it->to_string() << ", ";
            }
            std::cout << "}" << std::endl;
        }

#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
        bool can_recover_runtime_error() { return m_error_behavior != nullptr; }
        result recover_runtime_error(runtime& runtime)
        {
            if (!m_error_behavior) { return result::error; }
            switch (m_error_behavior->enact(runtime, *this))
            {
            case behavior::result::seek_end:
                seek(0, seekpos::end);
                return result::done;
            case behavior::result::seek_start:
                seek(0, seekpos::start);
                return result::ok;
            case behavior::result::exchange:
                m_instruction_set = m_error_behavior->get_instruction_set(*this);
                m_position = 0;
#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

                std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                    "    " << "    " << " " <<
                    "    " << "    " << " " <<
                    "    " << "    " << "\x1B[91mFrame\033[0m - Loaded assembly " << "[";

                for (auto& it : m_instruction_set)
                {
                    std::cout << it->to_string() << ", ";
                }
                std::cout << "]" << std::endl;

#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
                return result::ok;
            case behavior::result::fail:
                return result::error;
            }
            return result::ok;
        }

        void seek(long target, sqf::runtime::frame::seekpos from)
        {
            switch (from)
            {
            case sqf::runtime::frame::seekpos::end:
                m_position = m_instruction_set.size() + target;
#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

                std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                    "    " << "    " << " " <<
                    "    " << "    " << " " <<
                    "    " << "    " << "\x1B[91mFrame\033[0m - Seek End" << std::endl;

#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
                break;
            case sqf::runtime::frame::seekpos::start:
                m_position = target + position_invalid;
#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

                std::cout << "\x1B[33m[ASSEMBLY ASSERT]\033[0m" <<
                    "    " << "    " << " " <<
                    "    " << "    " << " " <<
                    "    " << "    " << "\x1B[91mFrame\033[0m - Seek Start" << std::endl;

#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
                break;
            default:
                break;
            }
        }
        size_t position() const
        {
            return m_position;
        }

        sqf::runtime::instruction_set::iterator peek() const { bool flag; return peek(flag); }
        sqf::runtime::instruction_set::iterator peek(bool& success) const
        {
            auto pos = m_position >= m_instruction_set.size() ? m_instruction_set.size() - 1 : m_position + 1;
            auto it = m_instruction_set.begin() + m_position;
            success = it != m_instruction_set.end();
            return it;
        }

        bool bubble_variable() const { return m_bubble_variable; }
        void bubble_variable(bool flag) { m_bubble_variable = flag; }

        sqf::runtime::instruction_set::iterator current() const { return m_instruction_set.begin() + m_position; }
        std::shared_ptr<sqf::runtime::value_scope> globals_value_scope() const { return m_globals_value_scope; }
        void globals_value_scope(std::shared_ptr<sqf::runtime::value_scope> scope) { m_globals_value_scope = scope; }

        /// <summary>
        /// Moves current to next instruction.
        /// Will not trigger possible attached behaviors
        /// </summary>
        /// <returns>Enum value describing the success state of the operation.</returns>
        result next()
        {
            if (m_position == m_instruction_set.size()) { return result::done; }
            return ++m_position == m_instruction_set.size() ? result::done : result::ok;
        }
        /// <summary>
        /// Moves current to next instruction.
        /// </summary>
        /// <returns>Enum value describing the success state of the operation.</returns>
        result next(runtime& runtime)
        {
            if (m_position != m_instruction_set.size())
            {
                auto res = next();
                if (m_position == m_instruction_set.size() && m_exit_behavior)
                {
                    switch (m_exit_behavior->enact(runtime, *this))
                    {
                    case behavior::result::seek_end:
                        seek(0, seekpos::end);
                        return result::done;
                    case behavior::result::seek_start:
                        seek(0, seekpos::start);
                        return next();
                    case behavior::result::exchange:
                        m_instruction_set = m_exit_behavior->get_instruction_set(*this);
                        m_position = 0;
#ifdef FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE

                        std::cout << "[ASSEMBLY ASSERT]" <<
                            "    " << "    " << " " <<
                            "    " << "    " << " " <<
                            "    " << "    " << "\x1B[91mFrame\033[0m - Loaded assembly " << "[";

                        for (auto& it : m_instruction_set)
                        {
                            std::cout << it->to_string();
                        }
                        std::cout << "]" << std::endl;

#endif // FLAG__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE
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
            if (m_position == position_invalid) { return result::done; }
            return --m_position == 0 ? result::done : result::ok;
        }
    };
}