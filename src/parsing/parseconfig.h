#pragma once
#include <string>
#include "astnode.h"
#include "logging.h"
#include "position_info.h"



namespace sqf
{
	namespace parse
	{
		namespace asttype
		{
			enum class config
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
		class config : public CanLog
		{
			position_info m_info;
			std::string_view m_contents;
			std::string m_file;

			void skip();

			size_t endchr(size_t off);
			size_t identifier(size_t off);
			size_t operator_(size_t off);
			size_t hexadecimal(size_t off);
			size_t numsub(size_t off);
			size_t num(size_t off);
			size_t anytext(size_t off);
			bool NODELIST_start(size_t curoff);
			void NODELIST(astnode& root, bool& errflag);
			bool NODE_start(size_t curoff);
			void NODE(astnode& root, bool& errflag);
			bool CONFIGNODE_start(size_t curoff);
			void CONFIGNODE(astnode& root, bool& errflag);
			bool VALUENODE_start(size_t curoff);
			void VALUENODE(astnode& root, bool& errflag);
			bool STRING_start(size_t curoff);
			void STRING(astnode& root, bool& errflag);
			bool NUMBER_start(size_t curoff);
			void NUMBER(astnode& root, bool& errflag);
			bool LOCALIZATION_start(size_t curoff);
			void LOCALIZATION(astnode& root, bool& errflag);
			bool ARRAY_start(size_t curoff);
			void ARRAY(astnode& root, bool& errflag);
			bool VALUE_start(size_t curoff);
			void VALUE(astnode& root, bool& errflag);


		public:
			config(
				Logger& logger,
				std::string_view contents,
				std::string_view file
			) : CanLog(logger),
				m_contents(contents),
				m_file(file)
			{
				position_info position_info = { 1, 0, 0, m_file };
				m_info = position_info;
			}

			astnode parse(bool& errflag);
			static const char* astkindname(short id)
			{
				switch (id)
				{
				case (short)asttype::config::NODELIST: return "NODELIST";
				case (short)asttype::config::NODE: return "NODE";
				case (short)asttype::config::CONFIGNODE: return "CONFIGNODE";
				case (short)asttype::config::CONFIGNODE_PARENTIDENT: return "CONFIGNODE_PARENTIDENT";
				case (short)asttype::config::VALUENODE: return "VALUENODE";
				case (short)asttype::config::STRING: return "STRING";
				case (short)asttype::config::NUMBER: return "NUMBER";
				case (short)asttype::config::HEXNUMBER: return "HEXNUMBER";
				case (short)asttype::config::LOCALIZATION: return "LOCALIZATION";
				case (short)asttype::config::ARRAY: return "ARRAY";
				case (short)asttype::config::VALUE: return "VALUE";
				default: return "NA";
				}
			}
		};
	}
}