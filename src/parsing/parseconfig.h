#pragma once
#include <string>
#include "astnode.h"
#include "helper.h"



namespace sqf
{
	namespace parse
	{
		namespace config
		{
			namespace configasttypes
			{
				enum configasttypes
				{
					NA = 0,
					NODELIST,
					NODE,
					CONFIGNODE,
					CONFIGNODE_PARENTIDENT,
					VALUENODE,
					STRING,
					NUMBER,
					HEXNUMBER,
					LOCALIZATION,
					ARRAY,
					VALUE
				};
			}
			void skip(const char *code, size_t &curoff);
			void skip(const char *code, size_t &line, size_t &col, std::string& file, size_t &curoff);

			size_t endchr(const char* code, size_t off);
			size_t identifier(const char* code, size_t off);
			size_t operator_(const char* code, size_t off);
			size_t hexadecimal(const char* code, size_t off);
			size_t numsub(const char* code, size_t off);
			size_t num(const char* code, size_t off);
			size_t anytext(const char* code, size_t off);
			bool NODELIST_start(const char* code, size_t curoff);
			void NODELIST(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag);
			bool NODE_start(const char* code, size_t curoff);
			void NODE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const std::string& file, bool &errflag);
			bool CONFIGNODE_start(const char* code, size_t curoff);
			void CONFIGNODE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag);
			bool VALUENODE_start(const char* code, size_t curoff);
			void VALUENODE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag);
			bool STRING_start(const char* code, size_t curoff);
			void STRING(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag);
			bool NUMBER_start(const char* code, size_t curoff);
			void NUMBER(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag);
			bool LOCALIZATION_start(const char* code, size_t curoff);
			void LOCALIZATION(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag);
			bool ARRAY_start(const char* code, size_t curoff);
			void ARRAY(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag);
			bool VALUE_start(const char* code, size_t curoff);
			void VALUE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, const std::string& file, bool &errflag);


			astnode parse_config(std::string_view codein, helper& h, bool &errflag);
			const char* astkindname(short id);
		}
	}
}