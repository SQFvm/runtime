#include <cwctype>

#include "full.h"
#include "compiletime.h"

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
//ASSIGNTOLOCAL = "assignToLocal" anytext
//CALLNULAR = "callNular" command
//GETVARIABLE = "getVariable" anytext
//MAKEARRAY = "makeArray" integer
//PUSH = "push" type { anytext }

namespace
{
	sqf::type parsetype(std::wstring);
	void skip(const wchar_t*, size_t&, size_t&, size_t&);
	//ident = [a-zA-Z]+;
	size_t ident(const wchar_t*, size_t);
	//command = [-+*/%a-zA-Z|&_><=\[\]]+;
	size_t command(const wchar_t*, size_t);
	//anytext = (?![ \t\r\n;])+;
	size_t anytext(const wchar_t*, size_t);
	//type = [a-zA-Z]+;
	size_t type(const wchar_t*, size_t);
	//integer = [0-9]+;
	size_t integer(const wchar_t*, size_t);
	//semicolon = ';';
	size_t semicolon(const wchar_t*, size_t);
	//ASSEMBLY = { INSTRUCTIONS ';' { ';' } };
	void assembly(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//INSTRUCTIONS = ENDSTATEMENT | ARG | PUSH;
	bool instructions_start(const wchar_t*, size_t);
	void instructions(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//ARG = CALLUNARY | CALLBINARY | ASSIGNTO | ASSIGNTOLOCAL | CALLNULLAR | GETVARIABLE | MAKEARRAY
	bool arg_start(const wchar_t*, size_t);
	void arg(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//ENDSTATEMENT = "endStatement"
	bool endstatement_start(const wchar_t*, size_t);
	void endstatement(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//CALLUNARY = "callUnary" command
	bool callunary_start(const wchar_t*, size_t);
	void callunary(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//CALLBINARY = "callBinary" command
	bool callbinary_start(const wchar_t*, size_t);
	void callbinary(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//ASSIGNTO = "assignTo" anytext
	bool assignto_start(const wchar_t*, size_t);
	void assignto(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//ASSIGNTOLOCAL = "assignToLocal" anytext
	bool assigntolocal_start(const wchar_t*, size_t);
	void assigntolocal(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//CALLNULLAR = "callNular" command
	bool callnullar_start(const wchar_t*, size_t);
	void callnullar(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//GETVARIABLE = "getVariable" anytext
	bool getvariable_start(const wchar_t*, size_t);
	void getvariable(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//MAKEARRAY = "makeArray" anytext
	bool makearray_start(const wchar_t*, size_t);
	void makearray(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//PUSH = "push" type { anytext }
	bool push_start(const wchar_t*, size_t);
	void push(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);


	sqf::type parsetype(std::wstring str)
	{
		if (wstr_cmpi(str.c_str(), compiletime::strlen(L"NOTHING"), L"NOTHING", compiletime::strlen(L"NOTHING")) == 0) { return sqf::type::NOTHING; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"ANY"), L"ANY", compiletime::strlen(L"ANY")) == 0) { return sqf::type::ANY; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"SCALAR"), L"SCALAR", compiletime::strlen(L"SCALAR")) == 0) { return sqf::type::SCALAR; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"BOOL"), L"BOOL", compiletime::strlen(L"BOOL")) == 0) { return sqf::type::BOOL; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"ARRAY"), L"ARRAY", compiletime::strlen(L"ARRAY")) == 0) { return sqf::type::ARRAY; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"STRING"), L"STRING", compiletime::strlen(L"STRING")) == 0) { return sqf::type::STRING; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"NAMESPACE"), L"NAMESPACE", compiletime::strlen(L"NAMESPACE")) == 0) { return sqf::type::NAMESPACE; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"NaN"), L"NaN", compiletime::strlen(L"NaN")) == 0) { return sqf::type::NaN; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"IF"), L"IF", compiletime::strlen(L"IF")) == 0) { return sqf::type::IF; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"WHILE"), L"WHILE", compiletime::strlen(L"WHILE")) == 0) { return sqf::type::WHILE; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"FOR"), L"FOR", compiletime::strlen(L"FOR")) == 0) { return sqf::type::FOR; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"SWITCH"), L"SWITCH", compiletime::strlen(L"SWITCH")) == 0) { return sqf::type::SWITCH; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"EXCEPTION"), L"EXCEPTION", compiletime::strlen(L"EXCEPTION")) == 0) { return sqf::type::EXCEPTION; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"WITH"), L"WITH", compiletime::strlen(L"WITH")) == 0) { return sqf::type::WITH; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"CODE"), L"CODE", compiletime::strlen(L"CODE")) == 0) { return sqf::type::CODE; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"OBJECT"), L"OBJECT", compiletime::strlen(L"OBJECT")) == 0) { return sqf::type::OBJECT; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"VECTOR"), L"VECTOR", compiletime::strlen(L"VECTOR")) == 0) { return sqf::type::VECTOR; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"TRANS"), L"TRANS", compiletime::strlen(L"TRANS")) == 0) { return sqf::type::TRANS; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"ORIENT"), L"ORIENT", compiletime::strlen(L"ORIENT")) == 0) { return sqf::type::ORIENT; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"SIDE"), L"SIDE", compiletime::strlen(L"SIDE")) == 0) { return sqf::type::SIDE; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"GROUP"), L"GROUP", compiletime::strlen(L"GROUP")) == 0) { return sqf::type::GROUP; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"TEXT"), L"TEXT", compiletime::strlen(L"TEXT")) == 0) { return sqf::type::TEXT; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"SCRIPT"), L"SCRIPT", compiletime::strlen(L"SCRIPT")) == 0) { return sqf::type::SCRIPT; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"TARGET"), L"TARGET", compiletime::strlen(L"TARGET")) == 0) { return sqf::type::TARGET; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"JCLASS"), L"JCLASS", compiletime::strlen(L"JCLASS")) == 0) { return sqf::type::JCLASS; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"CONFIG"), L"CONFIG", compiletime::strlen(L"CONFIG")) == 0) { return sqf::type::CONFIG; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"DISPLAY"), L"DISPLAY", compiletime::strlen(L"DISPLAY")) == 0) { return sqf::type::DISPLAY; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"CONTROL"), L"CONTROL", compiletime::strlen(L"CONTROL")) == 0) { return sqf::type::CONTROL; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"NetObject"), L"NetObject", compiletime::strlen(L"NetObject")) == 0) { return sqf::type::NetObject; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"SUBGROUP"), L"SUBGROUP", compiletime::strlen(L"SUBGROUP")) == 0) { return sqf::type::SUBGROUP; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"TEAM_MEMBER"), L"TEAM_MEMBER", compiletime::strlen(L"TEAM_MEMBER")) == 0) { return sqf::type::TEAM_MEMBER; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"TASK"), L"TASK", compiletime::strlen(L"TASK")) == 0) { return sqf::type::TASK; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"DIARY_RECORD"), L"DIARY_RECORD", compiletime::strlen(L"DIARY_RECORD")) == 0) { return sqf::type::DIARY_RECORD; }
		else if (wstr_cmpi(str.c_str(), compiletime::strlen(L"LOCATION"), L"LOCATION", compiletime::strlen(L"LOCATION")) == 0) { return sqf::type::LOCATION; }
		else { return sqf::type::NA; }
	}
	void skip(const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		while (1)
		{
			switch (code[curoff])
			{
			case L' ': curoff++; col++; continue;
			case L'\t': curoff++; col++; continue;
			case L'\r': curoff++; col++; continue;
			case L'\n': curoff++; line++; col = 0; continue;
			default: return;
			}
		}
	}
	//ident = [a-zA-Z]+;
	size_t ident(const wchar_t *code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z'); i++); return i - off; }
	//command = [-+*/%a-zA-Z|&_><=\[\]]+;
	size_t command(const wchar_t *code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z') || code[0] == L'-' || code[0] == L'+' || code[0] == L'*' || code[0] == L'/' || code[0] == L'%' || code[0] == L'|' || code[0] == L'&' || code[0] == L'_' || code[0] == L'>' || code[0] == L'<' || code[0] == L'=' || code[0] == L'[' || code[0] == L']'; i++); return i - off; }
	//anytext = (?![ \t\r\n;])+;
	size_t anytext(const wchar_t *code, size_t off) { size_t i; for (i = off; code[i] != L' ' && code[i] != L'\t' && code[i] != L'\r' && code[i] != L'\n' && code[i] != L';'; i++); return i - off; }
	//type = [a-zA-Z]+;
	size_t type(const wchar_t *code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z'); i++); return i - off; }
	//integer = [0-9]+;
	size_t integer(const wchar_t *code, size_t off) { size_t i; for (i = off; (code[i] >= L'0' && code[i] <= L'9'); i++); return i - off; }
	//semicolon = ';';
	size_t semicolon(const wchar_t *code, size_t off) { return code[off] == L';' ? 1 : 0; }
	//ASSEMBLY = { INSTRUCTIONS ';' { ';' } };
	void assembly(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
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
				vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected Semicolon.";
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
	bool instructions_start(const wchar_t *code, size_t off) { return endstatement_start(code, off) || arg_start(code, off) || push_start(code, off); }
	void instructions(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
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
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No viable alternative for INSTRUCTIONS.";
		}
	}
	//ARG = CALLUNARY | CALLBINARY | ASSIGNTO | ASSIGNTOLOCAL | CALLNULLAR | GETVARIABLE | MAKEARRAY
	bool arg_start(const wchar_t *code, size_t off) { return callunary_start(code, off) || callbinary_start(code, off) || assignto_start(code, off) || assigntolocal_start(code, off) || callnullar_start(code, off) || getvariable_start(code, off) || makearray_start(code, off); }
	void arg(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (callunary_start(code, curoff))
		{
			callunary(vm, code, line, col, curoff);
		}
		else if (callbinary_start(code, curoff))
		{
			callbinary(vm, code, line, col, curoff);
		}
		else if (assignto_start(code, curoff))
		{
			assignto(vm, code, line, col, curoff);
		}
		else if (assigntolocal_start(code, curoff))
		{
			assigntolocal(vm, code, line, col, curoff);
		}
		else if (callnullar_start(code, curoff))
		{
			callnullar(vm, code, line, col, curoff);
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
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No viable alternative for CALLUNARY.";
		}
	}
	//ENDSTATEMENT = "endStatement"
	bool endstatement_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, compiletime::strlen(L"endStatement"), L"endStatement", compiletime::strlen(L"endStatement")) == 0; }
	void endstatement(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (endstatement_start(code, curoff))
		{
			skip(code, line, col, curoff);
			auto inst = std::make_shared<sqf::inst::endstatement>();
			inst->setdbginf(line, col, std::wstring(), sqf::virtualmachine::dbgsegment(code, curoff, compiletime::strlen(L"endStatement")));
			vm->stack()->pushinst(inst);
			curoff += compiletime::strlen(L"endStatement");
			col += compiletime::strlen(L"endStatement");
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'endStatement'.";
		}
	}
	//CALLUNARY = "callUnary" command
	bool callunary_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, compiletime::strlen(L"callUnary"), L"callUnary", compiletime::strlen(L"callUnary")) == 0; }
	void callunary(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (callunary_start(code, curoff))
		{
			curoff += compiletime::strlen(L"callUnary");
			col += compiletime::strlen(L"callUnary");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'callUnary'.";
		}

		size_t cmdlen;
		if (!(cmdlen = command(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected command.";
		}
		else
		{
			std::wstring cmdname = std::wstring(code + curoff, code + curoff + cmdlen);
			auto cmdrange = sqf::commandmap::get().getrange_u(cmdname);
			if (cmdrange.size() == 0)
			{
				vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, cmdlen) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Command is unknown.";
			}
			else
			{
				auto inst = std::make_shared<sqf::inst::callunary>(cmdrange);
				inst->setdbginf(identline, identcol, std::wstring(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen(L"callUnary")));
				vm->stack()->pushinst(inst);
			}
			curoff += cmdlen;
			col += cmdlen;
			skip(code, line, col, curoff);
		}
	}
	//CALLBINARY = "callBinary" command
	bool callbinary_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, compiletime::strlen(L"callBinary"), L"callBinary", compiletime::strlen(L"callBinary")) == 0; }
	void callbinary(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (callbinary_start(code, curoff))
		{
			curoff += compiletime::strlen(L"callBinary");
			col += compiletime::strlen(L"callBinary");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'callBinary'.";
		}

		size_t cmdlen;
		if (!(cmdlen = command(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected command.";
		}
		else
		{
			std::wstring cmdname = std::wstring(code + curoff, code + curoff + cmdlen);
			auto cmdrange = sqf::commandmap::get().getrange_b(cmdname);
			if (cmdrange.size() == 0)
			{
				vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, cmdlen) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Command is unknown.";
			}
			else
			{
				auto inst = std::make_shared<sqf::inst::callbinary>(cmdrange);
				inst->setdbginf(identline, identcol, std::wstring(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen(L"callBinary")));
				vm->stack()->pushinst(inst);
			}
			curoff += cmdlen;
			col += cmdlen;
			skip(code, line, col, curoff);
		}
	}
	//ASSIGNTO = "assignTo" anytext
	bool assignto_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, compiletime::strlen(L"assignTo"), L"assignTo", compiletime::strlen(L"assignTo")) == 0; }
	void assignto(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (assignto_start(code, curoff))
		{
			curoff += compiletime::strlen(L"assignTo");
			col += compiletime::strlen(L"assignTo");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'assignTo'.";
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No text provided.";
		}
		else
		{
			std::wstring text = std::wstring(code + curoff, code + curoff + textlen);
			auto inst = std::make_shared<sqf::inst::assignto>(text);
			inst->setdbginf(identline, identcol, std::wstring(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen(L"assignTo")));
			vm->stack()->pushinst(inst);
			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//ASSIGNTOLOCAL = "assignToLocal" anytext
	bool assigntolocal_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, compiletime::strlen(L"assignToLocal"), L"assignToLocal", compiletime::strlen(L"assignToLocal")) == 0; }
	void assigntolocal(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (assigntolocal_start(code, curoff))
		{
			curoff += compiletime::strlen(L"assignToLocal");
			col += compiletime::strlen(L"assignToLocal");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'assignToLocal'.";
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No text provided.";
		}
		else
		{
			std::wstring text = std::wstring(code + curoff, code + curoff + textlen);
			auto inst = std::make_shared<sqf::inst::assigntolocal>(text);
			inst->setdbginf(identline, identcol, std::wstring(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen(L"assignToLocal")));
			vm->stack()->pushinst(inst);

			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//CALLNULLAR = "callNular" command
	bool callnullar_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, compiletime::strlen(L"callNular"), L"callNular", compiletime::strlen(L"callNular")) == 0 || wstr_cmpi(code + off, compiletime::strlen(L"callNullar"), L"callNullar", compiletime::strlen(L"callNullar")) == 0; }
	void callnullar(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (callnullar_start(code, curoff))
		{
			if (wstr_cmpi(code + curoff, compiletime::strlen(L"callNular"), L"callNular", compiletime::strlen(L"callNular")) == 0)
			{
				curoff += compiletime::strlen(L"callNular");
				col += compiletime::strlen(L"callNular");
				skip(code, line, col, curoff);
			}
			else
			{
				curoff += compiletime::strlen(L"callNullar");
				col += compiletime::strlen(L"callNullar");
				skip(code, line, col, curoff);
			}
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'callNullar'.";
		}

		size_t cmdlen;
		if (!(cmdlen = command(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected command.";
		}
		else
		{
			std::wstring cmdname = std::wstring(code + curoff, code + curoff + cmdlen);
			auto cmd = sqf::commandmap::get().get(cmdname);
			if (!cmd.get())
			{
				vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, cmdlen) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Command is unknown.";
			}
			else
			{
				auto inst = std::make_shared<sqf::inst::callnullar>(cmd);
				inst->setdbginf(identline, identcol, std::wstring(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen(L"callNullar")));
				vm->stack()->pushinst(inst);
			}
			curoff += cmdlen;
			col += cmdlen;
			skip(code, line, col, curoff);
		}
	}
	//GETVARIABLE = "getVariable" anytext
	bool getvariable_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, compiletime::strlen(L"getVariable"), L"getVariable", compiletime::strlen(L"getVariable")) == 0; }
	void getvariable(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (getvariable_start(code, curoff))
		{
			curoff += compiletime::strlen(L"getVariable");
			col += compiletime::strlen(L"getVariable");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'getVariable'.";
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No text provided.";
		}
		else
		{
			std::wstring text = std::wstring(code + curoff, code + curoff + textlen);
			auto inst = std::make_shared<sqf::inst::getvariable>(text);
			inst->setdbginf(identline, identcol, std::wstring(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen(L"getVariable")));
			vm->stack()->pushinst(inst);
			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//MAKEARRAY = "makeArray" anytext
	bool makearray_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, compiletime::strlen(L"makeArray"), L"makeArray", compiletime::strlen(L"makeArray")) == 0; }
	void makearray(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		if (assigntolocal_start(code, curoff))
		{
			curoff += compiletime::strlen(L"makeArray");
			col += compiletime::strlen(L"makeArray");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'makeArray'.";
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No text provided.";
		}
		else
		{
			std::wstring text = std::wstring(code + curoff, code + curoff + textlen);
			auto inst = std::make_shared<sqf::inst::makearray>((size_t)std::stoul(text));
			inst->setdbginf(identline, identcol, std::wstring(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen(L"makeArray")));
			vm->stack()->pushinst(inst);
			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//PUSH = "push" type { anytext }
	bool push_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, compiletime::strlen(L"push"), L"push", compiletime::strlen(L"push")) == 0; }
	void push(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		size_t identstart = curoff;
		size_t identcol = col;
		size_t identline = line;
		auto pushtype = sqf::type::NA;
		if (push_start(code, curoff))
		{
			curoff += compiletime::strlen(L"push");
			col += compiletime::strlen(L"push");
			skip(code, line, col, curoff);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'push'.";
		}

		size_t typelen;
		if (!(typelen = type(code, curoff)))
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No text provided.";
		}
		else
		{
			pushtype = parsetype(std::wstring(code + curoff, code + curoff + typelen));
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
			auto data = std::wstring(code + curoff, code + curoff + typelen);
			auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(sqf::convert(std::make_shared<sqf::stringdata>(data), pushtype), pushtype));
			inst->setdbginf(identline, identcol, std::wstring(), sqf::virtualmachine::dbgsegment(code, identstart, compiletime::strlen(L"push")));
			vm->stack()->pushinst(inst);
		}
	}	
}

void sqf::virtualmachine::parse_assembly(std::wstring codein)
{
	const wchar_t *code = codein.c_str();
	size_t line = 0;
	size_t col = 0;
	size_t curoff = 0;
	assembly(this, code, line, col, curoff);
}