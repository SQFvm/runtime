#ifndef _PARSECONFIG
#define _PARSECONFIG 1

#if !defined(_ASTNODE)
#error parsesqf requires "astnode.h" header
#endif // !_ASTNODE
#if !defined(_HELPER)
#error parsesqf requires "helper.h" header
#endif // !_ASTNODE


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
			void skip(const wchar_t *code, size_t &curoff);
			void skip(const wchar_t *code, size_t &line, size_t &col, size_t &curoff);

			size_t endchr(const wchar_t* code, size_t off);
			size_t identifier(const wchar_t* code, size_t off);
			size_t operator_(const wchar_t* code, size_t off);
			size_t hexadecimal(const wchar_t* code, size_t off);
			size_t numsub(const wchar_t* code, size_t off);
			size_t num(const wchar_t* code, size_t off);
			size_t anytext(const wchar_t* code, size_t off);
			bool NODELIST_start(const wchar_t* code, size_t curoff);
			void NODELIST(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
			bool NODE_start(const wchar_t* code, size_t curoff);
			void NODE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
			bool CONFIGNODE_start(const wchar_t* code, size_t curoff);
			void CONFIGNODE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
			bool VALUENODE_start(const wchar_t* code, size_t curoff);
			void VALUENODE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
			bool STRING_start(const wchar_t* code, size_t curoff);
			void STRING(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
			bool NUMBER_start(const wchar_t* code, size_t curoff);
			void NUMBER(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
			bool LOCALIZATION_start(const wchar_t* code, size_t curoff);
			void LOCALIZATION(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
			bool ARRAY_start(const wchar_t* code, size_t curoff);
			void ARRAY(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);
			bool VALUE_start(const wchar_t* code, size_t curoff);
			void VALUE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag);


			astnode parse_config(const wchar_t* codein, helper& h, bool &errflag);
			const wchar_t* astkindname(short id);
		}
	}
}

#endif // !_PARSECONFIG
