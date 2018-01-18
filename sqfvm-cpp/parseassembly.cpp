#include "full.h"

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
//anytext = [-+*/%a-zA-Z|&_><=]+;
//type = [a-zA-Z]+;
//semicolon = ';';
//ASSEMBLY = { INSTRUCTIONS ';' { ';' } };
//INSTRUCTIONS = ENDSTATEMENT | ARG | PUSH;
//ARG = CALLUNARY | CALLBINARY | ASSIGNTO | ASSIGNTOLOCAL | CALLNULAR | GETVARIABLE | MAKEARRAY
//ENDSTATEMENT = "endStatement"
//CALLUNARY = "callUnary" anytext
//CALLBINARY = "callBinary" anytext
//ASSIGNTO = "assignTo" anytext
//ASSIGNTOLOCAL = "assignToLocal" anytext
//CALLNULAR = "callNular" anytext
//GETVARIABLE = "getVariable" anytext
//MAKEARRAY = "makeArray" anytext
//PUSH = "push" type { anytext }

namespace
{
	//ident = [a-zA-Z]+;
	size_t semicolonn(const wchar_t *code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z'); i++); return i - off; }
	//anytext = [-+*/%a-zA-Z|&_><=\[\]]+;
	size_t semicolonn(const wchar_t *code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z') || code[0] == L'-' || code[0] == L'+' || code[0] == L'*' || code[0] == L'/' || code[0] == L'%' || code[0] == L'|' || code[0] == L'&' || code[0] == L'_' || code[0] == L'>' || code[0] == L'<' || code[0] == L'=' || code[0] == L'[' || code[0] == L']'; i++); return i - off; }
	//type = [a-zA-Z]+;
	size_t semicolonn(const wchar_t *code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z'); i++); return i - off; }
	//semicolon = ';';
	size_t semicolonn(const wchar_t *code, size_t off) { return code[0] == L';' ? 1 : 0; }
	//ASSEMBLY = { INSTRUCTIONS ';' { ';' } };
	void assembly(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
	{

	}
	//INSTRUCTIONS = ENDSTATEMENT | ARG | PUSH;
	bool instructions_start(const wchar_t *code, size_t off) { return endstatement_start(code, off) || arg_start(code, off) || push_start(code, off); }
	bool instructions(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
	//ARG = CALLUNARY | CALLBINARY | ASSIGNTO | ASSIGNTOLOCAL | CALLNULLAR | GETVARIABLE | MAKEARRAY
	bool arg_start(const wchar_t *code, size_t off) { return callunary_start(code, off) || callbinary_start(code, off) || assignto_start(code, off) || assigntolocal_start(code, off) || callnullar_start(code, off) || getvariable_start(code, off) || makearray_start(code, off); }
	bool arg(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
	//ENDSTATEMENT = "endStatement"
	bool endstatement_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"endStatement", -1) == 0; }
	bool endstatement(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
	//CALLUNARY = "callUnary" anytext
	bool callunary_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"callUnary", -1) == 0; }
	bool callunary(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
	//CALLBINARY = "callBinary" anytext
	bool callbinary_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"callBinary", -1) == 0; }
	bool callbinary(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
	//ASSIGNTO = "assignTo" anytext
	bool assignto_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"assignTo", -1) == 0; }
	bool assignto(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
	//ASSIGNTOLOCAL = "assignToLocal" anytext
	bool assigntolocal_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"assignToLocal", -1) == 0; }
	bool assigntolocal(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
	//CALLNULLAR = "callNular" anytext
	bool callnullar_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"callNular", -1) == 0 || wstr_cmpi(code + off, -1, L"callNullar", -1) == 0; }
	bool callnullar(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
	//GETVARIABLE = "getVariable" anytext
	bool getvariable_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"getVariable", -1) == 0; }
	bool getvariable(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
	//MAKEARRAY = "makeArray" anytext
	bool makearray_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"makeArray", -1) == 0; }
	bool makearray(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
	//PUSH = "push" type { anytext }
	bool push_start(const wchar_t *code, size_t off) { return wstr_cmpi(code + off, -1, L"push", -1) == 0; }
	bool push(sqf::virtualmachine* vm, const wchar_t *code, size_t &line, size_t &col, size_t &curoff) { }
}

void sqf::virtualmachine::parse_assembly(std::wstring codein)
{
	const wchar_t *code = codein.c_str();
	size_t line = 0;
	size_t col = 0;
	size_t curoff = 0;
	assembly(this, code, line, col, curoff);
}