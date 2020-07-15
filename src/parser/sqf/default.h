#pragma once
#include "../../runtime/parser/sqf.h"
#include "../../runtime/logging.h"
#include "../../runtime/diagnostics/diag_info.h"

#include <string>
#include <string_view>
#include <vector>

namespace sqf::parser::sqf
{
	class default : public ::sqf::runtime::parser::sqf, public CanLog
	{
	public:
		enum class nodetype
		{
			NA = 0,
			SQF,
			STATEMENT,
			ASSIGNMENT,
			ASSIGNMENTLOCAL,
			BEXP1,
			BEXP2,
			BEXP3,
			BEXP4,
			BEXP5,
			BEXP6,
			BEXP7,
			BEXP8,
			BEXP9,
			BEXP10,
			BINARYEXPRESSION,
			BINARYOP,
			BRACKETS,
			PRIMARYEXPRESSION,
			NULAROP,
			UNARYEXPRESSION,
			UNARYOP,
			NUMBER,
			HEXNUMBER,
			VARIABLE,
			STRING,
			CODE,
			ARRAY
		};
		struct astnode
		{
			size_t offset;
			size_t length;
			size_t line;
			size_t col;
			std::string content;
			std::string file;
			nodetype kind;
			std::vector<astnode> children;

			astnode() : offset(0), length(0), line(0), col(0), kind(nodetype::NA) {}
		};
	private:
		::sqf::runtime::diagnostics::diag_info m_info;
		const char* m_contents;
		std::string m_contents_actual;
		std::string m_file;
		bool(*m_contains_nular)(std::string_view);
		bool(*m_contains_unary)(std::string_view);
		//precedence 0 is being used as placeholder for `any`
		bool(*m_contains_binary)(std::string_view, short);
		short(*m_precedence)(std::string_view);

		void skip(size_t& curoff);
		void skip(::sqf::runtime::diagnostics::diag_info& info);

		size_t endchr(size_t off);
		size_t assidentifier(size_t off);
		size_t identifier(size_t off);
		size_t operator_(size_t off);
		size_t hexadecimal(size_t off);
		size_t scalarsub(size_t off);
		size_t scalar(size_t off);
		size_t anytext(size_t off);
		bool SQF_start(size_t curoff);
		void SQF(astnode& root, bool& errflag);
		bool STATEMENT_start(size_t curoff);
		void STATEMENT(astnode& root, bool& errflag);
		bool ASSIGNMENT_start(size_t curoff);
		void ASSIGNMENT(astnode& root, bool& errflag);
		void bexp_orderfix(astnode& root, astnode thisnode, nodetype plevel);
		bool bexp10_start(size_t curoff);
		void bexp10(astnode& root, bool& errflag);
		bool bexp9_start(size_t curoff);
		void bexp9(astnode& root, bool& errflag);
		bool bexp8_start(size_t curoff);
		void bexp8(astnode& root, bool& errflag);
		bool bexp7_start(size_t curoff);
		void bexp7(astnode& root, bool& errflag);
		bool bexp6_start(size_t curoff);
		void bexp6(astnode& root, bool& errflag);
		bool bexp5_start(size_t curoff);
		void bexp5(astnode& root, bool& errflag);
		bool bexp4_start(size_t curoff);
		void bexp4(astnode& root, bool& errflag);
		bool bexp3_start(size_t curoff);
		void bexp3(astnode& root, bool& errflag);
		bool bexp2_start(size_t curoff);
		void bexp2(astnode& root, bool& errflag);
		bool bexp1_start(size_t curoff);
		void bexp1(astnode& root, bool& errflag);
		bool BINARYEXPRESSION_start(size_t curoff);
		void BINARYEXPRESSION(astnode& root, bool& errflag);
		bool BRACKETS_start(size_t curoff);
		void BRACKETS(astnode& root, bool& errflag);
		bool PRIMARYEXPRESSION_start(size_t curoff);
		void PRIMARYEXPRESSION(astnode& root, bool& errflag);
		bool NULAREXPRESSION_start(size_t curoff);
		void NULAREXPRESSION(astnode& root, bool& errflag);
		bool UNARYEXPRESSION_start(size_t curoff);
		void UNARYEXPRESSION(astnode& root, bool& errflag);
		bool NUMBER_start(size_t curoff);
		void NUMBER(astnode& root, bool& errflag);
		bool VARIABLE_start(size_t curoff);
		void VARIABLE(astnode& root, bool& errflag);
		bool STRING_start(size_t curoff);
		void STRING(astnode& root, bool& errflag);
		bool CODE_start(size_t curoff);
		void CODE(astnode& root, bool& errflag);
		bool ARRAY_start(size_t curoff);
		void ARRAY(astnode& root, bool& errflag);
		astnode parse(bool& errflag);
		bool to_assembly(astnode& root, std::vector<::sqf::runtime::instruction::sptr>& set);
	public:
		default(
			Logger& logger,
			bool(*contains_nular)(std::string_view),
			bool(*contains_unary)(std::string_view),
			bool(*contains_binary)(std::string_view, short),
			short(*precedence)(std::string_view),
			std::string_view contents,
			std::string_view file
		) : CanLog(logger),
			m_contains_nular(contains_nular),
			m_contains_unary(contains_unary),
			m_contains_binary(contains_binary),
			m_precedence(precedence),
			m_contents_actual(contents),
			m_file(file)
		{
			m_contents = m_contents_actual.c_str();
			::sqf::runtime::diagnostics::diag_info dinf(1, 0, 0, m_file, "");
			m_info = dinf;
		}
		virtual ~default() override { };
		virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents) override
		{
			bool success = false;
			parse(success);
			return success;
		}
		virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents)
		{
			bool success = false;
			parse(success);
			if (!success)
			{
				return {};
			}
		}
	};
}