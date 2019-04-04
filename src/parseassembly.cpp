#include <cwctype>

#include "virtualmachine.h"
#include "compiletime.h"
#include "string_op.h"
#include "type.h"
#include "vmstack.h"
#include "instruction.h"
#include "instassignto.h"
#include "instassigntolocal.h"
#include "instcallbinary.h"
#include "instcallnular.h"
#include "instcallunary.h"
#include "instendstatement.h"
#include "instgetvariable.h"
#include "instmakearray.h"
#include "instpush.h"
#include "convert.h"
#include "commandmap.h"
#include "stringdata.h"
#include "value.h"

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

namespace
{
	sqf::type parsetype(std::string);
	void skip(const char*, size_t&, size_t&, size_t&);
	//ident = [a-zA-Z]+;
	//size_t ident(const char*, size_t);
	//command = [-+*/%a-zA-Z|&_><=\[\]]+;
	size_t command(const char*, size_t);
	//anytext = (?![ \t\r\n;])+;
	size_t anytext(const char*, size_t);
	//type = [a-zA-Z]+;
	size_t type(const char*, size_t);
	//integer = [0-9]+;
	//size_t integer(const char*, size_t);
	//semicolon = ';';
	size_t semicolon(const char*, size_t);
	//ASSEMBLY = { INSTRUCTIONS ';' { ';' } };
	void assembly(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//INSTRUCTIONS = ENDSTATEMENT | ARG | PUSH;
	bool instructions_start(const char*, size_t);
	void instructions(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//ARG = CALLUNARY | CALLBINARY | ASSIGNTO | ASSIGNTOLOCAL | CALLNULAR | GETVARIABLE | MAKEARRAY
	bool arg_start(const char*, size_t);
	void arg(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//ENDSTATEMENT = "endStatement"
	bool endstatement_start(const char*, size_t);
	void endstatement(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//CALLUNARY = "callUnary" command
	bool callunary_start(const char*, size_t);
	void callunary(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//CALLBINARY = "callBinary" command
	bool callbinary_start(const char*, size_t);
	void callbinary(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//ASSIGNTO = "assignTo" anytext
	bool assignto_start(const char*, size_t);
	void assignto(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//ASSIGNTOLOCAL = "assignToLoca" anytext
	bool assigntolocal_start(const char*, size_t);
	void assigntolocal(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//CALLNULAR = "callNular" command
	bool callnular_start(const char*, size_t);
	void callnular(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//GETVARIABLE = "getVariable" anytext
	bool getvariable_start(const char*, size_t);
	void getvariable(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//MAKEARRAY = "makeArray" anytext
	bool makearray_start(const char*, size_t);
	void makearray(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);
	//PUSH = "push" type { anytext }
	bool push_start(const char*, size_t);
	void push(sqf::virtualmachine*, const char*, size_t&, size_t&, size_t&);


	sqf::type parsetype(std::string str)
	{
		if (str_cmpi(str.c_str(), compiletime::strlen("NOTHING"), "NOTHING", compiletime::strlen("NOTHING")) == 0) { return sqf::type::NOTHING; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("ANY"), "ANY", compiletime::strlen("ANY")) == 0) { return sqf::type::ANY; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("SCALAR"), "SCALAR", compiletime::strlen("SCALAR")) == 0) { return sqf::type::SCALAR; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("BOOL"), "BOOL", compiletime::strlen("BOOL")) == 0) { return sqf::type::BOOL; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("ARRAY"), "ARRAY", compiletime::strlen("ARRAY")) == 0) { return sqf::type::ARRAY; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("STRING"), "STRING", compiletime::strlen("STRING")) == 0) { return sqf::type::STRING; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("NAMESPACE"), "NAMESPACE", compiletime::strlen("NAMESPACE")) == 0) { return sqf::type::NAMESPACE; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("NaN"), "NaN", compiletime::strlen("NaN")) == 0) { return sqf::type::NaN; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("IF"), "IF", compiletime::strlen("IF")) == 0) { return sqf::type::IF; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("WHILE"), "WHILE", compiletime::strlen("WHILE")) == 0) { return sqf::type::WHILE; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("FOR"), "FOR", compiletime::strlen("FOR")) == 0) { return sqf::type::FOR; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("SWITCH"), "SWITCH", compiletime::strlen("SWITCH")) == 0) { return sqf::type::SWITCH; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("EXCEPTION"), "EXCEPTION", compiletime::strlen("EXCEPTION")) == 0) { return sqf::type::EXCEPTION; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("WITH"), "WITH", compiletime::strlen("WITH")) == 0) { return sqf::type::WITH; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("CODE"), "CODE", compiletime::strlen("CODE")) == 0) { return sqf::type::CODE; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("OBJECT"), "OBJECT", compiletime::strlen("OBJECT")) == 0) { return sqf::type::OBJECT; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("VECTOR"), "VECTOR", compiletime::strlen("VECTOR")) == 0) { return sqf::type::VECTOR; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("TRANS"), "TRANS", compiletime::strlen("TRANS")) == 0) { return sqf::type::TRANS; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("ORIENT"), "ORIENT", compiletime::strlen("ORIENT")) == 0) { return sqf::type::ORIENT; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("SIDE"), "SIDE", compiletime::strlen("SIDE")) == 0) { return sqf::type::SIDE; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("GROUP"), "GROUP", compiletime::strlen("GROUP")) == 0) { return sqf::type::GROUP; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("TEXT"), "TEXT", compiletime::strlen("TEXT")) == 0) { return sqf::type::TEXT; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("SCRIPT"), "SCRIPT", compiletime::strlen("SCRIPT")) == 0) { return sqf::type::SCRIPT; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("TARGET"), "TARGET", compiletime::strlen("TARGET")) == 0) { return sqf::type::TARGET; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("JCLASS"), "JCLASS", compiletime::strlen("JCLASS")) == 0) { return sqf::type::JCLASS; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("CONFIG"), "CONFIG", compiletime::strlen("CONFIG")) == 0) { return sqf::type::CONFIG; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("DISPLAY"), "DISPLAY", compiletime::strlen("DISPLAY")) == 0) { return sqf::type::DISPLAY; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("CONTRO"), "CONTRO", compiletime::strlen("CONTRO")) == 0) { return sqf::type::CONTROL; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("NetObject"), "NetObject", compiletime::strlen("NetObject")) == 0) { return sqf::type::NetObject; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("SUBGROUP"), "SUBGROUP", compiletime::strlen("SUBGROUP")) == 0) { return sqf::type::SUBGROUP; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("TEAM_MEMBER"), "TEAM_MEMBER", compiletime::strlen("TEAM_MEMBER")) == 0) { return sqf::type::TEAM_MEMBER; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("TASK"), "TASK", compiletime::strlen("TASK")) == 0) { return sqf::type::TASK; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("DIARY_RECORD"), "DIARY_RECORD", compiletime::strlen("DIARY_RECORD")) == 0) { return sqf::type::DIARY_RECORD; }
		else if (str_cmpi(str.c_str(), compiletime::strlen("LOCATION"), "LOCATION", compiletime::strlen("LOCATION")) == 0) { return sqf::type::LOCATION; }
		else { return sqf::type::NA; }
	}
	void skip(const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		while (true)
		{
			switch (code[curoff])
			{
			case ' ': curoff++; col++; continue;
			case '\t': curoff++; col++; continue;
			case '\r': curoff++; col++; continue;
			case '\n': curoff++; line++; col = 0; continue;
			default: return;
			}
		}
	}
	//ident = [a-zA-Z]+;
	//size_t ident(const char *code, size_t off) { size_t i; for (i = off; (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z'); i++); return i - off; }
	//command = [-+*/%a-zA-Z|&_><=\[\]]+;
	size_t command(const char *code, size_t off) { size_t i; for (i = off; (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z') || code[i] == '-' || code[i] == '+' || code[i] == '*' || code[i] == '/' || code[i] == '%' || code[i] == '|' || code[i] == '&' || code[i] == '_' || code[i] == '>' || code[i] == '<' || code[i] == '=' || code[i] == '[' || code[i] == ']'; i++) {}; return i - off; }
	//anytext = (?![ \t\r\n;])+;
	size_t anytext(const char *code, size_t off) { size_t i; for (i = off; code[i] != ' ' && code[i] != '\t' && code[i] != '\r' && code[i] != '\n' && code[i] != ';'; i++) {}; return i - off; }
	//type = [a-zA-Z]+;
	size_t type(const char *code, size_t off) { size_t i; for (i = off; (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z'); i++) {}; return i - off; }
	//integer = [0-9]+;
	//size_t integer(const char *code, size_t off) { size_t i; for (i = off; (code[i] >= '0' && code[i] <= '9'); i++); return i - off; }
	//semicolon = ';';
	size_t semicolon(const char *code, size_t off) { return code[off] == ';' ? 1 : 0; }
	//ASSEMBLY = { INSTRUCTIONS ';' { ';' } };
	void assembly(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		skip(code, line, col, curoff);
		//Iterate over instructions as long as it is an instruction start.
		while (instructions_start(code, curoff))
		{
			instructions(vm, code, line, col, curoff);
			skip(code, line, col, curoff);
			//Make sure at least one semicolon is available
			if (!semicolon(code, curoff))
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected Semicolon." << std::endl;
			}
			else
			{
				size_t semicolonlen;
				//Add semicolons up until no semicolon is left
				while ((semicolonlen = semicolon(code, curoff)) > 0)
				{
					curoff += semicolonlen;
					col += semicolonlen;
					skip(code, line, col, curoff);
				}
			}
		}
	}
	//INSTRUCTIONS = ENDSTATEMENT | ARG | PUSH;
	bool instructions_start(const char *code, size_t off) { return endstatement_start(code, off) || arg_start(code, off) || push_start(code, off); }
	void instructions(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (endstatement_start(code, curoff))
		{
			endstatement(vm, code, line, col, curoff);
		}
		else if (arg_start(code, curoff))
		{
			arg(vm, code, line, col, curoff);
		}
		else if (push_start(code, curoff))
		{
			push(vm, code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No viable alternative for INSTRUCTIONS." << std::endl;
		}
	}
	//ARG = CALLUNARY | CALLBINARY | ASSIGNTOLOCAL | ASSIGNTO | CALLNULAR | GETVARIABLE | MAKEARRAY
	bool arg_start(const char *code, size_t off) { return callunary_start(code, off) || callbinary_start(code, off) || assignto_start(code, off) || assigntolocal_start(code, off) || callnular_start(code, off) || getvariable_start(code, off) || makearray_start(code, off); }
	void arg(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (callunary_start(code, curoff))
		{
			callunary(vm, code, line, col, curoff);
		}
		else if (callbinary_start(code, curoff))
		{
			callbinary(vm, code, line, col, curoff);
		}
		else if (assigntolocal_start(code, curoff))
		{
			assigntolocal(vm, code, line, col, curoff);
		}
		else if (assignto_start(code, curoff))
		{
			assignto(vm, code, line, col, curoff);
		}
		else if (callnular_start(code, curoff))
		{
			callnular(vm, code, line, col, curoff);
		}
		else if (getvariable_start(code, curoff))
		{
			getvariable(vm, code, line, col, curoff);
		}
		else if (makearray_start(code, curoff))
		{
			makearray(vm, code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No viable alternative for CALLUNARY." << std::endl;
		}
	}
	//ENDSTATEMENT = "endStatement"
	bool endstatement_start(const char *code, size_t off) { return str_cmpi(code + off, compiletime::strlen("endStatement"), "endStatement", compiletime::strlen("endStatement")) == 0; }
	void endstatement(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (endstatement_start(code, curoff))
		{
			skip(code, line, col, curoff);
			auto inst = std::make_shared<sqf::inst::endstatement>();
			inst->setdbginf(line, col, std::string(), sqf::virtualmachine::dbgsegment(code, curoff, compiletime::strlen("endStatement")));
			vm->stack()->pushinst(vm, inst);
			curoff += compiletime::strlen("endStatement");
			col += compiletime::strlen("endStatement");
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected 'endStatement'." << std::endl;
		}
	}
	//CALLUNARY = "callUnary" command
	bool callunary_start(const char *code, size_t off) { return str_cmpi(code + off, compiletime::strlen("callUnary"), "callUnary", compiletime::strlen("callUnary")) == 0; }
	void callunary(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (callunary_start(code, curoff))
		{
			curoff += compiletime::strlen("callUnary");
			col += compiletime::strlen("callUnary");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected 'callUnary'." << std::endl;
		}

		size_t cmdlen;
		if (!(cmdlen = command(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected command." << std::endl;
		}
		else
		{
			std::string cmdname = std::string(code + curoff, code + curoff + cmdlen);
			auto cmdrange = sqf::commandmap::get().getrange_u(cmdname);
			if (cmdrange->size() == 0)
			{
				vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, cmdlen) << "[ERR][L" << line << "|C" << col << "]\t" << "Command is unknown." << std::endl;
			}
			else
			{
				auto inst = std::make_shared<sqf::inst::callunary>(cmdrange);
				inst->setdbginf(identline, identcol, std::string(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen("callUnary")));
				vm->stack()->pushinst(vm, inst);
			}
			curoff += cmdlen;
			col += cmdlen;
			skip(code, line, col, curoff);
		}
	}
	//CALLBINARY = "callBinary" command
	bool callbinary_start(const char *code, size_t off) { return str_cmpi(code + off, compiletime::strlen("callBinary"), "callBinary", compiletime::strlen("callBinary")) == 0; }
	void callbinary(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (callbinary_start(code, curoff))
		{
			curoff += compiletime::strlen("callBinary");
			col += compiletime::strlen("callBinary");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected 'callBinary'." << std::endl;
		}

		size_t cmdlen;
		if (!(cmdlen = command(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected command." << std::endl;
		}
		else
		{
			std::string cmdname = std::string(code + curoff, code + curoff + cmdlen);
			auto cmdrange = sqf::commandmap::get().getrange_b(cmdname);
			if (cmdrange->size() == 0)
			{
				vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, cmdlen) << "[ERR][L" << line << "|C" << col << "]\t" << "Command is unknown." << std::endl;
			}
			else
			{
				auto inst = std::make_shared<sqf::inst::callbinary>(cmdrange);
				inst->setdbginf(identline, identcol, std::string(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen("callBinary")));
				vm->stack()->pushinst(vm, inst);
			}
			curoff += cmdlen;
			col += cmdlen;
			skip(code, line, col, curoff);
		}
	}
	//ASSIGNTO = "assignTo" anytext
	bool assignto_start(const char *code, size_t off) { return str_cmpi(code + off, compiletime::strlen("assignTo"), "assignTo", compiletime::strlen("assignTo")) == 0; }
	void assignto(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (assignto_start(code, curoff))
		{
			curoff += compiletime::strlen("assignTo");
			col += compiletime::strlen("assignTo");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected 'assignTo'." << std::endl;
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No text provided." << std::endl;
		}
		else
		{
			std::string text = std::string(code + curoff, code + curoff + textlen);
			auto inst = std::make_shared<sqf::inst::assignto>(text);
			inst->setdbginf(identline, identcol, std::string(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen("assignTo")));
			vm->stack()->pushinst(vm, inst);
			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//ASSIGNTOLOCAL = "assignToLoca" anytext
	bool assigntolocal_start(const char *code, size_t off) { return str_cmpi(code + off, compiletime::strlen("assignToLoca"), "assignToLoca", compiletime::strlen("assignToLoca")) == 0; }
	void assigntolocal(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (assigntolocal_start(code, curoff))
		{
			curoff += compiletime::strlen("assignToLoca");
			col += compiletime::strlen("assignToLoca");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected 'assignToLocal'." << std::endl;
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No text provided." << std::endl;
		}
		else
		{
			std::string text = std::string(code + curoff, code + curoff + textlen);
			auto inst = std::make_shared<sqf::inst::assigntolocal>(text);
			inst->setdbginf(identline, identcol, std::string(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen("assignToLoca")));
			vm->stack()->pushinst(vm, inst);

			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//CALLNULAR = "callNular" command
	bool callnular_start(const char *code, size_t off) { return str_cmpi(code + off, compiletime::strlen("callNular"), "callNular", compiletime::strlen("callNular")) == 0; }
	void callnular(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (callnular_start(code, curoff))
		{
			if (str_cmpi(code + curoff, compiletime::strlen("callNular"), "callNular", compiletime::strlen("callNular")) == 0)
			{
				curoff += compiletime::strlen("callNular");
				col += compiletime::strlen("callNular");
				skip(code, line, col, curoff);
			}
			else
			{
				curoff += compiletime::strlen("callNular");
				col += compiletime::strlen("callNular");
				skip(code, line, col, curoff);
			}
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected 'callNular'." << std::endl;
		}

		size_t cmdlen;
		if (!(cmdlen = command(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected command." << std::endl;
		}
		else
		{
			std::string cmdname = std::string(code + curoff, code + curoff + cmdlen);
			auto cmd = sqf::commandmap::get().get(cmdname);
			if (!cmd.get())
			{
				vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, cmdlen) << "[ERR][L" << line << "|C" << col << "]\t" << "Command is unknown." << std::endl;
			}
			else
			{
				auto inst = std::make_shared<sqf::inst::callnular>(cmd);
				inst->setdbginf(identline, identcol, std::string(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen("callNular")));
				vm->stack()->pushinst(vm, inst);
			}
			curoff += cmdlen;
			col += cmdlen;
			skip(code, line, col, curoff);
		}
	}
	//GETVARIABLE = "getVariable" anytext
	bool getvariable_start(const char *code, size_t off) { return str_cmpi(code + off, compiletime::strlen("getVariable"), "getVariable", compiletime::strlen("getVariable")) == 0; }
	void getvariable(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (getvariable_start(code, curoff))
		{
			curoff += compiletime::strlen("getVariable");
			col += compiletime::strlen("getVariable");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected 'getVariable'." << std::endl;
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No text provided." << std::endl;
		}
		else
		{
			std::string text = std::string(code + curoff, code + curoff + textlen);
			auto inst = std::make_shared<sqf::inst::getvariable>(text);
			inst->setdbginf(identline, identcol, std::string(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen("getVariable")));
			vm->stack()->pushinst(vm, inst);
			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//MAKEARRAY = "makeArray" anytext
	bool makearray_start(const char *code, size_t off) { return str_cmpi(code + off, compiletime::strlen("makeArray"), "makeArray", compiletime::strlen("makeArray")) == 0; }
	void makearray(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (makearray_start(code, curoff))
		{
			curoff += compiletime::strlen("makeArray");
			col += compiletime::strlen("makeArray");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected 'makeArray'." << std::endl;
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No text provided." << std::endl;
		}
		else
		{
			std::string text = std::string(code + curoff, code + curoff + textlen);
			auto inst = std::make_shared<sqf::inst::makearray>(static_cast<size_t>(std::stoul(text)));
			inst->setdbginf(identline, identcol, std::string(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen("makeArray")));
			vm->stack()->pushinst(vm, inst);
			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//PUSH = "push" type { anytext }
	bool push_start(const char *code, size_t off) { return str_cmpi(code + off, compiletime::strlen("push"), "push", compiletime::strlen("push")) == 0; }
	void push(sqf::virtualmachine* vm, const char *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		auto pushtype = sqf::type::NA;
		if (push_start(code, curoff))
		{
			curoff += compiletime::strlen("push");
			col += compiletime::strlen("push");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected 'push'." << std::endl;
		}

		size_t typelen;
		if (!(typelen = type(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No type provided." << std::endl;
		}
		else
		{
			pushtype = parsetype(std::string(code + curoff, code + curoff + typelen));
			curoff += typelen;
			col += typelen;
			skip(code, line, col, curoff);
		}
		size_t textlen = 0;
		size_t curtextlen;
		while ((curtextlen = anytext(code, curoff)) > 0)
		{
			textlen += curtextlen;
			curoff += curtextlen;
			col += curtextlen;
			skip(code, line, col, curoff);
		}
		if (textlen > 0)
		{
			auto data = std::string(code + curoff - textlen, code + curoff);
			auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(sqf::convert(std::make_shared<sqf::stringdata>(data), pushtype), pushtype));
			inst->setdbginf(identline, identcol, std::string(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen("push")));
			vm->stack()->pushinst(vm, inst);
		}
	}	
}

void sqf::virtualmachine::parse_assembly(std::string codein)
{
	const char *code = codein.c_str();
	size_t line = 1;
	size_t col = 0;
	size_t curoff = 0;
	assembly(this, code, line, col, curoff);
}