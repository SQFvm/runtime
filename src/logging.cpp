#include "logging.h"
#include "parsepreprocessor.h"
#include "instruction.h"
using namespace std::string_view_literals;

#pragma region Logger

Logger::Logger(std::ostream& target): logTarget(target) {
    std::fill(enabledWarningLevels.begin(), enabledWarningLevels.end(), true);

#ifndef _DEBUG
    setEnabled(loglevel::trace, false);
#endif
}

Logger::Logger(std::ostream& target, std::ostream& targetErr): logTarget(target) {}

void Logger::log(loglevel level, std::string_view message) {
    std::unique_lock lock(streamLock);

    switch (level) {
        case loglevel::fatal: logTarget << "[FAT]"; break;
        case loglevel::error: logTarget << "[ERR]"; break;
        case loglevel::warning: logTarget << "[WARN]"; break;
        case loglevel::info: logTarget << "[INFO]"; break;
        case loglevel::trace: logTarget << "[T]"; break;
        case loglevel::verbose: logTarget << "[V]"; break;
        default: ;
    }

}


void Logger::log(loglevel, const char* format, ...) {
    
}

#pragma endregion Logger

#pragma region LogLocationInfo

LogLocationInfo::LogLocationInfo(const sqf::parse::preprocessor::finfo& info) {
    path = info.path;
    line = info.line;
    col = info.col;
}

LogLocationInfo::LogLocationInfo(const sqf::instruction& info) {
    path = info.file();
    line = info.line();
    col = info.col();
}

std::string LogLocationInfo::format() const {
    std::string output;

    output.reserve(
        path.length()
        + 6 //col/line good average
        + "[L"sv.length()
        + "|C"sv.length()
        + (path.empty() ? 0 : "|"sv.length())
        + "]"sv.length()
        + "\t"sv.length()
    );

    output.append("[L"sv);
    output.append(std::to_string(line));
    output.append("[C"sv);
    output.append(std::to_string(col));
    if (!path.empty()) {
        output.append("|"sv);
        output.append(path);
    }

    output.append("]\t"sv);

    return output;
}

#pragma endregion LogLocationInfo

void CanLog::log(LogMessageBase&& message) const {
    logger.log(message.getLevel(), message.formatMessage());


    log(logmessage::preprocessor::ArgCountMissmatch(LogLocationInfo()));

}


namespace logmessage::preprocessor {
    ArgCountMissmatch::ArgCountMissmatch(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}

    std::string ArgCountMissmatch::formatMessage() {
        auto output = location.format();
        auto const message = "Arg Count Missmatch."sv;

        output.reserve(output.length() + message.length());
        output.append(message);
        return output;
    }

}


