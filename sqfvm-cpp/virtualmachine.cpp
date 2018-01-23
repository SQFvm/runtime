#include <iostream>
#include <cwctype>
#include <sstream>

#include "full.h"
#include "astnode.h"
#include "parsesqf.h"
#include "sqfasttypes.h"


sqf::virtualmachine::virtualmachine(unsigned long long maxinst)
{
	mout = &std::wcout;
	mwrn = &std::wcerr;
	merr = &std::wcerr;
	minstcount = 0;
	mmaxinst = maxinst;
	mstack = std::make_shared<vmstack>();
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
std::wstring sqf::virtualmachine::dbgsegment(const wchar_t* full, size_t off, size_t length)
{
	auto sstream = std::wstringstream();
	size_t i = off < 15 ? 0 : off - 15;
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
	sstream << std::wstring(full + i, full + i + len) << std::endl
		<< std::wstring(off - i, L' ') << std::wstring(length, L'^') << std::endl;
	return sstream.str();
}
bool contains_nular(std::wstring ident)
{
	return sqf::commandmap::get().contains_n(ident);
}
bool contains_unary(std::wstring ident)
{
	return sqf::commandmap::get().contains_u(ident);
}
bool contains_binary(std::wstring ident)
{
	return sqf::commandmap::get().contains_b(ident);
}
short precedence(std::wstring s)
{
	auto srange = sqf::commandmap::get().getrange_b(s);
	if (srange->empty())
	{
		return 0;
	}
	return srange->begin()->get()->precedence();
}
const wchar_t* sqfasttypename(short id)
{
	switch (id)
	{
	case sqfasttypes::SQF: return L"SQF";
	case sqfasttypes::STATEMENT: return L"STATEMENT";
	case sqfasttypes::ASSIGNMENT: return L"ASSIGNMENT";
	case sqfasttypes::ASSIGNMENTLOCAL: return L"ASSIGNMENTLOCAL";
	case sqfasttypes::BINARYEXPRESSION: return L"BINARYEXPRESSION";
	case sqfasttypes::BINARYOP: return L"BINARYOP";
	case sqfasttypes::BRACKETS: return L"BRACKETS";
	case sqfasttypes::PRIMARYEXPRESSION: return L"PRIMARYEXPRESSION";
	case sqfasttypes::NULLAREXPRESSION: return L"NULLAREXPRESSION";
	case sqfasttypes::UNARYEXPRESSION: return L"UNARYEXPRESSION";
	case sqfasttypes::HEXNUMBER: return L"HEXNUMBER";
	case sqfasttypes::NUMBER: return L"NUMBER";
	case sqfasttypes::VARIABLE: return L"VARIABLE";
	case sqfasttypes::STRING: return L"STRING";
	case sqfasttypes::CODE: return L"CODE";
	case sqfasttypes::ARRAY: return L"ARRAY";
	default: return L"NA";
	}
}
void navigate(astnode node, unsigned short level = 0)
{
	if (level == 0)
	{
		std::wcout << L"Node Type;Offset;Length;Content" << std::endl;
	}
	std::wcout << std::wstring(level, L'\t') << sqfasttypename(node.kind) << L';' << node.offset << L';' << node.length << L';' << node.content << std::endl;
	for each (auto it in node.children)
	{
		navigate(it, level + 1);
	}
}

void sqf::virtualmachine::parse_sqf(std::wstring code)
{
	auto h = sqf::parse::helper(merr, dbgsegment, contains_nular, contains_unary, contains_binary, precedence);
	auto node = sqf::parse::parse_sqf(code.c_str(), h);
	navigate(node);
}