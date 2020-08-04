#pragma once
#include "../../runtime/parser/sqf.h"
#include "../../runtime/logging.h"
#include "../../runtime/diagnostics/diag_info.h"
#include "../../runtime/fileio.h"
#include "../../runtime/util.h"

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
			::sqf::runtime::fileio::pathinfo path;
			nodetype kind;
			std::vector<astnode> children;

			astnode() : offset(0), length(0), line(0), col(0), kind(nodetype::NA) {}
		};
	private:

		class instance
		{
		private:
			::sqf::runtime::runtime& m_runtime;
			default& m_owner;
			::sqf::parser::util::string_ref m_contents;
			::sqf::runtime::fileio::pathinfo m_file;
			::sqf::runtime::diagnostics::diag_info m_info;

		public:
			instance(::sqf::runtime::runtime& runtime, default& owner, std::string& contents, ::sqf::runtime::fileio::pathinfo file) :
				m_runtime(runtime), m_owner(owner), m_contents(contents), m_file(file)
			{
				::sqf::runtime::diagnostics::diag_info dinf(1, 0, 0, m_file, {});
				m_info = dinf;
			}
			bool m_contains_nular(std::string_view view);
			bool m_contains_unary(std::string_view view);
			//precedence 0 is being used as placeholder for `any`
			bool m_contains_binary(std::string_view view, short precedence);
			short m_precedence(std::string_view view);

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
		};
	public:
		default(
			Logger& logger
		) : CanLog(logger)
		{
		}
		virtual ~default() override { };
		virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override
		{
			bool errflag = false;
			instance i(runtime, *this, contents, file);
			i.parse(errflag);
			return !errflag;
		}
		virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override
		{
			bool errflag = false;
			instance i(runtime, *this, contents, file);
			auto root = i.parse(errflag);
			if (errflag)
			{
				return {};
			}
			std::vector<::sqf::runtime::instruction::sptr> source;
			if (!i.to_assembly(root, source))
			{
				return {};
			}
			return source;
		}
	};
}