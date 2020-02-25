#pragma once
#include <string>
#include <string_view>
#include <vector>
#include "macro.h"
#include "position_info.h"

#include <logging.h>

namespace sqf
{
	class virtualmachine;
	namespace parse
	{
		class assembly : public CanLog
		{
			/*
			endStatement
			push <type> <value>
			callUnary <command>
			callBinary <command>
			assignTo <name>
			assignToLocal <name>
			callNular <name>
			getVariable <name>
			makeArray <size>
			*/


			//ident = [a-zA-Z]+;
			//command = ?![-+*/%a-zA-Z|&_><=]+;
			//anytext = (?![ \t\r\n;])+;
			//type = [a-zA-Z]+;
			//integer = [0-9]+;
			//semicolon = ';';
			//ASSEMBLY = { INSTRUCTIONS ';' { ';' } };
			//INSTRUCTIONS = ENDSTATEMENT | ARG | PUSH;
			//ARG = CALLUNARY | CALLBINARY | ASSIGNTO | ASSIGNTOLOCAL | CALLNULAR | GETVARIABLE | MAKEARRAY
			//ENDSTATEMENT = "endStatement"
			//CALLUNARY = "callUnary" command
			//CALLBINARY = "callBinary" command
			//ASSIGNTO = "assignTo" anytext
			//ASSIGNTOLOCAL = "assignToLoca" anytext
			//CALLNULAR = "callNular" command
			//GETVARIABLE = "getVariable" anytext
			//MAKEARRAY = "makeArray" integer
			//PUSH = "push" type { anytext }

		public:
		private:
			virtualmachine* m_vm;
			const char* m_contents;
			std::string m_contents_actual;
			std::string m_file;

			void skip(position_info& info);
			//ident = [a-zA-Z]+;
			//size_t ident(size_t);
			//command = [-+*/%a-zA-Z|&_><=\[\]]+;
			size_t command(size_t);
			//anytext = (?![ \t\r\n;])+;
			size_t anytext(size_t);
			//type = [a-zA-Z]+;
			size_t type(size_t);
			//integer = [0-9]+;
			size_t integer(size_t);
			//semicolon = ';';
			size_t semicolon(size_t);
			//ASSEMBLY = { INSTRUCTIONS ';' { ';' } };
			void assembly_root(position_info& info);
			//INSTRUCTIONS = ENDSTATEMENT | ARG | PUSH;
			bool instructions_start(size_t);
			void instructions(position_info& info);
			//ARG = CALLUNARY | CALLBINARY | ASSIGNTO | ASSIGNTOLOCAL | CALLNULAR | GETVARIABLE | MAKEARRAY
			bool arg_start(size_t);
			void arg(position_info& info);
			//ENDSTATEMENT = "endStatement"
			bool endstatement_start(size_t);
			void endstatement(position_info& info);
			//CALLUNARY = "callUnary" command
			bool callunary_start(size_t);
			void callunary(position_info& info);
			//CALLBINARY = "callBinary" command
			bool callbinary_start(size_t);
			void callbinary(position_info& info);
			//ASSIGNTO = "assignTo" anytext
			bool assignto_start(size_t);
			void assignto(position_info& info);
			//ASSIGNTOLOCAL = "assignToLoca" anytext
			bool assigntolocal_start(size_t);
			void assigntolocal(position_info& info);
			//CALLNULAR = "callNular" command
			bool callnular_start(size_t);
			void callnular(position_info& info);
			//GETVARIABLE = "getVariable" anytext
			bool getvariable_start(size_t);
			void getvariable(position_info& info);
			//MAKEARRAY = "makeArray" anytext
			bool makearray_start(size_t);
			void makearray(position_info& info);
			//PUSH = "push" type { anytext }
			bool push_start(size_t);
			void push(position_info& info);
		public:
			assembly(virtualmachine* vm, Logger& ref_logger, std::string_view contents, std::string file) :
				CanLog(ref_logger),
				m_vm(vm),
				m_contents_actual(contents),
				m_file(file) {
				m_contents = m_contents_actual.c_str();
			}
			void parse();
		};
	}
}