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

std::string rebuild_from_assembly_recursive(const sqf::codedata* data, int& index, short parent_precedence, bool binary_left)
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
			return instpush->get_value().tosqf();
		} break;
		case sqf::instruction::assignto:
		{
			auto instassignto = std::static_pointer_cast<sqf::inst::assignto>(inst);
			auto expression = rebuild_from_assembly_recursive(data, --index, 0, false);
			return instassignto->variable_name() + " = " + expression;
		} break;
		case sqf::instruction::assigntolocal:
		{
			auto instassigntolocal = std::static_pointer_cast<sqf::inst::assigntolocal>(inst);
			auto expression = rebuild_from_assembly_recursive(data, --index, 0, false);
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

			auto expression = rebuild_from_assembly_recursive(data, --index, 10, false);
			return instcallunary->commands()->front()->name() + " " + expression;
		} break;
		case sqf::instruction::callbinary:
		{
			auto instcallbinary = std::static_pointer_cast<sqf::inst::callbinary>(inst);
			auto prec = instcallbinary->commands()->front()->precedence();
			auto rexpression = rebuild_from_assembly_recursive(data, --index, prec, false);
			auto lexpression = rebuild_from_assembly_recursive(data, --index, prec, true);
			if (binary_left ? parent_precedence > prec : parent_precedence >= prec)
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
			auto strvec = std::vector<std::string>(instmakearray->size());
			sstream << "[";
			for (size_t i = 0; i < instmakearray->size(); i++)
			{
				strvec[i] = rebuild_from_assembly_recursive(data, --index, 0, false);
			}
			for (auto it = strvec.rbegin(); it != strvec.rend(); it++)
			{
				if (it != strvec.rbegin())
				{
					sstream << ", ";
				}
				sstream << *it;
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
	int index = static_cast<int>(this->minsts.size()) - 1;
	std::vector<std::string> results;
	std::stringstream sstream;
	sstream << "{ ";
	while (index >= 0)
	{
		results.push_back(rebuild_from_assembly_recursive(this, index, 0, false));
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
	stack->push_back(cs);
}

void sqf::codedata::loadinto(std::shared_ptr<sqf::vmstack> stack, std::shared_ptr<sqf::callstack> cs)
{
	for (auto& it : minsts)
	{
		cs->push_back(it);
	}
}

bool sqf::codedata::equals(std::shared_ptr<data> d) const
{
	auto cd = std::dynamic_pointer_cast<codedata>(d);
	if (cd->minsts.size() != minsts.size())
	{
		return false;
	}
	for (size_t i = 0; i < minsts.size(); i++)
	{
		const auto l = cd->minsts[i];
		const auto r = minsts[i];
		if (l->thistype() != r->thistype())
		{
			return false;
		}
		switch (l->thistype())
		{
		case sqf::instruction::assignto:
		{
			const auto assignto_l = std::dynamic_pointer_cast<inst::assignto>(l);
			const auto assignto_r = std::dynamic_pointer_cast<inst::assignto>(r);
			if (assignto_l->variable_name() != assignto_r->variable_name())
			{
				return false;
			}
		} break;
		case sqf::instruction::assigntolocal:
		{
			const auto assigntolocal_l = std::dynamic_pointer_cast<inst::assigntolocal>(l);
			const auto assigntolocal_r = std::dynamic_pointer_cast<inst::assigntolocal>(r);
			if (assigntolocal_l->variable_name() != assigntolocal_r->variable_name())
			{
				return false;
			}
		} break;
		case sqf::instruction::callbinary:
		{
			const auto callbinary_l = std::dynamic_pointer_cast<inst::callbinary>(l);
			const auto callbinary_r = std::dynamic_pointer_cast<inst::callbinary>(r);
			if (callbinary_l->commands()->front()->name() != callbinary_r->commands()->front()->name())
			{
				return false;
			}
		} break;
		case sqf::instruction::callunary:
		{
			const auto callunary_l = std::dynamic_pointer_cast<inst::callunary>(l);
			const auto callunary_r = std::dynamic_pointer_cast<inst::callunary>(r);
			if (callunary_l->commands()->front()->name() != callunary_r->commands()->front()->name())
			{
				return false;
			}
		} break;
		case sqf::instruction::callnular:
		{
			const auto callnular_l = std::dynamic_pointer_cast<inst::callnular>(l);
			const auto callnular_r = std::dynamic_pointer_cast<inst::callnular>(r);
			if (callnular_l->command()->name() != callnular_r->command()->name())
			{
				return false;
			}
		} break;
		case sqf::instruction::getvariable:
		{
			const auto getvariable_l = std::dynamic_pointer_cast<inst::getvariable>(l);
			const auto getvariable_r = std::dynamic_pointer_cast<inst::getvariable>(r);
			if (getvariable_l->variable_name() != getvariable_r->variable_name())
			{
				return false;
			}
		} break;
		case sqf::instruction::makearray:
		{
			const auto makearray_l = std::dynamic_pointer_cast<inst::makearray>(l);
			const auto makearray_r = std::dynamic_pointer_cast<inst::makearray>(r);
			if (makearray_l->size() != makearray_r->size())
			{
				return false;
			}
		} break;
		case sqf::instruction::push:
		{
			const auto push_l = std::dynamic_pointer_cast<inst::push>(l);
			const auto push_r = std::dynamic_pointer_cast<inst::push>(r);
			if (!push_l->get_value().equals(push_r->get_value()))
			{
				return false;
			}
		}break;
		}
	}
	return true;
}