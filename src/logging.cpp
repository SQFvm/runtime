#include "logging.h"
#include "parsing/parsepreprocessor.h"
#include "parsing/position_info.h"
#include "instruction.h"
using namespace std::string_view_literals;

#pragma region Logger

Logger::Logger(std::ostream& target): logTarget(target) {
    std::fill(enabledWarningLevels.begin(), enabledWarningLevels.end(), true);

#ifndef _DEBUG
    setEnabled(loglevel::trace, false);
#endif
}

void Logger::log(loglevel level, std::string_view message) {
    std::unique_lock lock(streamLock);

    switch (level) {
        case loglevel::fatal: logTarget << "[FAT]"; break;
        case loglevel::error: logTarget << "[ERR]"; break;
        case loglevel::warning: logTarget << "[WRN]"; break;
        case loglevel::info: logTarget << "[INF]"; break;
		case loglevel::verbose: logTarget << "[VBS]"; break;
        case loglevel::trace: logTarget << "[TRC]"; break;
        default: ;
    }

    logTarget << message;
}


void Logger::log(loglevel, const char* format, ...) {
    
}

#pragma endregion Logger

#pragma region LogLocationInfo

LogLocationInfo::LogLocationInfo(const sqf::parse::preprocessorfileinfo& info) {
    path = info.path;
    line = info.line;
    col = info.col;
}

LogLocationInfo::LogLocationInfo(const sqf::parse::position_info& info)
{
	path = info.file;
	line = info.line;
	col = info.column;
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
    if (!m_logger.isEnabled(message.getLevel())) return;
	m_logger.log(message.getLevel(), message.formatMessage());


    //log(logmessage::preprocessor::ArgCountMissmatch(LogLocationInfo()));

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
        const auto message = "Unexpected ENDIF. Not inside of a IFDEF or IFNDEF enclosure."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string MissingEndif::formatMessage() const {
        auto output = location.format();
        const auto message = "Missing ENDIF. Still inside of a IFDEF or IFNDEF enclosure at end of file."sv;

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

namespace logmessage::assembly {
	std::string ExpectedSemicolon::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected Semicolon."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string NoViableAlternativeInstructions::formatMessage() const
	{
		auto output = location.format();
		const auto message = "No viable alternative at Instruction Path."sv 
			"Expected: ENDSTATEMENT or CALLUNARY or CALLBINARY or ASSIGNTO" 
			"or ASSIGNTOLOCAL or CALLNULAR or GETVARIABLE or MAKEARRAY or PUSH"sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string NoViableAlternativeArg::formatMessage() const
	{
		auto output = location.format();
		const auto message = "No viable alternative at Instruction Path."sv
			"Expected: CALLUNARY or CALLBINARY or ASSIGNTO"
			"or ASSIGNTOLOCAL or CALLNULAR or GETVARIABLE or MAKEARRAY"sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedEndStatement::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected literal 'endStatement'."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedCallNular::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected literal 'callNular'."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedNularOperator::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected name of nular operator."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string UnknownNularOperator::formatMessage() const
	{
		auto output = location.format();

		output.reserve(
			output.length()
			+ "Unknown nular operator '"sv.length()
			+ operator_name.length()
			+ "'."sv.length()
		);

		output.append("Unknown nular operator '"sv);
		output.append(operator_name);
		output.append("'."sv);
		return output;
	}
	std::string ExpectedCallUnary::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected literal 'callUnary'."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedUnaryOperator::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected name of unary operator."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string UnknownUnaryOperator::formatMessage() const
	{
		auto output = location.format();

		output.reserve(
			output.length()
			+ "Unknown unary operator '"sv.length()
			+ operator_name.length()
			+ "'."sv.length()
		);

		output.append("Unknown unary operator '"sv);
		output.append(operator_name);
		output.append("'."sv);
		return output;
	}
	std::string ExpectedCallBinary::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected literal 'callBinary'."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedBinaryOperator::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected name of binary operator."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string UnknownBinaryOperator::formatMessage() const
	{
		auto output = location.format();

		output.reserve(
			output.length()
			+ "Unknown binary operator '"sv.length()
			+ operator_name.length()
			+ "'."sv.length()
		);

		output.append("Unknown binary operator '"sv);
		output.append(operator_name);
		output.append("'."sv);
		return output;
	}
	std::string ExpectedAssignTo::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected literal 'assignTo'."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedVariableName::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected variable name."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedAssignToLocal::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected literal 'assignToLocal'."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedGetVariable::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected literal 'getVariable'."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedMakeArray::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected literal 'makeArray'."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedInteger::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected integer."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedPush::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected literal 'push'."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedTypeName::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected type name."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
}

namespace logmessage::sqf
{
	std::string ExpectedStatementTerminator::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected Statement termination using `;` or `,`."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string NoViableAlternativeStatement::formatMessage() const
	{
		auto output = location.format();
		const auto message = "No viable alternative for Statement. Expected Assignment or Expression."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string MissingUnderscoreOnPrivateVariable::formatMessage() const
	{
		auto output = location.format();

		output.reserve(
			output.length()
			+ "Missing underscore (`_`) for private variable with '"sv.length()
			+ m_variable_name.length()
			+ "'."sv.length()
		);

		output.append("Missing underscore (`_`) for private variable with '"sv);
		output.append(m_variable_name);
		output.append("'."sv);
		return output;
	}
	std::string ExpectedBinaryExpression::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected Expression."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string MissingRightArgument::formatMessage() const
	{
		// m_operator_name
		auto output = location.format();

		output.reserve(
			output.length()
			+ "Missing right argument on operator '"sv.length()
			+ m_operator_name.length()
			+ "'."sv.length()
		);

		output.append("Missing right argument on operator '"sv);
		output.append(m_operator_name);
		output.append("'."sv);
		return output;
	}
	std::string MissingRoundClosingBracket::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Missing round closing bracket (`)`)."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string MissingCurlyClosingBracket::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Missing round curly bracket (`}`)."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string MissingSquareClosingBracket::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Missing square closing bracket (`]`)."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string NoViableAlternativePrimaryExpression::formatMessage() const
	{
		auto output = location.format();
		const auto message = "No viable alternative for Primary Expression. Expected NUMBER or UNARYEXPRESSION or NULAREXPRESSION or VARIABLE or STRING or CODE or BRACKETS or ARRAY."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string EmptyNumber::formatMessage() const
	{
		auto output = location.format();
		const auto message = "The materialized numeric value is empty."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string ExpectedSQF::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Expected SQF."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
	std::string EndOfFile::formatMessage() const
	{
		auto output = location.format();
		const auto message = "Unexpected end-of-file reached."sv;

		output.reserve(
			output.length()
			+ message.length()
		);

		output.append(message);
		return output;
	}
}