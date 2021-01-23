/*
#include "dllexports.h"
#include "virtualmachine.h"
#include "commandmap.h"
#include "value.h"
#include "vmstack.h"
#include "configdata.h"
#include "Entry.h"
#include <iostream>
#include <sstream>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <cstring>
#else
#include <limits.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <execinfo.h>
#endif

std::string get_working_dir()
{
#if defined(_WIN32) || defined(_WIN64)
    char buffer[MAX_PATH];
    _getcwd(buffer, MAX_PATH);
    return std::string(buffer);
#elif defined(__GNUC__)
    char buffer[PATH_MAX];
    getcwd(buffer, PATH_MAX);
    return std::string(buffer);
#else
#error "NO IMPLEMENTATION AVAILABLE"
#endif
}
extern "C" {
    DLLEXPORT_PREFIX void sqfvm_init(unsigned long long limit)
    {
        sqfvm_virtualmachine = std::make_shared<sqf::virtualmachine>(sqfvm_exportstarget, limit);
        sqfvm_virtualmachine->allow_suspension(false);
#if !defined(FILESYSTEM_DISABLE_DISALLOW)
        sqfvm_virtualmachine->get_filesystem().disallow(true);
#endif
        sqfvm_virtualmachine->disable_networking();
        sqf::commandmap::get().init();
    }
    DLLEXPORT_PREFIX void sqfvm_exec(const char* code, char* buffer, unsigned int bufferlen)
    {
        std::stringstream sstream;
        bool err;
        std::string executable_path;
        {
#if defined(_WIN32) || defined(_WIN64)
            char buffer[MAX_PATH];
            _getcwd(buffer, MAX_PATH);
            executable_path = sqf::filesystem::sanitize(buffer);
#elif defined(__GNUC__)
            char buffer[PATH_MAX];
            getcwd(buffer, PATH_MAX);
            executable_path = sqf::filesystem::sanitize(buffer);
#else
#error "NO IMPLEMENTATION AVAILABLE"
#endif
        }
        auto inputAfterPP = sqfvm_virtualmachine->preprocess(code, err, "__libraryfeed.sqf");
        if (!err)
        {
            sqfvm_virtualmachine->parse_sqf(inputAfterPP, "__libraryfeed.sqf");
            auto result = sqfvm_virtualmachine->execute(sqf::virtualmachine::execaction::start);
            if (result != sqf::virtualmachine::execresult::OK)
            {
                sqfvm_virtualmachine->execute(sqf::virtualmachine::execaction::abort);
            }
            sstream << sqfvm_exportstarget.str();
            sqfvm_exportstarget.clear();
            auto val = sqfvm_virtualmachine->active_vmstack()->last_value();
            if (val.data() != nullptr)
            {
                sstream << "[WORK]\t<" << sqf::type_str(val.dtype()) << ">\t" << val.as_string() << std::endl;
            }
            else
            {
                sstream << "[WORK]\t<" << "EMPTY" << ">\t" << std::endl;
            }
        }
        else
        {
            sstream << sqfvm_exportstarget.str();
            sqfvm_exportstarget.clear();
        }
        auto str = sstream.str();
        memset(buffer, 0, sizeof(char) * bufferlen);
#ifdef _WIN32
        strncpy_s(buffer, str.length() + 1, str.c_str(), bufferlen);
#else
        std::strncpy(buffer, str.c_str(), bufferlen);
#endif
    }

    DLLEXPORT_PREFIX void sqfvm_loadconfig(const char* cfg)
    {
        sqfvm_virtualmachine->parse_config(cfg);
    }

    DLLEXPORT_PREFIX void sqfvm_uninit()
    {
        sqf::commandmap::get().uninit();
        sqfvm_virtualmachine = std::shared_ptr<sqf::virtualmachine>();
    }
}

void exportstarget::log(loglevel level, std::string_view message)
{
    m_sstream << Logger::loglevelstring(level) << " " << message << std::endl;
}
*/


#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../parser/config/default.h"
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

        if (actual->seq[0] == 'S' && actual->seq[1] == 'Q' && actual->seq[2] == 'F' && actual->seq[3] == 'L')
        {
            return func(*actual);
        }
        else
        {
            return {};
        }
    }
    static void* create_instance(void* user_data, sqfvm_log_callback callback, float max_runtime_seconds)
    {
        auto actual = new instance();
        actual->seq[0] = 'S';
        actual->seq[1] = 'Q';
        actual->seq[2] = 'F';
        actual->seq[3] = 'E';

        actual->logger = new target();

        sqf::runtime::runtime::runtime_conf conf;
        auto duration = std::chrono::duration<float>(max_runtime_seconds);
        auto durationCasted = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        conf.max_runtime = durationCasted;
        conf.disable_sleep = false;
        conf.enable_classname_check = true;
        conf.disable_networking = true;
        conf.print_context_work_to_log_on_exit = false;
        actual->runtime = new sqf::runtime::runtime(*actual->logger, conf);

        actual->runtime->fileio(std::make_unique<sqf::fileio::impl_default>(*actual->logger));
        actual->runtime->parser_config(std::make_unique<sqf::parser::config::impl_default>(*actual->logger));
        actual->runtime->parser_preprocessor(std::make_unique<sqf::parser::preprocessor::impl_default>(*actual->logger));
        actual->runtime->parser_sqf(std::make_unique<sqf::parser::sqf::parser>(*actual->logger));
        sqf::operators::ops(*actual->runtime);

        return actual;
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

extern "C" DLLEXPORT_PREFIX void* sqfvm_create_instance(void* user_data, sqfvm_log_callback callback, float max_runtime_seconds)
{

}
extern "C" DLLEXPORT_PREFIX void sqfvm_destroy_instance(void* instance)
{
    
}
extern "C" DLLEXPORT_PREFIX int32_t sqfvm_load_config(void* instance, const char* contents, uint32_t length)
{
    
}
extern "C" int32_t sqfvm_call(void* instance, void* call_data, char type, const char* code, uint32_t length)
{
    const int32_t instance_invalid = -1;
    const int32_t preprocessing_failed = -2;
    const int32_t parsing_failed = -3;
    const int32_t instance_running = -4;
    const int32_t invalid_type = -5;
    const int32_t result_ok = 0;
    const int32_t result_failed = -6;
    const char type_assembly = 'a';
    const char type_sqf = 's';
    const char type_sqc = 'c';
    const char type_preprocess = 'p';


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
            case type_assembly:
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
            case type_sqf: {
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
            case type_sqc: {
                auto sqc = std::make_shared<sqf::sqc::parser>(*ref.logger);
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
            }
            case type_preprocess: {
                ref.logger->callback(ref.logger->user_data, call_data, -1, ppedStr->data(), ppedStr->length());
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
extern "C" int32_t sqfvm_status(void* instance)
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