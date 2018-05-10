#pragma once
#include "astnode.h"
#include "helper.h"


namespace sqf
{
	namespace parse
	{
		namespace sqf
		{
			namespace sqfasttypes
			{
				enum sqfasttypes
				{
					NA = 0,
					SQF,
					STATEMENT,
					ASSIGNMENT,
					ASSIGNMENTLOCAL,
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
					ARRAY,
					BINARYSTATEMENT
				};
			}
			size_t endchr(const char* code, size_t off);
			size_t identifier(const char* code, size_t off);
			size_t operator_(const char* code, size_t off);
			size_t hexadecimal(const char* code, size_t off);
			size_t scalarsub(const char* code, size_t off);
			size_t scalar(const char* code, size_t off);
			size_t anytext(const char* code, size_t off);
			bool SQF_start(helper &h, const char* code, size_t curoff);
			void SQF(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool STATEMENT_start(helper &h, const char* code, size_t curoff);
			void STATEMENT(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool ASSIGNMENT_start(helper &h, const char* code, size_t curoff);
			void ASSIGNMENT(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool BINARYEXPRESSION_start(helper &h, const char* code, size_t curoff);
			void BINARYEXPRESSION(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool BRACKETS_start(helper &h, const char* code, size_t curoff);
			void BRACKETS(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool PRIMARYEXPRESSION_start(helper &h, const char* code, size_t curoff);
			void PRIMARYEXPRESSION(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool NULAREXPRESSION_start(helper &h, const char* code, size_t curoff);
			void NULAREXPRESSION(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool UNARYEXPRESSION_start(helper &h, const char* code, size_t curoff);
			void UNARYEXPRESSION(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool NUMBER_start(helper &h, const char* code, size_t curoff);
			void NUMBER(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool VARIABLE_start(helper &h, const char* code, size_t curoff);
			void VARIABLE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool STRING_start(helper &h, const char* code, size_t curoff);
			void STRING(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool CODE_start(helper &h, const char* code, size_t curoff);
			void CODE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			bool ARRAY_start(helper &h, const char* code, size_t curoff);
			void ARRAY(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const char* file, bool &errflag);
			astnode parse_sqf(const char* codein, helper& h, bool &errflag);
			const char* astkindname(short id);
		}
	}
}