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


#include "runtime/logging.h"
#include "runtime/runtime.h"
#include "dllexports.h"
#include <chrono>
#include <functional>
#include <optional>

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
    
}
extern "C" int32_t sqfvm_status(void* instance)
{
    auto result = dllexports::with_instance_do(instance, [](dllexports::instance& ref) -> int32_t {
        return static_cast<int32_t>(ref.runtime->runtime_state());
    });
}