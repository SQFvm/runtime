#include "vmstack.h"
#include "value.h"
#include "virtualmachine.h"
#include "callstack.h"
#include "instruction.h"

std::vector<sqf::vmstack::stackdump> sqf::vmstack::dump_callstack_diff(std::shared_ptr<sqf::callstack> target)
{
	std::vector<sqf::vmstack::stackdump> vec;

	auto start = this->m_stacks.rbegin();
	for (auto it = start; it != this->m_stacks.rend(); it++)
	{
		auto inst = (*it)->current_instruction();

		stackdump dump;
		dump.namespace_used = (*it)->get_namespace();
		if (inst)
		{
			dump.dbginf = inst->dbginf("STT");
			dump.line = inst->line();
			dump.column = inst->col();
			dump.file = inst->file();
		}
		else
		{
			dump.dbginf = "NA";
			dump.line = -1;
			dump.column = -1;
			dump.file = "";
		}
		dump.callstack_name = (*it)->get_name();
		dump.scope_name = (*it)->get_scopename();
		vec.push_back(dump);

		if ((*it) == target)
		{
			break;
		}
	}
	return vec;
}

void sqf::vmstack::push_back(sqf::virtualmachine * vm, std::shared_ptr<instruction> inst)
{
	if (m_stacks.empty())
	{
		m_stacks.push_back(std::make_shared<callstack>(vm->missionnamespace()));
	}
	m_stacks.back()->push_back(std::move(inst));
}

sqf::value sqf::vmstack::get_variable(std::string_view varname, bool& ref_success)
{
	for (auto it = stacks_begin(); it != stacks_end(); ++it)
	{
		if (it->get()->has_variable(varname))
		{
			ref_success = true;
			return it->get()->get_variable(varname);
		}
	}
	ref_success = false;
	return sqf::value();
}

void sqf::vmstack::sleep(std::chrono::milliseconds ms)
{
	m_wakeup_stamp = sqf::virtualmachine::system_time() + ms;
	m_is_asleep = true;
}
