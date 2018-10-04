#include "codedata.h"
#include "value.h"
#include "instruction.h"
#include "instassignto.h"
#include "instassigntolocal.h"
#include "instcallbinary.h"
#include "instcallnular.h"
#include "instcallunary.h"
#include "instendstatement.h"
#include "instgetvariable.h"
#include "instmakearray.h"
#include "instpush.h"
#include "virtualmachine.h"

sqf::codedata::codedata(std::shared_ptr<sqf::callstack> cs)
{
	std::shared_ptr<sqf::instruction> inst;
	minsts = std::vector<std::shared_ptr<sqf::instruction>>(cs->inststacksize());
	for (size_t i = cs->inststacksize() - 1; i != (size_t)~0; i--)
	{
		//only valid for sqf::callstack!
		//all others might crash here
		inst = cs->popinst(0);
		minsts[i] = inst;
	}
}

std::string sqf::codedata::tosqf() const
{
	return "{...}";
	//ToDo: Mark values that belong to other instructions
	std::vector<size_t> marks;
	for (size_t i = minsts.size(); i != (size_t)~0; i--)
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
		case sqf::instruction::push:
		case sqf::instruction::callnular:
		case sqf::instruction::getvariable:
		case sqf::instruction::endstatement:
			break;
		}
	}
	for (size_t i = minsts.size(); i != (size_t)~0; i--)
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

void sqf::codedata::loadinto(sqf::virtualmachine * vm, std::shared_ptr<sqf::vmstack> stack)
{
	auto cs = std::make_shared<callstack>(vm->missionnamespace());
	loadinto(stack, cs);
	stack->pushcallstack(cs);
}

void sqf::codedata::loadinto(std::shared_ptr<sqf::vmstack> stack, std::shared_ptr<sqf::callstack> cs)
{
	for (auto it = minsts.rbegin(); it != minsts.rend(); it++)
	{
		cs->pushinst(*it);
	}
}