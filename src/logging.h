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

namespace sqf {
    class instruction;
    namespace parse {
		class preprocessorfileinfo;
		class position_info;
    }
}

class LogLocationInfo {
public:
    LogLocationInfo() = default;
    LogLocationInfo(const sqf::parse::preprocessorfileinfo&);
    LogLocationInfo(const sqf::parse::position_info&);
    LogLocationInfo(const sqf::instruction&);

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

class Logger {
    std::ostream& logTarget;
    std::mutex streamLock;

    std::vector<bool> enabledWarningLevels;
public:
    Logger(std::ostream& target);

    [[nodiscard]] bool isEnabled(loglevel level) const {
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
    Logger& m_logger;
protected:
	const Logger& get_logger() { return m_logger; }
public:
    CanLog(Logger& logger) : m_logger(logger) {}
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
            std::string_view line;
            const std::runtime_error& exception;
        public:
            IncludeFailed(LogLocationInfo loc, std::string_view line, const std::runtime_error& exception) :
                PreprocBase(level, errorCode, std::move(loc)), line(line), exception(exception) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class MacroDefinedTwice : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 10005;
            std::string_view macroname;
        public:
            MacroDefinedTwice(LogLocationInfo loc, std::string_view macroname) :
                PreprocBase(level, errorCode, std::move(loc)), macroname(macroname) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class MacroNotFound : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 10006;
            std::string_view macroname;
        public:
            MacroNotFound(LogLocationInfo loc, std::string_view macroname) :
                PreprocBase(level, errorCode, std::move(loc)), macroname(macroname) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedIfdef : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 10007;
        public:
            UnexpectedIfdef(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedIfndef : public PreprocBase {
            static const loglevel level = loglevel::error;
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
            std::string_view instruction;
        public:
            UnknownInstruction(LogLocationInfo loc, std::string_view instruction) :
                PreprocBase(level, errorCode, std::move(loc)), instruction(instruction) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

    }
	namespace assembly
	{
		class AssemblyBase : public LogMessageBase {
		public:
			AssemblyBase(loglevel level, size_t errorCode, LogLocationInfo location) :
				LogMessageBase(level, errorCode), location(std::move(location)) {}
		protected:
			LogLocationInfo location;
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
			std::string_view operator_name;
		public:
			UnknownNularOperator(LogLocationInfo loc, std::string_view operator_name) :
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
			std::string_view operator_name;
		public:
			UnknownUnaryOperator(LogLocationInfo loc, std::string_view operator_name) :
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
			std::string_view operator_name;
		public:
			UnknownBinaryOperator(LogLocationInfo loc, std::string_view operator_name) :
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
	}
	namespace sqf
	{
		class SqfBase : public LogMessageBase {
		public:
			SqfBase(loglevel level, size_t errorCode, LogLocationInfo location) :
				LogMessageBase(level, errorCode), location(std::move(location)) {}
		protected:
			LogLocationInfo location;
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
			std::string_view m_variable_name;
		public:
			MissingUnderscoreOnPrivateVariable(LogLocationInfo loc, std::string_view variable_name) :
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
			std::string_view m_operator_name;
		public:
			MissingRightArgument(LogLocationInfo loc, std::string_view operator_name) :
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
	}
	namespace config
	{
		class ConfigBase : public LogMessageBase {
		public:
			ConfigBase(loglevel level, size_t errorCode, LogLocationInfo location) :
				LogMessageBase(level, errorCode), location(std::move(location)) {}
		protected:
			LogLocationInfo location;
		};

		// To be replaced when config parser is rewritten
		class ConfigErrorPlaceholder : public ConfigBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 40001;
			std::string m_errormessage;
		public:
			ConfigErrorPlaceholder(LogLocationInfo loc, std::string errormessage) : ConfigBase(level, errorCode, std::move(loc)), m_errormessage(errormessage) {}
			[[nodiscard]] std::string formatMessage() const override { return m_errormessage; }
		};
	}
}

