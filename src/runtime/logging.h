#pragma once
#include <ostream>
#include <mutex>
#include <utility>
#include <vector>
#include <string_view>
#include <memory>
#include <array>
#include "type.h"
#include "diagnostics/diag_info.h"
#include "diagnostics/stacktrace.h"

using namespace std::string_view_literals;

enum class loglevel {
    fatal,
    error,
    warning,
    info,
    verbose,
    trace
};

class LogLocationInfo {
public:
    LogLocationInfo() = default;
    LogLocationInfo(const sqf::runtime::diagnostics::diag_info&);
    LogLocationInfo(std::string path, size_t line, size_t col) : path(path), line(line), col(col) {}

    std::string path;
    size_t line;
    size_t col;

    [[nodiscard]] std::string format() const;
};


class LogMessageBase {
public:
    LogMessageBase(loglevel level, size_t code) : level(level), errorCode(code) {}
    virtual ~LogMessageBase() = default;

    [[nodiscard]] virtual std::string formatMessage() const = 0;
    virtual loglevel getLevel() const {
        return level;
    }
    virtual size_t getErrorCode() const {
        return errorCode;
    }
    operator LogMessageBase*() {
        return this;
    }
    operator const LogMessageBase*() const {
        return this;
    }
protected:
    loglevel level = loglevel::verbose;
    size_t errorCode;
};
class Logger {
protected:
    std::vector<bool> enabledWarningLevels;
public:
    Logger() : enabledWarningLevels({ true, true, true, true, true, true }) {}
    [[nodiscard]] bool isEnabled(loglevel level) const {
        return enabledWarningLevels[static_cast<size_t>(level)];
    }
    void setEnabled(loglevel level, bool isEnabled) {
        enabledWarningLevels[static_cast<size_t>(level)] = isEnabled;
    }
    static std::string_view loglevelstring(loglevel level)
    {
        switch (level) {
        case loglevel::fatal: return "[FAT]"sv;
        case loglevel::error: return "[ERR]"sv;
        case loglevel::warning: return "[WRN]"sv;
        case loglevel::info: return "[INF]"sv;
        case loglevel::verbose: return "[VBS]"sv;
        case loglevel::trace: return "[TRC]"sv;
        default: return "[???]"sv;
        }
    }
    virtual void log(const LogMessageBase& message) = 0;
};
class StdOutLogger : public Logger {
public:
    StdOutLogger() : Logger() {}

    virtual void log(const LogMessageBase& message) override;
};

//Classes that can log, inherit from this
class CanLog {
    Logger& m_logger;
protected:
    Logger& get_logger() { return m_logger; }
    void log(LogMessageBase& message) const;
    void log(LogMessageBase&& message) const;
public:
    CanLog(Logger& logger) : m_logger(logger) {}
};


namespace logmessage {
    class RuntimeLogMessageBase : public LogMessageBase
    {
    public:
        RuntimeLogMessageBase(loglevel level, size_t errorCode, LogLocationInfo location) :
            LogMessageBase(level, errorCode), m_location(std::move(location)) {}
        LogLocationInfo location() const { return m_location; }
    protected:
        LogLocationInfo m_location;
    };
    namespace preprocessor {
        class PreprocBase : public RuntimeLogMessageBase {
        public:
            PreprocBase(loglevel level, size_t errorCode, LogLocationInfo location) :
                RuntimeLogMessageBase(level, errorCode, std::move(location)) {}
        };

        class ArgCountMissmatch : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 10001;
        public:
            ArgCountMissmatch(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedDataAfterInclude : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 10002;
        public:
            UnexpectedDataAfterInclude(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class RecursiveInclude : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 10003;
            std::string includeTree;
        public:
            RecursiveInclude(LogLocationInfo loc, std::string includeTree) :
                PreprocBase(level, errorCode, std::move(loc)), includeTree(std::move(includeTree)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class IncludeFailed : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 10004;
            std::string line;
            std::string m_exception;
        public:
            IncludeFailed(LogLocationInfo loc, std::string line, const std::runtime_error& exception) :
                PreprocBase(level, errorCode, std::move(loc)), line(line), m_exception(exception.what()) {}
            IncludeFailed(LogLocationInfo loc, std::string line, std::string exception) :
                PreprocBase(level, errorCode, std::move(loc)), line(line), m_exception(exception) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class MacroDefinedTwice : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 10005;
            std::string macroname;
        public:
            MacroDefinedTwice(LogLocationInfo loc, std::string macroname) :
                PreprocBase(level, errorCode, std::move(loc)), macroname(macroname) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class MacroNotFound : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 10006;
            std::string macroname;
        public:
            MacroNotFound(LogLocationInfo loc, std::string macroname) :
                PreprocBase(level, errorCode, std::move(loc)), macroname(macroname) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedIfdef : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 10007;
        public:
            UnexpectedIfdef(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedIfndef : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 10008;
        public:
            UnexpectedIfndef(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedElse : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 10009;
        public:
            UnexpectedElse(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedEndif : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 10010;
        public:
            UnexpectedEndif(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class MissingEndif : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 10011;
        public:
            MissingEndif(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnknownInstruction : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 10012;
            std::string instruction;
        public:
            UnknownInstruction(LogLocationInfo loc, std::string instruction) :
                PreprocBase(level, errorCode, std::move(loc)), instruction(instruction) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class EmptyArgument : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 10013;
        public:
            EmptyArgument(LogLocationInfo loc) :
                PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnknownPragma : public PreprocBase
        {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 10013;
            std::string instruction;
        public:
            UnknownPragma(LogLocationInfo loc, std::string instruction) :
                PreprocBase(level, errorCode, std::move(loc)), instruction(instruction)
            {
            }
            [[nodiscard]] std::string formatMessage() const override;
        };

    }
    namespace assembly
    {
        class AssemblyBase : public RuntimeLogMessageBase {
        public:
            AssemblyBase(loglevel level, size_t errorCode, LogLocationInfo location) :
                RuntimeLogMessageBase(level, errorCode, std::move(location)) {}
        };

        class ExpectedSemicolon : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20001;
        public:
            ExpectedSemicolon(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NoViableAlternativeInstructions : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20002;
        public:
            NoViableAlternativeInstructions(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NoViableAlternativeArg : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20003;
        public:
            NoViableAlternativeArg(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedEndStatement : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20004;
        public:
            ExpectedEndStatement(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedCallNular : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20005;
        public:
            ExpectedCallNular(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedNularOperator : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20006;
        public:
            ExpectedNularOperator(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class UnknownNularOperator : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20007;
            std::string operator_name;
        public:
            UnknownNularOperator(LogLocationInfo loc, std::string operator_name) :
                AssemblyBase(level, errorCode, std::move(loc)), operator_name(operator_name) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedCallUnary : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20008;
        public:
            ExpectedCallUnary(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedUnaryOperator : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20009;
        public:
            ExpectedUnaryOperator(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class UnknownUnaryOperator : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20010;
            std::string operator_name;
        public:
            UnknownUnaryOperator(LogLocationInfo loc, std::string operator_name) :
                AssemblyBase(level, errorCode, std::move(loc)), operator_name(operator_name) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedCallBinary : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20011;
        public:
            ExpectedCallBinary(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedBinaryOperator : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20012;
        public:
            ExpectedBinaryOperator(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class UnknownBinaryOperator : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20013;
            std::string operator_name;
        public:
            UnknownBinaryOperator(LogLocationInfo loc, std::string operator_name) :
                AssemblyBase(level, errorCode, std::move(loc)), operator_name(operator_name) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedAssignTo : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20014;
        public:
            ExpectedAssignTo(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedVariableName : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20015;
        public:
            ExpectedVariableName(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedAssignToLocal : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20016;
        public:
            ExpectedAssignToLocal(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedGetVariable : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20017;
        public:
            ExpectedGetVariable(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedMakeArray : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20018;
        public:
            ExpectedMakeArray(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedInteger : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20019;
        public:
            ExpectedInteger(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedPush : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20020;
        public:
            ExpectedPush(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedTypeName : public AssemblyBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 20021;
        public:
            ExpectedTypeName(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NumberOutOfRange : public AssemblyBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 20022;
        public:
            NumberOutOfRange(LogLocationInfo loc) : AssemblyBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
    }
    namespace sqf
    {
        class SqfBase : public RuntimeLogMessageBase {
        public:
            SqfBase(loglevel level, size_t errorCode, LogLocationInfo location) :
                RuntimeLogMessageBase(level, errorCode, std::move(location)) {}
        };

        class ExpectedStatementTerminator : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30001;
        public:
            ExpectedStatementTerminator(LogLocationInfo loc) : SqfBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NoViableAlternativeStatement : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30002;
        public:
            NoViableAlternativeStatement(LogLocationInfo loc) : SqfBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingUnderscoreOnPrivateVariable : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30003;
            std::string m_variable_name;
        public:
            MissingUnderscoreOnPrivateVariable(LogLocationInfo loc, std::string variable_name) :
                SqfBase(level, errorCode, std::move(loc)),
            m_variable_name(variable_name) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedBinaryExpression : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30004;
        public:
            ExpectedBinaryExpression(LogLocationInfo loc) : SqfBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingRightArgument : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30005;
            std::string m_operator_name;
        public:
            MissingRightArgument(LogLocationInfo loc, std::string operator_name) :
                SqfBase(level, errorCode, std::move(loc)),
                m_operator_name(operator_name) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingRoundClosingBracket : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30006;
        public:
            MissingRoundClosingBracket(LogLocationInfo loc) :
                SqfBase(level, errorCode, std::move(loc)){ }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingCurlyClosingBracket : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30007;
        public:
            MissingCurlyClosingBracket(LogLocationInfo loc) :
                SqfBase(level, errorCode, std::move(loc)){ }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingSquareClosingBracket : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30008;
        public:
            MissingSquareClosingBracket(LogLocationInfo loc) :
                SqfBase(level, errorCode, std::move(loc)){ }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NoViableAlternativePrimaryExpression : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30009;
        public:
            NoViableAlternativePrimaryExpression(LogLocationInfo loc) :
                SqfBase(level, errorCode, std::move(loc)){ }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class EmptyNumber : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30010;
        public:
            EmptyNumber(LogLocationInfo loc) : SqfBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedSQF : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30011;
        public:
            ExpectedSQF(LogLocationInfo loc) : SqfBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class EndOfFile : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30012;
        public:
            EndOfFile(LogLocationInfo loc) : SqfBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class InvalidStartOfGlobalVariable : public SqfBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 30013;
            std::string m_variable_name;
        public:
            InvalidStartOfGlobalVariable(LogLocationInfo loc, std::string variable_name) :
                SqfBase(level, errorCode, std::move(loc)),
                m_variable_name(variable_name) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingStringTermination : public SqfBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 30014;
        public:
            MissingStringTermination(LogLocationInfo loc) :
                SqfBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ParseError : public SqfBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 30015;
            std::string_view msg;
        public:
            ParseError(LogLocationInfo loc, std::string_view msg) :
                SqfBase(level, errorCode, std::move(loc)),
                msg(msg) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
    }
    namespace config
    {
        class ConfigBase : public RuntimeLogMessageBase {
        public:
            ConfigBase(loglevel level, size_t errorCode, LogLocationInfo location) :
                RuntimeLogMessageBase(level, errorCode, std::move(location)) {}
        };
        class ExpectedStatementTerminator : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40001;
        public:
            ExpectedStatementTerminator(LogLocationInfo loc) : ConfigBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NoViableAlternativeNode : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40002;
        public:
            NoViableAlternativeNode(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedIdentifier : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40003;
        public:
            ExpectedIdentifier(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingRoundClosingBracket : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40004;
        public:
            MissingRoundClosingBracket(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingCurlyOpeningBracket : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40005;
        public:
            MissingCurlyOpeningBracket(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingCurlyClosingBracket : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40006;
        public:
            MissingCurlyClosingBracket(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingSquareClosingBracket : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40007;
        public:
            MissingSquareClosingBracket(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MissingEqualSign : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40008;
        public:
            MissingEqualSign(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedArray : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40009;
        public:
            ExpectedArray(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedValue : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40010;
        public:
            ExpectedValue(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NoViableAlternativeValue : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40011;
        public:
            NoViableAlternativeValue(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class EndOfFileNotReached : public ConfigBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40012;
        public:
            EndOfFileNotReached(LogLocationInfo loc) :
                ConfigBase(level, errorCode, std::move(loc)) { }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ParseError : public ConfigBase
        {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 40013;
            std::string_view msg;
        public:
            ParseError(LogLocationInfo loc, std::string_view msg) :
                ConfigBase(level, errorCode, std::move(loc)),
                msg(msg)
            {
            }
            [[nodiscard]] std::string formatMessage() const override;
        };
        class InheritedParentNotFound : public ConfigBase
        {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 40014;
            std::string_view node_name;
            std::string_view parent_name;
        public:
            InheritedParentNotFound(LogLocationInfo loc, std::string_view node_name, std::string_view parent_name) :
                ConfigBase(level, errorCode, std::move(loc)),
                node_name(node_name),
                parent_name(parent_name)
            {
            }
            [[nodiscard]] std::string formatMessage() const override;
        };
    }
    namespace linting
    {
        class LintingBase : public RuntimeLogMessageBase {
        public:
            LintingBase(loglevel level, size_t errorCode, LogLocationInfo location) :
                RuntimeLogMessageBase(level, errorCode, std::move(location)) {}
        };
        class UnassignedVariable : public LintingBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 50001;
            std::string m_variable_name;
        public:
            UnassignedVariable(LogLocationInfo loc, std::string variable_name) :
                LintingBase(level, errorCode, std::move(loc)),
                m_variable_name(variable_name) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
    }
    namespace runtime
    {
        class RuntimeBase : public RuntimeLogMessageBase {
        public:
            RuntimeBase(loglevel level, size_t errorCode, LogLocationInfo location) :
                RuntimeLogMessageBase(level, errorCode, std::move(location)) {}
        };
        class Stacktrace : public RuntimeBase {
            static const loglevel level = loglevel::fatal;
            static const size_t errorCode = 60001;
            ::sqf::runtime::diagnostics::stacktrace m_stacktrace;
        public:
            Stacktrace(LogLocationInfo loc, ::sqf::runtime::diagnostics::stacktrace stacktrace) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_stacktrace(stacktrace)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MaximumRuntimeReached : public RuntimeBase {
            static const loglevel level = loglevel::fatal;
            static const size_t errorCode = 60002;
            std::chrono::milliseconds m_maximum_runtime;
        public:
            MaximumRuntimeReached(LogLocationInfo loc, std::chrono::milliseconds duration) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_maximum_runtime(duration)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedArraySizeMissmatch : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60003;
            size_t m_expected_min;
            size_t m_expected_max;
            size_t m_got;
        public:
            ExpectedArraySizeMissmatch(LogLocationInfo loc, size_t expected, size_t got) :
                ExpectedArraySizeMissmatch(loc, expected, expected, got)
            {}
            ExpectedArraySizeMissmatch(LogLocationInfo loc, size_t expected_min, size_t expected_max, size_t got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_expected_min(expected_min),
                m_expected_max(expected_max),
                m_got(got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedArraySizeMissmatchWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60004;
            size_t m_expected_min;
            size_t m_expected_max;
            size_t m_got;
        public:
            ExpectedArraySizeMissmatchWeak(LogLocationInfo loc, size_t expected, size_t got) :
                ExpectedArraySizeMissmatchWeak(loc, expected, expected, got)
            {}
            ExpectedArraySizeMissmatchWeak(LogLocationInfo loc, size_t expected_min, size_t expected_max, size_t got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_expected_min(expected_min),
                m_expected_max(expected_max),
                m_got(got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedMinimumArraySizeMissmatch : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60005;
            size_t m_expected;
            size_t m_got;
        public:
            ExpectedMinimumArraySizeMissmatch(LogLocationInfo loc, size_t expected, size_t got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_expected(expected),
                m_got(got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedMinimumArraySizeMissmatchWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60006;
            size_t m_expected;
            size_t m_got;
        public:
            ExpectedMinimumArraySizeMissmatchWeak(LogLocationInfo loc, size_t expected, size_t got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_expected(expected),
                m_got(got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedArrayTypeMissmatch : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60007;
            size_t m_position;
            std::vector<::sqf::runtime::type> m_expected;
            ::sqf::runtime::type m_got;
        public:
            ExpectedArrayTypeMissmatch(LogLocationInfo loc, size_t position, std::vector<::sqf::runtime::type> expected, ::sqf::runtime::type got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_position(position),
                m_expected(expected),
                m_got(got)
            {}
            ExpectedArrayTypeMissmatch(LogLocationInfo loc, size_t position, ::sqf::runtime::type expected, ::sqf::runtime::type got) :
                ExpectedArrayTypeMissmatch(loc, position, std::array<::sqf::runtime::type, 1> { expected }, got)
            {}
            template<size_t size>
            ExpectedArrayTypeMissmatch(LogLocationInfo loc, size_t position, std::array<::sqf::runtime::type, size> expected, ::sqf::runtime::type got) :
                ExpectedArrayTypeMissmatch(loc, position, std::vector<::sqf::runtime::type>(expected.begin(), expected.end()), got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedArrayTypeMissmatchWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60008;
            size_t m_position;
            std::vector<::sqf::runtime::type> m_expected;
            ::sqf::runtime::type m_got;
        public:
            ExpectedArrayTypeMissmatchWeak(LogLocationInfo loc, size_t position, std::vector<::sqf::runtime::type> expected, ::sqf::runtime::type got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_position(position),
                m_expected(expected),
                m_got(got)
            {}
            ExpectedArrayTypeMissmatchWeak(LogLocationInfo loc, size_t position, ::sqf::runtime::type expected, ::sqf::runtime::type got) :
                ExpectedArrayTypeMissmatchWeak(loc, position, std::array<::sqf::runtime::type, 1> { expected }, got)
            {}
            template<size_t size>
            ExpectedArrayTypeMissmatchWeak(LogLocationInfo loc, size_t position, std::array<::sqf::runtime::type, size> expected, ::sqf::runtime::type got) :
                ExpectedArrayTypeMissmatchWeak(loc, position, std::vector<::sqf::runtime::type>(expected.begin(), expected.end()), got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class IndexOutOfRange : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60009;
            size_t m_range;
            size_t m_index;
        public:
            IndexOutOfRange(LogLocationInfo loc, size_t range, size_t index) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_range(range),
                m_index(index)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class IndexOutOfRangeWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60010;
            size_t m_range;
            size_t m_index;
        public:
            IndexOutOfRangeWeak(LogLocationInfo loc, size_t range, size_t index) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_range(range),
                m_index(index)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class NegativeIndex : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60011;
        public:
            NegativeIndex(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NegativeIndexWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60012;
        public:
            NegativeIndexWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class IndexEqualsRange : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60013;
            size_t m_range;
            size_t m_index;
        public:
            IndexEqualsRange(LogLocationInfo loc, size_t range, size_t index) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_range(range),
                m_index(index)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ReturningNil : public RuntimeBase {
            static const loglevel level = loglevel::verbose;
            static const size_t errorCode = 60014;
        public:
            ReturningNil(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ReturningEmptyArray : public RuntimeBase {
            static const loglevel level = loglevel::verbose;
            static const size_t errorCode = 60015;
        public:
            ReturningEmptyArray(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NegativeSize : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60016;
        public:
            NegativeSize(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NegativeSizeWeak: public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60017;
        public:
            NegativeSizeWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ArrayRecursion: public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60018;
        public:
            ArrayRecursion(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class InfoMessage: public RuntimeBase {
            static const loglevel level = loglevel::info;
            static const size_t errorCode = 60019;
            std::string m_source;
            std::string m_message;
        public:
            InfoMessage(LogLocationInfo loc, std::string source, std::string message) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_source(source),
                m_message(message)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class SuspensionDisabled : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60020;
        public:
            SuspensionDisabled(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class SuspensionInUnscheduledEnvironment : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60021;
        public:
            SuspensionInUnscheduledEnvironment(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ReturningConfigNull : public RuntimeBase {
            static const loglevel level = loglevel::verbose;
            static const size_t errorCode = 60022;
        public:
            ReturningConfigNull(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class AssertFailed : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60023;
        public:
            AssertFailed(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class StartIndexExceedsToIndex : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60024;
            size_t m_from;
            size_t m_to;
        public:
            StartIndexExceedsToIndex(LogLocationInfo loc, size_t from, size_t to) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_from(from),
                m_to(to)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class StartIndexExceedsToIndexWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60025;
            size_t m_from;
            size_t m_to;
        public:
            StartIndexExceedsToIndexWeak(LogLocationInfo loc, size_t from, size_t to) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_from(from),
                m_to(to)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MagicVariableTypeMissmatch : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60026;
            std::string m_variable_name;
            ::sqf::runtime::type m_expected;
            ::sqf::runtime::type m_got;
        public:
            MagicVariableTypeMissmatch(LogLocationInfo loc, std::string variable_name, ::sqf::runtime::type expected, ::sqf::runtime::type got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_variable_name(variable_name),
                m_expected(expected),
                m_got(got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ScriptHandleAlreadyTerminated : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60027;
        public:
            ScriptHandleAlreadyTerminated(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ScriptHandleAlreadyFinished : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60028;
        public:
            ScriptHandleAlreadyFinished(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExtensionLoaded : public RuntimeBase {
            static const loglevel level = loglevel::verbose;
            static const size_t errorCode = 60029;
            std::string m_extension_name;
            std::string m_version;
        public:
            ExtensionLoaded(LogLocationInfo loc, std::string extension_name, std::string version) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_extension_name(extension_name),
                m_version(version)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExtensionNotTerminatingVersionString : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60030;
            std::string m_extension_name;
        public:
            ExtensionNotTerminatingVersionString(LogLocationInfo loc, std::string extension_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_extension_name(extension_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class ExtensionNotTerminatingCallExtensionBufferString : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60031;
            std::string m_extension_name;
        public:
            ExtensionNotTerminatingCallExtensionBufferString(LogLocationInfo loc, std::string extension_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_extension_name(extension_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExtensionNotTerminatingCallExtensionArgBufferString : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60032;
            std::string m_extension_name;
        public:
            ExtensionNotTerminatingCallExtensionArgBufferString(LogLocationInfo loc, std::string extension_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_extension_name(extension_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class LibraryNameContainsPath : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60033;
            std::string m_extension_name;
        public:
            LibraryNameContainsPath(LogLocationInfo loc, std::string extension_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_extension_name(extension_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ReturningEmptyString : public RuntimeBase {
            static const loglevel level = loglevel::verbose;
            static const size_t errorCode = 60034;
        public:
            ReturningEmptyString(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExtensionRuntimeError : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60035;
            std::string m_extension_name;
            std::string m_what;
        public:
            ExtensionRuntimeError(LogLocationInfo loc, std::string extension_name, std::string what) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_extension_name(extension_name),
                m_what(what)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class FileNotFound : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60036;
            std::string m_filename;
        public:
            FileNotFound(LogLocationInfo loc, std::string filename) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_filename(filename)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ScopeNameAlreadySet : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60037;
        public:
            ScopeNameAlreadySet(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ScriptNameAlreadySet : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60038;
        public:
            ScriptNameAlreadySet(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ReturningEmptyScriptHandle : public RuntimeBase {
            static const loglevel level = loglevel::verbose;
            static const size_t errorCode = 60039;
        public:
            ReturningEmptyScriptHandle(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ReturningErrorCode : public RuntimeBase {
            static const loglevel level = loglevel::verbose;
            static const size_t errorCode = 60040;
            std::string m_error_code;
        public:
            ReturningErrorCode(LogLocationInfo loc, std::string error_code) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_error_code(error_code)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class ExpectedSubArrayTypeMissmatch : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60041;
            std::vector<size_t> m_position;
            std::vector<::sqf::runtime::type> m_expected;
            ::sqf::runtime::type m_got;
        public:
            ExpectedSubArrayTypeMissmatch(LogLocationInfo loc, std::vector<size_t> position, std::vector<::sqf::runtime::type> expected, ::sqf::runtime::type got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_position(position),
                m_expected(expected),
                m_got(got)
            {}
            template<size_t size>
            ExpectedSubArrayTypeMissmatch(LogLocationInfo loc, std::array<size_t, size> position, ::sqf::runtime::type expected, ::sqf::runtime::type got) :
                ExpectedSubArrayTypeMissmatch(loc, position, std::array<::sqf::runtime::type, 1> { expected }, got)
            {}
            template<size_t size1, size_t size2>
            ExpectedSubArrayTypeMissmatch(LogLocationInfo loc, std::array<size_t, size1> position, std::array<::sqf::runtime::type, size2> expected, ::sqf::runtime::type got) :
                ExpectedSubArrayTypeMissmatch(loc, std::vector<size_t>(position.begin(), position.end()), std::vector<::sqf::runtime::type>(expected.begin(), expected.end()), got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedSubArrayTypeMissmatchWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60042;
            std::vector<size_t> m_position;
            std::vector<::sqf::runtime::type> m_expected;
            ::sqf::runtime::type m_got;
        public:
            ExpectedSubArrayTypeMissmatchWeak(LogLocationInfo loc, std::vector<size_t> position, std::vector<::sqf::runtime::type> expected, ::sqf::runtime::type got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_position(position),
                m_expected(expected),
                m_got(got)
            {}
            template<size_t size>
            ExpectedSubArrayTypeMissmatchWeak(LogLocationInfo loc, std::array<size_t, size> position, ::sqf::runtime::type expected, ::sqf::runtime::type got) :
                ExpectedSubArrayTypeMissmatchWeak(loc, position, std::array<::sqf::runtime::type, 1> { expected }, got)
            {}
            template<size_t size1, size_t size2>
            ExpectedSubArrayTypeMissmatchWeak(LogLocationInfo loc, std::array<size_t, size1> position, std::array<::sqf::runtime::type, size2> expected, ::sqf::runtime::type got) :
                ExpectedSubArrayTypeMissmatchWeak(loc, std::vector<size_t>(position.begin(), position.end()), std::vector<::sqf::runtime::type>(expected.begin(), expected.end()), got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ErrorMessage : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60043;
            std::string m_source;
            std::string m_message;
        public:
            ErrorMessage(LogLocationInfo loc, std::string source, std::string message) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_source(source),
                m_message(message)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class FileSystemDisabled : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60044;
        public:
            FileSystemDisabled(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NetworkingDisabled : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60045;
        public:
            NetworkingDisabled(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class AlreadyConnected : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60046;
        public:
            AlreadyConnected(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NetworkingFormatMissmatch : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60047;
            std::string m_provided;
        public:
            NetworkingFormatMissmatch(LogLocationInfo loc, std::string provided) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_provided(provided)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class FailedToEstablishConnection : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60048;
        public:
            FailedToEstablishConnection(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedArrayToHaveElements : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60049;
        public:
            ExpectedArrayToHaveElements(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedArrayToHaveElementsWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60050;
        public:
            ExpectedArrayToHaveElementsWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class ClipboardDisabled : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60051;
        public:
            ClipboardDisabled(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class FailedToCopyToClipboard : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60052;
        public:
            FailedToCopyToClipboard(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class FormatInvalidPlaceholder : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60053;
            char m_placeholder;
            size_t m_index;
        public:
            FormatInvalidPlaceholder(LogLocationInfo loc, char placeholder, size_t index) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_placeholder(placeholder),
                m_index(index)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ZeroDivisor : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60054;
        public:
            ZeroDivisor(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MarkerNotExisting : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60055;
            std::string m_marker_name;
        public:
            MarkerNotExisting(LogLocationInfo loc, std::string marker_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_marker_name(marker_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ReturningDefaultArray : public RuntimeBase {
            static const loglevel level = loglevel::verbose;
            static const size_t errorCode = 60056;
            size_t m_size;
        public:
            ReturningDefaultArray(LogLocationInfo loc, size_t size) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_size(size)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ReturningScalarZero : public RuntimeBase {
            static const loglevel level = loglevel::verbose;
            static const size_t errorCode = 60057;
        public:
            ReturningScalarZero(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedNonNullValue : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60058;
        public:
            ExpectedNonNullValue(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedNonNullValueWeak: public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60059;
        public:
            ExpectedNonNullValueWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ConfigEntryNotFound : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60060;
            std::vector<std::string> m_config_path;
            std::string m_config_name;
        public:
            ConfigEntryNotFound(LogLocationInfo loc, std::vector<std::string> config_path, std::string config_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_config_path(config_path),
                m_config_name(config_name)
            {}
            template<size_t size>
            ConfigEntryNotFound(LogLocationInfo loc, std::array<std::string, size> config_path, std::string config_name) :
                ConfigEntryNotFound(loc, std::vector<std::string>(config_path.begin(), config_path.end()), config_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class ConfigEntryNotFoundWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60061;
            std::vector<std::string> m_config_path;
            std::string m_config_name;
        public:
            ConfigEntryNotFoundWeak(LogLocationInfo loc, std::vector<std::string> config_path, std::string config_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_config_path(config_path),
                m_config_name(config_name)
            {}
            template<size_t size>
            ConfigEntryNotFoundWeak(LogLocationInfo loc, std::array<std::string, size> config_path, std::string config_name) :
                ConfigEntryNotFoundWeak(loc, std::vector<std::string>(config_path.begin(), config_path.end()), config_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedVehicle : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60062;
        public:
            ExpectedVehicle(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedVehicleWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60063;
        public:
            ExpectedVehicleWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedUnit : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60064;
        public:
            ExpectedUnit(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ExpectedUnitWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60065;
        public:
            ExpectedUnitWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ReturningFalse : public RuntimeBase {
            static const loglevel level = loglevel::verbose;
            static const size_t errorCode = 60066;
        public:
            ReturningFalse(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MarkerAlreadyExisting : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60067;
            std::string m_marker_name;
        public:
            MarkerAlreadyExisting(LogLocationInfo loc, std::string marker_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_marker_name(marker_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class InvalidMarkershape : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60067;
            std::string m_shape_name;
        public:
            InvalidMarkershape(LogLocationInfo loc, std::string shape_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_shape_name(shape_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class TypeMissmatch : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60068;
            ::sqf::runtime::type m_expected;
            ::sqf::runtime::type m_got;
        public:
            TypeMissmatch(LogLocationInfo loc, ::sqf::runtime::type expected, ::sqf::runtime::type got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_expected(expected),
                m_got(got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class TypeMissmatchWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60069;
            ::sqf::runtime::type m_expected;
            ::sqf::runtime::type m_got;
        public:
            TypeMissmatchWeak(LogLocationInfo loc, ::sqf::runtime::type expected, ::sqf::runtime::type got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_expected(expected),
                m_got(got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class VariableNotFound : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60070;
            std::string m_variable_name;
        public:
            VariableNotFound(LogLocationInfo loc, std::string variable_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_variable_name(variable_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class StackCorruptionMissingValues : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60071;
            size_t m_expected;
            size_t m_got;
        public:
            StackCorruptionMissingValues(LogLocationInfo loc, size_t expected, size_t got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_expected(expected),
                m_got(got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NoValueFoundForRightArgument : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60072;
        public:
            NoValueFoundForRightArgument(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NoValueFoundForRightArgumentWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60073;
        public:
            NoValueFoundForRightArgumentWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NoValueFoundForLeftArgument : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60074;
        public:
            NoValueFoundForLeftArgument(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NoValueFoundForLeftArgumentWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60075;
        public:
            NoValueFoundForLeftArgumentWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class UnknownInputTypeCombinationBinary : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60076;
            std::string m_operator;
            ::sqf::runtime::type m_left_got;
            ::sqf::runtime::type m_right_got;
        public:
            UnknownInputTypeCombinationBinary(LogLocationInfo loc, ::sqf::runtime::type left_got, std::string op, ::sqf::runtime::type right_got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_operator(op),
                m_left_got(left_got),
                m_right_got(right_got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class UnknownInputTypeCombinationUnary : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60077;
            std::string m_operator;
            ::sqf::runtime::type m_right_got;
        public:
            UnknownInputTypeCombinationUnary(LogLocationInfo loc, std::string op, ::sqf::runtime::type right_got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_operator(op),
                m_right_got(right_got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class UnknownInputTypeCombinationNular : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60078;
            std::string m_operator;
        public:
            UnknownInputTypeCombinationNular(LogLocationInfo loc, std::string op) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_operator(op)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class FoundNoValue : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60079;
        public:
            FoundNoValue(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class FoundNoValueWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60080;
        public:
            FoundNoValueWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class CallstackFoundNoValue : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60081;
            std::string m_callstack_name;
        public:
            CallstackFoundNoValue(LogLocationInfo loc, std::string callstack_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_callstack_name(callstack_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class CallstackFoundNoValueWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60082;
            std::string m_callstack_name;
        public:
            CallstackFoundNoValueWeak(LogLocationInfo loc, std::string callstack_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_callstack_name(callstack_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class GroupNotEmpty : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60083;
            std::string m_group_name;
        public:
            GroupNotEmpty(LogLocationInfo loc, std::string group_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_group_name(group_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class ForStepVariableTypeMissmatch : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60084;
            std::string m_variable_name;
            ::sqf::runtime::type m_expected;
            ::sqf::runtime::type m_got;
        public:
            ForStepVariableTypeMissmatch(LogLocationInfo loc, std::string variable_name, ::sqf::runtime::type expected, ::sqf::runtime::type got) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_variable_name(variable_name),
                m_expected(expected),
                m_got(got)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ForStepNoWorkShouldBeDone : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60085;
            double m_step;
            double m_from;
            double m_to;
        public:
            ForStepNoWorkShouldBeDone(LogLocationInfo loc, double step, double from, double to) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_step(step),
                m_from(from),
                m_to(to)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ReconstructionOfCodeFailed : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60086;
        public:
            ReconstructionOfCodeFailed(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class WaitUntilMaxLoopReached : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60087;
        public:
            WaitUntilMaxLoopReached(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ArraySizeChanged : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60088;
            size_t m_before;
            size_t m_after;
        public:
            ArraySizeChanged(LogLocationInfo loc, size_t before, size_t after) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_before(before),
                m_after(after)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class MagicVariableNotPresent : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60089;
            std::string m_variable_name;
        public:
            MagicVariableNotPresent(LogLocationInfo loc, std::string variable_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_variable_name(variable_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class GroupLeaderNotPartOfGroup : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60090;
        public:
            GroupLeaderNotPartOfGroup(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class AssigningNilValue : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60091;
            std::string m_variable_name;
        public:
            AssigningNilValue(LogLocationInfo loc, std::string variable_name) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_variable_name(variable_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ConditionEmpty : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60092;
        public:
            ConditionEmpty(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NilValueFoundForRightArgumentWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60093;
        public:
            NilValueFoundForRightArgumentWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class NilValueFoundForLeftArgumentWeak : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60094;
        public:
            NilValueFoundForLeftArgumentWeak(LogLocationInfo loc) :
                RuntimeBase(level, errorCode, std::move(loc))
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ContextValuePrint : public RuntimeBase {
            static const loglevel level = loglevel::info;
            static const size_t errorCode = 60095;
            ::sqf::runtime::value value;
        public:
            ContextValuePrint(::sqf::runtime::value val) :
                RuntimeBase(level, errorCode, {}), value(val)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ScopeNameNotFound : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60096;
            std::string m_scope_name;
        public:
            ScopeNameNotFound(LogLocationInfo loc, std::string scope_name) :
                RuntimeBase(level, errorCode, std::move(loc)), m_scope_name(scope_name)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class WarningMessage : public RuntimeBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 60097;
            std::string m_source;
            std::string m_message;
        public:
            WarningMessage(LogLocationInfo loc, std::string source, std::string message) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_source(source),
                m_message(message)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class InvalidAssemblyInstruction : public RuntimeBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 60098;
            std::string m_assembly;
        public:
            InvalidAssemblyInstruction(LogLocationInfo loc, std::string assembly) :
                RuntimeBase(level, errorCode, std::move(loc)),
                m_assembly(assembly)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
    }
    namespace fileio
    {
        class FileIoBase : public RuntimeLogMessageBase {
        public:
            FileIoBase(loglevel level, size_t errorCode, LogLocationInfo location) :
                RuntimeLogMessageBase(level, errorCode, std::move(location)) {}
        };
        class ResolveVirtualRequested : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70001;
            std::string_view m_physical;
            std::string_view m_virtual;
        public:
            ResolveVirtualRequested(std::string_view phys, std::string_view virt) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolveVirtualFileNotFound : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70002;
            std::string_view m_physical;
            std::string_view m_virtual;
        public:
            ResolveVirtualFileNotFound(std::string_view phys, std::string_view virt) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolveVirtualFileMatched : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70003;
            std::string_view m_physical;
            std::string_view m_virtual;
            std::string_view m_match_physical;
        public:
            ResolveVirtualFileMatched(std::string_view phys, std::string_view virt, std::string_view match_phys) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt),
                m_match_physical(match_phys) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolveVirtualNavigateUp : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70004;
            std::string_view m_physical;
            std::string_view m_virtual;
        public:
            ResolveVirtualNavigateUp(std::string_view phys, std::string_view virt) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolveVirtualNavigateDown : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70005;
            std::string_view m_physical;
            std::string_view m_virtual;
            std::string_view m_matched;
        public:
            ResolveVirtualNavigateDown(std::string_view phys, std::string_view virt, std::string_view matched) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt),
                m_matched(matched) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolveVirtualNavigateDeadEnd : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70006;
            std::string_view m_physical;
            std::string_view m_virtual;
            std::string_view m_matched;
        public:
            ResolveVirtualNavigateDeadEnd(std::string_view phys, std::string_view virt, std::string_view matched) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt),
                m_matched(matched) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolveVirtualNavigateNoNodesLeftForExploring : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70007;
            std::string_view m_physical;
            std::string_view m_virtual;
        public:
            ResolveVirtualNavigateNoNodesLeftForExploring(std::string_view phys, std::string_view virt) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolveVirtualTestFileExists : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70008;
            std::string_view m_physical;
            std::string_view m_virtual;
            std::string_view m_matched;
        public:
            ResolveVirtualTestFileExists(std::string_view phys, std::string_view virt, std::string_view matched) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt),
                m_matched(matched) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolveVirtualGotRemainder : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70009;
            std::string_view m_physical;
            std::string_view m_virtual;
        public:
            ResolveVirtualGotRemainder(std::string_view phys, std::string_view virt) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolvePhysicalRequested : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70010;
            std::string_view m_physical;
            std::string_view m_virtual;
            std::string_view m_matched;
        public:
            ResolvePhysicalRequested(std::string_view phys, std::string_view virt, std::string_view matched) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt),
                m_matched(matched) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolvePhysicalAdjustedPath : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70011;
            std::string_view m_physical;
            std::string_view m_virtual;
            std::string_view m_matched;
        public:
            ResolvePhysicalAdjustedPath(std::string_view phys, std::string_view virt, std::string_view matched) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt),
                m_matched(matched) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolvePhysicalTestingAgainst : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70012;
            std::string_view m_physical;
            std::string_view m_virtual;
            std::string_view m_matched;
        public:
            ResolvePhysicalTestingAgainst(std::string_view phys, std::string_view virt, std::string_view matched) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt),
                m_matched(matched) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolvePhysicalMatched : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70013;
            std::string_view m_physical;
            std::string_view m_virtual;
            std::string_view m_matched;
        public:
            ResolvePhysicalMatched(std::string_view phys, std::string_view virt, std::string_view matched) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt),
                m_matched(matched) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class ResolvePhysicalFailedToLookup : public FileIoBase {
            static const loglevel level = loglevel::trace;
            static const size_t errorCode = 70013;
            std::string_view m_physical;
            std::string_view m_virtual;
            std::string_view m_matched;
        public:
            ResolvePhysicalFailedToLookup(std::string_view phys, std::string_view virt, std::string_view matched) :
                FileIoBase(level, errorCode, {}),
                m_physical(phys),
                m_virtual(virt),
                m_matched(matched) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class PBOAlreadyAdded : public FileIoBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 70015;
            std::string_view m_path;
        public:
            PBOAlreadyAdded(std::string_view path) :
                FileIoBase(level, errorCode, {}),
                m_path(path) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class FailedToParsePBO : public FileIoBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 70016;
            std::string_view m_path;
        public:
            FailedToParsePBO(std::string_view path) :
                FileIoBase(level, errorCode, {}),
                m_path(path) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class PBOHasNoPrefixAttribute : public FileIoBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 70017;
            std::string_view m_path;
        public:
            PBOHasNoPrefixAttribute(std::string_view path) :
                FileIoBase(level, errorCode, {}),
                m_path(path) {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class PBOFileAlreadyRegistered : public FileIoBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 70018;
            std::string_view m_path;
            std::string_view m_file;
        public:
            PBOFileAlreadyRegistered(std::string_view path, std::string_view file) :
                FileIoBase(level, errorCode, {}),
                m_path(path),
                m_file(path)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class PBOFileNotFound : public FileIoBase {
            static const loglevel level = loglevel::fatal;
            static const size_t errorCode = 70019;
            std::string_view m_path;
        public:
            PBOFileNotFound(std::string_view path) :
                FileIoBase(level, errorCode, {}),
                m_path(path)
            {}
            [[nodiscard]] std::string formatMessage() const override;
        };
        class PBOFailedToReadFile : public FileIoBase {
            static const loglevel level = loglevel::fatal;
            static const size_t errorCode = 70020;
            std::string_view m_path;
            std::string_view m_file;
        public:
            PBOFailedToReadFile(std::string_view path, std::string_view file) :
                FileIoBase(level, errorCode, {}),
                m_path(path),
                m_file(path)
            {
            }
            [[nodiscard]] std::string formatMessage() const override;
        };
    }
}

