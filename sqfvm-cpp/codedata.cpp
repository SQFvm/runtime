#include "full.h"

sqf::codedata::codedata(callstack_s cs)
{
	instruction_s inst;
	minsts = std::vector<instruction_s>(cs->inststacksize());
	for (size_t i = cs->inststacksize(); i != ~0; i--)
	{
		inst = cs->popinst();
		minsts[i] = inst;
	}
}

std::wstring sqf::codedata::to_string(void) const
{
	return std::wstring();
}
