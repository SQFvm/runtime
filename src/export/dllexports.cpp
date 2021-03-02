#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../parser/config/config_parser.hpp"
#include "../parser/assembly/assembly_parser.h"
#include "../parser/sqf/sqf_parser.hpp"
#include "../parser/preprocessor/default.h"
#include "../operators/ops.h"
#include "../fileio/default.h"
#include "../sqc/sqc_parser.h"
#include "dllexports.h"

#include <chrono>
#include <functional>
#include <optional>
#include <memory>

using namespace std::string_view_literals;

namespace dllexports
{
    class target : public Logger
    {
        // Inherited via Logger
        virtual void log(const LogMessageBase& message) override
        {
            auto str = message.formatMessage();
            callback(user_data, call_data, static_cast<int32_t>(message.getLevel()), str.data(), str.length());
        }
    public:
        void* user_data;
        void* call_data;
        sqfvm_log_callback callback;

        target() : Logger() {}
    };
    struct instance
    {
        char seq[4];
        target* logger;
        sqf::runtime::runtime* runtime;
    };
    // Performs the provided action on an instance
    // @returns empty optional if in is no valid instace
    static std::optional<int32_t> with_instance_do(void* in, std::function<int32_t(instance&)> func)
    {
        auto actual = reinterpret_cast<instance*>(in);

        if (in && actual->seq[0] == 'S' && actual->seq[1] == 'Q' && actual->seq[2] == 'F' && actual->seq[3] == 'E')
        {
            return func(*actual);
        }
        else
        {
            return {};
        }
    }
    enum class ops_set
    {
        none,
        basic,
        full
    };
    static void* create_instance(void* user_data, sqfvm_log_callback callback, float max_runtime_seconds, ops_set ops)
    {
        auto actual = new instance();
        actual->seq[0] = 'S';
        actual->seq[1] = 'Q';
        actual->seq[2] = 'F';
        actual->seq[3] = 'E';

        actual->logger = new target();
        actual->logger->callback = callback;
        actual->logger->user_data = user_data;

        sqf::runtime::runtime::runtime_conf conf;
        auto duration = std::chrono::duration<float>(max_runtime_seconds);
        auto durationCasted = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        conf.max_runtime = durationCasted;
        conf.disable_sleep = false;
        conf.enable_classname_check = true;
        conf.disable_networking = true;
        conf.print_context_work_to_log_on_exit = true;
        actual->runtime = new sqf::runtime::runtime(*actual->logger, conf);

        actual->runtime->fileio(std::make_unique<sqf::fileio::impl_default>(*actual->logger));
        actual->runtime->parser_config(std::make_unique<sqf::parser::config::parser>(*actual->logger));
        actual->runtime->parser_preprocessor(std::make_unique<sqf::parser::preprocessor::impl_default>(*actual->logger));
        actual->runtime->parser_sqf(std::make_unique<sqf::parser::sqf::parser>(*actual->logger));
        switch (ops)
        {
            case dllexports::ops_set::none:
            default:
            break;
            case dllexports::ops_set::basic:
            sqf::operators::ops_config(*actual->runtime);
            sqf::operators::ops_diag(*actual->runtime);
            sqf::operators::ops_generic(*actual->runtime);
            // sqf::operators::ops_group(*actual->runtime);
            sqf::operators::ops_logic(*actual->runtime);
            // sqf::operators::ops_markers(*actual->runtime);
            sqf::operators::ops_math(*actual->runtime);
            sqf::operators::ops_namespace(*actual->runtime);
            // sqf::operators::ops_object(*actual->runtime);
            sqf::operators::ops_sqfvm(*actual->runtime);
            sqf::operators::ops_string(*actual->runtime);
            sqf::operators::ops_text(*actual->runtime);
            // sqf::operators::ops_dummy_nular(*actual->runtime);
            // sqf::operators::ops_dummy_unary(*actual->runtime);
            // sqf::operators::ops_dummy_binary(*actual->runtime);
            sqf::operators::ops_osspecific(*actual->runtime);
            sqf::operators::ops_hashmap(*actual->runtime);
            break;
            case dllexports::ops_set::full:
            sqf::operators::ops(*actual->runtime);
            break;
        }

        return actual;
    }
    static void destroy_instance(void* in)
    {
        auto actual = reinterpret_cast<instance*>(in);

        if (actual->seq[0] == 'S' && actual->seq[1] == 'Q' && actual->seq[2] == 'F' && actual->seq[3] == 'E')
        {
            actual->seq[0] = '\0';
            actual->seq[1] = '\0';
            actual->seq[2] = '\0';
            actual->seq[3] = '\0';
            delete actual->runtime;
            delete actual->logger;
            delete actual;
        }
    }
    namespace time
    {
        using unixstamp = std::chrono::seconds;
        static unixstamp get_unixstamp()
        {
            auto timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
            auto casted = std::chrono::duration_cast<unixstamp>(timeSinceEpoch);
            return casted;
        }
    }
}
extern "C" {
    DLLEXPORT_PREFIX void* sqfvm_create_instance(void* user_data, sqfvm_log_callback callback, float max_runtime_seconds)
    {
        return dllexports::create_instance(user_data, callback, max_runtime_seconds, dllexports::ops_set::full);
    }
    DLLEXPORT_PREFIX void* sqfvm_create_instance_basic(void* user_data, sqfvm_log_callback callback, float max_runtime_seconds)
    {
        return dllexports::create_instance(user_data, callback, max_runtime_seconds, dllexports::ops_set::basic);
    }
    DLLEXPORT_PREFIX void* sqfvm_create_instance_empty(void* user_data, sqfvm_log_callback callback, float max_runtime_seconds)
    {
        return dllexports::create_instance(user_data, callback, max_runtime_seconds, dllexports::ops_set::none);
    }
    DLLEXPORT_PREFIX void sqfvm_destroy_instance(void* instance)
    {
        return dllexports::destroy_instance(instance);
    }
    DLLEXPORT_PREFIX int32_t sqfvm_load_config(void* instance, const char* contents, uint32_t length)
    {
        const int32_t instance_invalid = -1;
        const int32_t preprocessing_failed = -2;
        const int32_t parsing_failed = -3;
        const int32_t result_ok = 0;
        auto result = dllexports::with_instance_do(instance, [&](dllexports::instance& ref) -> int32_t {
            auto ppedStr = ref.runtime->parser_preprocessor().preprocess(
                *ref.runtime, std::string_view(contents, length), { "dllexports"sv, {} });

            if (!ppedStr.has_value())
            {
                ref.logger->callback(ref.logger->user_data, NULL, -1, ppedStr->data(), ppedStr->length());
                return preprocessing_failed;
            }
            auto success = ref.runtime->parser_config().parse(ref.runtime->confighost(), *ppedStr, { "dllexports"sv, {} });
            return success ? result_ok : parsing_failed;
        });
        if (result.has_value())
        {
            return result.value();
        }
        else
        {
            return instance_invalid;
        }
    }
    DLLEXPORT_PREFIX int32_t sqfvm_call(void* instance, void* call_data, char type, const char* code, uint32_t length)
    {
        const int32_t instance_invalid = -1;
        const int32_t preprocessing_failed = -2;
        const int32_t parsing_failed = -3;
        const int32_t instance_running = -4;
        const int32_t invalid_type = -5;
        const int32_t result_ok = 0;
        const int32_t result_failed = -6;


        auto result = dllexports::with_instance_do(instance, [&](dllexports::instance& ref) -> int32_t {
            if (ref.runtime->runtime_state() != sqf::runtime::runtime::state::empty)
            {
                return instance_running;
            }
            ref.logger->call_data = call_data;
            auto ppedStr = ref.runtime->parser_preprocessor().preprocess(
                *ref.runtime, std::string_view(code, length), { "dllexports"sv, {} });

            if (!ppedStr.has_value())
            {
                ref.logger->callback(ref.logger->user_data, call_data, -1, ppedStr->data(), ppedStr->length());
                return preprocessing_failed;
            }
            switch (type)
            {
                case 'a':
                {
                    auto sqc = std::make_shared<sqf::parser::assembly::parser>(*ref.logger);
                    auto set = sqc->parse(*ref.runtime, ppedStr.value(), { "dllexports"sv, {} });
                    if (!set.has_value())
                    {
                        return parsing_failed;
                    }
                    else
                    {
                        auto wptr = ref.runtime->context_create();
                        auto context = wptr.lock();
                        context->push_frame({ ref.runtime->default_value_scope(), set.value() });
                        auto result = ref.runtime->execute(sqf::runtime::runtime::action::start);
                        switch (result)
                        {
                            case sqf::runtime::runtime::result::ok:
                            case sqf::runtime::runtime::result::empty:
                            return result_ok;

                            case sqf::runtime::runtime::result::invalid:
                            case sqf::runtime::runtime::result::action_error:
                            case sqf::runtime::runtime::result::runtime_error:
                            ref.runtime->execute(sqf::runtime::runtime::action::abort);
                            default:
                            return result_failed;
                        }
                    }
                } break;
                case 's':
                {
                    auto set = ref.runtime->parser_sqf().parse(*ref.runtime, ppedStr.value(), { "dllexports"sv, {} });
                    if (!set.has_value())
                    {
                        return parsing_failed;
                    }
                    else
                    {
                        auto wptr = ref.runtime->context_create();
                        auto context = wptr.lock();
                        context->push_frame({ ref.runtime->default_value_scope(), set.value() });
                        auto result = ref.runtime->execute(sqf::runtime::runtime::action::start);
                        switch (result)
                        {
                            case sqf::runtime::runtime::result::ok:
                            case sqf::runtime::runtime::result::empty:
                            return result_ok;

                            case sqf::runtime::runtime::result::invalid:
                            case sqf::runtime::runtime::result::action_error:
                            case sqf::runtime::runtime::result::runtime_error:
                            ref.runtime->execute(sqf::runtime::runtime::action::abort);
                            default:
                            return result_failed;
                        }
                    }
                }
                case 'c':
                {
                    sqf::sqc::parser sqc(*ref.logger);
                    auto set = sqc.parse(*ref.runtime, ppedStr.value(), { "dllexports"sv, {} });
                    if (!set.has_value())
                    {
                        return parsing_failed;
                    }
                    else
                    {
                        auto wptr = ref.runtime->context_create();
                        auto context = wptr.lock();
                        context->push_frame({ ref.runtime->default_value_scope(), set.value() });
                        auto result = ref.runtime->execute(sqf::runtime::runtime::action::start);
                        switch (result)
                        {
                            case sqf::runtime::runtime::result::ok:
                            case sqf::runtime::runtime::result::empty:
                            return result_ok;

                            case sqf::runtime::runtime::result::invalid:
                            case sqf::runtime::runtime::result::action_error:
                            case sqf::runtime::runtime::result::runtime_error:
                            ref.runtime->execute(sqf::runtime::runtime::action::abort);
                            default:
                            return result_failed;
                        }
                    }
                }
                case 'p':
                {
                    ref.logger->callback(ref.logger->user_data, call_data, -1, ppedStr->data(), ppedStr->length());
                    return result_ok;
                }
                case '1':
                {
                    auto set = ref.runtime->parser_sqf().parse(*ref.runtime, ppedStr.value(), { "dllexports"sv, {} });
                    if (!set.has_value())
                    {
                        return parsing_failed;
                    }
                    else
                    {
                        sqf::sqc::parser sqc(*ref.logger);
                        auto res = sqc.to_sqc(set.value());
                        ref.logger->callback(ref.logger->user_data, call_data, (int32_t)loglevel::info, "", 0);
                        ref.logger->callback(ref.logger->user_data, call_data, (int32_t)loglevel::info, res.data(), res.length());
                    }
                    return result_ok;
                }
                default: return invalid_type;
            }
            return static_cast<int32_t>(ref.runtime->runtime_state());
        });
        if (result.has_value())
        {
            return result.value();
        }
        else
        {
            return instance_invalid;
        }
    }
    DLLEXPORT_PREFIX int32_t sqfvm_status(void* instance)
    {
        auto result = dllexports::with_instance_do(instance, [](dllexports::instance& ref) -> int32_t {
            return static_cast<int32_t>(ref.runtime->runtime_state());
        });
        if (result.has_value())
        {
            return result.value();
        }
        else
        {
            return -1;
        }
    }
}