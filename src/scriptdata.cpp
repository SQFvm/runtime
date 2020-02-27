#include "scriptdata.h"
#include "vmstack.h"
#include "callstack.h"


sqf::scriptdata::scriptdata()
{
	static size_t scriptnum = 0;
	m_script_id = ++scriptnum;
	m_vmstack = std::make_shared<sqf::vmstack>(true);
	auto cs = std::make_shared<callstack>();
	cs->set_variable("_thisScript", this);
	m_vmstack->push_back(cs);
}

bool sqf::scriptdata::is_done() const
{
	return m_vmstack->empty();
}

std::string sqf::scriptdata::tosqf() const
{
	if (this->m_vmstack->script_name().empty())
	{
		return "SCRIPT";
	}
	else
	{
		return this->m_vmstack->script_name();
	}
}
