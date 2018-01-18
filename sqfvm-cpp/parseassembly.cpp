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
	bool instructions(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//ARG = CALLUNARY | CALLBINARY | ASSIGNTO | ASSIGNTOLOCAL | CALLNULLAR | GETVARIABLE | MAKEARRAY
	bool arg_start(const wchar_t*, size_t);
	bool arg(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//ENDSTATEMENT = "endStatement"
	bool endstatement_start(const wchar_t*, size_t);
	bool endstatement(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//CALLUNARY = "callUnary" command
	bool callunary_start(const wchar_t*, size_t);
	bool callunary(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//CALLBINARY = "callBinary" command
	bool callbinary_start(const wchar_t*, size_t);
	bool callbinary(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//ASSIGNTO = "assignTo" anytext
	bool assignto_start(const wchar_t*, size_t);
	bool assignto(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//ASSIGNTOLOCAL = "assignToLocal" anytext
	bool assigntolocal_start(const wchar_t*, size_t);
	bool assigntolocal(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//CALLNULLAR = "callNular" command
	bool callnullar_start(const wchar_t*, size_t);
	bool callnullar(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//GETVARIABLE = "getVariable" anytext
	bool getvariable_start(const wchar_t*, size_t);
	bool getvariable(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//MAKEARRAY = "makeArray" anytext
	bool makearray_start(const wchar_t*, size_t);
	bool makearray(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);
	//PUSH = "push" type { anytext }
	bool push_start(const wchar_t*, size_t);
	bool push(sqf::virtualmachine*, const wchar_t*, size_t&, size_t&, size_t&);


	sqf::type parsetype(std::wstring str)
	{
		if (wstr_cmpi(str.c_str(), -1, L"NOTHING", -1) == 0) { return sqf::type::NOTHING; }
		else if (wstr_cmpi(str.c_str(), -1, L"ANY", -1) == 0) { return sqf::type::ANY; }
		else if (wstr_cmpi(str.c_str(), -1, L"SCALAR", -1) == 0) { return sqf::type::SCALAR; }
		else if (wstr_cmpi(str.c_str(), -1, L"BOOL", -1) == 0) { return sqf::type::BOOL; }
		else if (wstr_cmpi(str.c_str(), -1, L"ARRAY", -1) == 0) { return sqf::type::ARRAY; }
		else if (wstr_cmpi(str.c_str(), -1, L"STRING", -1) == 0) { return sqf::type::STRING; }
		else if (wstr_cmpi(str.c_str(), -1, L"NAMESPACE", -1) == 0) { return sqf::type::NAMESPACE; }
		else if (wstr_cmpi(str.c_str(), -1, L"NaN", -1) == 0) { return sqf::type::NaN; }
		else if (wstr_cmpi(str.c_str(), -1, L"IF", -1) == 0) { return sqf::type::IF; }
		else if (wstr_cmpi(str.c_str(), -1, L"WHILE", -1) == 0) { return sqf::type::WHILE; }
		else if (wstr_cmpi(str.c_str(), -1, L"FOR", -1) == 0) { return sqf::type::FOR; }
		else if (wstr_cmpi(str.c_str(), -1, L"SWITCH", -1) == 0) { return sqf::type::SWITCH; }
		else if (wstr_cmpi(str.c_str(), -1, L"EXCEPTION", -1) == 0) { return sqf::type::EXCEPTION; }
		else if (wstr_cmpi(str.c_str(), -1, L"WITH", -1) == 0) { return sqf::type::WITH; }
		else if (wstr_cmpi(str.c_str(), -1, L"CODE", -1) == 0) { return sqf::type::CODE; }
		else if (wstr_cmpi(str.c_str(), -1, L"OBJECT", -1) == 0) { return sqf::type::OBJECT; }
		else if (wstr_cmpi(str.c_str(), -1, L"VECTOR", -1) == 0) { return sqf::type::VECTOR; }
		else if (wstr_cmpi(str.c_str(), -1, L"TRANS", -1) == 0) { return sqf::type::TRANS; }
		else if (wstr_cmpi(str.c_str(), -1, L"ORIENT", -1) == 0) { return sqf::type::ORIENT; }
		else if (wstr_cmpi(str.c_str(), -1, L"SIDE", -1) == 0) { return sqf::type::SIDE; }
		else if (wstr_cmpi(str.c_str(), -1, L"GROUP", -1) == 0) { return sqf::type::GROUP; }
		else if (wstr_cmpi(str.c_str(), -1, L"TEXT", -1) == 0) { return sqf::type::TEXT; }
		else if (wstr_cmpi(str.c_str(), -1, L"SCRIPT", -1) == 0) { return sqf::type::SCRIPT; }
		else if (wstr_cmpi(str.c_str(), -1, L"TARGET", -1) == 0) { return sqf::type::TARGET; }
		else if (wstr_cmpi(str.c_str(), -1, L"JCLASS", -1) == 0) { return sqf::type::JCLASS; }
		else if (wstr_cmpi(str.c_str(), -1, L"CONFIG", -1) == 0) { return sqf::type::CONFIG; }
		else if (wstr_cmpi(str.c_str(), -1, L"DISPLAY", -1) == 0) { return sqf::type::DISPLAY; }
		else if (wstr_cmpi(str.c_str(), -1, L"CONTROL", -1) == 0) { return sqf::type::CONTROL; }
		else if (wstr_cmpi(str.c_str(), -1, L"NetObject", -1) == 0) { return sqf::type::NetObject; }
		else if (wstr_cmpi(str.c_str(), -1, L"SUBGROUP", -1) == 0) { return sqf::type::SUBGROUP; }
		else if (wstr_cmpi(str.c_str(), -1, L"TEAM_MEMBER", -1) == 0) { return sqf::type::TEAM_MEMBER; }
		else if (wstr_cmpi(str.c_str(), -1, L"TASK", -1) == 0) { return sqf::type::TASK; }
		else if (wstr_cmpi(str.c_str(), -1, L"DIARY_RECORD", -1) == 0) { return sqf::type::DIARY_RECORD; }
		else if (wstr_cmpi(str.c_str(), -1, L"LOCATION", -1) == 0) { return sqf::type::LOCATION; }
		else { return sqf::type::NA; }
	}
	void skip(const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		while (1)
		{
			col++;
			switch (code[curoff++])
			{
			case L' ': continue;
			case L'\t': continue;
			case L'\r': continue;
			case L'\n': line++; col = 0; continue;
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
	size_t semicolon(const wchar_t *code, size_t off) { return code[0] == L';' ? 1 : 0; }
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
				int i;
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
	bool instructions(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
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
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No viable alternative for INSTRUCTIONS.";
		}
	}
	//ARG = CALLUNARY | CALLBINARY | ASSIGNTO | ASSIGNTOLOCAL | CALLNULLAR | GETVARIABLE | MAKEARRAY
	bool arg_start(const wchar_t *code, size_t off) { return callunary_start(code, off) || callbinary_start(code, off) || assignto_start(code, off) || assigntolocal_start(code, off) || callnullar_start(code, off) || getvariable_start(code, off) || makearray_start(code, off); }
	bool arg(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
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
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No viable alternative for CALLUNARY.";
		}
	}
	//ENDSTATEMENT = "endStatement"
	bool endstatement_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"endStatement", -1) == 0; }
	bool endstatement(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (endstatement_start(code, curoff))
		{
			curoff += compiletime::strlen(L"endStatement");
			col += compiletime::strlen(L"endStatement");
			skip(code, line, col, curoff);
			vm->stack()->pushinst(std::make_shared<sqf::inst::endstatement>());
		}
		else
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'endStatement'.";
		}
	}
	//CALLUNARY = "callUnary" command
	bool callunary_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"callUnary", -1) == 0; }
	bool callunary(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (callunary_start(code, curoff))
		{
			curoff += compiletime::strlen(L"callUnary");
			col += compiletime::strlen(L"callUnary");
			skip(code, line, col, curoff);
		}
		else
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'callUnary'.";
		}

		size_t cmdlen;
		if (!(cmdlen = command(code, curoff)))
		{
			int i;
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
				vm->stack()->pushinst(std::make_shared<sqf::inst::callunary>(cmdrange));
			}
			curoff += cmdlen;
			col += cmdlen;
			skip(code, line, col, curoff);
		}
	}
	//CALLBINARY = "callBinary" command
	bool callbinary_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"callBinary", -1) == 0; }
	bool callbinary(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (callbinary_start(code, curoff))
		{
			curoff += compiletime::strlen(L"callBinary");
			col += compiletime::strlen(L"callBinary");
			skip(code, line, col, curoff);
		}
		else
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'callBinary'.";
		}

		size_t cmdlen;
		if (!(cmdlen = command(code, curoff)))
		{
			int i;
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
				vm->stack()->pushinst(std::make_shared<sqf::inst::callbinary>(cmdrange));
			}
			curoff += cmdlen;
			col += cmdlen;
			skip(code, line, col, curoff);
		}
	}
	//ASSIGNTO = "assignTo" anytext
	bool assignto_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"assignTo", -1) == 0; }
	bool assignto(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (assignto_start(code, curoff))
		{
			curoff += compiletime::strlen(L"assignTo");
			col += compiletime::strlen(L"assignTo");
			skip(code, line, col, curoff);
		}
		else
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'assignTo'.";
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No text provided.";
		}
		else
		{
			std::wstring text = std::wstring(code + curoff, code + curoff + textlen);
			vm->stack()->pushinst(std::make_shared<sqf::inst::assignto>(text));
			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//ASSIGNTOLOCAL = "assignToLocal" anytext
	bool assigntolocal_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"assignToLocal", -1) == 0; }
	bool assigntolocal(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (assigntolocal_start(code, curoff))
		{
			curoff += compiletime::strlen(L"assignToLocal");
			col += compiletime::strlen(L"assignToLocal");
			skip(code, line, col, curoff);
		}
		else
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'assignToLocal'.";
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No text provided.";
		}
		else
		{
			std::wstring text = std::wstring(code + curoff, code + curoff + textlen);
			vm->stack()->pushinst(std::make_shared<sqf::inst::assigntolocal>(text));
			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//CALLNULLAR = "callNular" command
	bool callnullar_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"callNular", -1) == 0 || wstr_cmpi(code + off, -1, L"callNullar", -1) == 0; }
	bool callnullar(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (callnullar_start(code, curoff))
		{
			if (wstr_cmpi(code + curoff, -1, L"callNular", -1) == 0)
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
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'callNullar'.";
		}

		size_t cmdlen;
		if (!(cmdlen = command(code, curoff)))
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected command.";
		}
		else
		{
			std::wstring cmdname = std::wstring(code + curoff, code + curoff + cmdlen);
			auto cmd = sqf::commandmap::get().get(cmdname);
			if (!cmd.get() == 0)
			{
				vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, cmdlen) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Command is unknown.";
			}
			else
			{
				vm->stack()->pushinst(std::make_shared<sqf::inst::callnullar>(cmd));
			}
			curoff += cmdlen;
			col += cmdlen;
			skip(code, line, col, curoff);
		}
	}
	//GETVARIABLE = "getVariable" anytext
	bool getvariable_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"getVariable", -1) == 0; }
	bool getvariable(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (getvariable_start(code, curoff))
		{
			curoff += compiletime::strlen(L"getVariable");
			col += compiletime::strlen(L"getVariable");
			skip(code, line, col, curoff);
		}
		else
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'getVariable'.";
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No text provided.";
		}
		else
		{
			std::wstring text = std::wstring(code + curoff, code + curoff + textlen);
			vm->stack()->pushinst(std::make_shared<sqf::inst::getvariable>(text));
			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//MAKEARRAY = "makeArray" anytext
	bool makearray_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"makeArray", -1) == 0; }
	bool makearray(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		if (assigntolocal_start(code, curoff))
		{
			curoff += compiletime::strlen(L"makeArray");
			col += compiletime::strlen(L"makeArray");
			skip(code, line, col, curoff);
		}
		else
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'makeArray'.";
		}

		size_t textlen;
		if (!(textlen = anytext(code, curoff)))
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No text provided.";
		}
		else
		{
			std::wstring text = std::wstring(code + curoff, code + curoff + textlen);
			vm->stack()->pushinst(std::make_shared<sqf::inst::makearray>((size_t)std::stoul(text)));
			curoff += textlen;
			col += textlen;
			skip(code, line, col, curoff);
		}
	}
	//PUSH = "push" type { anytext }
	bool push_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"push", -1) == 0; }
	bool push(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{
		auto pushtype = sqf::type::NA;
		if (push_start(code, curoff))
		{
			curoff += compiletime::strlen(L"push");
			col += compiletime::strlen(L"push");
			skip(code, line, col, curoff);
		}
		else
		{
			int i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected 'push'.";
		}

		size_t typelen;
		if (!(typelen = type(code, curoff)))
		{
			int i;
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
			vm->stack()->pushinst(std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(sqf::convert(std::make_shared<sqf::stringdata>(data), pushtype), pushtype)));
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