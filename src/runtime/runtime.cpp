#include "runtime.h"

#include <optional>

static sqf::runtime::runtime::result execute_do(sqf::runtime::runtime* runtime, size_t exit_after)
{
	while (
		!runtime->is_exit_requested() &&
		exit_after != 0 &&
		!runtime->active_context()->suspended() &&
		runtime->runtime_state() == sqf::runtime::runtime::state::running)
	{
		bool success;
		//auto instruction = runtime->active_context()->current_frame().next(runtime->active_context());


		/*
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
				if (m_evaluate_halt)
				{
					m_status = vmstatus::evaluating;
					while (m_evaluate_halt);
					if (m_status == vmstatus::evaluating)
					{
						m_status = vmstatus::running;
					}
				}
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
			*/
	}
	return sqf::runtime::runtime::result::ok;
}

sqf::runtime::runtime::result sqf::runtime::runtime::execute(sqf::runtime::runtime::action action)
{
	sqf::runtime::runtime::result res = result::invalid;
	bool expected = false;
	switch (action)
	{
	case action::leave_scope:
		if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
		{
			m_is_exit_requested = false;
			m_is_halt_requested = false;
			auto scopeNum = m_active_context->size() - 1;
			m_state = state::running;
			while (!m_is_exit_requested && !m_is_halt_requested && !m_contexts.size() == 0)
			{
				res = execute_do(this, 1);
				if (res != result::ok)
				{
					break;
				}
				if (m_active_context->size() <= scopeNum)
				{
					break;
				}
			}
			switch (res)
			{
			case sqf::runtime::runtime::result::empty:
				m_state = state::empty;
				break;
			case sqf::runtime::runtime::result::ok:
				m_state = state::halted;
				break;
			case sqf::runtime::runtime::result::invalid:
			case sqf::runtime::runtime::result::action_error:
			case sqf::runtime::runtime::result::runtime_error:
				m_state = state::halted_error;
				break;
			default:
				break;
			}
			if (m_is_exit_requested)
			{
				m_contexts.clear();
				m_state = state::empty;
			}
			m_run_atomic = false;
		}
		else
		{
			res = result::action_error;
		}
		break;
	case action::start:
		if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
		{
			m_is_exit_requested = false;
			m_is_halt_requested = false;
			m_state = state::running;
			for (auto iterator = m_contexts.begin(); iterator != m_contexts.end(); iterator++)
			{
				m_active_context = iterator;
				if (m_active_context->suspended())
				{
					if (m_active_context->wakeup_timestamp() <= std::chrono::system_clock::now())
					{
						m_active_context->unsuspend();
						res = execute_do(this, 150);
					}
					else
					{
						res = result::ok;
					}
				}
				else
				{
					res = execute_do(this, 150);
				}
				switch (res)
				{
				case sqf::runtime::runtime::result::empty:
					m_contexts.erase(iterator);
					iterator = m_contexts.begin();
					break;
				case sqf::runtime::runtime::result::invalid:
				case sqf::runtime::runtime::result::action_error:
				case sqf::runtime::runtime::result::runtime_error:
					goto start_loop_exit;
				}
			}
		start_loop_exit:

			switch (res)
			{
			case sqf::runtime::runtime::result::empty:
				m_state = state::empty;
				break;
			case sqf::runtime::runtime::result::ok:
				m_state = state::halted;
				break;
			case sqf::runtime::runtime::result::invalid:
			case sqf::runtime::runtime::result::action_error:
			case sqf::runtime::runtime::result::runtime_error:
				m_state = state::halted_error;
				break;
			default:
				break;
			}
			if (m_is_exit_requested)
			{
				m_contexts.clear();
				m_state = state::empty;
			}
			m_run_atomic = false;
		}
		else
		{
			res = result::action_error;
		}
		break;
	case action::assembly_step:
		if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
		{
			m_is_exit_requested = false;
			m_is_halt_requested = false;
			m_state = state::running;
			res = execute_do(this, 1);
			switch (res)
			{
			case sqf::runtime::runtime::result::empty:
				m_state = state::empty;
				break;
			case sqf::runtime::runtime::result::ok:
				m_state = state::halted;
				break;
			case sqf::runtime::runtime::result::invalid:
			case sqf::runtime::runtime::result::action_error:
			case sqf::runtime::runtime::result::runtime_error:
				m_state = state::halted_error;
				break;
			default:
				break;
			}
			if (m_is_exit_requested)
			{
				m_contexts.clear();
				m_state = state::empty;
			}
			m_run_atomic = false;
		}
		else
		{
			res = result::action_error;
		}
		break;
	case action::line_step:
		if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
		{
			m_is_exit_requested = false;
			m_is_halt_requested = false;
			bool success;
			m_state = state::running;
			std::optional<diagnostics::diag_info> dinf;
			while (!m_is_exit_requested && !m_is_halt_requested && !m_contexts.size() == 0)
			{
				if (!dinf.has_value())
				{
					auto next_inst = m_active_context->current_frame().peek(success);
					if (success)
					{
						dinf = { (*next_inst)->diag_info() };
					}
				}

				res = execute_do(this, 1);
				if (res != result::ok)
				{
					break;
				}
				if (dinf.has_value())
				{
					auto next_inst = m_active_context->current_frame().peek(success);
					if (success && dinf.value() != (*next_inst)->diag_info())
					{
						break;
					}
				}
			}
			switch (res)
			{
			case sqf::runtime::runtime::result::empty:
				m_state = state::empty;
				break;
			case sqf::runtime::runtime::result::ok:
				m_state = state::halted;
				break;
			case sqf::runtime::runtime::result::invalid:
			case sqf::runtime::runtime::result::action_error:
			case sqf::runtime::runtime::result::runtime_error:
				m_state = state::halted_error;
				break;
			default:
				break;
			}
			if (m_is_exit_requested)
			{
				m_contexts.clear();
				m_state = state::empty;
			}
			m_run_atomic = false;
		}
		else
		{
			res = result::action_error;
		}
		break;


	case action::stop:
		if (m_state != state::running)
		{
			res = result::action_error;
		}
		else if (!m_run_atomic)
		{
			res = result::action_error;
		}
		else
		{
			m_is_exit_requested = true;
			res = result::ok;
		}
		break;
	case action::abort:
		if (m_state == state::running)
		{
			if (!m_run_atomic)
			{
				res = result::action_error;
			}
			else
			{
				m_is_exit_requested = true;
				res = result::ok;
			}
		}
		else if (m_state == state::halted_error || m_state == state::halted)
		{
			if (m_run_atomic.compare_exchange_weak(expected, true, std::memory_order::memory_order_seq_cst, std::memory_order::memory_order_seq_cst))
			{
				m_contexts.clear();
				m_state = state::empty;
				res = result::ok;
				m_run_atomic = false;
			}
			else
			{
				res = result::action_error;
			}
		}
		else
		{
			res = result::action_error;
		}
		break;
	case action::reset_run_atomic:
		m_run_atomic = false;
		break;
	default:
		res = result::action_error;
		break;
	}
	return res;
}

