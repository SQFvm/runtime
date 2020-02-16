#ifdef _WIN32
// Required due to some headers using WinSock2.h
// & some headers requiring windows.h
// If this was not here, a link conflict would emerge due to
// windows.h including winsock1
#include <WinSock2.h>
#endif

#include "virtualmachine.h"
#include "parsing/astnode.h"
#include "parsing/debugsegment.h"
#include "parsing/parsesqf.h"
#include "parsing/parseconfig.h"
#include "parsing/parsepreprocessor.h"
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
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "configdata.h"
#include "value.h"
#include "sidedata.h"
#include "groupdata.h"
#include "scriptdata.h"
#include "callstack_sqftry.h"
#include "sqfnamespace.h"
#include "innerobj.h"
#include "scalardata.h"
#include "networking/network_client.h"
#include "networking/network_server.h"
//#include "parsepp_handler.h"

#include <iostream>
#include <cwctype>
#include <sstream>
#include <algorithm>
#include <utility>
#include <stringdata.h>

// #define DEBUG_VM_ASSEMBLY

#if !defined(_DEBUG) && defined(DEBUG_VM_ASSEMBLY)
#undef DEBUG_VM_ASSEMBLY
#endif // !_RELEASE




sqf::virtualmachine::virtualmachine(Logger& logger, unsigned long long maxinst) : CanLog(logger)
{
	m_instructions_count = 0;
	m_max_instructions = maxinst;
	m_main_vmstack = std::make_shared<vmstack>();
	m_active_vmstack = m_main_vmstack;
	mmissionnamespace = std::make_shared<sqf::sqfnamespace>("missionNamespace");
	muinamespace = std::make_shared< sqf::sqfnamespace>("uiNamespace");
	mparsingnamespace = std::make_shared<sqf::sqfnamespace>("parsingNamespace");
	mprofilenamespace = std::make_shared<sqf::sqfnamespace>("profileNamespace");
	m_perform_classname_checks = true;
	m_exit_flag = false;
	m_allow_suspension = true;
	m_allow_networking = true;
	m_status = vmstatus::empty;
	m_runtime_error = false;
	mplayer_obj = innerobj::create(this, "CAManBase", false);
	m_created_timestamp = system_time();
	m_current_time = system_time();
	m_run_atomic = false;
	m_last_breakpoint_line_hit = -1;
}
sqf::virtualmachine::~virtualmachine()
{
	if (m_current_networking_client)
	{
		m_current_networking_client->stop();
	}
	if (m_current_networking_server)
	{
		m_current_networking_server->stop();
	}
}
void sqf::virtualmachine::release_networking()
{
	{
		if (m_current_networking_client)
		{
			m_current_networking_client->stop();
		}
		if (m_current_networking_server)
		{
			m_current_networking_server->stop();
		}
	}
}
void sqf::virtualmachine::execute_helper_execution_abort()
{
	while (!m_main_vmstack->empty()) { m_main_vmstack->drop_callstack(); }
	for (auto& it : mspawns)
	{
		while (!it->stack()->empty()) { it->stack()->drop_callstack(); }
	}
	mspawns.clear();
}
bool sqf::virtualmachine::execute_helper_execution_end()
{
	if (m_exit_flag)
	{
		execute_helper_execution_abort();
		return true;
	}
	if (this->m_main_vmstack->stacks_size() == 0 && this->mspawns.size() == 0)
	{
		return true;
	}
	return false;
}
sqf::virtualmachine::execresult sqf::virtualmachine::execute(execaction action)
{
	execresult res = execresult::invalid;
	bool expected = false;
	switch (action)
	{
	case sqf::virtualmachine::execaction::leave_scope:
		if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
		{
			m_exit_flag = false;
			auto scopeNum = m_active_vmstack->stacks_size() - 1;
			bool flag = true;
			m_status = vmstatus::running;
			while (m_status == vmstatus::running && !execute_helper_execution_end())
			{
				flag = performexecute(1);
				if (!flag)
				{
					break;
				}
				if (m_active_vmstack->stacks_size() <= scopeNum)
				{
					break;
				}
			}
			if (m_status == vmstatus::requested_abort)
			{
				execute_helper_execution_abort();
				m_status = vmstatus::empty;
			}
			else if (flag)
			{
				m_status = this->m_main_vmstack->stacks_size() == 0 && this->mspawns.size() == 0 ? vmstatus::empty : vmstatus::halted;
				res = execresult::OK;
			}
			else
			{
				m_status = vmstatus::halt_error;
				res = execresult::runtime_error;
			}
			m_run_atomic = false;
		}
		else
		{
			res = execresult::action_error;
		}
		break;
	case sqf::virtualmachine::execaction::start:
		if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
		{
			m_exit_flag = false;
			bool flag = true;
			m_status = vmstatus::running;
			while (m_status == vmstatus::running && !execute_helper_execution_end())
			{
				if (is_networking_set())
				{
					handle_networking();
				}
				m_active_vmstack = m_main_vmstack;
				flag = performexecute(150);
				if (!flag)
				{
					break;
				}
				for (auto& it : mspawns)
				{
					m_active_vmstack = it->stack();
					if (m_active_vmstack->asleep())
					{
						if (m_active_vmstack->get_wakeupstamp() <= virtualmachine::system_time())
						{
							m_active_vmstack->wakeup();
						}
						else
						{
							continue;
						}
					}
					flag = performexecute(150);
					if (!flag)
					{
						break;
					}
				}
				mspawns.remove_if([](std::shared_ptr<scriptdata> it) { return it->hasfinished() || it->stack()->terminate(); });
			}
			if (m_status == vmstatus::requested_abort)
			{
				execute_helper_execution_abort();
				m_status = vmstatus::empty;
			}
			else if (flag)
			{
				m_status = this->m_main_vmstack->stacks_size() == 0 && this->mspawns.size() == 0 ? vmstatus::empty : vmstatus::halted;
				res = execresult::OK;
			}
			else 
			{
				m_status = vmstatus::halt_error;
				res = execresult::runtime_error;
			}
			m_run_atomic = false;
		}
		else
		{
			res = execresult::action_error;
		}
		break;
	case sqf::virtualmachine::execaction::assembly_step:
		if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
		{
			m_exit_flag = false;
			m_status = vmstatus::running;
			if (m_status == vmstatus::requested_abort)
			{
				execute_helper_execution_abort();
				m_status = vmstatus::empty;
			}
			else if (performexecute(1))
			{
				m_status = this->m_main_vmstack->stacks_size() == 0 && this->mspawns.size() == 0 ? vmstatus::empty : vmstatus::halted;
				res = execresult::OK;
			}
			else 
			{
				m_status = vmstatus::halt_error;
				res = execresult::runtime_error;
			}
			m_run_atomic = false;
		}
		else
		{
			res = execresult::action_error;
		}
		break;



	case sqf::virtualmachine::execaction::stop:
		if (m_status != vmstatus::running)
		{
			res = execresult::action_error;
		}
		else if (!m_run_atomic)
		{
			res = execresult::action_error;
		}
		else
		{
			m_status = vmstatus::requested_halt;
			res = execresult::OK;
		}
		break;
	case sqf::virtualmachine::execaction::abort:
		if (m_status == vmstatus::running)
		{
			if (!m_run_atomic)
			{
				res = execresult::action_error;
			}
			else
			{
				m_status = vmstatus::requested_abort;
				res = execresult::OK;
			}
		}
		else if (m_status == vmstatus::halt_error || m_status == vmstatus::halted)
		{
			if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
			{
				execute_helper_execution_abort();
				m_status = vmstatus::empty;
				res = execresult::OK;
				m_run_atomic = false;
			}
			else
			{
				res = execresult::action_error;
			}
		}
		else
		{
			res = execresult::action_error;
		}
		break;
	case sqf::virtualmachine::execaction::reset_run_atomic:
		m_run_atomic = false;
		break;
	default:
		res = execresult::action_error;
		break;
	}
	return res;
}
bool sqf::virtualmachine::performexecute(size_t exitAfter)
{
	while (
		!m_exit_flag &&
		exitAfter != 0 &&
		!m_active_vmstack->asleep() &&
		!m_active_vmstack->terminate() &&
		m_status == sqf::virtualmachine::vmstatus::running &&
		(m_current_instruction = m_active_vmstack->pop_back_instruction(this)).get() &&
		m_active_vmstack->stacks_top()->previous_nextresult() != sqf::callstack::nextinstres::suspend
		)
	{
		m_instructions_count++;
		if (exitAfter > 0)
		{
			exitAfter--;
		}
		if (m_max_instructions != 0 && m_max_instructions == m_instructions_count)
		{
			logmsg(logmessage::runtime::MaximumInstructionCountReached(*m_current_instruction, static_cast<size_t>(m_max_instructions)));
			return false;
		}
#ifdef DEBUG_VM_ASSEMBLY
		(*mout) << m_current_instruction->to_string() << std::endl;
#endif
		m_current_instruction->execute(this);
#ifdef DEBUG_VM_ASSEMBLY
		bool success;
		std::vector<sqf::value> vals;
		do {
			auto val = m_active_vmstack->popval(success);
			if (success)
			{
				vals.push_back(val);
				std::cout << "[WORK]\t<" << sqf::type_str(val.dtype()) << ">\t" << val.tosqf() << std::endl;
			}
		} while (success);
		while (!vals.empty())
		{
			auto it = vals.back();
			vals.pop_back();
			m_active_vmstack->pushval(it);
		}
#endif
		if (m_runtime_error)
		{
			m_runtime_error = false;
			// Try to find a callstack_sqftry
			auto res = std::find_if(m_active_vmstack->stacks_begin(), m_active_vmstack->stacks_end(), [](std::shared_ptr<sqf::callstack> cs) -> bool {
				return cs->can_recover();
			});
			if (res == m_active_vmstack->stacks_end())
			{
				m_runtime_error = false;
				// Only for non-scheduled (and thus the mainstack)
				if (!m_active_vmstack->scheduled())
				{
					// ToDo: Move stackdump into its own file and then use that in logging message instead of std::string
					std::stringstream sstream;
					sstream << "Stacktrace:" << std::endl;
					auto stackdump = m_active_vmstack->dump_callstack_diff({});
					int i = 1;
					for (auto& it : stackdump)
					{
						sstream << i++ << ":\tnamespace: " << it.namespace_used->get_name()
							<< "\tscopename: " << it.scope_name
							<< "\tcallstack: " << it.callstack_name
							<< std::endl << it.dbginf << std::endl;
					}
					log(logmessage::runtime::Stacktrace(*m_current_instruction, sstream.str()));
					return false;
				}
			}
			else
			{
				auto sqftry = std::dynamic_pointer_cast<sqf::callstack_sqftry>(*res);
				auto stackdump = m_active_vmstack->dump_callstack_diff(sqftry);
				auto sqfarr = std::make_shared<arraydata>();
				for (auto& it : stackdump)
				{
					std::vector<sqf::value> vec = {
							sqf::value(it.namespace_used->get_name()),
							sqf::value(it.scope_name),
							sqf::value(it.callstack_name),
							sqf::value(it.line),
							sqf::value(it.column),
							sqf::value(it.file),
							sqf::value(it.dbginf)
						};
					sqfarr->push_back(sqf::value(std::make_shared<arraydata>(vec)));
				}
				while (m_active_vmstack->stacks_top() != sqftry)
				{
					m_active_vmstack->drop_callstack();
				}
				sqftry->except(sqfarr);
			}
		}
		// Check if breakpoint was hit
		{
			auto line = m_current_instruction->line();
			for (const auto& breakpoint : m_breakpoints)
			{
				if (breakpoint.is_enabled() && breakpoint.line() == line)
				{
					m_last_breakpoint_line_hit = line;
					m_status = vmstatus::halted;
					return true;
				}
			}
		}
	}
	return true;
}
bool contains_nular(std::string_view ident)
{
	return sqf::commandmap::get().contains_n(ident);
}
bool contains_unary(std::string_view ident)
{
	return sqf::commandmap::get().contains_u(ident);
}
bool contains_binary(std::string_view ident, short p)
{
	auto flag = sqf::commandmap::get().contains_b(ident);
	if (flag && p > 0)
	{
		auto cmds = sqf::commandmap::get().getrange_b(ident);
		return cmds->front()->precedence() == p;
	}
	return flag;
}
short precedence(std::string_view s)
{
	auto srange = sqf::commandmap::get().getrange_b(s);
	if (!srange.get() || srange->empty())
	{
		return 0;
	}
	return srange->begin()->get()->precedence();
}
void sqf::virtualmachine::navigate_sqf(const char* full, std::shared_ptr<sqf::callstack> stack, const sqf::parse::astnode& node, bool& errorflag)
{
	execute_parsing_callbacks(full, node, evaction::enter);
	switch (node.kind)
	{
		case (short)sqf::parse::asttype::sqf::BEXP1:
		case (short)sqf::parse::asttype::sqf::BEXP2:
		case (short)sqf::parse::asttype::sqf::BEXP3:
		case (short)sqf::parse::asttype::sqf::BEXP4:
		case (short)sqf::parse::asttype::sqf::BEXP5:
		case (short)sqf::parse::asttype::sqf::BEXP6:
		case (short)sqf::parse::asttype::sqf::BEXP7:
		case (short)sqf::parse::asttype::sqf::BEXP8:
		case (short)sqf::parse::asttype::sqf::BEXP9:
		case (short)sqf::parse::asttype::sqf::BEXP10:
		case (short)sqf::parse::asttype::sqf::BINARYEXPRESSION:
		{
			navigate_sqf(full, stack, node.children[0], errorflag);
			navigate_sqf(full, stack, node.children[2], errorflag);
			auto inst = std::make_shared<sqf::inst::callbinary>(sqf::commandmap::get().getrange_b(node.children[1].content));
			inst->setdbginf(node.children[1].line, node.children[1].col, node.file, sqf::parse::dbgsegment(full, node.children[1].offset, node.children[1].length));
			stack->push_back(inst);
		}
		break;
		case (short)sqf::parse::asttype::sqf::UNARYEXPRESSION:
		{
			navigate_sqf(full, stack, node.children[1], errorflag);
			auto inst = std::make_shared<sqf::inst::callunary>(sqf::commandmap::get().getrange_u(node.children[0].content));
			inst->setdbginf(node.children[0].line, node.children[0].col, node.file, sqf::parse::dbgsegment(full, node.children[0].offset, node.children[0].length));
			stack->push_back(inst);
		}
		break;
		case (short)sqf::parse::asttype::sqf::NULAROP:
		{
			auto inst = std::make_shared<sqf::inst::callnular>(sqf::commandmap::get().get(node.content));
			inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
			stack->push_back(inst);
		}
		break;
		case (short)sqf::parse::asttype::sqf::HEXNUMBER:
		{
			try
			{
				auto inst = std::make_shared<sqf::inst::push>(sqf::value(std::stol(node.content, nullptr, 16)));
				inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
				stack->push_back(inst);
			}
			catch (std::out_of_range&)
			{
				auto inst = std::make_shared<sqf::inst::push>(sqf::value(std::make_shared<sqf::scalardata>(std::nanf(""))));
				inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
				log(logmessage::assembly::NumberOutOfRange(*inst));
				stack->push_back(inst);
			}
		}
		break;
		case (short)sqf::parse::asttype::sqf::NUMBER:
		{
			try
			{
				auto inst = std::make_shared<sqf::inst::push>(sqf::value(std::stod(node.content)));
				inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
				stack->push_back(inst);
			}
			catch (std::out_of_range&)
			{
				auto inst = std::make_shared<sqf::inst::push>(sqf::value(std::make_shared<sqf::scalardata>(std::nanf(""))));
				inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
				log(logmessage::assembly::NumberOutOfRange(*inst));
				stack->push_back(inst);
			}
		}
		break;
		case (short)sqf::parse::asttype::sqf::STRING:
		{
			auto inst = std::make_shared<sqf::inst::push>(sqf::value(std::make_shared<sqf::stringdata>(node.content, true)));
			inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
			stack->push_back(inst);
		}
		break;
		case (short)sqf::parse::asttype::sqf::CODE:
		{
			auto cs = std::make_shared<sqf::callstack>(missionnamespace());
			for (size_t i = 0; i < node.children.size(); i++)
			{
				if (i != 0)
				{
					auto inst = std::make_shared<sqf::inst::endstatement>();
					inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
					cs->push_back(inst);
				}
				auto subnode = node.children[i];
				navigate_sqf(full, cs, subnode, errorflag);
			}
			auto inst = std::make_shared<sqf::inst::push>(sqf::value(cs));
			inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
			stack->push_back(inst);
		}
		break;
		case (short)sqf::parse::asttype::sqf::ARRAY:
		{
			for (auto& subnode : node.children)
			{
				navigate_sqf(full, stack, subnode, errorflag);
			}
			auto inst = std::make_shared<sqf::inst::makearray>(node.children.size());
			inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
			stack->push_back(inst);
		}
		break;
		case (short)sqf::parse::asttype::sqf::ASSIGNMENT:
		{
			navigate_sqf(full, stack, node.children[1], errorflag);
			auto inst = std::make_shared<sqf::inst::assignto>(node.children[0].content);
			inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
			stack->push_back(inst);
		}
		break;
		case (short)sqf::parse::asttype::sqf::ASSIGNMENTLOCAL:
		{
			navigate_sqf(full, stack, node.children[1], errorflag);
			auto inst = std::make_shared<sqf::inst::assigntolocal>(node.children[0].content);
			inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
			stack->push_back(inst);
		}
		break;
		case (short)sqf::parse::asttype::sqf::VARIABLE:
		{
			auto inst = std::make_shared<sqf::inst::getvariable>(node.content);
			inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
			stack->push_back(inst);
		}
		break;
		default:
		{
			for (size_t i = 0; i < node.children.size(); i++)
			{
				if (i != 0)
				{
					auto inst = std::make_shared<sqf::inst::endstatement>();
					inst->setdbginf(node.line, node.col, node.file, sqf::parse::dbgsegment(full, node.offset, node.length));
					stack->push_back(inst);
				}
				auto subnode = node.children[i];
				navigate_sqf(full, stack, subnode, errorflag);
			}
		}
	}
	execute_parsing_callbacks(full, node, evaction::exit);
}
void navigate_pretty_print_sqf(std::stringstream& sstream, const char* full, sqf::virtualmachine* vm, sqf::parse::astnode& node, size_t depth)
{
	switch (node.kind)
	{
		case (short)sqf::parse::asttype::sqf::BEXP1:
		case (short)sqf::parse::asttype::sqf::BEXP2:
		case (short)sqf::parse::asttype::sqf::BEXP3:
		case (short)sqf::parse::asttype::sqf::BEXP4:
		case (short)sqf::parse::asttype::sqf::BEXP5:
		case (short)sqf::parse::asttype::sqf::BEXP6:
		case (short)sqf::parse::asttype::sqf::BEXP7:
		case (short)sqf::parse::asttype::sqf::BEXP8:
		case (short)sqf::parse::asttype::sqf::BEXP9:
		case (short)sqf::parse::asttype::sqf::BEXP10:
		case (short)sqf::parse::asttype::sqf::BINARYEXPRESSION:
		{
			navigate_pretty_print_sqf(sstream, full, vm, node.children[0], depth);
			sstream << ' ' << node.children[1].content << ' ';
			navigate_pretty_print_sqf(sstream, full, vm, node.children[2], depth);
		}
		break;
		case (short)sqf::parse::asttype::sqf::UNARYEXPRESSION:
		{
			sstream << node.children[0].content << ' ';
			navigate_pretty_print_sqf(sstream, full, vm, node.children[1], depth);
		}
		break;
		case (short)sqf::parse::asttype::sqf::NUMBER:
		case (short)sqf::parse::asttype::sqf::HEXNUMBER:
		case (short)sqf::parse::asttype::sqf::NULAROP:
		case (short)sqf::parse::asttype::sqf::STRING:
		case (short)sqf::parse::asttype::sqf::VARIABLE:
		{
			sstream << node.content;
		}
		break;
		case (short)sqf::parse::asttype::sqf::BRACKETS:
		{
			sstream << "(";
			navigate_pretty_print_sqf(sstream, full, vm, node.children[0], depth);
			sstream << ")";
		}
		break;
		case (short)sqf::parse::asttype::sqf::CODE:
		{
			sstream << "{" << std::endl;
			depth++;
			for (auto& subnode : node.children)
			{
				sstream << std::string(depth * 4, ' ');
				navigate_pretty_print_sqf(sstream, full, vm, subnode, depth);
				sstream << ";" << std::endl;
			}
			depth--;
			sstream << std::string(depth * 4, ' ') << "}";
		}
		break;
		case (short)sqf::parse::asttype::sqf::ARRAY:
		{
			sstream << "[";
			bool flag = false;
			for (auto& subnode : node.children)
			{
				if (flag)
				{
					sstream << ", ";
				}
				else
				{
					flag = true;
				}
				navigate_pretty_print_sqf(sstream, full, vm, subnode, depth);
			}
			sstream << "]";
		}
		break;
		case (short)sqf::parse::asttype::sqf::ASSIGNMENT:
		{
			sstream << node.children[0].content << " = ";
			navigate_pretty_print_sqf(sstream, full, vm, node.children[1], depth);
		}
		break;
		case (short)sqf::parse::asttype::sqf::ASSIGNMENTLOCAL:
		{
			sstream << "private " << node.children[0].content << " = ";
			navigate_pretty_print_sqf(sstream, full, vm, node.children[1], depth);
		}
		break;
		default:
		{
			for (auto& subnode : node.children)
			{
				sstream << std::string(depth, '\t');
				navigate_pretty_print_sqf(sstream, full, vm, subnode, depth);
				sstream << ";" << std::endl;
			}
		}
	}
}
sqf::parse::astnode sqf::virtualmachine::parse_sqf_cst(std::string_view code, bool& errorflag, std::string filename)
{
	auto parser = sqf::parse::sqf(get_logger(), contains_nular, contains_unary, contains_binary, precedence, code, filename);
	return parser.parse(errorflag);
}
void sqf::virtualmachine::parse_sqf_tree(std::string_view code, std::stringstream* sstream)
{
	auto parser = sqf::parse::sqf(get_logger(), contains_nular, contains_unary, contains_binary, precedence, code, ""sv);
	bool errorflag = false;
	auto node = parser.parse(errorflag);
	print_navigate_ast(sstream, node, sqf::parse::sqf::astkindname);
}
bool sqf::virtualmachine::parse_sqf(std::shared_ptr<sqf::vmstack> vmstck, std::string_view code, std::shared_ptr<sqf::callstack> cs, std::string filename)
{
	if (!cs.get())
	{
		cs = std::make_shared<sqf::callstack>(this->missionnamespace());
		vmstck->push_back(cs);
	}
	auto parser = sqf::parse::sqf(get_logger(), contains_nular, contains_unary, contains_binary, precedence, code, filename);
	bool errorflag = false;
	auto node = parser.parse(errorflag);
	if (!errorflag)
	{
		navigate_sqf(code.data(), cs, node, errorflag);
	}
	return errorflag;
}
std::string sqf::virtualmachine::preprocess(std::string input, bool& errflag, std::string filename)
{
	auto parser = sqf::parse::preprocessor(get_logger(), this);
	auto parsedcontents = parser.parse(this, std::move(input), errflag, filename);
	return parsedcontents;
}
std::string sqf::virtualmachine::pretty_print_sqf(std::string_view code)
{
	auto parser = sqf::parse::sqf(get_logger(), contains_nular, contains_unary, contains_binary, precedence, code, "");
	bool errorflag = false;
	auto node = parser.parse(errorflag);
	if (!errorflag)
	{
		std::stringstream sstream;
		navigate_pretty_print_sqf(sstream, code.data(), this, node, 0);
		return sstream.str();
	}
	return "";
}
void navigate_config(const char* full, sqf::virtualmachine* vm, std::shared_ptr<sqf::configdata> parent, sqf::parse::astnode& node)
{
	auto kind = static_cast<sqf::parse::asttype::config>(node.kind);
	switch (kind)
	{
	case sqf::parse::asttype::config::CONFIGNODE:
	{
		std::shared_ptr<sqf::configdata> curnode;
		if (!node.children.empty() && node.children.front().kind == (short)sqf::parse::asttype::config::CONFIGNODE_PARENTIDENT)
		{
			curnode = std::make_shared<sqf::configdata>(parent, node.content, node.children.front().content);
			for (size_t i = 1; i < node.children.size(); i++)
			{
				auto subnode = node.children[i];
				navigate_config(full, vm, curnode, subnode);
			}
		}
		else
		{
			curnode = std::make_shared<sqf::configdata>(parent, node.content);
			for (auto subnode : node.children)
			{
				navigate_config(full, vm, curnode, subnode);
			}
		}
		parent->push_back(sqf::value(curnode));
	} break;
	case sqf::parse::asttype::config::VALUENODE:
	{
		std::shared_ptr<sqf::configdata> curnode = std::make_shared<sqf::configdata>(parent, node.content);
		for (auto& subnode : node.children)
		{
			navigate_config(full, vm, curnode, subnode);
		}
		parent->push_back(sqf::value(curnode));
	} break;
	case sqf::parse::asttype::config::STRING:
		parent->set_cfgvalue(sqf::value(node.content));
		break;
	case sqf::parse::asttype::config::NUMBER:
		parent->set_cfgvalue(sqf::value(std::stod(node.content)));
		break;
	case sqf::parse::asttype::config::HEXNUMBER:
		parent->set_cfgvalue(sqf::value(std::stol(node.content, nullptr, 16)));
		break;
	case sqf::parse::asttype::config::LOCALIZATION:
		parent->set_cfgvalue(sqf::value(node.content));
		break;
	case sqf::parse::asttype::config::ARRAY:
	{
		std::vector<sqf::value> values;
		for (auto& subnode : node.children)
		{
			navigate_config(full, vm, parent, subnode);
			values.push_back(parent->cfgvalue());
		}
		parent->set_cfgvalue(sqf::value(values));
	} break;
	case sqf::parse::asttype::config::VALUE:
		break;
	default:
	{
		for (auto& subnode : node.children)
		{
			navigate_config(full, vm, parent, subnode);
		}
	}
	}
}
void sqf::virtualmachine::parse_config(std::string_view code, std::shared_ptr<configdata> parent)
{
	auto parser = sqf::parse::config(get_logger(), code, "");
	bool errorflag = false;
	auto node = parser.parse(errorflag);
//#if defined(_DEBUG)
//	static bool isinitial = true;
//	if (isinitial)
//	{
//		isinitial = false;
//		out() << "-------------------------------" << std::endl;
//		print_navigate_ast(mout, node, sqf::parse::config::astkindname);
//		out() << "-------------------------------" << std::endl;
//	}
//#endif

	if (!errorflag)
	{
		navigate_config(code.data(), this, std::move(parent), node);
	}
}
size_t sqf::virtualmachine::push_obj(std::shared_ptr<sqf::innerobj> obj)
{
	if (!mfreeobjids.empty())
	{
		auto id = mfreeobjids.back();
		mfreeobjids.pop_back();
		mobjlist[id] = obj;
		return id;
	}
	else
	{
		auto id = mobjlist.size();
		mobjlist.push_back(obj);
		return id;
	}
}
void sqf::virtualmachine::drop_obj(const sqf::innerobj * obj)
{
	auto id = obj->netid();
	mobjlist[id] = std::shared_ptr<sqf::innerobj>();
	mfreeobjids.push_back(id);
}
std::shared_ptr<sqf::innerobj> sqf::virtualmachine::get_obj_netid(size_t netid)
{
	if (mobjlist.size() <= netid)
	{
		return std::shared_ptr<innerobj>();
	}
	return mobjlist[netid];
}
std::string sqf::virtualmachine::get_group_id(std::shared_ptr<sqf::sidedata> side)
{
	int sidenum = side->side();
	size_t id = mgroupidcounter[sidenum]++;
	std::stringstream sstream;
	sstream << side->tosqf() << " ALPHA " << id;
	return sstream.str();
}
void sqf::virtualmachine::push_group(std::shared_ptr<sqf::groupdata> d)
{
	mgroups[d->side()->side()].push_back(d);
}
void sqf::virtualmachine::drop_group(std::shared_ptr<sqf::groupdata> grp)
{
	auto& grpList = mgroups[grp->side()->side()];
	for (size_t i = 0; i < grpList.size(); i++)
	{
		if (grpList[i].get() == grp.get())
		{
			grpList[i] = grpList.back();
			grpList.pop_back();
			return;
		}
	}
}
std::chrono::system_clock::time_point sqf::virtualmachine::system_time()
{
	return std::chrono::system_clock::now();
}
//std::string sqf::virtualmachine::preprocess_file(std::string inputfile)
//{
//	//parse::preprocessor::ppparser parser;
//	//return parser.parse(this, inputfile);
//	return "";
//}
void sqf::virtualmachine::handle_networking()
{

}