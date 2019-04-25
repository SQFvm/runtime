#pragma once
#include <ostream>
#include <mutex>
#include <utility>
#include <vector>

enum class loglevel {
    fatal,
    error,
    warning,
    info,
    verbose,
    trace
};

class LogMessageBase {
public:
    LogMessageBase(loglevel level, size_t code) : level(level), errorCode(code) {}
    virtual ~LogMessageBase() = default;

    virtual std::string formatMessage();
    virtual loglevel getLevel() {
        return level;
    }
    virtual size_t getErrorCode() {
        return errorCode;
    }
    operator LogMessageBase*(){
        return this;
    }
protected:
    loglevel level = loglevel::verbose;
    size_t errorCode;
};

namespace sqf {
    class instruction;
    namespace parse {
        namespace preprocessor {
            class finfo;
        }
    }
}

class LogLocationInfo {
public:
    LogLocationInfo() = default;
    LogLocationInfo(const sqf::parse::preprocessor::finfo&);
    LogLocationInfo(const sqf::instruction&);

    std::string path;
    size_t line;
    size_t col;

    std::string format() const;
};


//XX(name, number, level, arguments)
//CATS(name) category start
//CATE category end
#define LOG_MESSAGES(XX, CATS, CATE) \
    CATS(preprocessor) \
    XX(ArgCountMissmatch, 1, error) \
    CATE\


class Logger {
    std::ostream& logTarget;
    std::mutex streamLock;

    std::vector<bool> enabledWarningLevels;
public:
    Logger(std::ostream& target);
    Logger(std::ostream& target, std::ostream& targetErr);

    bool isEnabled(loglevel level) {
        return enabledWarningLevels[static_cast<size_t>(level)];
    }

    void setEnabled(loglevel level, bool isEnabled) {
        enabledWarningLevels[static_cast<size_t>(level)] = isEnabled;
    }

    void log(loglevel, std::string_view message);
    void log(loglevel, const char* format, ...);
};

//Classes that can log, inherit from this
class CanLog {
    Logger& logger;
public:
    CanLog(Logger& logger) : logger(logger) {}
    void log(LogMessageBase&& message) const;
};


namespace logmessage {
    
    namespace preprocessor {
        class PreprocBase : public LogMessageBase {
        public:
            PreprocBase(loglevel level, size_t errorCode, LogLocationInfo location) : 
            LogMessageBase(level, errorCode), location(std::move(location)) {}
        protected:
            LogLocationInfo location;
        };



        class ArgCountMissmatch : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 1;
        public:
            ArgCountMissmatch(LogLocationInfo);
            std::string formatMessage() override;
        };

    }
}

