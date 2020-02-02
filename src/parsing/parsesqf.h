#pragma once
#include "astnode.h"
#include "logging.h"
#include "position_info.h"


namespace sqf
{
	namespace parse
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
			std::string_view m_contents;
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
			void SQF(astnode& root, position_info& info, bool& errflag);
			bool STATEMENT_start(size_t curoff);
			void STATEMENT(astnode& root, position_info& info, bool& errflag);
			bool ASSIGNMENT_start(size_t curoff);
			void ASSIGNMENT(astnode& root, position_info& info, bool& errflag);
			void bexp_orderfix(::sqf::parse::astnode& root, ::sqf::parse::astnode thisnode, short plevel);
			bool bexp10_start(size_t curoff);
			void bexp10(::sqf::parse::astnode& root, position_info& info, bool& errflag);
			bool bexp9_start(size_t curoff);
			void bexp9(::sqf::parse::astnode& root, position_info& info, bool& errflag);
			bool bexp8_start(size_t curoff);
			void bexp8(::sqf::parse::astnode& root, position_info& info, bool& errflag);
			bool bexp7_start(size_t curoff);
			void bexp7(::sqf::parse::astnode& root, position_info& info, bool& errflag);
			bool bexp6_start(size_t curoff);
			void bexp6(::sqf::parse::astnode& root, position_info& info, bool& errflag);
			bool bexp5_start(size_t curoff);
			void bexp5(::sqf::parse::astnode& root, position_info& info, bool& errflag);
			bool bexp4_start(size_t curoff);
			void bexp4(::sqf::parse::astnode& root, position_info& info, bool& errflag);
			bool bexp3_start(size_t curoff);
			void bexp3(::sqf::parse::astnode& root, position_info& info, bool& errflag);
			bool bexp2_start(size_t curoff);
			void bexp2(::sqf::parse::astnode& root, position_info& info, bool& errflag);
			bool bexp1_start(size_t curoff);
			void bexp1(::sqf::parse::astnode& root, position_info& info, bool& errflag);
			bool BINARYEXPRESSION_start(size_t curoff);
			void BINARYEXPRESSION(astnode& root, position_info& info, bool& errflag);
			bool BRACKETS_start(size_t curoff);
			void BRACKETS(astnode& root, position_info& info, bool& errflag);
			bool PRIMARYEXPRESSION_start(size_t curoff);
			void PRIMARYEXPRESSION(astnode& root, position_info& info, bool& errflag);
			bool NULAREXPRESSION_start(size_t curoff);
			void NULAREXPRESSION(astnode& root, position_info& info, bool& errflag);
			bool UNARYEXPRESSION_start(size_t curoff);
			void UNARYEXPRESSION(astnode& root, position_info& info, bool& errflag);
			bool NUMBER_start(size_t curoff);
			void NUMBER(astnode& root, position_info& info, bool& errflag);
			bool VARIABLE_start(size_t curoff);
			void VARIABLE(astnode& root, position_info& info, bool& errflag);
			bool STRING_start(size_t curoff);
			void STRING(astnode& root, position_info& info, bool& errflag);
			bool CODE_start(size_t curoff);
			void CODE(astnode& root, position_info& info, bool& errflag);
			bool ARRAY_start(size_t curoff);
			void ARRAY(astnode& root, position_info& info, bool& errflag);

		public:

			sqf(
				Logger logger,
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
				m_contents(contents),
				m_file(file) {}

			astnode parse_sqf(bool& errflag);
			const char* astkindname(short id);
		};
	}
}