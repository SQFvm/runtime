#include "full.h"

sqf::codedata::codedata(callstack_s cs)
{
	instruction_s inst;
	minsts = std::vector<instruction_s>(cs->inststacksize());
	for (size_t i = cs->inststacksize() - 1; i != ~0; i--)
	{
		inst = cs->popinst();
		minsts[i] = inst;
	}
}

std::wstring sqf::codedata::tosqf(void) const
{
	return std::wstring();
}
