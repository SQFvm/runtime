#pragma once
#include "astnode.h"
#include "logging.h"
#include "position_info.h"


namespace sqf::parse
{
	namespace asttype
	{
		enum class sqf
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
	}
	class sqf : public CanLog
	{
	private:
		position_info m_info;
		const char* m_contents;
		std::string m_contents_actual;
		std::string m_file;
		bool(*m_contains_nular)(std::string_view);
		bool(*m_contains_unary)(std::string_view);
		//precedence 0 is being used as placeholder for `any`
		bool(*m_contains_binary)(std::string_view, short);
		short(*m_precedence)(std::string_view);

		void skip(size_t& curoff);
		void skip(position_info& info);

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
		void bexp_orderfix(::sqf::parse::astnode& root, ::sqf::parse::astnode thisnode, short plevel);
		bool bexp10_start(size_t curoff);
		void bexp10(::sqf::parse::astnode& root, bool& errflag);
		bool bexp9_start(size_t curoff);
		void bexp9(::sqf::parse::astnode& root, bool& errflag);
		bool bexp8_start(size_t curoff);
		void bexp8(::sqf::parse::astnode& root, bool& errflag);
		bool bexp7_start(size_t curoff);
		void bexp7(::sqf::parse::astnode& root, bool& errflag);
		bool bexp6_start(size_t curoff);
		void bexp6(::sqf::parse::astnode& root, bool& errflag);
		bool bexp5_start(size_t curoff);
		void bexp5(::sqf::parse::astnode& root, bool& errflag);
		bool bexp4_start(size_t curoff);
		void bexp4(::sqf::parse::astnode& root, bool& errflag);
		bool bexp3_start(size_t curoff);
		void bexp3(::sqf::parse::astnode& root, bool& errflag);
		bool bexp2_start(size_t curoff);
		void bexp2(::sqf::parse::astnode& root, bool& errflag);
		bool bexp1_start(size_t curoff);
		void bexp1(::sqf::parse::astnode& root, bool& errflag);
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

	public:

		sqf(
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
			position_info position_info = { 1, 0, 0, m_file };
			m_info = position_info;
		}

		astnode parse(bool& errflag);
		static const char* astkindname(short id)
		{
			switch (id)
			{
			case (short)asttype::sqf::SQF: return "SQF";
			case (short)asttype::sqf::STATEMENT: return "STATEMENT";
			case (short)asttype::sqf::ASSIGNMENT: return "ASSIGNMENT";
			case (short)asttype::sqf::ASSIGNMENTLOCAL: return "ASSIGNMENTLOCA";
			case (short)asttype::sqf::BEXP1: return "BEXP1";
			case (short)asttype::sqf::BEXP2: return "BEXP2";
			case (short)asttype::sqf::BEXP3: return "BEXP3";
			case (short)asttype::sqf::BEXP4: return "BEXP4";
			case (short)asttype::sqf::BEXP5: return "BEXP5";
			case (short)asttype::sqf::BEXP6: return "BEXP6";
			case (short)asttype::sqf::BEXP7: return "BEXP7";
			case (short)asttype::sqf::BEXP8: return "BEXP8";
			case (short)asttype::sqf::BEXP9: return "BEXP9";
			case (short)asttype::sqf::BEXP10: return "BEXP10";
			case (short)asttype::sqf::BINARYEXPRESSION: return "BINARYEXPRESSION";
			case (short)asttype::sqf::BINARYOP: return "BINARYOP";
			case (short)asttype::sqf::BRACKETS: return "BRACKETS";
			case (short)asttype::sqf::UNARYOP: return "UNARYOP";
			case (short)asttype::sqf::PRIMARYEXPRESSION: return "PRIMARYEXPRESSION";
			case (short)asttype::sqf::NULAROP: return "NULAROP";
			case (short)asttype::sqf::UNARYEXPRESSION: return "UNARYEXPRESSION";
			case (short)asttype::sqf::HEXNUMBER: return "HEXNUMBER";
			case (short)asttype::sqf::NUMBER: return "NUMBER";
			case (short)asttype::sqf::VARIABLE: return "VARIABLE";
			case (short)asttype::sqf::STRING: return "STRING";
			case (short)asttype::sqf::CODE: return "m_contents";
			case (short)asttype::sqf::ARRAY: return "ARRAY";
			default: return "NA";
			}
		}
	};
}
