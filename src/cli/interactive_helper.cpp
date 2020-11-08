#include "interactive_helper.h"
#include "../runtime/runtime.h"
#include <thread>
#include <string>
#include <string_view>
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std::string_literals;
using namespace std::string_view_literals;
void interactive_helper::virtualmachine_thread()
{
    while (!m_thread_die)
    {
        auto tmp = m_runtime_apply_action;
        m_runtime_apply_action = sqf::runtime::runtime::action::invalid;

        switch (tmp)
        {
        case sqf::runtime::runtime::action::invalid:
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            break;
        default:
            m_runtime.execute(m_runtime_apply_action);
            break;
        }


        switch (m_runtime.runtime_state())
        {
        case sqf::runtime::runtime::state::empty:
            std::cout << "runtime Done!" << std::endl;
            break;
        case sqf::runtime::runtime::state::halted:
            std::cout << "runtime Halted!" << std::endl;
            break;
        case sqf::runtime::runtime::state::halted_error:
            std::cout << "runtime Error!" << std::endl;
            break;
        case sqf::runtime::runtime::state::running: /* nothing */ break;
        case sqf::runtime::runtime::state::evaluating: /* nothing */ break;
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
            for (auto it = interactive.commands_begin(); it != interactive.commands_end(); ++it)
            {
                bool flag = false;
                size_t len = 0;
                for (const auto name : it->names)
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
            for (auto it = interactive.commands_begin(); it != interactive.commands_end(); ++it)
            {
                bool flag = false;
                size_t len = 0;
                for (const auto name : it->names)
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
                while ((pos = it->description.find('\n', pos + 1)) != std::string_view::npos)
                {
                    if (flag)
                    {
                        std::cout << std::string(max_names_len + 6, ' ');
                    }
                    else
                    {
                        flag = true;
                    }
                    std::cout << it->description.substr(previous, pos - previous) << std::endl;
                    previous = pos + 1;
                }
                if (flag)
                {
                    std::cout << std::string(max_names_len + 6, ' ');
                }
                std::cout << it->description.substr(previous) << std::endl;
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
            interactive.runtime().push_back({ line, file });
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
            auto res = std::find_if(interactive.runtime().breakpoints_begin(), interactive.runtime().breakpoints_end(),
                [line, file](const sqf::runtime::diagnostics::breakpoint& bp)
                { return bp.line() == line && bp.file() == file; });
            if (interactive.runtime().breakpoints_end() == res)
            {
                std::cerr << "remove-breapoint is unable to locate existing breakpoint." << std::endl;
            }
            else
            {
                interactive.runtime().erase(res);
            }
        });
    register_command(std::array{ "rbpa"s, "clear-breapoints"s },
        "Removes all breakpoints.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            interactive.runtime().erase(interactive.runtime().breakpoints_begin(), interactive.runtime().breakpoints_end());
        });
    register_command(std::array{ "r"s, "run"s, "start"s },
        "Starts the runtime execution.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            switch (interactive.runtime().runtime_state())
            {
            case sqf::runtime::runtime::state::running:
            case sqf::runtime::runtime::state::evaluating:
                std::cerr << "run cannot start runtime. Already running." << std::endl;
                return;
            default:
                interactive.execute_next(sqf::runtime::runtime::action::start);
                break;
            }
        });
    register_command(std::array{ "h"s, "halt"s },
        "Halts the runtime execution wherever it is right now.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            switch (interactive.runtime().runtime_state())
            {
            case sqf::runtime::runtime::state::running:
            case sqf::runtime::runtime::state::evaluating:
                interactive.runtime().execute(sqf::runtime::runtime::action::stop);
                return;
            default:
                std::cerr << "halt cannot halt runtime. Already halted or stopped." << std::endl;
                break;
            }
        });
    register_command(std::array{ "a"s, "abort"s, "stop"s },
        "Stops the execution of the runtime and clears whatever is currently in the execution queue.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            interactive.runtime().execute(sqf::runtime::runtime::action::abort);
        });
    register_command(std::array{ "ls"s, "line-step"s, "step"s },
        "Executes a single line-step in the runtime. Blocking call.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            switch (interactive.runtime().runtime_state())
            {
            case sqf::runtime::runtime::state::running:
            case sqf::runtime::runtime::state::evaluating:
                std::cerr << "Runtime not halted." << std::endl;
                return;
            default:
                interactive.execute_next(sqf::runtime::runtime::action::line_step);
                break;
            }
        });
    register_command(std::array{ "as"s, "assembly-step"s, },
        "Executes a single assembly-step in the runtime. Blocking call.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            switch (interactive.runtime().runtime_state())
            {
            case sqf::runtime::runtime::state::running:
            case sqf::runtime::runtime::state::evaluating:
                std::cerr << "Runtime not halted." << std::endl;
                return;
            default:
                interactive.execute_next(sqf::runtime::runtime::action::assembly_step);
                break;
            }
        });
    register_command(std::array{ "leave-scope"s, },
        "Executes a leave-scope operation in the runtime.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            switch (interactive.runtime().runtime_state())
            {
            case sqf::runtime::runtime::state::running:
            case sqf::runtime::runtime::state::evaluating:
                std::cerr << "Runtime not halted." << std::endl;
                break;
            default:
                interactive.execute_next(sqf::runtime::runtime::action::leave_scope);
                break;
            }
        });
    register_command(std::array{ "cs"s, "callstack"s },
        "Receives the callstack from the current active vmstack and displays it.\n"
        "Affected by Enacted Script.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            switch (interactive.runtime().runtime_state())
            {
            case sqf::runtime::runtime::state::running:
            case sqf::runtime::runtime::state::evaluating:
                std::cerr << "Runtime not halted." << std::endl;
            default:
            {
                auto selected_context = interactive.context_selected();
                if (!selected_context)
                {
                    interactive.context_selected(interactive.runtime().context_active_as_shared());
                    std::cout << "Selected context no longer available. Changing to Currently Active." << std::endl;
                }

                // Dump frames to console
                int i = 0;
                for (auto it = selected_context->frames_rbegin(); it != selected_context->frames_rend(); ++it)
                {
                    std::cout << i++ << ":\tnamespace: " << it->globals_value_scope()->scope_name()
                        << "\tscopename: " << it->scope_name()
                        << std::endl << (*it->current())->diag_info().code_segment << std::endl;
                }
            } break;
            }
        });
    register_command(std::array{ "ll"s, "locals"s, "list-locals"s, "vars"s },
        "Lists all local variables.\n"
        "Affected by Enacted Script.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            switch (interactive.runtime().runtime_state())
            {
            case sqf::runtime::runtime::state::running:
            case sqf::runtime::runtime::state::evaluating:
                std::cerr << "Runtime not halted." << std::endl;
            default:
            {
                auto selected_context = interactive.context_selected();
                if (!selected_context)
                {
                    interactive.context_selected(interactive.runtime().context_active_as_shared());
                    std::cout << "Selected context no longer available. Changing to Currently Active." << std::endl;
                }

                // Dump variables to console
                size_t space_index = 0;
                for (auto it_frame = selected_context->frames_rbegin(); it_frame != selected_context->frames_rend(); ++it_frame)
                {
                    for (auto it_var = it_frame->begin(); it_var != it_frame->end(); ++it_var)
                    {
                        std::cout << "[" << std::setw(3) << space_index << "] '" << it_var->first << "' := " << it_var->second.to_string_sqf() << std::endl;
                    }
                }
            } break;
            }
        });
    register_command(std::array{ "e"s, "eval"s, "evaluate"s },
        "Evaluates the provided expression and returns the result.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            bool success = false;
            auto value = interactive.runtime().evaluate_expression(std::string(arg), success);
            if (success)
            {
                std::cout << value.to_string_sqf() << std::endl;
            }
            else
            {
                std::cout << "Failed to evaluate." << std::endl;
            }
        });
    register_command(std::array{ "q"s, "exit"s, "quit"s },
        "Exits the execution and terminates the program.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            interactive.runtime().exit(0);
        });
    register_command(std::array{ "ls"s, "scripts"s, "list-scripts"s },
        "Displays a list of all script instances currently running and their reference number.",
        [](interactive_helper& interactive, std::string_view arg) -> void {
            size_t i = 0;
            auto selected = interactive.context_selected();
            for (auto it = interactive.runtime().context_begin(); it != interactive.runtime().context_end(); ++it)
            {
                if ((*it)->name().empty())
                {
                    std::cout << "[" << std::setw(3) << i << "] " <<
                        (interactive.runtime().context_active_as_shared() == *it ? '>' : ' ') <<
                        " '" << (*it)->name() << "'" <<
                        (selected == *it ? " [SELECTED]" : "") << std::endl;
                }
                else
                {
                    std::cout << "[" << std::setw(3) << i << "] " <<
                        (interactive.runtime().context_active_as_shared() == *it ? '>' : ' ') <<
                        " UNNAMED" <<
                        (selected == *it ? " [SELECTED]" : "") << std::endl;
                }
                i++;
            }
        });
    register_command(std::array{ "ss"s, "set-script"s },
        "Sets the script to enact to the provided number.\n"
        "Example: `ss -1`\n",
        [](interactive_helper& interactive, std::string_view arg) -> void {

            switch (interactive.runtime().runtime_state())
            {
            case sqf::runtime::runtime::state::running:
            case sqf::runtime::runtime::state::evaluating:
                std::cerr << "Runtime not halted." << std::endl;
            default:
            {
                long i = std::stol(arg.data());
                long index = 0;
                for (auto it = interactive.runtime().context_begin(); it != interactive.runtime().context_end(); ++it)
                {
                    if (i == index++)
                    {
                        interactive.context_selected(*it);
                        if ((*it)->name().empty())
                        {
                            std::cout << "Selected context " << index << " '" << (*it)->name() << "'" << std::endl;
                        }
                        else
                        {
                            std::cout << "Selected context " << index << " UNNAMED" << std::endl;
                        }
                        return;
                    }
                }
                std::cout << "Cannot set selected context to " << i << " (Not Found)." << std::endl;
            } break;
            }
        });
}
void interactive_helper::run()
{
    std::thread thread(&interactive_helper::virtualmachine_thread, this);
    while (!m_exit)
    {
        std::cout << "> ";
        std::cin.getline(m_buffer, buffer_size);
        auto end = std::strlen(m_buffer);
        std::string_view line(m_buffer, end);

        auto split = line.find(' ');
        std::string_view command;
        if (split == std::string_view::npos)
        {
            command = line;
        }
        else
        {
            command = std::string_view(m_buffer, split);
        }
        auto res = std::find_if(m_commands.begin(), m_commands.end(),
            [command](const interactive_helper::command& cmd) -> bool {
                return std::find(cmd.names.begin(), cmd.names.end(), command) != cmd.names.end();
            });
        if (res == m_commands.end())
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
        << "In interactive, you can control the behavior of the runtime while it is running." << std::endl
        << "Quick Reference:" << std::endl
        << "cmds, commands -> Lists all commands available." << std::endl
        << "q, exit, quit  -> Exits the execution and terminates the program." << std::endl;
}
