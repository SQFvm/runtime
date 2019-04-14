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
#include "cmd.h"

sqf::codedata::codedata(std::shared_ptr<sqf::callstack> cs)
{
	minsts = std::vector<std::shared_ptr<sqf::instruction>>(cs->size_instructions());
	auto queue = cs->instruction_queue();
	size_t i = 0;
	while (!queue.empty())
	{
		minsts[i++] = queue.front();
		queue.pop();
	}
}

std::string rebuild_from_assembly_recursive(const sqf::codedata* data, int& index, short parent_precedence)
{
	for (; index >= 0; index--)
	{
		auto& inst = data->instructions()[index];
		auto insttype = inst->thistype();
		switch (insttype)
		{
		case sqf::instruction::push:
		{
			auto instpush = std::static_pointer_cast<sqf::inst::push>(inst);
			return instpush->value()->tosqf();
		} break;
		case sqf::instruction::assignto:
		{
			auto instassignto = std::static_pointer_cast<sqf::inst::assignto>(inst);
			auto expression = rebuild_from_assembly_recursive(data, --index, 0);
			return instassignto->variable_name() + " = " + expression;
		} break;
		case sqf::instruction::assigntolocal:
		{
			auto instassigntolocal = std::static_pointer_cast<sqf::inst::assigntolocal>(inst);
			auto expression = rebuild_from_assembly_recursive(data, --index, 0);
			return "private " + instassigntolocal->variable_name() + " = " + expression;
		} break;
		case sqf::instruction::callnular:
		{
			auto instcallnular = std::static_pointer_cast<sqf::inst::callnular>(inst);
			return instcallnular->command()->name();
		} break;
		case sqf::instruction::callunary:
		{
			auto instcallunary = std::static_pointer_cast<sqf::inst::callunary>(inst);

			auto expression = rebuild_from_assembly_recursive(data, --index, 10);
			return instcallunary->commands()->front()->name() + " " + expression;
		} break;
		case sqf::instruction::callbinary:
		{
			auto instcallbinary = std::static_pointer_cast<sqf::inst::callbinary>(inst);
			auto prec = instcallbinary->commands()->front()->precedence();
			auto rexpression = rebuild_from_assembly_recursive(data, --index, prec);
			auto lexpression = rebuild_from_assembly_recursive(data, --index, prec);
			if (parent_precedence > prec)
			{
				return "(" + lexpression + " " + instcallbinary->commands()->front()->name() + " " + rexpression + ")";
			}
			else
			{
				return lexpression + " " + instcallbinary->commands()->front()->name() + " " + rexpression;
			}
		} break;
		case sqf::instruction::endstatement:
		{
			// auto instendstatement = std::static_pointer_cast<sqf::inst::endstatement>(inst);
			return "; ";
		} break;
		case sqf::instruction::makearray:
		{
			auto instmakearray = std::static_pointer_cast<sqf::inst::makearray>(inst);
			std::stringstream sstream;
			sstream << "[";
			for (auto i = 0; i < instmakearray->size(); i++)
			{
				if (i > 0)
				{
					sstream << ", ";
				}
				auto expression = rebuild_from_assembly_recursive(data, --index, 0);
				sstream << expression;
			}
			sstream << "]";
			return sstream.str();
		} break;
		case sqf::instruction::getvariable:
		{
			auto instgetvariable = std::static_pointer_cast<sqf::inst::getvariable>(inst);
			return instgetvariable->variable_name();
		} break;
		}
	}
	return "";
}

std::string sqf::codedata::tosqf() const
{
	int index = this->minsts.size() - 1;
	std::vector<std::string> results;
	std::stringstream sstream;
	sstream << "{ ";
	while (index >= 0)
	{
		results.push_back(rebuild_from_assembly_recursive(this, index, 0));
		--index;
	}
	std::reverse(results.begin(), results.end());
	for (auto& str : results)
	{
		sstream << str;
	}
	sstream << " }";
	return sstream.str();
}

void sqf::codedata::loadinto(sqf::virtualmachine * vm, std::shared_ptr<sqf::vmstack> stack)
{
	auto cs = std::make_shared<callstack>(vm->missionnamespace());
	loadinto(stack, cs);
	stack->pushcallstack(cs);
}

void sqf::codedata::loadinto(std::shared_ptr<sqf::vmstack> stack, std::shared_ptr<sqf::callstack> cs)
{
	for (auto& it : minsts)
	{
		cs->push_back(it);
	}
}