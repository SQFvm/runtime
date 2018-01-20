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
	sstream << std::wstring(full + i, full + i + len) << std::endl
		<< std::wstring(length, L' ') << L'^' << std::endl;
	return sstream.str();
}

