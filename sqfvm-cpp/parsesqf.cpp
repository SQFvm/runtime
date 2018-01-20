#include <cwctype>
#include "full.h"
#include "compiletime.h"


namespace
{
	/*
	endchr = [,;];
	identifier = [_a-zA-Z][_a-zA-Z0-9]*;
	hexadecimal = [0-9a-fA-F]+;
	scalarsub = [0-9]+;
	scalar = scalarsub(.scalarsub)?;
	anytext = (?![ \t\r\n;])+;
	operator_ = [-*+/a-zA-Z><=%_]+;
	boperator = <codecontext>;
	uoperator = <codecontext>;
	noperator = <codecontext>;
	SQF = { STATEMENT endchr{ endchr } }
	STATEMENT = ASSIGNMENT | BINARYEXPRESSION;
	ASSIGNMENT(2) = identifier '=' BINARYEXPRESSION | "private" identifier '=' BINARYEXPRESSION;
	BINARYEXPRESSION = BINARYEXPRESSION<codecontext> boperator BINARYEXPRESSION | PRIMARYEXPRESSION;
	BINARYEXPRESSIONINBRACKETS = '(' BINARYEXPRESSION ')';
	PRIMARYEXPRESSION = NUMBER | UNARYEXPRESSION | NULLAREXPRESSION | VARIABLE | STRING | CODE | BINARYEXPRESSIONINBRACKETS | ARRAY;
	NULLAREXPRESSION = noperator;
	UNARYEXPRESSION = uoperator PRIMARYEXPRESSION;
	NUMBER = ("0x" | '$') hexadecimal | scalar;
	VARIABLE = identifier;
	STRING = '"' { any | "\"\"" } '"' | '\'' { any | "''" } '\'';
	CODE = "{" SQF "}";
	ARRAY = '[' [ BINARYEXPRESSION { ',' BINARYEXPRESSION } ] ']';
	*/

	void skip(const wchar_t *code, size_t &curoff)
	{
		while (1)
		{
			switch (code[curoff])
			{
			case L' ': curoff++; continue;
			case L'\t': curoff++; continue;
			case L'\r': curoff++; continue;
			case L'\n': curoff++; continue;
			default: return;
			}
		}
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
	
	//endchr = [,;];
	size_t endchr(const wchar_t* code, size_t off) { return code[off] == L';' | code[off] == L',' ? 1 : 0; }
	//identifier = [_a-zA-Z][_a-zA-Z0-9]*;
	size_t identifier(const wchar_t* code, size_t off) { size_t i; if (!((code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z') || code[i] == L'_')) return 0; for (i = off + 1; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z') || (code[i] >= L'0' && code[i] <= L'9') || code[i] == L'_'; i++); return i - off; }
	//operator_ = [-*+/a-zA-Z><=%_]+;
	//ToDo: Add clearer non-alphabetical checks (-- should not be detected as SINGLE operator but rather as two operators)
	size_t operator_(const wchar_t* code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z') || code[i] >= L'++' || code[i] >= L'-' || code[i] >= L'*' || code[i] >= L'/' || code[i] >= L'>' || code[i] >= L'<' || code[i] >= L'=' || code[i] >= L'%' || code[i] >= L'_'; i++); return i - off; }
	//hexadecimal = [0-9a-fA-F]+;
	size_t hexadecimal(const wchar_t* code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'f') || (code[i] >= L'A' && code[i] <= L'F') || (code[i] >= L'0' && code[i] <= L'9'); i++); return i - off; }
	//scalarsub = [0-9]+;
	size_t scalarsub(const wchar_t* code, size_t off) { size_t i; for (i = off; code[i] >= L'0' && code[i] <= L'9'; i++); return i - off; }
	//scalar = scalarsub(.scalarsub)?;
	size_t scalar(const wchar_t* code, size_t off) { size_t i = off + scalarsub(code, off); if (code[off] == L'.') i += scalarsub(code, off); return i - off; }
	//anytext = (?![ \t\r\n;])+;
	size_t anytext(const wchar_t* code, size_t off) { size_t i; for (i = off; code[i] != L' ' && code[i] != L'\t' && code[i] != L'\r' && code[i] != L'\n' && code[i] != L';'; i++); return i - off; }
	//SQF = { STATEMENT endchr{ endchr } }
	bool SQF_start(const wchar_t* code, size_t curoff) { return STATEMENT_start(code, curoff); }
	void SQF(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		skip(code, line, col, curoff);
		//Iterate over statements as long as it is an instruction start.
		while (STATEMENT_start(code, curoff))
		{
			STATEMENT(vm, cs, code, line, col, curoff, file);
			skip(code, line, col, curoff);
			//Make sure at least one endchr is available
			if (!endchr(code, curoff))
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected either ';' or ','.";
			}
			else
			{
				size_t endchrlen;
				//Add endchr up until no semicolon is left
				while ((endchrlen = endchr(code, curoff)) > 0)
				{
					curoff += endchrlen;
					col += endchrlen;
					skip(code, line, col, curoff);
				}
			}
		}
	}
	//STATEMENT = ASSIGNMENT | BINARYEXPRESSION;
	bool STATEMENT_start(const wchar_t* code, size_t curoff) { return ASSIGNMENT_start(code, curoff) | BINARYEXPRESSION_start(code, curoff); }
	void STATEMENT(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		if (ASSIGNMENT_start(code, curoff))
		{
			ASSIGNMENT(vm, cs, code, line, col, curoff, file);
		}
		else if (BINARYEXPRESSION_start(code, curoff))
		{
			BINARYEXPRESSION(vm, cs, code, line, col, curoff, file);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No viable alternative for STATEMENT.";
		}
	}
	//ASSIGNMENT(2) = identifier '=' BINARYEXPRESSION | "private" identifier '=' BINARYEXPRESSION;
	bool ASSIGNMENT_start(const wchar_t* code, size_t curoff)
	{
		size_t len;
		if (wstr_cmpi(code + curoff, compiletime::strlen(L"private"), L"private", compiletime::strlen(L"private")) == 0)
		{
			curoff += compiletime::strlen(L"private");
			skip(code, curoff);
		}
		if ((len = identifier(code, curoff)) > 0)
		{
			curoff += len;
			skip(code, curoff);
			return code[curoff] == L'=';
		}
		else
		{
			return false;
		}
	}
	void ASSIGNMENT(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		size_t len;
		bool assignlocal = false;
		//check if prefixed by a 'private'
		if (wstr_cmpi(code + curoff, compiletime::strlen(L"private"), L"private", compiletime::strlen(L"private")) == 0)
		{
			curoff += compiletime::strlen(L"private");
			col += compiletime::strlen(L"private");
			skip(code, line, col, curoff);
			assignlocal = true;
		}
		//receive the ident
		len = identifier(code, curoff);
		auto ident = std::wstring(code + curoff, code + curoff + len);
		if (assignlocal && ident[0] != L'_')
		{
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, len) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Private variables need to be prefixed with an underscore '_'.";
		}
		curoff += len;
		skip(code, line, col, curoff);
		//store the dbginf for the '='
		size_t dbgstart = curoff;
		size_t dbgcol = col;
		size_t dbgline = line;
		//skip the '=' (is confirmed to be present in ASSIGNMENT_start)
		curoff++;
		col++;
		skip(code, line, col, curoff);

		if (BINARYEXPRESSION_start(code, curoff))
		{
			BINARYEXPRESSION(vm, cs, code, line, col, curoff, file);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected start of BINARYEXPRESSION.";
		}
		if (assignlocal)
		{
			auto inst = std::make_shared<sqf::inst::assigntolocal>(ident);
			inst->setdbginf(dbgline, dbgcol, file, sqf::virtualmachine::dbgsegment(code, dbgstart, 1));
			cs->pushinst(inst);
		}
		else
		{
			auto inst = std::make_shared<sqf::inst::assignto>(ident);
			inst->setdbginf(dbgline, dbgcol, file, sqf::virtualmachine::dbgsegment(code, dbgstart, 1));
			cs->pushinst(inst);
		}
	}
	//BINARYEXPRESSION = BINARYEXPRESSION<codecontext> boperator BINARYEXPRESSION | PRIMARYEXPRESSION;
	bool BINARYEXPRESSION_start(const wchar_t* code, size_t curoff) { return PRIMARYEXPRESSION_start(code, curoff); }
	void BINARYEXPRESSION(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{

	}
	//BINARYEXPRESSIONINBRACKETS = '(' BINARYEXPRESSION ')';
	bool BINARYEXPRESSIONINBRACKETS_start(const wchar_t* code, size_t curoff) { return code[curoff] == L'('; }
	void BINARYEXPRESSIONINBRACKETS(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		curoff++;
		col++;
		skip(code, line, col, curoff);
		if (BINARYEXPRESSION_start(code, curoff))
		{
			BINARYEXPRESSION(vm, cs, code, line, col, curoff, file);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected start of BINARYEXPRESSION.";
		}
		skip(code, line, col, curoff);
		if (code[curoff] == L')')
		{
			curoff++;
			col++;
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected ')'.";
		}
	}
	//PRIMARYEXPRESSION = NUMBER | UNARYEXPRESSION | NULLAREXPRESSION | VARIABLE | STRING | CODE | BINARYEXPRESSIONINBRACKETS | ARRAY;
	bool PRIMARYEXPRESSION_start(const wchar_t* code, size_t curoff) { return NUMBER_start(code, curoff) || UNARYEXPRESSION_start(code, curoff) || NULLAREXPRESSION_start(code, curoff) || VARIABLE_start(code, curoff) || STRING_start(code, curoff) || CODE_start(code, curoff) || BINARYEXPRESSIONINBRACKETS_start(code, curoff) || ARRAY_start(code, curoff); }
	void PRIMARYEXPRESSION(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		if (NUMBER_start(code, curoff))
		{
			NUMBER(vm, cs, code, line, col, curoff, file);
		}
		else if(UNARYEXPRESSION_start(code, curoff))
		{
			UNARYEXPRESSION(vm, cs, code, line, col, curoff, file);
		}
		else if(NULLAREXPRESSION_start(code, curoff))
		{
			NULLAREXPRESSION(vm, cs, code, line, col, curoff, file);
		}
		else if(VARIABLE_start(code, curoff))
		{
			VARIABLE(vm, cs, code, line, col, curoff, file);
		}
		else if(STRING_start(code, curoff))
		{
			STRING(vm, cs, code, line, col, curoff, file);
		}
		else if(CODE_start(code, curoff))
		{
			CODE(vm, cs, code, line, col, curoff, file);
		}
		else if(BINARYEXPRESSIONINBRACKETS_start(code, curoff))
		{
			BINARYEXPRESSIONINBRACKETS(vm, cs, code, line, col, curoff, file);
		}
		else if(ARRAY_start(code, curoff))
		{
			ARRAY(vm, cs, code, line, col, curoff, file);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"No viable alternative for PRIMARYEXPRESSION.";
		}

	}
	//NULLAREXPRESSION = operator;
	bool NULLAREXPRESSION_start(const wchar_t* code, size_t curoff) { return sqf::commandmap::get().contains_n(std::wstring(code + curoff, code + curoff + operator_(code, curoff))); }
	void NULLAREXPRESSION(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		auto len = operator_(code, curoff);
		auto ident = std::wstring(code + curoff, code + curoff + len);

		auto inst = std::make_shared<sqf::inst::callnullar>(sqf::commandmap::get().get(ident));
		inst->setdbginf(line, col, file, sqf::virtualmachine::dbgsegment(code, curoff, len));
		cs->pushinst(inst);
		curoff += len;
		col += len;
	}
	//UNARYEXPRESSION = operator PRIMARYEXPRESSION;
	bool UNARYEXPRESSION_start(const wchar_t* code, size_t curoff) { return sqf::commandmap::get().contains_u(std::wstring(code + curoff, code + curoff + operator_(code, curoff))); }
	void UNARYEXPRESSION(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		size_t dbgstart = curoff;
		size_t dbgcol = col;
		size_t dbgline = line;

		auto len = operator_(code, curoff);
		auto ident = std::wstring(code + curoff, code + curoff + len);
		curoff += len;
		col += len;
		skip(code, line, col, curoff);

		if (PRIMARYEXPRESSION_start(code, curoff))
		{
			PRIMARYEXPRESSION(vm, cs, code, line, col, curoff, file);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected start of PRIMARYEXPRESSION.";
		}
		auto inst = std::make_shared<sqf::inst::callunary>(sqf::commandmap::get().getrange_u(ident));
		inst->setdbginf(dbgline, dbgcol, file, sqf::virtualmachine::dbgsegment(code, dbgstart, len));
		cs->pushinst(inst);
	}
	//NUMBER = ("0x" | '$') hexadecimal | scalar;
	bool NUMBER_start(const wchar_t* code, size_t curoff) { return code[curoff] == L'$' || (code[curoff] >= L'0' && code[curoff] <= L'9'); }
	void NUMBER(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		double number = 0;
		if (code[curoff] == L'$')
		{
			size_t i;
			for (i = curoff; code[i] >= L'0' && code[i] <= L'9' || code[i] >= L'A' && code[i] <= L'F' || code[i] >= L'a' && code[i] <= L'f'; i++);
			number = std::stol(std::wstring(code + curoff, code + i), 0, 16);
			col += i - curoff;
			curoff = i;
		}
		else if (code[curoff] == L'0' && code[curoff + 1] == L'x')
		{
			size_t i;
			for (i = curoff + 2; code[i] >= L'0' && code[i] <= L'9' || code[i] >= L'A' && code[i] <= L'F' || code[i] >= L'a' && code[i] <= L'f'; i++);
			number = std::stol(std::wstring(code + curoff, code + i), 0, 16);
			col += i - curoff;
			curoff = i;
		}
		else
		{
			size_t i = curoff;
			bool numhaddot = false;
			unsigned short numhadexp = 0;
			while (1)
			{
				if (code[i] >= L'0' && code[i] <= L'9')
				{
					i++;
					continue;
				}
				else if (!numhaddot && code[i] == L'.')
				{
					i++;
					numhaddot = true;
					continue;
				}
				else if (numhadexp == 0 && code[i] == L'e' || code[i] == L'E')
				{
					i++;
					numhadexp++;
					continue;
				}
				else if (numhadexp == 1 && code[i] == L'+' || code[i] == L'-')
				{
					i++;
					numhadexp++;
					continue;
				}
				else
				{
					break;
				}

			}
			number = std::stod(std::wstring(code + curoff, code + i));
			col += i - curoff;
			curoff = i;
		}
		cs->pushinst(std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(number)));
	}
	//VARIABLE = identifier;
	bool VARIABLE_start(const wchar_t* code, size_t curoff) { return identifier(code, curoff) > 0; }
	void VARIABLE(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		auto len = identifier(code, curoff);
		auto ident = std::wstring(code + curoff, code + curoff + len);

		auto inst = std::make_shared<sqf::inst::getvariable>(ident);
		inst->setdbginf(line, col, file, sqf::virtualmachine::dbgsegment(code, curoff, len));
		cs->pushinst(inst);
		curoff += len;
		col += len;
	}
	//STRING = '"' { any | "\"\"" } '"' | '\'' { any | "''" } '\'';
	bool STRING_start(const wchar_t* code, size_t curoff) { return code[curoff] == L'\'' || code[curoff] == L'"'; }
	void STRING(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		size_t i;
		auto startchr = code[curoff];
		col++;
		for (i = curoff + 1; code[i] != L'\0' && (code[i] != startchr || code[i + 1] == startchr); i++)
		{
			switch (code[i])
			{
			case L'\n':
				col = 0;
				line++;
				break;
			default:
				col++;
				break;
			}
		}
		auto str = sqf::stringdata::parse_from_sqf(std::wstring(code + curoff, code + i));
		cs->pushinst(std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(str)));
		curoff = i;
		curoff++;
		col++;
	}
	//CODE = "{" SQF "}";
	bool CODE_start(const wchar_t* code, size_t curoff) { return code[curoff] == L'{'; }
	void CODE(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		curoff++;
		col++;
		skip(code, line, col, curoff);

		auto newstack = std::make_shared<sqf::callstack>();

		if (SQF_start(code, curoff))
		{
			BINARYEXPRESSION(vm, newstack, code, line, col, curoff, file);
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected SQF start.";
		}

		if (code[curoff] == L'}')
		{
			curoff++;
			col++;
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected '}'.";
		}
		cs->pushinst(std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(newstack)));
	}
	//ARRAY = '[' [ BINARYEXPRESSION { ',' BINARYEXPRESSION } ] ']';
	bool ARRAY_start(const wchar_t* code, size_t curoff) { return code[curoff] == L'['; }
	void ARRAY(sqf::virtualmachine* vm, sqf::callstack_s cs, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, std::wstring file)
	{
		curoff++;
		col++;
		skip(code, line, col, curoff);
		if (BINARYEXPRESSION_start(code, curoff))
		{
			BINARYEXPRESSION(vm, cs, code, line, col, curoff, file);
			skip(code, line, col, curoff);
			while (code[curoff] == L',')
			{
				col++;
				curoff++;
				skip(code, line, col, curoff);

				if (BINARYEXPRESSION_start(code, curoff))
				{
					BINARYEXPRESSION(vm, cs, code, line, col, curoff, file);
					skip(code, line, col, curoff);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected BINARYEXPRESSION start.";
				}
			}
		}
		if (code[curoff] == L']')
		{
			curoff++;
			col++;
		}
		else
		{
			size_t i;
			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
			vm->err() << sqf::virtualmachine::dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"\t" << L"Expected ']'.";
		}
	}
}


void sqf::virtualmachine::parse_sqf(std::wstring codein)
{
	const wchar_t *code = codein.c_str();
	size_t line = 0;
	size_t col = 0;
	size_t curoff = 0;
	auto cs = std::make_shared<callstack>();
	SQF(this, cs, code, line, col, curoff);
}