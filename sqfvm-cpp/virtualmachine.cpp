#include "full.h"
#include <iostream>
#include <cwctype>
#include <sstream>

sqf::virtualmachine::virtualmachine(unsigned long long maxinst)
{
	mout = &std::wcout;
	mwrn = &std::wcerr;
	merr = &std::wcerr;
	minstcount = 0;
	mmaxinst = maxinst;
}

void sqf::virtualmachine::execute(void)
{
	instruction_s inst;
	while ((inst = mstack->popinst()).get())
	{
		minstcount++;
		if (mmaxinst != 0 && mmaxinst == minstcount)
		{
			err() << L"MAX INST COUNT REACHED (" << mmaxinst << L")" << std::endl;
			break;
		}
		inst->execute(this);
	}
}
std::wstring segment(std::wstring & full, size_t off, size_t length)
{
	auto sstream = std::wstringstream();
	size_t i = off - 15;
	size_t len = 30 + length;
	if (i < 0)
	{
		len += i;
		i = 0;
	}
	for (size_t j = i; j < i + len; j++)
	{
		wchar_t wc = full[j];
		if (wc == L'\0' || wc == L'\n')
		{
			if (j < off)
			{
				i = j + 1;
			}
			else
			{
				len = j - i;
				break;
			}
		}
	}
	sstream << full.substr(i, len) << std::endl
		<< std::wstring(length, L' ') << L'^' << std::endl;
	return sstream.str();
}

void sqf::virtualmachine::parse_assembly(std::wstring codein)
{
	/*
	endStatement
	push <type> <value>
	callUnary <command>
	callBinary <command>
	assignToLocal <name>
	assignTo <name>
	callNular <name>
	getVariable <name>
	makeArray <size>
	*/
	const wchar_t *code = codein.c_str();
	const wchar_t *codend = codein.c_str() + codein.length();
	const wchar_t *fnd = code;
	size_t index = 0;
	size_t line = 0;
	size_t col;
	sqf::instruction_s inst;
	while (fnd++ != codend)
	{
		col++;
		if (*fnd == L'\n')
		{
			line++;
			col = 0;
		}
		if (*fnd != L';')
			continue;
		const wchar_t *start = fnd;
		//trim
		for (; !std::iswalnum(*fnd); fnd++);
		const wchar_t *end = fnd;
		//find cmd end
		for (; std::iswalnum(*end); end++);

		if (wstr_cmpi(fnd, -1, L"endStatement", -1) == 0)
		{//endStatement
			inst = std::make_shared<sqf::inst::endstatement>();
			inst->setdbginf(line, col, std::wstring(), segment(codein, fnd - code, end - fnd));
			col += fnd - start;
			stack()->pushinst(inst);
		}
		else if (wstr_cmpi(fnd, -1, L"push", -1) == 0)
		{//push <type> <value>
			//trim to type
			for (; !std::iswalnum(*fnd); fnd++);
			sqf::type t = sqf::type::NA;

		}
		else if (wstr_cmpi(fnd, -1, L"callUnary", -1) == 0)
		{//callUnary <command>
		}
		else if (wstr_cmpi(fnd, -1, L"callBinary", -1) == 0)
		{//callBinary <command>
		}
		else if (wstr_cmpi(fnd, -1, L"assignToLocal", -1) == 0)
		{//assignToLocal <name>
		}
		else if (wstr_cmpi(fnd, -1, L"assignToLocal", -1) == 0)
		{//assignTo <name>
		}
		else if (wstr_cmpi(fnd, -1, L"assignToLocal", -1) == 0)
		{//callNular <name>
		}
		else if (wstr_cmpi(fnd, -1, L"assignToLocal", -1) == 0)
		{//getVariable <name>
		}
		else if (wstr_cmpi(fnd, -1, L"assignToLocal", -1) == 0)
		{//makeArray <size>
		}
		else
		{
			err() << segment(codein, fnd - code, end - fnd) << std::endl
				<< L"[ERR][L" << line << L"|C" << col << L"\t" << L"Unknown assembly instruction.";
		}
	}
}

void sqf::virtualmachine::parse_sqf(std::wstring codein)
{
}
