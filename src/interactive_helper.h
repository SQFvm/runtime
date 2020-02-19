#pragma once
#include <string>
#include <vector>
#include <array>
namespace sqf
{
	class virtualmachine;
}
class interactive_helper
{
	typedef void (*interactive_callback)(interactive_helper&, std::string_view);
	struct command
	{
		std::vector<std::string> names;
		interactive_callback callback;
		std::string description;
	};
	std::vector<command> m_cmds;
	sqf::virtualmachine& m_vm;
	bool m_thread_die;
	bool m_exit;
	int m_run_virtualmachine;
	void virtualmachine_thread();
public:
	template<size_t size>
	void register_command(std::array<std::string, size> names, std::string description, interactive_callback callback)
	{
		m_cmds.push_back({ std::vector(names.begin(), names.end()), callback, description });
	}
	interactive_helper(sqf::virtualmachine& vm) : m_vm(vm), m_cmds(), m_thread_die(false), m_exit(false), m_run_virtualmachine(0) {}

	void init();
	void run();
	void set_exit()
	{
		m_exit = 1;
		m_thread_die = true;
	}
	void set_start_vm() { m_run_virtualmachine = 1; }
	void set_leave_scope_vm() { m_run_virtualmachine = 2; }
	void set_pause_vm() { m_run_virtualmachine = -1; }
	void set_kill_thread() { m_thread_die = true; }
	void print_welcome();
	const std::vector<command>& cmds() const { return m_cmds; }
	sqf::virtualmachine& vm() const { return m_vm; }
};