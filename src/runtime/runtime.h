#pragma once
#include "logging.h"
#include "diagnostics/breakpoint.h"
#include "context.h"
#include "confighost.h"
#include "fileio.h"
#include "parser/config.h"
#include "parser/sqf.h"
#include "parser/preprocessor.h"
#include "value_scope.h"
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
            // Invalid action
            invalid,


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
        struct runtime_conf
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

            /// <summary>
            /// If true, a context containing values will push them before being erased from the context list.
            /// </summary>
            bool print_context_work_to_log_on_exit;


            runtime_conf() :
                max_runtime(std::chrono::milliseconds::zero()),
                disable_sleep(false),
                enable_classname_check(true),
                disable_networking(false),
                print_context_work_to_log_on_exit(false)
            {}
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
        std::optional<int> exit_code() const { return m_is_exit_requested ? m_exit_code : std::optional<int>(); }
        state runtime_state() const { return m_state; }

#pragma endregion
#pragma region Runtime Diagnostics

    private:
        std::vector<sqf::runtime::diagnostics::breakpoint> m_breakpoints;
        sqf::runtime::diagnostics::breakpoint m_last_breakpoint_hit;
    public:
        using breakpoints_iterator = std::vector<sqf::runtime::diagnostics::breakpoint>::iterator;
        const std::vector<sqf::runtime::diagnostics::breakpoint>& breakpoints() const { return m_breakpoints; }
        std::vector<sqf::runtime::diagnostics::breakpoint>& breakpoints() { return m_breakpoints; }

        breakpoints_iterator breakpoints_begin() { return m_breakpoints.begin(); }
        breakpoints_iterator breakpoints_end() { return m_breakpoints.end(); }
        void erase(breakpoints_iterator from, breakpoints_iterator to) { m_breakpoints.erase(from, to); }
        void erase(breakpoints_iterator iterator) { m_breakpoints.erase(iterator); }
        void push_back(sqf::runtime::diagnostics::breakpoint bp) { m_breakpoints.push_back(bp); }

        void breakpoint_hit(sqf::runtime::diagnostics::breakpoint breakpoint) { m_last_breakpoint_hit = breakpoint; m_is_halt_requested = true; }

#pragma endregion
#pragma region Storage

    public:
        class datastorage
        {
        public:
            virtual ~datastorage() {};
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
                m_data_storage[key] = std::make_unique<TStorage>();
                return static_cast<TStorage&>(*m_data_storage[key]);
            }
            else
            {
                return static_cast<TStorage&>(*res->second);
            }
        }
        template<class TStorage>
        TStorage& storage() { return storage<TStorage, TStorage>(); }

#pragma endregion
#pragma region Operators

    private:
        std::unordered_map<sqf::runtime::sqfop_binary::key, sqf::runtime::sqfop_binary> m_operators_binary;
        std::unordered_map<std::string, std::vector<sqf::runtime::sqfop_binary::cwref>> m_operators_by_name_binary;

        std::unordered_map<sqf::runtime::sqfop_unary::key, sqf::runtime::sqfop_unary> m_operators_unary;
        std::unordered_map<std::string, std::vector<sqf::runtime::sqfop_unary::cwref>> m_operators_by_name_unary;

        std::unordered_map<sqf::runtime::sqfop_nular::key, sqf::runtime::sqfop_nular> m_operators_nular;
    public:
        using sqfop_binary_iterator = std::unordered_map<sqf::runtime::sqfop_binary::key, sqf::runtime::sqfop_binary>::const_iterator;
        using sqfop_unary_iterator = std::unordered_map<sqf::runtime::sqfop_unary::key, sqf::runtime::sqfop_unary>::const_iterator;
        using sqfop_nular_iterator = std::unordered_map<sqf::runtime::sqfop_nular::key, sqf::runtime::sqfop_nular>::const_iterator;

        sqfop_binary_iterator sqfop_binary_begin() const { return m_operators_binary.begin(); }
        sqfop_binary_iterator sqfop_binary_end() const { return m_operators_binary.end(); }
        bool sqfop_exists(const sqf::runtime::sqfop_binary::key key) const { return m_operators_binary.find(key) != m_operators_binary.end(); }
        sqf::runtime::sqfop_binary::cref sqfop_at(const sqf::runtime::sqfop_binary::key key) const { return m_operators_binary.at(key); }
        const std::vector<sqf::runtime::sqfop_binary::cwref>& sqfop_binary_by_name(const std::string key) const { return m_operators_by_name_binary.at(key); }
        bool sqfop_exists_binary(std::string key) const
        {
            std::transform(key.begin(), key.end(), key.begin(), [](char& c) { return (char)std::tolower((int)c); });
            return m_operators_by_name_binary.find(key) != m_operators_by_name_binary.end();
        }
        void register_sqfop(sqf::runtime::sqfop_binary op)
        {
            m_operators_binary.insert({ op.get_key(), op });
            m_operators_by_name_binary[std::string(op.name())].push_back(m_operators_binary[op.get_key()]);
        }

        sqfop_unary_iterator sqfop_unary_begin() const { return m_operators_unary.begin(); }
        sqfop_unary_iterator sqfop_unary_end() const { return m_operators_unary.end(); }
        bool sqfop_exists(const sqf::runtime::sqfop_unary::key key) const { return m_operators_unary.find(key) != m_operators_unary.end(); }
        sqf::runtime::sqfop_unary::cref sqfop_at(const sqf::runtime::sqfop_unary::key key) const { return m_operators_unary.at(key); }
        const std::vector<sqf::runtime::sqfop_unary::cwref>& sqfop_unary_by_name(const std::string key) const { return m_operators_by_name_unary.at(key); }
        bool sqfop_exists_unary(std::string key) const
        {
            std::transform(key.begin(), key.end(), key.begin(), [](char& c) { return (char)std::tolower((int)c); }); 
            return m_operators_by_name_unary.find(key) != m_operators_by_name_unary.end();
        }
        void register_sqfop(sqf::runtime::sqfop_unary op)
        {
            m_operators_unary.insert({ op.get_key(), op });
            m_operators_by_name_unary[std::string(op.name())].push_back(m_operators_unary[op.get_key()]);
        }

        sqfop_nular_iterator sqfop_nular_begin() const { return m_operators_nular.begin(); }
        sqfop_nular_iterator sqfop_nular_end() const { return m_operators_nular.end(); }
        bool sqfop_exists(const sqf::runtime::sqfop_nular::key key) const { return m_operators_nular.find(key) != m_operators_nular.end(); }
        sqf::runtime::sqfop_nular::cref sqfop_at(const sqf::runtime::sqfop_nular::key key) const { return m_operators_nular.at(key); }
        bool sqfop_exists_nular(std::string key) const
        {
            std::transform(key.begin(), key.end(), key.begin(), [](char& c) { return (char)std::tolower((int)c); });
            return sqfop_exists(sqf::runtime::sqfop_nular::key{ key });
        }
        void register_sqfop(sqf::runtime::sqfop_nular op)
        {
            m_operators_nular.insert({ op.get_key(), op });
        }

#pragma endregion
#pragma region Namespaces

    private:
        std::unordered_map<std::string, std::shared_ptr<sqf::runtime::value_scope>> m_namespaces;
        std::string m_default_scope_key;
    public:
        std::shared_ptr<sqf::runtime::value_scope> get_value_scope(std::string key)
        {
            auto needle = m_namespaces.find(key);
            if (needle != m_namespaces.end())
            {
                return needle->second;
            }
            else
            {
                auto& value_scope = m_namespaces[key] = std::make_shared<sqf::runtime::value_scope>();
                value_scope->scope_name(key);
                return value_scope;
            }
        }
        std::shared_ptr<sqf::runtime::value_scope> default_value_scope() { return get_value_scope(m_default_scope_key); }
        void default_value_scope(std::string key) { m_default_scope_key = key; }

#pragma endregion
#pragma region Code Evaluation

        private:
            bool m_evaluate_halt;

            void perform_evaluate()
            {
                if (m_evaluate_halt)
                {
                    m_state = state::evaluating;
                    while (m_evaluate_halt);
                    if (m_state == state::evaluating)
                    {
                        m_state = state::running;
                    }
                }
            }

        public:
            /// <summary>
            /// Executing this method will request a temporary halt of the runtime, to then execute
            /// whatever is passed in view until.
            /// </summary>
            /// <remarks>
            /// A deadlock will happen, if this operation is occuring from within an operator due
            /// to the evaluate expression then waiting for the operator to finish that called
            /// the evaluate method.
            /// </remarks>
            /// <param name="view">The contents to parse.</param>
            /// <param name="success">Wether the operation was successful or something moved wrong.</param>
            /// <param name="request_halt">Allows to disable the runtime-halt request, allowing this to be called from within operators.</param>
            /// <returns></returns>
            value evaluate_expression(std::string str, bool& success, bool request_halt = true);

#pragma endregion
#pragma region Runtime-Context Handling

            private:
                std::vector<std::shared_ptr<sqf::runtime::context>> m_contexts;
                std::shared_ptr<sqf::runtime::context> m_context_active;

            public:
                using context_iterator = std::vector<std::shared_ptr<sqf::runtime::context>>::iterator;
                sqf::runtime::context& context_active()
                { 
                    if (m_context_active) { return *m_context_active; }
                    else if (m_contexts.empty()) { m_context_active = context_create().lock(); return *m_context_active; }
                    else  { m_context_active = m_contexts.front(); return *m_context_active; }
                };
                std::shared_ptr<sqf::runtime::context> context_active_as_shared() const { return m_context_active; };
                std::weak_ptr<context> context_create() { auto ptr = std::make_shared<context>(); m_contexts.push_back(ptr); return ptr; }
                context_iterator context_begin() { return m_contexts.begin(); }
                context_iterator context_end() { return m_contexts.end(); }

#pragma endregion



    private:
        runtime_conf m_configuration;
        std::chrono::system_clock::time_point m_runtime_timestamp;
        bool m_runtime_error;

        std::chrono::system_clock::time_point m_created_timestamp;
        std::chrono::system_clock::time_point m_current_time;
        sqf::runtime::confighost m_confighost;

        std::unique_ptr<sqf::runtime::fileio> m_fileio;
        std::unique_ptr<sqf::runtime::parser::sqf> m_parser_sqf;
        std::unique_ptr<sqf::runtime::parser::config> m_parser_config;
        std::unique_ptr<sqf::runtime::parser::preprocessor> m_parser_preprocessor;

    public:
        runtime(Logger& logger, runtime_conf config) :
            CanLog(logger),
            m_is_halt_requested(false),
            m_is_exit_requested(false),
            m_state(state::empty),
            m_exit_code(0),
            m_run_atomic(false),
            m_breakpoints(),
            m_last_breakpoint_hit(~((size_t)0), {}),
            m_default_scope_key("default"),
            m_evaluate_halt(false),
            m_configuration(config),
            m_runtime_timestamp(std::chrono::system_clock::now()),
            m_runtime_error(false),
            m_created_timestamp(m_runtime_timestamp),
            m_confighost(),
            m_fileio(std::make_unique<sqf::fileio::disabled>()),
            m_parser_sqf(std::make_unique<sqf::parser::sqf::disabled>()),
            m_parser_config(std::make_unique<sqf::parser::config::disabled>()),
            m_parser_preprocessor(std::make_unique<sqf::parser::preprocessor::passthrough>())
        {
        }


        sqf::runtime::runtime::result execute(sqf::runtime::runtime::action action);
        sqf::runtime::runtime::runtime_conf& configuration() { return m_configuration; }
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


    public:
        /// <summary>
        /// Special field that keeps track of the messages that occured (in order) during the current execution.
        /// Cleared after each instruction execution.
        /// </summary>
        std::vector<std::string> log_messages;
        /// <summary>
        /// Breaks encapsulation to provide access to local-logger of runtime to operators.
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
                log_messages.push_back(message.formatMessage());
            }
        }

        /// <summary>
        /// Method that can tell wether a __logmsg operation has
        /// an errored.
        /// </summary>
        /// <returns></returns>
        inline bool& __runtime_error() { return m_runtime_error; }
    };
}
