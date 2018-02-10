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
	return L"{...}";
	//ToDo: Mark values that belong to other instructions
	std::vector<size_t> marks;
	for (size_t i = minsts.size(); i != ~0; i--)
	{
		auto inst = minsts[i];
		auto insttype = inst->thistype();
		switch (insttype)
		{
		case sqf::instruction::callbinary:
			marks.push_back(i - 2);
		case sqf::instruction::callunary:
		case sqf::instruction::assignto:
		case sqf::instruction::assigntolocal:
			marks.push_back(i - 1);
			break;
		case sqf::instruction::makearray:
		{
			auto instmakearray = std::static_pointer_cast<sqf::inst::makearray>(inst);
			marks.push_back(instmakearray->size());
		} break;
		}
	}
	for (size_t i = minsts.size(); i != ~0; i--)
	{
		auto inst = minsts[i];
		auto insttype = inst->thistype();
		switch (insttype)
		{
		case sqf::instruction::push:
		{
			auto instpush = std::static_pointer_cast<sqf::inst::push>(inst);
		} break;
		case sqf::instruction::assignto:
		{
			auto instassignto = std::static_pointer_cast<sqf::inst::assignto>(inst);
		} break;
		case sqf::instruction::assigntolocal:
		{
			auto instassigntolocal = std::static_pointer_cast<sqf::inst::assigntolocal>(inst);
		} break;
		case sqf::instruction::callnular:
		{
			auto instcallnular = std::static_pointer_cast<sqf::inst::callnular>(inst);
		} break;
		case sqf::instruction::callunary:
		{
			auto instcallunary = std::static_pointer_cast<sqf::inst::callunary>(inst);
		} break;
		case sqf::instruction::callbinary:
		{
			auto instcallbinary = std::static_pointer_cast<sqf::inst::callbinary>(inst);
		} break;
		case sqf::instruction::endstatement:
		{
			auto instendstatement = std::static_pointer_cast<sqf::inst::endstatement>(inst);
		} break;
		case sqf::instruction::makearray:
		{
			auto instmakearray = std::static_pointer_cast<sqf::inst::makearray>(inst);
		} break;
		case sqf::instruction::getvariable:
		{
			auto instgetvariable = std::static_pointer_cast<sqf::inst::getvariable>(inst);
		} break;
		}
	}
}

void sqf::codedata::loadinto(vmstack_s stack, callstack_s cs)
{
	for (auto it = minsts.rbegin(); it != minsts.rend(); it++)
	{
		cs->pushinst(*it);
	}
}