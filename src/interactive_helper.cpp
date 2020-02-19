#include "interactive_helper.h"
#include "virtualmachine.h"
#include "vmstack.h"
#include <thread>
#include <string>
#include <iostream>

using namespace std::string_literals;
using namespace std::string_view_literals;
void interactive_helper::virtualmachine_thread()
{
	while (!m_thread_die)
	{
		if (m_run_virtualmachine <= 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}
		auto action = m_run_virtualmachine;
		m_run_virtualmachine = 0;

		m_vm.execute(action == 1 ? sqf::virtualmachine::execaction::start : sqf::virtualmachine::execaction::leave_scope);
		if (m_run_virtualmachine == 0)
		{
			switch (m_vm.status())
			{
			case sqf::virtualmachine::vmstatus::empty:
				std::cout << "VM Done!" << std::endl;
				break;
			case sqf::virtualmachine::vmstatus::halted:
				std::cout << "VM Halted!" << std::endl;
				break;
			case sqf::virtualmachine::vmstatus::halt_error:
				std::cout << "VM Error!" << std::endl;
				break;
			}
		}
	}
}

void interactive_helper::init()
{

	register_command(std::array{ "cmds"s, "commands"s }, "Displays all commands and their description.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			std::cout << "Syntax: COMMAND ARGUMENTS [ENTER]" << std::endl;
			size_t max_names_len = 0;

			// Calculate max name length
			for (const auto& it : interactive.cmds())
			{
				bool flag = false;
				size_t len = 0;
				for (const auto& name : it.names)
				{
					if (flag)
					{
						len += name.length() + 2;
					}
					else
					{
						len += name.length();
						flag = true;
					}
				}
				if (len > max_names_len)
				{
					max_names_len = len;
				}
			}
			for (const auto& it : interactive.cmds())
			{
				bool flag = false;
				size_t len = 0;
				for (const auto& name : it.names)
				{
					if (flag)
					{
						len += name.length() + 2;
						std::cout << ", " << name;
					}
					else
					{
						len += name.length();
						std::cout << "  " << name;
						flag = true;
					}
				}
				std::cout << std::string(max_names_len - len + 1, ' ') << "-> ";
				size_t pos = 0;
				size_t previous = 0;
				flag = false;
				while ((pos = it.description.find('\n', pos + 1)) != std::string_view::npos)
				{
					if (flag)
					{
						std::cout << std::string(max_names_len + 6, ' ');
					}
					else
					{
						flag = true;
					}
					std::cout << it.description.substr(previous, pos - previous) << std::endl;
					previous = pos + 1;
				}
				if (flag)
				{
					std::cout << std::string(max_names_len + 6, ' ');
				}
				std::cout << it.description.substr(previous) << std::endl;
			}
		});
	register_command(std::array{ "bp"s, "set-breapoint"s },
		"Sets a breakpoint at the provided line & file.\n"
		"Example: `bp 20 C:\\file\\path\\dot.sqf`\n"
		"with the number marking the line and the string in quotes being the file path.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			size_t line;
			std::string file;

			auto split = arg.find(' ');
			if (split == std::string::npos)
			{
				std::cerr << "set-breapoint is unable to find file." << std::endl;
				return;
			}

			line = std::stol(arg.data());
			file = arg.substr(split);
			interactive.vm().push_back({ line, file });
		});
	register_command(std::array{ "rbp"s, "remove-breapoint"s },
		"Removes a breakpoint from the provided line & file.\n"
		"Example: `rbp 20 C:\\file\\path\\dot.sqf`\n"
		"with the number marking the line and the string in quotes being the file path.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			size_t line;
			std::string file;

			auto split = arg.find(' ');
			if (split == std::string::npos)
			{
				std::cerr << "remove-breapoint is unable to find file." << std::endl;
				return;
			}

			line = std::stol(arg.data());
			file = arg.substr(split);
			auto res = std::find_if(interactive.vm().breakpoints_begin(), interactive.vm().breakpoints_end(),
				[line, file](const sqf::diagnostics::breakpoint& bp) -> bool { return bp.line() == line && bp.file() == file; });
			if (interactive.vm().breakpoints_end() == res)
			{
				std::cerr << "remove-breapoint is unable to locate existing breakpoint." << std::endl;
			}
			else
			{
				interactive.vm().breakpoints_erase(res, res);
			}
		});
	register_command(std::array{ "rbpa"s, "clear-breapoints"s },
		"Removes all breakpoints.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			interactive.vm().breakpoints_erase(interactive.vm().breakpoints_begin(), interactive.vm().breakpoints_end());
		});
	register_command(std::array{ "r"s, "run"s, "start"s },
		"Starts the VM execution.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			switch (interactive.vm().status())
			{
			case sqf::virtualmachine::vmstatus::running:
			case sqf::virtualmachine::vmstatus::evaluating:
				std::cerr << "run cannot start vm. Already running." << std::endl;
				return;
			default:
				interactive.set_start_vm();
				break;
			}
		});
	register_command(std::array{ "h"s, "halt"s },
		"Halts the VM execution wherever it is right now.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			switch (interactive.vm().status())
			{
			case sqf::virtualmachine::vmstatus::running:
			case sqf::virtualmachine::vmstatus::evaluating:
				interactive.vm().execute(sqf::virtualmachine::execaction::stop);
				return;
			default:
				std::cerr << "halt cannot halt vm. Already halted or stopped." << std::endl;
				break;
			}
		});
	register_command(std::array{ "a"s, "abort"s, "stop"s },
		"Stops the execution of the VM and clears whatever is currently in the execution queue.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			interactive.vm().execute(sqf::virtualmachine::execaction::abort);
		});
	register_command(std::array{ "ls"s, "line-step"s, "step"s },
		"Executes a single line-step in the VM. Blocking call.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			switch (interactive.vm().status())
			{
			case sqf::virtualmachine::vmstatus::running:
			case sqf::virtualmachine::vmstatus::evaluating:
				std::cerr << "step vm still running, halt first." << std::endl;
				return;
			default:
				interactive.vm().execute(sqf::virtualmachine::execaction::line_step);
				break;
			}
		});
	register_command(std::array{ "as"s, "assembly-step"s, },
		"Executes a single assembly-step in the VM. Blocking call.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			switch (interactive.vm().status())
			{
			case sqf::virtualmachine::vmstatus::running:
			case sqf::virtualmachine::vmstatus::evaluating:
				std::cerr << "step vm still running, halt first." << std::endl;
				return;
			default:
				interactive.vm().execute(sqf::virtualmachine::execaction::assembly_step);
				break;
			}
		});
	register_command(std::array{ "leave-scope"s, },
		"Executes until current scope is one higher in the VM.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			switch (interactive.vm().status())
			{
			case sqf::virtualmachine::vmstatus::running:
			case sqf::virtualmachine::vmstatus::evaluating:
				interactive.set_leave_scope_vm();
				interactive.vm().execute(sqf::virtualmachine::execaction::stop);
				while (interactive.vm().status() == sqf::virtualmachine::vmstatus::running || interactive.vm().status() == sqf::virtualmachine::vmstatus::evaluating)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				break;
			default:
				interactive.vm().execute(sqf::virtualmachine::execaction::assembly_step);
				break;
			}
		});
	register_command(std::array{ "cs"s, "callstack"s },
		"Receives the callstack from the current active vmstack and displays it.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			switch (interactive.vm().status())
			{
			case sqf::virtualmachine::vmstatus::running:
			case sqf::virtualmachine::vmstatus::evaluating:
			{
				interactive.set_pause_vm();
				while (interactive.vm().status() == sqf::virtualmachine::vmstatus::running || interactive.vm().status() == sqf::virtualmachine::vmstatus::evaluating)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				auto stackdump = interactive.vm().active_vmstack()->dump_callstack_diff({});
				// ToDo: Print
				interactive.set_start_vm();
			}
			break;
			default:
				interactive.vm().execute(sqf::virtualmachine::execaction::line_step);
				break;
			}
		});
	register_command(std::array{ "parse-sqf"s, "sqf"s },
		"Will parse the input code into the vm. 'Filepath' will be '__interactive.sqf'.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			switch (interactive.vm().status())
			{
			case sqf::virtualmachine::vmstatus::running:
			case sqf::virtualmachine::vmstatus::evaluating:
			{
				interactive.set_pause_vm();
				while (interactive.vm().status() == sqf::virtualmachine::vmstatus::running || interactive.vm().status() == sqf::virtualmachine::vmstatus::evaluating)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				auto stackdump = interactive.vm().active_vmstack()->dump_callstack_diff({});
				interactive.vm().parse_sqf(arg, "__interactive.sqf");
				interactive.set_start_vm();
			}
			break;
			default:
				interactive.vm().parse_sqf(arg, "__interactive.sqf");
				break;
			}
		});
	register_command(std::array{ "e"s, "eval"s, "evaluate"s },
		"Evaluates the provided expression and returns the result.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			bool success = false;
			auto value = interactive.vm().evaluate_expression(arg, success);
			if (success)
			{
				std::cout << value.tosqf() << std::endl;
			}
			else
			{
				std::cout << "Failed to evaluate." << std::endl;
			}
		});
	register_command(std::array{ "q"s, "exit"s, "quit"s },
		"Exits the execution and terminates the program.",
		[](interactive_helper& interactive, std::string_view arg) -> void {
			interactive.set_exit();
		});
}
void interactive_helper::run()
{
	const size_t buffer_size = 16384;
	char buffer[buffer_size];
	std::thread thread(&interactive_helper::virtualmachine_thread, this);
	while (!m_exit)
	{
		std::cout << "> ";
		std::cin.getline(buffer, buffer_size);
		auto end = std::strlen(buffer);
		std::string_view line(buffer, end);

		auto split = line.find(' ');
		std::string_view command;
		if (split == std::string_view::npos)
		{
			command = line;
		}
		else
		{
			command = std::string_view(buffer, split);
		}
		auto res = std::find_if(m_cmds.begin(), m_cmds.end(),
			[command](const interactive_helper::command& cmd) -> bool {
				return std::find(cmd.names.begin(), cmd.names.end(), command) != cmd.names.end();
			});
		if (res == m_cmds.end())
		{
			std::cerr << "Unknwon command '" << command << "'" << std::endl;
		}
		else
		{
			res->callback(*this, split == std::string_view::npos ? ""sv : line.substr(split + 1));
		}
	}
	thread.join();
}

void interactive_helper::print_welcome()
{
	std::cout
		<< "Welcome to Interactive mode." << std::endl
		<< "In interactive, you can control the behavior of the VM while it is running." << std::endl
		<< "Quick Reference:" << std::endl
		<< "cmds, commands -> Lists all commands available." << std::endl
		<< "q, exit, quit  -> Exits the execution and terminates the program." << std::endl;
}
