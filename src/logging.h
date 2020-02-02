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
			static const size_t errorCode = 1;
        public:
            ArgCountMissmatch(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedDataAfterInclude : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 2;
        public:
            UnexpectedDataAfterInclude(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class RecursiveInclude : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 3;
            std::string includeTree;
        public:
            RecursiveInclude(LogLocationInfo loc, std::string includeTree) :
                PreprocBase(level, errorCode, std::move(loc)), includeTree(std::move(includeTree)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class IncludeFailed : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 4;
            std::string_view line;
            const std::runtime_error& exception;
        public:
            IncludeFailed(LogLocationInfo loc, std::string_view line, const std::runtime_error& exception) :
                PreprocBase(level, errorCode, std::move(loc)), line(line), exception(exception) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class MacroDefinedTwice : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 5;
            std::string_view macroname;
        public:
            MacroDefinedTwice(LogLocationInfo loc, std::string_view macroname) :
                PreprocBase(level, errorCode, std::move(loc)), macroname(macroname) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class MacroNotFound : public PreprocBase {
            static const loglevel level = loglevel::warning;
            static const size_t errorCode = 6;
            std::string_view macroname;
        public:
            MacroNotFound(LogLocationInfo loc, std::string_view macroname) :
                PreprocBase(level, errorCode, std::move(loc)), macroname(macroname) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedIfdef : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 7;
        public:
            UnexpectedIfdef(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedIfndef : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 8;
        public:
            UnexpectedIfndef(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

        class UnexpectedElse : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 9;
        public:
            UnexpectedElse(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

		class UnexpectedEndif : public PreprocBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 10;
		public:
			UnexpectedEndif(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};

		class MissingEndif : public PreprocBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 11;
		public:
			MissingEndif(LogLocationInfo loc) : PreprocBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};

        class UnknownInstruction : public PreprocBase {
            static const loglevel level = loglevel::error;
            static const size_t errorCode = 12;
            std::string_view instruction;
        public:
            UnknownInstruction(LogLocationInfo loc, std::string_view instruction) :
                PreprocBase(level, errorCode, std::move(loc)), instruction(instruction) {}
            [[nodiscard]] std::string formatMessage() const override;
        };

    }
	namespace assembly
	{
		class ParseBase : public LogMessageBase {
		public:
			ParseBase(loglevel level, size_t errorCode, LogLocationInfo location) :
				LogMessageBase(level, errorCode), location(std::move(location)) {}
		protected:
			LogLocationInfo location;
		};

		class ExpectedSemicolon : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 13;
		public:
			ExpectedSemicolon(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class NoViableAlternativeInstructions : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 13;
		public:
			NoViableAlternativeInstructions(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class NoViableAlternativeArg : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 15;
		public:
			NoViableAlternativeArg(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedEndStatement : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 16;
		public:
			ExpectedEndStatement(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedCallNular : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 17;
		public:
			ExpectedCallNular(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedNularOperator : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 18;
		public:
			ExpectedNularOperator(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class UnknownNularOperator : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 19;
			std::string_view operator_name;
		public:
			UnknownNularOperator(LogLocationInfo loc, std::string_view operator_name) :
				ParseBase(level, errorCode, std::move(loc)), operator_name(operator_name) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedCallUnary : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 20;
		public:
			ExpectedCallUnary(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedUnaryOperator : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 13;
		public:
			ExpectedUnaryOperator(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class UnknownUnaryOperator : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 21;
			std::string_view operator_name;
		public:
			UnknownUnaryOperator(LogLocationInfo loc, std::string_view operator_name) :
				ParseBase(level, errorCode, std::move(loc)), operator_name(operator_name) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedCallBinary : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 22;
		public:
			ExpectedCallBinary(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedBinaryOperator : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 23;
		public:
			ExpectedBinaryOperator(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class UnknownBinaryOperator : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 24;
			std::string_view operator_name;
		public:
			UnknownBinaryOperator(LogLocationInfo loc, std::string_view operator_name) :
				ParseBase(level, errorCode, std::move(loc)), operator_name(operator_name) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedAssignTo : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 25;
		public:
			ExpectedAssignTo(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedVariableName : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 26;
		public:
			ExpectedVariableName(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedAssignToLocal : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 27;
		public:
			ExpectedAssignToLocal(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedGetVariable : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 28;
		public:
			ExpectedGetVariable(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedMakeArray : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 29;
		public:
			ExpectedMakeArray(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedInteger : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 30;
		public:
			ExpectedInteger(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedPush : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 31;
		public:
			ExpectedPush(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
		class ExpectedTypeName : public ParseBase {
			static const loglevel level = loglevel::error;
			static const size_t errorCode = 32;
		public:
			ExpectedTypeName(LogLocationInfo loc) : ParseBase(level, errorCode, std::move(loc)) {}
			[[nodiscard]] std::string formatMessage() const override;
		};
	}
}

