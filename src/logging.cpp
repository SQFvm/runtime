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

    std::string ArgCountMissmatch::formatMessage() const {
        auto output = location.format();
        auto const message = "Arg Count Missmatch."sv;

        output.reserve(output.length() + message.length());
        output.append(message);
        return output;
    }

    std::string UnexpectedDataAfterInclude::formatMessage() const {
        auto output = location.format();
        auto const message = "Unexpected data after include path."sv;

        output.reserve(output.length() + message.length());
        output.append(message);
        return output;

    }

    std::string RecursiveInclude::formatMessage() const {
        auto output = location.format();
        auto const message = "Recursive include detected. Include Tree:"sv;

        output.reserve(output.length() + message.length() + includeTree.length());
        output.append(message);
        output.append(includeTree);
        return output;
    }

    std::string IncludeFailed::formatMessage() const {
        auto output = location.format();

        const std::string_view exceptionMessage(exception.what());

        output.reserve(
            output.length() 
            + "Failed to include '"sv.length()
            + line.length()
            + "' into file '"sv.length()
            + location.path.length()
            + "']\t':"sv.length()
            + exceptionMessage.length()
        );
        output.append("Failed to include '"sv);
        output.append(line);
        output.append("' into file '"sv);
        output.append(location.path);
        output.append("']\t':"sv);
        output.append(exceptionMessage);
        return output;

    }

    std::string MacroDefinedTwice::formatMessage() const {
        auto output = location.format();

        output.reserve(
            output.length() 
            + "Macro '"sv.length()
            + macroname.length()
            + "' defined twice."sv.length()
        );

        output.append("Macro '"sv);
        output.append(macroname);
        output.append("' defined twice."sv);
        return output;
    }

    std::string MacroNotFound::formatMessage() const {
        auto output = location.format();

        output.reserve(
            output.length() 
            + "Macro '"sv.length()
            + macroname.length()
            + "' not found."sv.length()
        );

        output.append("Macro '"sv);
        output.append(macroname);
        output.append("' not found."sv);
        return output;
    }

    std::string UnexpectedIfdef::formatMessage() const {
        auto output = location.format();
        const auto message = "Unexpected IFDEF. Already inside of a IFDEF or IFNDEF enclosure."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }

    std::string UnexpectedIfndef::formatMessage() const {
        auto output = location.format();
        const auto message = "Unexpected IFNDEF. Already inside of a IFDEF or IFNDEF enclosure."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }

    std::string UnexpectedElse::formatMessage() const {
        auto output = location.format();
        const auto message = "Unexpected ELSE. Not inside of a IFDEF or IFNDEF enclosure."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }

    std::string UnexpectedEndif::formatMessage() const {
        auto output = location.format();
        const auto message = "Unexpected ENDIF. Not inside inside of a IFDEF or IFNDEF enclosure."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }

    std::string UnknownInstruction::formatMessage() const {
        auto output = location.format();

        output.reserve(
            output.length() 
            + "Unknown PreProcessor instruction '"sv.length()
            + instruction.length()
            + "'."sv.length()
        );

        output.append("Unknown PreProcessor instruction '"sv);
        output.append(instruction);
        output.append("'."sv);
        return output;
    }

}


