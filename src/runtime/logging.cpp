#include "logging.h"

#include <iostream>
#include <sstream>
using namespace std::string_view_literals;

#pragma region StdOutLogger
void StdOutLogger::log(const LogMessageBase& message) {
    auto& logTarget = std::cout;
    logTarget << Logger::loglevelstring(message.getLevel()) << ' ' << message.formatMessage() << std::endl;
}
#pragma endregion StdOutLogger

#pragma region LogLocationInfo
LogLocationInfo::LogLocationInfo(const sqf::runtime::diagnostics::diag_info& info)
{
    path = info.path.physical;
    line = info.line;
    col =  info.column;
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
    output.append("|C"sv);
    output.append(std::to_string(col));
    if (!path.empty()) {
        output.append("|"sv);
        output.append(path);
    }

    output.append("]\t"sv);

    return output;
}

#pragma endregion LogLocationInfo

void CanLog::log(LogMessageBase& message) const {
    if (!m_logger.isEnabled(message.getLevel())) return;
    m_logger.log(message);


    //log(logmessage::preprocessor::ArgCountMissmatch(LogLocationInfo()));

}
void CanLog::log(LogMessageBase&& message) const {
    if (!m_logger.isEnabled(message.getLevel())) return;
    m_logger.log(message);


    //log(logmessage::preprocessor::ArgCountMissmatch(LogLocationInfo()));

}


namespace logmessage::preprocessor {

    std::string ArgCountMissmatch::formatMessage() const {
        auto output = m_location.format();
        auto const message = "Arg Count Missmatch."sv;

        output.reserve(output.length() + message.length());
        output.append(message);
        return output;
    }

    std::string UnexpectedDataAfterInclude::formatMessage() const {
        auto output = m_location.format();
        auto const message = "Unexpected data after include path."sv;

        output.reserve(output.length() + message.length());
        output.append(message);
        return output;

    }

    std::string RecursiveInclude::formatMessage() const {
        auto output = m_location.format();
        auto const message = "Recursive include detected. Include Tree:"sv;

        output.reserve(output.length() + message.length() + includeTree.length());
        output.append(message);
        output.append(includeTree);
        return output;
    }

    std::string IncludeFailed::formatMessage() const {
        auto output = m_location.format();

        output.reserve(
            output.length() 
            + "Failed to include '"sv.length()
            + line.length()
            + "' into file '"sv.length()
            + m_location.path.length()
            + "']\t':"sv.length()
            + m_exception.length()
        );
        output.append("Failed to include '"sv);
        output.append(line);
        output.append("' into file '"sv);
        output.append(m_location.path);
        output.append("']\t':"sv);
        output.append(m_exception);
        return output;

    }

    std::string MacroDefinedTwice::formatMessage() const {
        auto output = m_location.format();

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
        auto output = m_location.format();

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
        auto output = m_location.format();
        const auto message = "Unexpected IFDEF. Already inside of a IFDEF or IFNDEF enclosure."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }

    std::string UnexpectedIfndef::formatMessage() const {
        auto output = m_location.format();
        const auto message = "Unexpected IFNDEF. Already inside of a IFDEF or IFNDEF enclosure."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }

    std::string UnexpectedElse::formatMessage() const {
        auto output = m_location.format();
        const auto message = "Unexpected ELSE. Not inside of a IFDEF or IFNDEF enclosure."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }

    std::string UnexpectedEndif::formatMessage() const {
        auto output = m_location.format();
        const auto message = "Unexpected ENDIF. Not inside of a IFDEF or IFNDEF enclosure."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string MissingEndif::formatMessage() const {
        auto output = m_location.format();
        const auto message = "Missing ENDIF. Still inside of a IFDEF or IFNDEF enclosure at end of file."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }

    std::string UnknownInstruction::formatMessage() const {
        auto output = m_location.format();

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

    std::string EmptyArgument::formatMessage() const {
        auto output = m_location.format();
        const auto message = "Empty argument passed to macro."sv;

        output.reserve(
            output.length() 
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string UnknownPragma::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "Unknown pragma instruction '"sv.length()
            + instruction.length()
            + "'."sv.length()
        );

        output.append("Unknown pragma instruction '"sv);
        output.append(instruction);
        output.append("'."sv);
        return output;
    }
}

namespace logmessage::assembly {
    std::string ExpectedSemicolon::formatMessage() const
    {
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();

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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();

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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();

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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
        const auto message = "Expected type name."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NumberOutOfRange::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Number out of range. Creating NaN value."sv;

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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();

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
        auto output = m_location.format();
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
        auto output = m_location.format();

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
        auto output = m_location.format();
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
        auto output = m_location.format();
        const auto message = "Missing curly closing bracket (`}`)."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string MissingSquareClosingBracket::formatMessage() const
    {
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
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
        auto output = m_location.format();
        const auto message = "Unexpected end-of-file reached."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string InvalidStartOfGlobalVariable::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "Provided variable name '"sv.length()
            + m_variable_name.length()
            + "' cannot be used in code."sv.length()
        );

        output.append("Provided variable name '"sv);
        output.append(m_variable_name);
        output.append("' cannot be used in code."sv);
        return output;
    }
    std::string MissingStringTermination::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "Unterminated string."sv.length()
        );

        output.append("Unterminated string."sv);
        return output;
    }
    std::string ParseError::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "Parse Error: "sv.length()
            + msg.length()
        );

        output.append("Parse Error: "sv);
        output.append(msg);
        return output;
    }
}

namespace logmessage::config
{
    std::string ExpectedStatementTerminator::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Expected Statement termination using `;`."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NoViableAlternativeNode::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "No viable alternative for Statement. Expected Confignode or Valuenode."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExpectedIdentifier::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Expected Identifier."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string MissingRoundClosingBracket::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Missing round closing bracket (`)`)."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string MissingCurlyOpeningBracket::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Missing curly opening bracket (`{`)."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string MissingCurlyClosingBracket::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Missing curly closing bracket (`}`)."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string MissingSquareClosingBracket::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Missing square closing bracket (`]`)."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string MissingEqualSign::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Missing equal sign (`=`)."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExpectedArray::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Expected Array."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExpectedValue::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Expected Value."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NoViableAlternativeValue::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "No viable alternative for Value. Expected String or Number or Localization or Array."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string EndOfFileNotReached::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Parsing terminated before end of file was reached. "sv
            "This usually means that your provided config contains errors that the parser could not catch."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ParseError::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "Parse Error: "sv.length()
            + msg.length()
        );

        output.append("Parse Error: "sv);
        output.append(msg);
        return output;
    }
    std::string InheritedParentNotFound::formatMessage() const
    {
        auto output = m_location.format();
        const auto message_a = "The provided inherited config node was not located on current or upper levels (class "sv;
        const auto message_b = " : "sv;
        const auto message_c = ")."sv;

        output.reserve(
            output.length()
            + message_a.length()
            + node_name.length()
            + message_b.length()
            + parent_name.length()
            + message_c.length()
        );

        output.append(message_a);
        output.append(node_name);
        output.append(message_b);
        output.append(parent_name);
        output.append(message_c);
        return output;
    }
}
namespace logmessage::linting
{
    std::string UnassignedVariable::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "Unassigned variable '"sv.length()
            + m_variable_name.length()
            + "'."sv.length()
        );

        output.append("Unassigned variable '"sv);
        output.append(m_variable_name);
        output.append("'."sv);
        return output;
    }
}
namespace logmessage::runtime
{
    std::string to_string(double num)
    {
        std::stringstream sstream;
        sstream << num;
        return sstream.str();
    }
    std::string to_string(size_t num)
    {
        std::stringstream sstream;
        sstream << num;
        return sstream.str();
    }
    // one, two, three, [...], 13, 14, 15, [...]
    std::string to_cardinal_string(size_t num)
    {
        switch (num)
        {
        case 0:
            return "zero";
        case 1:
            return "one";
        case 2:
            return "two";
        case 3:
            return "three";
        case 4:
            return "four";
        case 5:
            return "five";
        case 6:
            return "six";
        case 7:
            return "seven";
        case 8:
            return "eight";
        case 9:
            return "nine";
        case 10:
            return "ten";
        case 11:
            return "eleven";
        case 12:
            return "twelve";
        default:
            std::stringstream sstream;
            sstream << num;
            return sstream.str();
            break;
        }
    }
    // first, second, third, [...], 13., 14., 15., [...]
    std::string to_ordinal_string(size_t num)
    {
        switch (num)
        {
        case 0:
            return "null";
        case 1:
            return "first";
        case 2:
            return "second";
        case 3:
            return "third";
        case 4:
            return "fourth";
        case 5:
            return "fifth";
        case 6:
            return "sixth";
        case 7:
            return "seventh";
        case 8:
            return "eighth";
        case 9:
            return "ninth";
        case 10:
            return "tenth";
        case 11:
            return "eleventh";
        case 12:
            return "twelfth";
        default:
            std::stringstream sstream;
            sstream << num << '.';
            return sstream.str();
            break;
        }
    }


    std::string Stacktrace::formatMessage() const
    {
        std::stringstream sstream;
        sstream << m_location.format()  << "Stacktrace:" << m_stacktrace.to_string() << std::endl;
        return sstream.str();
    }
    std::string MaximumRuntimeReached::formatMessage() const
    {
        auto output = m_location.format();
        auto maximum_runtime = to_cardinal_string(m_maximum_runtime.count());

        output.reserve(
            output.length()
            + "Maxium runtime of "sv.length()
            + maximum_runtime.length()
            + "ms reached."sv.length()
        );

        output.append("Maxium runtime of "sv);
        output.append(maximum_runtime);
        output.append("ms reached."sv);
        return output;
    }
    std::string ExpectedArraySizeMissmatch::formatMessage() const
    {
        if (m_expected_min == m_expected_max)
        {
            auto output = m_location.format();
            auto expected_min = to_cardinal_string(m_expected_min);
            auto got = to_cardinal_string(m_got);

            output.reserve(
                output.length()
                + "Array was expected to have "sv.length()
                + expected_min.length()
                + " elements but has "sv.length()
                + got.length()
                + "."sv.length()
            );

            output.append("Array was expected to have "sv);
            output.append(expected_min);
            output.append(" elements but has "sv);
            output.append(got);
            output.append("."sv);
            return output;
        }
        else
        {
            auto output = m_location.format();
            auto expected_min = to_cardinal_string(m_expected_min);
            auto expected_max = to_cardinal_string(m_expected_max);
            auto got = to_cardinal_string(m_got);

            output.reserve(
                output.length()
                + "Array was expected to have "sv.length()
                + expected_min.length()
                + " to "sv.length()
                + expected_max.length()
                + " elements but has "sv.length()
                + got.length()
                + "."sv.length()
            );

            output.append("Array was expected to have "sv);
            output.append(expected_min);
            output.append(" to "sv);
            output.append(expected_max);
            output.append(" elements but has "sv);
            output.append(got);
            output.append("."sv);
            return output;
        }
    }
    std::string ExpectedArraySizeMissmatchWeak::formatMessage() const
    {
        if (m_expected_min == m_expected_max)
        {
            auto output = m_location.format();
            auto expected_min = to_cardinal_string(m_expected_min);
            auto got = to_cardinal_string(m_got);

            output.reserve(
                output.length()
                + "Array was expected to have "sv.length()
                + expected_min.length()
                + " elements but has "sv.length()
                + got.length()
                + "."sv.length()
            );

            output.append("Array was expected to have "sv);
            output.append(expected_min);
            output.append(" elements but has "sv);
            output.append(got);
            output.append("."sv);
            return output;
        }
        else
        {
            auto output = m_location.format();
            auto expected_min = to_cardinal_string(m_expected_min);
            auto expected_max = to_cardinal_string(m_expected_max);
            auto got = to_cardinal_string(m_got);

            output.reserve(
                output.length()
                + "Array was expected to have "sv.length()
                + expected_min.length()
                + " to "sv.length()
                + expected_max.length()
                + " elements but has "sv.length()
                + got.length()
                + "."sv.length()
            );

            output.append("Array was expected to have "sv);
            output.append(expected_min);
            output.append(" to "sv);
            output.append(expected_max);
            output.append(" elements but has "sv);
            output.append(got);
            output.append("."sv);
            return output;
        }
    }
    std::string ExpectedMinimumArraySizeMissmatch::formatMessage() const
    {
        auto output = m_location.format();
        auto expected = to_cardinal_string(m_expected);
        auto got = to_cardinal_string(m_got);

        output.reserve(
            output.length()
            + "Array was expected to have at least "sv.length()
            + expected.length()
            + " elements but has "sv.length()
            + got.length()
            + "."sv.length()
        );

        output.append("Array was expected to have at least "sv);
        output.append(expected);
        output.append(" elements but has "sv);
        output.append(got);
        output.append("."sv);
        return output;
    }
    std::string ExpectedMinimumArraySizeMissmatchWeak::formatMessage() const
    {
        auto output = m_location.format();
        auto expected = to_cardinal_string(m_expected);
        auto got = to_cardinal_string(m_got);

        output.reserve(
            output.length()
            + "Array was expected to have at least "sv.length()
            + expected.length()
            + " elements but has "sv.length()
            + got.length()
            + "."sv.length()
        );

        output.append("Array was expected to have at least "sv);
        output.append(expected);
        output.append(" elements but has "sv);
        output.append(got);
        output.append("."sv);
        return output;
    }
    std::string ExpectedArrayTypeMissmatch::formatMessage() const
    {
        std::stringstream sstream;
        auto output = m_location.format();
        auto position = to_ordinal_string(m_position);
        auto got = m_got.to_string();
        sstream << output;
        sstream << "Expected the "sv;
        sstream << position;
        sstream << " element of the array to be of the type "sv;
        bool flag = false;
        for (const auto& it : m_expected)
        {
            if (flag)
            {
                sstream << " or ";
            }
            flag = true;
            sstream << it.to_string();
        }
        sstream << " but got "sv;
        sstream << got;
        sstream << "."sv;
        return sstream.str();
    }
    std::string ExpectedArrayTypeMissmatchWeak::formatMessage() const
    {
        std::stringstream sstream;
        auto output = m_location.format();
        auto position = to_ordinal_string(m_position);
        auto got = m_got.to_string();
        sstream << output;
        sstream << "Expected the "sv;
        sstream << position;
        sstream << " element of the array to be of the type "sv;
        bool flag = false;
        for (const auto& it : m_expected)
        {
            if (flag)
            {
                sstream << " or ";
            }
            flag = true;
            sstream << it.to_string();
        }
        sstream << " but got "sv;
        sstream << got;
        sstream << "."sv;
        return sstream.str();
    }
    std::string IndexOutOfRange::formatMessage() const
    {
        auto output = m_location.format();
        auto range = to_cardinal_string(m_range);
        auto index = to_cardinal_string(m_index);

        output.reserve(
            output.length()
            + "The requested index "sv.length()
            + index.length()
            + " exceeds the available range of "sv.length()
            + range.length()
            + "."sv.length()
        );

        output.append("The requested index "sv);
        output.append(index);
        output.append(" exceeds the available range of "sv);
        output.append(range);
        output.append("."sv);
        return output;
    }
    std::string IndexOutOfRangeWeak::formatMessage() const
    {
        auto output = m_location.format();
        auto range = to_cardinal_string(m_range);
        auto index = to_cardinal_string(m_index);

        output.reserve(
            output.length()
            + "The requested index "sv.length()
            + index.length()
            + " exceeds the available range of "sv.length()
            + range.length()
            + "."sv.length()
        );

        output.append("The requested index "sv);
        output.append(index);
        output.append(" exceeds the available range of "sv);
        output.append(range);
        output.append("."sv);
        return output;
    }

    std::string NegativeIndex::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "The requested index is negative."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NegativeIndexWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "The requested index is negative."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string IndexEqualsRange::formatMessage() const
    {
        auto output = m_location.format();
        auto range = to_cardinal_string(m_range);
        auto index = to_cardinal_string(m_index);

        output.reserve(
            output.length()
            + "The requested index "sv.length()
            + index.length()
            + " is equal to the available range of "sv.length()
            + range.length()
            + "."sv.length()
        );

        output.append("The requested index "sv);
        output.append(index);
        output.append(" is equal to the available range of "sv);
        output.append(range);
        output.append("."sv);
        return output;
    }
    std::string ReturningNil::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Returning nil."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ReturningEmptyArray::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Returning empty array."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NegativeSize::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "The requested size is negative."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NegativeSizeWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "The requested size is negative."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ArrayRecursion::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Array recursion."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string InfoMessage::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "["sv.length()
            + m_source.length()
            + "] "sv.length()
            + m_message.length()
        );

        output.append("["sv);
        output.append(m_source);
        output.append("] "sv);
        output.append(m_message);
        return output;
    }
    std::string SuspensionDisabled::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Suspension disabled."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }

    std::string SuspensionInUnscheduledEnvironment::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Cannot suspend in non-scheduled environment."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ReturningConfigNull::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Returning config null."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string AssertFailed::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Assertion Failed."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string StartIndexExceedsToIndex::formatMessage() const
    {
        auto output = m_location.format();
        auto from = to_cardinal_string(m_from);
        auto to = to_cardinal_string(m_to);

        output.reserve(
            output.length()
            + "Start index "sv.length()
            + from.length()
            + " exceeds end index "sv.length()
            + to.length()
            + "."sv.length()
        );

        output.append("Start index "sv);
        output.append(from);
        output.append(" exceeds end index "sv);
        output.append(to);
        output.append("."sv);
        return output;
    }
    std::string StartIndexExceedsToIndexWeak::formatMessage() const
    {
        auto output = m_location.format();
        auto from = to_cardinal_string(m_from);
        auto to = to_cardinal_string(m_to);

        output.reserve(
            output.length()
            + "Start index "sv.length()
            + from.length()
            + " exceeds end index "sv.length()
            + to.length()
            + "."sv.length()
        );

        output.append("Start index "sv);
        output.append(from);
        output.append(" exceeds end index "sv);
        output.append(to);
        output.append("."sv);
        return output;
    }
    std::string MagicVariableTypeMissmatch::formatMessage() const
    {
        auto output = m_location.format();
        auto variable_name = m_variable_name;
        auto expected = m_expected.to_string();
        auto got = m_got.to_string();

        output.reserve(
            output.length()
            + "The Magic variable '"sv.length()
            + variable_name.length()
            + "' was expected to be of the type "sv.length()
            + expected.length()
            + " but is "sv.length()
            + got.length()
            + "."sv.length()
        );

        output.append("The Magic variable '"sv);
        output.append(variable_name);
        output.append("' was expected to be of the type "sv);
        output.append(expected);
        output.append(" but is "sv);
        output.append(got);
        output.append("."sv);
        return output;
    }
    std::string ScriptHandleAlreadyTerminated::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Scripthandle already terminated."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ScriptHandleAlreadyFinished::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Scripthandle already finished."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExtensionLoaded::formatMessage() const
    {
        auto output = m_location.format();
        auto extension_name = m_extension_name;
        auto version = m_version;

        output.reserve(
            output.length()
            + "Loaded extension '"sv.length()
            + extension_name.length()
            + "' with version "sv.length()
            + version.length()
            + "."sv.length()
        );

        output.append("Loaded extension '"sv);
        output.append(extension_name);
        output.append("' with version "sv);
        output.append(version);
        output.append("."sv);
        return output;
    }
    std::string ExtensionNotTerminatingVersionString::formatMessage() const
    {
        auto output = m_location.format();
        auto extension_name = m_extension_name;

        output.reserve(
            output.length()
            + "Extension '"sv.length()
            + extension_name.length()
            + "' is not terminating the RVExtensionVersion buffer with a '\\0'."sv.length()
        );

        output.append("Extension '"sv);
        output.append(extension_name);
        output.append("' is not terminating the RVExtensionVersion buffer with a '\\0'."sv);
        return output;
    }

    std::string ExtensionNotTerminatingCallExtensionBufferString::formatMessage() const
    {
        auto output = m_location.format();
        auto extension_name = m_extension_name;

        output.reserve(
            output.length()
            + "Extension '"sv.length()
            + extension_name.length()
            + "' is not terminating the RVExtension buffer with a '\\0'."sv.length()
        );

        output.append("Extension '"sv);
        output.append(extension_name);
        output.append("' is not terminating the RVExtension buffer with a '\\0'."sv);
        return output;
    }
    std::string ExtensionNotTerminatingCallExtensionArgBufferString::formatMessage() const
    {
        auto output = m_location.format();
        auto extension_name = m_extension_name;

        output.reserve(
            output.length()
            + "Extension '"sv.length()
            + extension_name.length()
            + "' is not terminating the RVExtensionArgs buffer with a '\\0'."sv.length()
        );

        output.append("Extension '"sv);
        output.append(extension_name);
        output.append("' is not terminating the RVExtensionArgs buffer with a '\\0'."sv);
        return output;
    }
    std::string LibraryNameContainsPath::formatMessage() const
    {
        auto output = m_location.format();
        auto extension_name = m_extension_name;

        output.reserve(
            output.length()
            + "The extension name '"sv.length()
            + extension_name.length()
            + "' is not valid as it contains path characters."sv.length()
        );

        output.append("The extension name '"sv);
        output.append(extension_name);
        output.append("' is not valid as it contains path characters."sv);
        return output;
    }
    std::string ReturningEmptyString::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Returning empty string."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExtensionRuntimeError::formatMessage() const
    {
        auto output = m_location.format();
        auto extension_name = m_extension_name;
        auto what = m_what;

        output.reserve(
            output.length()
            + "Extension '"sv.length()
            + extension_name.length()
            + "' caused a runtime exception with the contents '"sv.length()
            + what.length()
            + "'."sv.length()
        );

        output.append("Extension '"sv);
        output.append(extension_name);
        output.append("' caused a runtime exception with the contents '"sv);
        output.append(what);
        output.append("'."sv);
        return output;
    }
    std::string FileNotFound::formatMessage() const
    {
        auto output = m_location.format();
        auto filename = m_filename;

        output.reserve(
            output.length()
            + "File '"sv.length()
            + filename.length()
            + "' could not be located."sv.length()
        );

        output.append("File '"sv);
        output.append(filename);
        output.append("' could not be located."sv);
        return output;
    }
    std::string ScopeNameAlreadySet::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Scropename already set."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ScriptNameAlreadySet::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Scriptname already set."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ReturningEmptyScriptHandle::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Returning empty script handle."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ReturningErrorCode::formatMessage() const
    {
        auto output = m_location.format();
        auto error_code = m_error_code;

        output.reserve(
            output.length()
            + "Returning error code "sv.length()
            + error_code.length()
        );

        output.append("Returning error code "sv);
        output.append(error_code);
        return output;
    }

    std::string ExpectedSubArrayTypeMissmatch::formatMessage() const
    {
        std::stringstream sstream;
        auto output = m_location.format();
        auto got = m_got.to_string();
        sstream << "Expected the subarray at index "sv;
        bool flag = false;
        for (const auto& it : m_position)
        {
            if (flag)
            {
                sstream << " -> ";
            }
            flag = true;
            sstream << to_ordinal_string(it);
        }
        sstream << " element of the array to be of the type "sv;
        flag = false;
        for (const auto& it : m_expected)
        {
            if (flag)
            {
                sstream << " or ";
            }
            flag = true;
            sstream << it.to_string();
        }
        sstream << " but got "sv;
        sstream << got;
        sstream << "."sv;
        return sstream.str();
    }
    std::string ExpectedSubArrayTypeMissmatchWeak::formatMessage() const
    {
        std::stringstream sstream;
        auto output = m_location.format();
        auto got = m_got.to_string();
        sstream << "Expected the subarray at index "sv;
        bool flag = false;
        for (const auto& it : m_position)
        {
            if (flag)
            {
                sstream << " -> ";
            }
            flag = true;
            sstream << to_ordinal_string(it);
        }
        sstream << " element of the array to be of the type "sv;
        flag = false;
        for (const auto& it : m_expected)
        {
            if (flag)
            {
                sstream << " or ";
            }
            flag = true;
            sstream << it.to_string();
        }
        sstream << " but got "sv;
        sstream << got;
        sstream << "."sv;
        return sstream.str();
    }
    std::string ErrorMessage::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "["sv.length()
            + m_source.length()
            + "] "sv.length()
            + m_message.length()
        );

        output.append("["sv);
        output.append(m_source);
        output.append("] "sv);
        output.append(m_message);
        return output;
    }
    std::string FileSystemDisabled::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Filesystem disabled."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NetworkingDisabled::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Networking disabled."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string AlreadyConnected::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Failed to establish connection as one is existing already."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NetworkingFormatMissmatch::formatMessage() const
    {
        auto output = m_location.format();
        

        output.reserve(
            output.length()
            + "The provided format '"sv.length()
            + m_provided.length()
            + "' was not matching the expected format 'ADDRESS:PORT'."sv.length()
        );

        output.append("The provided format '"sv);
        output.append(m_provided);
        output.append("' was not matching the expected format."sv);
        return output;
    }
    std::string FailedToEstablishConnection::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Failed to establish connection for unknown reason."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExpectedArrayToHaveElements::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Expected array to have elements."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExpectedArrayToHaveElementsWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Expected array to have elements."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }

    std::string ClipboardDisabled::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Clipboard disabled."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string FailedToCopyToClipboard::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Failed to copy to clipboard."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string FormatInvalidPlaceholder::formatMessage() const
    {
        auto output = m_location.format();
        auto index = to_cardinal_string(m_index);

        output.reserve(
            output.length()
            + "The placeholder '"sv.length()
            + 1
            + "' is no valid placeholder at index"sv.length()
            + index.length()
            + "."sv.length()
        );

        output.append("The placeholder '"sv);
        output.append(&m_placeholder, &m_placeholder);
        output.append("' is no valid placeholder at index"sv);
        output.append(index);
        output.append("."sv);
        return output;
    }
    std::string ZeroDivisor::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Zero divisor."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string MarkerNotExisting::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "The marker '"sv.length()
            + m_marker_name.length()
            + "' is not existing."sv.length()
        );

        output.append("The marker '"sv);
        output.append(m_marker_name);
        output.append("' is not existing."sv);
        return output;
    }
    std::string ReturningDefaultArray::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "Returning default array ["sv.length()
            + (m_size * 3 - 2)
            + "]."sv.length()
        );

        output.append("Returning default array ["sv);
        output.append("0"sv);
        for (size_t i = 1; i < m_size; i++)
        {
            output.append(", 0"sv);
        }
        output.append("]."sv);
        return output;
    }
    std::string ReturningScalarZero::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Returning zero (0)."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExpectedNonNullValue::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Provided value is null."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExpectedNonNullValueWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Provided value is null."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ConfigEntryNotFound::formatMessage() const
    {
        auto output = m_location.format();
        size_t size = 0;
        for (const auto& it : m_config_path)
        {
            size += it.length();
        }
        size += (m_config_path.size() - 1) * 4;
        output.reserve(
            output.length()
            + "No config entry with the name '"sv.length()
            + m_config_name.length()
            + "' could be located at path "sv.length()
            + size
            + "."sv.length()
        );

        output.append("No config entry with the name '"sv);
        output.append(m_config_name);
        output.append("' could be located at path "sv);
        bool flag = false;
        for (const auto& it : m_config_path)
        {
            if (flag)
            {
                output.append(" >> ");
            }
            flag = true;
            output.append(it);
        }
        output.append("."sv);
        return output;
    }

    std::string ConfigEntryNotFoundWeak::formatMessage() const
    {
        auto output = m_location.format();
        size_t size = 0;
        for (const auto& it : m_config_path)
        {
            size += it.length();
        }
        size += (m_config_path.size() - 1) * 4;
        output.reserve(
            output.length()
            + "No config entry with the name '"sv.length()
            + m_config_name.length()
            + "' could be located at path "sv.length()
            + size
            + "."sv.length()
        );

        output.append("No config entry with the name '"sv);
        output.append(m_config_name);
        output.append("' could be located at path "sv);
        bool flag = false;
        for (const auto& it : m_config_path)
        {
            if (flag)
            {
                output.append(" >> ");
            }
            flag = true;
            output.append(it);
        }
        output.append("."sv);
        return output;
    }
    std::string ExpectedVehicle::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Provided object was expected to be a vehicle, got unit."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExpectedVehicleWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Provided object was expected to be a vehicle, got unit."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExpectedUnit::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Provided object was expected to be a unit, got vehicle."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ExpectedUnitWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Provided object was expected to be a unit, got vehicle."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ReturningFalse::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Returning false."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string MarkerAlreadyExisting::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "The marker '"sv.length()
            + m_marker_name.length()
            + "' is not existing."sv.length()
        );

        output.append("The marker '"sv);
        output.append(m_marker_name);
        output.append("' is not existing."sv);
        return output;
    }
    std::string InvalidMarkershape::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "Invalid marker-shape name '"sv.length()
            + m_shape_name.length()
            + "' provided. Valid ones are: ICON, RECTANGLE, ELLIPSE, POLYLINE."sv.length()
        );

        output.append("Invalid marker-shape name '"sv);
        output.append(m_shape_name);
        output.append("' provided. Valid ones are: ICON, RECTANGLE, ELLIPSE, POLYLINE."sv);
        return output;
    }
    std::string TypeMissmatch::formatMessage() const
    {
        auto output = m_location.format();
        auto expected = m_expected.to_string();
        auto got = m_got.to_string();

        output.reserve(
            output.length()
            + "Expected the type "sv.length()
            + expected.length()
            + " but got "sv.length()
            + got.length()
            + "."sv.length()
        );

        output.append("Expected the type "sv);
        output.append(expected);
        output.append(" but got "sv);
        output.append(got);
        output.append("."sv);
        return output;
    }
    std::string TypeMissmatchWeak::formatMessage() const
    {
        auto output = m_location.format();
        auto expected = m_expected.to_string();
        auto got = m_got.to_string();

        output.reserve(
            output.length()
            + "Expected the type "sv.length()
            + expected.length()
            + " but got "sv.length()
            + got.length()
            + "."sv.length()
        );

        output.append("Expected the type "sv);
        output.append(expected);
        output.append(" but got "sv);
        output.append(got);
        output.append("."sv);
        return output;
    }

    std::string VariableNotFound::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "A variable with the name '"sv.length()
            + m_variable_name.length()
            + "' could not be found."sv.length()
        );

        output.append("A variable with the name '"sv);
        output.append(m_variable_name);
        output.append("' could not be found.");
        return output;
    }
    std::string StackCorruptionMissingValues::formatMessage() const
    {
        auto output = m_location.format();
        auto expected = to_cardinal_string(m_expected);
        auto got = to_cardinal_string(m_got);

        output.reserve(
            output.length()
            + "Stack-Corruption detected. Expected "sv.length()
            + expected.length()
            + " values on value stack but got "sv.length()
            + got.length()
            + "."sv.length()
        );

        output.append("Stack-Corruption detected. Expected "sv);
        output.append(expected);
        output.append(" values on value stack but got "sv);
        output.append(got);
        output.append("."sv);
        return output;
    }
    std::string NoValueFoundForRightArgument::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "No value found on value stack for right argument."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NoValueFoundForRightArgumentWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "No value found on value stack for right argument."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NoValueFoundForLeftArgument::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "No value found on value stack for left argument."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NoValueFoundForLeftArgumentWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "No value found on value stack for left argument."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string UnknownInputTypeCombinationBinary::formatMessage() const
    {
        auto output = m_location.format();
        auto left_got = m_left_got.to_string();
        auto right_got = m_right_got.to_string();
        output.reserve(
            output.length()
            + "Unknown input combination "sv.length()
            + left_got.length()
            + " "sv.length()
            + m_operator.length()
            + " "sv.length()
            + right_got.length()
            + "."sv.length()
        );

        output.append("Unknown input combination "sv);
        output.append(left_got);
        output.append(" "sv);
        output.append(m_operator);
        output.append(" "sv);
        output.append(right_got);
        output.append("."sv);
        return output;
    }
    std::string UnknownInputTypeCombinationUnary::formatMessage() const
    {
        auto output = m_location.format();
        auto right_got = m_right_got.to_string();
        output.reserve(
            output.length()
            + "Unknown input combination "sv.length()
            + m_operator.length()
            + " "sv.length()
            + right_got.length()
            + "."sv.length()
        );

        output.append("Unknown input combination "sv);
        output.append(m_operator);
        output.append(" "sv);
        output.append(right_got);
        output.append("."sv);
        return output;
    }
    std::string UnknownInputTypeCombinationNular::formatMessage() const
    {
        auto output = m_location.format();
        output.reserve(
            output.length()
            + "Unknown input combination "sv.length()
            + m_operator.length()
            + "."sv.length()
        );

        output.append("Unknown input combination "sv);
        output.append(m_operator);
        output.append("."sv);
        return output;
    }
    std::string FoundNoValue::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "No value found on value stack."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string FoundNoValueWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "No value found on value stack."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string CallstackFoundNoValue::formatMessage() const
    {
        auto output = m_location.format();
        auto callstack_name = m_callstack_name;

        output.reserve(
            output.length()
            + "Callstack "sv.length()
            + callstack_name.length()
            + " found no value."sv.length()
        );

        output.append("Callstack "sv);
        output.append(callstack_name);
        output.append(" found no value."sv);
        return output;
    }
    std::string CallstackFoundNoValueWeak::formatMessage() const
    {
        auto output = m_location.format();
        auto callstack_name = m_callstack_name;

        output.reserve(
            output.length()
            + "Callstack "sv.length()
            + callstack_name.length()
            + " found no value."sv.length()
        );

        output.append("Callstack "sv);
        output.append(callstack_name);
        output.append(" found no value."sv);
        return output;
    }
    std::string GroupNotEmpty::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "Group '"sv.length()
            + m_group_name.length()
            + "' is not empty."sv.length()
        );

        output.append("Group '"sv);
        output.append(m_group_name);
        output.append(" is not empty."sv);
        return output;
    }
    std::string ForStepVariableTypeMissmatch::formatMessage() const
    {
        auto output = m_location.format();
        auto expected = m_expected.to_string();
        auto got = m_got.to_string();

        output.reserve(
            output.length()
            + "ForStep variable '"sv.length()
            + m_variable_name.length()
            + "' was expected to be of type "sv.length()
            + expected.length()
            + " but got "sv.length()
            + got.length()
            + "."sv.length()
        );

        output.append("ForStep variable '"sv);
        output.append(m_variable_name);
        output.append("' was expected to be of type "sv);
        output.append(expected);
        output.append(" but got "sv);
        output.append(got);
        output.append("."sv);
        return output;
    }
    std::string ForStepNoWorkShouldBeDone::formatMessage() const
    {
        auto output = m_location.format();
        auto step = to_string(m_step);
        auto from = to_string(m_from);
        auto to = to_string(m_to);

        output.reserve(
            output.length()
            + "ForStep with stepsize "sv.length()
            + step.length()
            + " will not do anything as the range is from "sv.length()
            + from.length()
            + " to "sv.length()
            + to.length()
            + "."sv.length()
        );

        output.append("ForStep with stepsize "sv);
        output.append(step);
        output.append(" will not do anything as the range is from "sv);
        output.append(from);
        output.append(" to "sv);
        output.append(to);
        output.append("."sv);
        return output;
    }
    std::string ReconstructionOfCodeFailed::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Reconstructing code from assembly failed."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string WaitUntilMaxLoopReached::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Max-Loop reached for WaitUntil."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ArraySizeChanged::formatMessage() const
    {
        auto output = m_location.format();
        auto before = to_string(m_before);
        auto after = to_string(m_after);

        output.reserve(
            output.length()
            + "Array size changed from "sv.length()
            + before.length()
            + " to "sv.length()
            + after.length()
            + "."sv.length()
        );

        output.append("Array size changed from "sv);
        output.append(before);
        output.append(" to "sv);
        output.append(after);
        output.append("."sv);
        return output;
    }
    std::string MagicVariableNotPresent::formatMessage() const
    {
        auto output = m_location.format();
        auto variable_name = m_variable_name;

        output.reserve(
            output.length()
            + "The Magic variable '"sv.length()
            + variable_name.length()
            + "' is not present."sv.length()
        );

        output.append("The Magic variable '"sv);
        output.append(variable_name);
        output.append("' is not present."sv);
        return output;
    }
    std::string GroupLeaderNotPartOfGroup::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Leader set is not part of group."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string AssigningNilValue::formatMessage() const
    {
        auto output = m_location.format();
        auto variable_name = m_variable_name;

        output.reserve(
            output.length()
            + "Assigning nil to variable '"sv.length()
            + variable_name.length()
            + "'."sv.length()
        );

        output.append("Assigning nil to variable '"sv);
        output.append(variable_name);
        output.append("'."sv);
        return output;
    }
    std::string ConditionEmpty::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Condition provided is empty."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NilValueFoundForRightArgumentWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Nil value provided for right-handed argument."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string NilValueFoundForLeftArgumentWeak::formatMessage() const
    {
        auto output = m_location.format();
        const auto message = "Nil value provided for left-handed argument."sv;

        output.reserve(
            output.length()
            + message.length()
        );

        output.append(message);
        return output;
    }
    std::string ContextValuePrint::formatMessage() const
    {
        const auto messageA = "Context droped with return value `"sv;
        const auto messageB = "`."sv;
        auto value_sqf = value.to_string_sqf();

        std::string output;
        output.reserve(
            messageA.length() +
            value_sqf.length() +
            messageB.length()
        );

        output.append(messageA);
        output.append(value_sqf);
        output.append(messageB);
        return output;
    }
    std::string ScopeNameNotFound::formatMessage() const
    {
        const auto messageA = "Scopename `"sv;
        const auto messageB = "` was not found."sv;

        std::string output;
        output.reserve(
            messageA.length() +
            m_scope_name.length() +
            messageB.length()
        );

        output.append(messageA);
        output.append(m_scope_name);
        output.append(messageB);
        return output;
    }
    std::string WarningMessage::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "["sv.length()
            + m_source.length()
            + "] "sv.length()
            + m_message.length()
        );

        output.append("["sv);
        output.append(m_source);
        output.append("] "sv);
        output.append(m_message);
        return output;
    }
    std::string InvalidAssemblyInstruction::formatMessage() const
    {
        auto output = m_location.format();

        output.reserve(
            output.length()
            + "Invalid assembly instruction { "sv.length()
            + m_assembly.length()
            + " }"sv.length()
        );

        output.append("Invalid assembly instruction { "sv);
        output.append(m_assembly);
        output.append(" }"sv);
        return output;
    }
}

std::string logmessage::fileio::ResolveVirtualRequested::formatMessage() const
{
    const auto messageA = "Virtual Lookup of `"sv;
    const auto messageB = "` got requested (Physical Reference Path: `"sv;
    const auto messageC = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_virtual.length() +
        messageB.length() +
        m_physical.length() +
        messageC.length()
    );

    output.append(messageA);
    output.append(m_virtual);
    output.append(messageB);
    output.append(m_physical);
    output.append(messageC);
    return output;
}

std::string logmessage::fileio::ResolveVirtualFileNotFound::formatMessage() const
{
    const auto messageA = "Virtual Lookup could not locate a file for `"sv;
    const auto messageB = "` (Physical Reference Path: `"sv;
    const auto messageC = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_virtual.length() +
        messageB.length() +
        m_physical.length() +
        messageC.length()
    );

    output.append(messageA);
    output.append(m_virtual);
    output.append(messageB);
    output.append(m_physical);
    output.append(messageC);
    return output;
}

std::string logmessage::fileio::ResolveVirtualFileMatched::formatMessage() const
{
    const auto messageA = "Virtual Lookup succeeded for `"sv;
    const auto messageB = "` with `"sv;
    const auto messageC = "` (Physical Reference Path : `"sv;
    const auto messageD = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_virtual.length() +
        messageB.length() +
        m_match_physical.length() +
        messageC.length() +
        m_physical.length() +
        messageD.length()
    );

    output.append(messageA);
    output.append(m_virtual);
    output.append(messageB);
    output.append(m_match_physical);
    output.append(messageC);
    output.append(m_physical);
    output.append(messageD);
    return output;
}

std::string logmessage::fileio::ResolveVirtualNavigateUp::formatMessage() const
{
    const auto messageA = "Virtual Lookup encountered `..`. Navigating up (Virtual Reference path: `"sv;
    const auto messageB = "`, Physical Reference Path: `"sv;
    const auto messageC = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_virtual.length() +
        messageB.length() +
        m_physical.length() +
        messageC.length()
    );

    output.append(messageA);
    output.append(m_virtual);
    output.append(messageB);
    output.append(m_physical);
    output.append(messageC);
    return output;
}
std::string logmessage::fileio::ResolveVirtualNavigateDown::formatMessage() const
{
    const auto messageA = "Virtual Lookup matched down-navigation path `"sv;
    const auto messageB = "` (Virtual Reference path: `"sv;
    const auto messageC = "`, Physical Reference Path : `"sv;
    const auto messageD = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_matched.length() +
        messageB.length() +
        m_virtual.length() +
        messageC.length() +
        m_physical.length() +
        messageD.length()
    );

    output.append(messageA);
    output.append(m_matched);
    output.append(messageB);
    output.append(m_virtual);
    output.append(messageC);
    output.append(m_physical);
    output.append(messageD);
    return output;
}

std::string logmessage::fileio::ResolveVirtualNavigateDeadEnd::formatMessage() const
{
    const auto messageA = "Virtual Lookup failed to locate down-navigation path `"sv;
    const auto messageB = "` (Virtual Reference path: `"sv;
    const auto messageC = "`, Physical Reference Path : `"sv;
    const auto messageD = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_matched.length() +
        messageB.length() +
        m_virtual.length() +
        messageC.length() +
        m_physical.length() +
        messageD.length()
    );

    output.append(messageA);
    output.append(m_matched);
    output.append(messageB);
    output.append(m_virtual);
    output.append(messageC);
    output.append(m_physical);
    output.append(messageD);
    return output;
}

std::string logmessage::fileio::ResolveVirtualNavigateNoNodesLeftForExploring::formatMessage() const
{
    const auto messageA = "Virtual Lookup got no more nodes left for exploring (Virtual Reference path: `"sv;
    const auto messageB = "`, Physical Reference Path: `"sv;
    const auto messageC = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_virtual.length() +
        messageB.length() +
        m_physical.length() +
        messageC.length()
    );

    output.append(messageA);
    output.append(m_virtual);
    output.append(messageB);
    output.append(m_physical);
    output.append(messageC);
    return output;
}

std::string logmessage::fileio::ResolveVirtualTestFileExists::formatMessage() const
{
    const auto messageA = "Virtual Lookup testing if file `"sv;
    const auto messageB = "` exists (Virtual Reference path: `"sv;
    const auto messageC = "`, Physical Reference Path : `"sv;
    const auto messageD = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_matched.length() +
        messageB.length() +
        m_virtual.length() +
        messageC.length() +
        m_physical.length() +
        messageD.length()
    );

    output.append(messageA);
    output.append(m_matched);
    output.append(messageB);
    output.append(m_virtual);
    output.append(messageC);
    output.append(m_physical);
    output.append(messageD);
    return output;
}

std::string logmessage::fileio::ResolveVirtualGotRemainder::formatMessage() const
{
    const auto messageA = "Virtual Lookup reduced path to `"sv;
    const auto messageB = "` (Physical Reference Path : `"sv;
    const auto messageC = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_virtual.length() +
        messageB.length() +
        m_physical.length() +
        messageC.length()
    );

    output.append(messageA);
    output.append(m_virtual);
    output.append(messageB);
    output.append(m_physical);
    output.append(messageC);
    return output;
}

std::string logmessage::fileio::ResolvePhysicalRequested::formatMessage() const
{
    const auto messageA = "Physical Lookup requested for `"sv;
    const auto messageB = "` (Virtual Reference path: `"sv;
    const auto messageC = "`, Physical Reference Path : `"sv;
    const auto messageD = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_matched.length() +
        messageB.length() +
        m_virtual.length() +
        messageC.length() +
        m_physical.length() +
        messageD.length()
    );

    output.append(messageA);
    output.append(m_matched);
    output.append(messageB);
    output.append(m_virtual);
    output.append(messageC);
    output.append(m_physical);
    output.append(messageD);
    return output;
}

std::string logmessage::fileio::ResolvePhysicalAdjustedPath::formatMessage() const
{
    const auto messageA = "Physical Lookup adjusted lookup path to `"sv;
    const auto messageB = "` (Virtual Reference path: `"sv;
    const auto messageC = "`, Physical Reference Path : `"sv;
    const auto messageD = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_matched.length() +
        messageB.length() +
        m_virtual.length() +
        messageC.length() +
        m_physical.length() +
        messageD.length()
    );

    output.append(messageA);
    output.append(m_matched);
    output.append(messageB);
    output.append(m_virtual);
    output.append(messageC);
    output.append(m_physical);
    output.append(messageD);
    return output;
}

std::string logmessage::fileio::ResolvePhysicalTestingAgainst::formatMessage() const
{
    const auto messageA = "Physical Lookup testing against `"sv;
    const auto messageB = "` (Virtual Reference path: `"sv;
    const auto messageC = "`, Physical Reference Path : `"sv;
    const auto messageD = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_matched.length() +
        messageB.length() +
        m_virtual.length() +
        messageC.length() +
        m_physical.length() +
        messageD.length()
    );

    output.append(messageA);
    output.append(m_matched);
    output.append(messageB);
    output.append(m_virtual);
    output.append(messageC);
    output.append(m_physical);
    output.append(messageD);
    return output;
}

std::string logmessage::fileio::ResolvePhysicalMatched::formatMessage() const
{
    const auto messageA = "Physical Lookup matched file `"sv;
    const auto messageB = "` (Virtual Reference path: `"sv;
    const auto messageC = "`, Physical Reference Path : `"sv;
    const auto messageD = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_matched.length() +
        messageB.length() +
        m_virtual.length() +
        messageC.length() +
        m_physical.length() +
        messageD.length()
    );

    output.append(messageA);
    output.append(m_matched);
    output.append(messageB);
    output.append(m_virtual);
    output.append(messageC);
    output.append(m_physical);
    output.append(messageD);
    return output;
}

std::string logmessage::fileio::ResolvePhysicalFailedToLookup::formatMessage() const
{
    const auto messageA = "Physical Lookup failed to match file `"sv;
    const auto messageB = "` (Virtual Reference path: `"sv;
    const auto messageC = "`, Physical Reference Path : `"sv;
    const auto messageD = "`)."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_matched.length() +
        messageB.length() +
        m_virtual.length() +
        messageC.length() +
        m_physical.length() +
        messageD.length()
    );

    output.append(messageA);
    output.append(m_matched);
    output.append(messageB);
    output.append(m_virtual);
    output.append(messageC);
    output.append(m_physical);
    output.append(messageD);
    return output;
}

std::string logmessage::fileio::PBOFileAlreadyRegistered::formatMessage() const
{
    const auto messageA = "File `"sv;
    const auto messageB = "` of PBO `"sv;
    const auto messageC = "` already is registred."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_file.length() +
        messageB.length() +
        m_path.length() +
        messageC.length()
    );

    output.append(messageA);
    output.append(m_file);
    output.append(messageB);
    output.append(m_path);
    output.append(messageC);
    return output;
}

std::string logmessage::fileio::PBOHasNoPrefixAttribute::formatMessage() const
{
    const auto messageA = "PBO `"sv;
    const auto messageB = "` has no `prefix` attribute."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_path.length() +
        messageB.length()
    );

    output.append(messageA);
    output.append(m_path);
    output.append(messageB);
    return output;
}

std::string logmessage::fileio::FailedToParsePBO::formatMessage() const
{
    const auto messageA = "Failed to parse PBO `"sv;
    const auto messageB = "`."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_path.length() +
        messageB.length()
    );

    output.append(messageA);
    output.append(m_path);
    output.append(messageB);
    return output;
}

std::string logmessage::fileio::PBOAlreadyAdded::formatMessage() const
{
    const auto messageA = "The PBO `"sv;
    const auto messageB = "` was added already."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_path.length() +
        messageB.length()
    );

    output.append(messageA);
    output.append(m_path);
    output.append(messageB);
    return output;
}

std::string logmessage::fileio::PBOFileNotFound::formatMessage() const
{
    const auto messageA = "PBO `"sv;
    const auto messageB = "` is supposed to be loaded but is not."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_path.length() +
        messageB.length()
    );

    output.append(messageA);
    output.append(m_path);
    output.append(messageB);
    return output;
}

std::string logmessage::fileio::PBOFailedToReadFile::formatMessage() const
{
    const auto messageA = "Failed to read `"sv;
    const auto messageB = "` from PBO `"sv;
    const auto messageC = "`."sv;

    std::string output;
    output.reserve(
        messageA.length() +
        m_file.length() +
        messageB.length() +
        m_path.length() +
        messageC.length()
    );

    output.append(messageA);
    output.append(m_file);
    output.append(messageB);
    output.append(m_path);
    output.append(messageC);
    return output;
}
