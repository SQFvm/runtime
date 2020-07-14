#pragma once
#include "logging.h"
#include "diagnostics/breakpoint.h"
#include "context.h"
#include "confighost.h"
#include "fileio.h"
#include "parser/config.h"
#include "parser/sqf.h"
#include "parser/preprocessor.h"
#include "sqfop.h"

#include <chrono>
#include <atomic>
#include <vector>
#include <typeinfo>
#include <typeindex>

namespace sqf::runtime
{
    class runtime final : public CanLog
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

#pragma region storage

    public:
        class datastorage
        {
        public:
            virtual ~datastorage() = 0;
        };
    private:
        std::unordered_map<std::type_index, std::unique_ptr<datastorage>> m_data_storage;
    public:
        template<class TSource, class TStorage>
        TStorage& storage()
        {
            static_assert(std::is_base_of<datastorage, TStorage>::value,
                "sqf::runtime::runtime::storage<TSource, TStorage>() expects TStorage to be a derivative of sqf::runtime::runtime::datastorage.");
            std::type_index key(typeid(TSource));
            auto res = m_data_storage.find(key);
            if (res == m_data_storage.end())
            {
                return *(m_data_storage[key] = std::make_unique<TStorage>());
            }
            else
            {
                return *res->second;
            }
        }

#pragma endregion

#pragma region operators

    private:
        std::unordered_map<sqf::runtime::sqfop_binary::key, sqf::runtime::sqfop_binary> m_operators_binary;
        std::unordered_map<std::string_view, std::vector<sqf::runtime::sqfop_binary::cref>> m_operators_by_name_binary;

        std::unordered_map<sqf::runtime::sqfop_unary::key, sqf::runtime::sqfop_unary> m_operators_unary;
        std::unordered_map<std::string_view, std::vector<sqf::runtime::sqfop_unary::cref>> m_operators_by_name_unary;

        std::unordered_map<sqf::runtime::sqfop_nular::key, sqf::runtime::sqfop_nular> m_operators_nular;
    public:
        using sqfop_binary_iterator = std::unordered_map<sqf::runtime::sqfop_binary::key, sqf::runtime::sqfop_binary>::const_iterator;
        sqfop_binary_iterator sqfop_binary_begin() const { return m_operators_binary.begin(); }
        sqfop_binary_iterator sqfop_binary_end() const { return m_operators_binary.end(); }
        bool sqfop_exists(const sqf::runtime::sqfop_binary::key key) const { return m_operators_binary.find(key) != m_operators_binary.end(); }
        sqf::runtime::sqfop_binary::cref sqfop_at(const sqf::runtime::sqfop_binary::key key) const { return m_operators_binary.at(key); }
        std::vector<sqf::runtime::sqfop_binary::cref> sqfop_binary_by_name(const std::string_view key) const { return m_operators_by_name_binary.at(key); }
        void register_sqfop(sqf::runtime::sqfop_binary op)
        {
            m_operators_binary.emplace(op.get_key(), op);
            m_operators_by_name_binary[op.name()].push_back(m_operators_binary[op.get_key()]);
        }

        using sqfop_unary_iterator = std::unordered_map<sqf::runtime::sqfop_unary::key, sqf::runtime::sqfop_unary>::const_iterator;
        sqfop_unary_iterator sqfop_unary_begin() const { return m_operators_unary.begin(); }
        sqfop_unary_iterator sqfop_unary_end() const { return m_operators_unary.end(); }
        bool sqfop_exists(const sqf::runtime::sqfop_unary::key key) const { return m_operators_unary.find(key) != m_operators_unary.end(); }
        sqf::runtime::sqfop_unary::cref sqfop_at(const sqf::runtime::sqfop_unary::key key) const { return m_operators_unary.at(key); }
        std::vector<sqf::runtime::sqfop_unary::cref> sqfop_unary_by_name(const std::string_view key) const { return m_operators_by_name_unary.at(key); }
        void register_sqfop(sqf::runtime::sqfop_unary op)
        {
            m_operators_unary[op.get_key()] = op;
            m_operators_by_name_unary[op.name()].push_back(m_operators_unary[op.get_key()]);
        }

        using sqfop_nular_iterator = std::unordered_map<sqf::runtime::sqfop_nular::key, sqf::runtime::sqfop_nular>::const_iterator;
        sqfop_nular_iterator sqfop_nular_begin() const { return m_operators_nular.begin(); }
        sqfop_nular_iterator sqfop_nular_end() const { return m_operators_nular.end(); }
        bool sqfop_exists(const sqf::runtime::sqfop_nular::key key) const { return m_operators_nular.find(key) != m_operators_nular.end(); }
        sqf::runtime::sqfop_nular::cref sqfop_at(const sqf::runtime::sqfop_nular::key key) const { return m_operators_nular.at(key); }
        void register_sqfop(sqf::runtime::sqfop_nular op)
        {
            m_operators_nular[op.get_key()] = op;
        }

#pragma endregion


    private:
        configuration m_configuration;
        std::chrono::system_clock::time_point m_runtime_timestamp;
        bool m_runtime_error;

        std::vector<sqf::runtime::context> m_contexts;
        std::vector<sqf::runtime::context>::iterator m_active_context;

        std::chrono::system_clock::time_point m_created_timestamp;
        std::chrono::system_clock::time_point m_current_time;
        sqf::runtime::confighost m_confighost;

        std::unique_ptr<sqf::runtime::fileio> m_fileio;
        std::unique_ptr<sqf::runtime::parser::sqf> m_parser_sqf;
        std::unique_ptr<sqf::runtime::parser::config> m_parser_config;
        std::unique_ptr<sqf::runtime::parser::preprocessor> m_parser_preprocessor;

    public:
        runtime(Logger& logger, configuration config) :
            CanLog(logger),
            m_state(state::halted),
            m_exit_code(0),
            m_is_exit_requested(false),
            m_is_halt_requested(false),
            m_last_breakpoint_hit(~((size_t)0), {}),
            m_configuration(config),
            m_runtime_error(false),
            m_fileio(std::make_unique<sqf::fileio::disabled>()),
            m_parser_sqf(std::make_unique<sqf::parser::sqf::disabled>()),
            m_parser_config(std::make_unique<sqf::parser::config::disabled>()),
            m_parser_preprocessor(std::make_unique<sqf::parser::preprocessor::passthrough>())
        {
        }

        void push_back(sqf::runtime::context context) { m_contexts.push_back(context); }
        sqf::runtime::runtime::result execute(sqf::runtime::runtime::action action);
        const sqf::runtime::runtime::configuration configuration() const { return m_configuration; }
        std::chrono::system_clock::time_point runtime_timestamp() { return m_runtime_timestamp; }
        void runtime_timestamp_reset() { m_runtime_timestamp = std::chrono::system_clock::now(); }

        sqf::runtime::confighost& confighost() { return m_confighost; }

        void fileio(std::unique_ptr<sqf::runtime::fileio> ptr) { m_fileio = std::move(ptr); }
        void parser_sqf(std::unique_ptr<sqf::runtime::parser::sqf> ptr) { m_parser_sqf = std::move(ptr); }
        void parser_config(std::unique_ptr<sqf::runtime::parser::config> ptr) { m_parser_config = std::move(ptr); }
        void parser_preprocessor(std::unique_ptr<sqf::runtime::parser::preprocessor> ptr) { m_parser_preprocessor = std::move(ptr); }
        sqf::runtime::fileio& fileio() { return *m_fileio; }
        sqf::runtime::parser::sqf& parser_sqf() { return *m_parser_sqf; }
        sqf::runtime::parser::config& parser_config() { return *m_parser_config; }
        sqf::runtime::parser::preprocessor& parser_preprocessor() { return *m_parser_preprocessor; }

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