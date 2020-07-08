#pragma once
#include <typeinfo>
#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <map>
#include <list>
#include <sstream>
#include <chrono>
#include <utility>
#include <functional>
#include <atomic>

#include "dlops.h"
#include "marker.h"
#include "logging.h"
#include "filesystem.h"
#include "parsing/astnode.h"
#include "parsing/macro.h"
#include "breakpoint.h"


namespace sqf
{
	class innerobj;
	class configdata;
	class callstack;
	class vmstack;
	class objectdata;
	class groupdata;
	class sidedata;
	class scriptdata;
	class value;
	class sqfnamespace;
	namespace networking
	{
		class client;
		class server;
	}
	class virtualmachine : public CanLog
	{
	public:
		enum class evaction
		{
			enter,
			exit
		};
		enum class execaction
		{
			start,
			stop,
			abort,
			assembly_step,
			line_step,
			leave_scope,
			// Special execaction to reset the run_atomic flag in case of
			// exception being thrown by the virtualmachine execution method.
			reset_run_atomic
		};
		enum class execresult
		{
			invalid = -1,
			OK,
			action_error,
			runtime_error
		};
		enum class vmstatus
		{
			empty,
			halted,
			running,
			requested_halt,
			halt_error,
			requested_abort,
			evaluating
		};
	private:
		unsigned long long m_instructions_count;
		unsigned long long m_max_instructions;

		std::shared_ptr<sqf::vmstack> m_main_vmstack;
		std::shared_ptr<sqf::vmstack> m_active_vmstack;
		std::list<std::shared_ptr<scriptdata>> m_scripts;
		std::vector<size_t> mfreeobjids;
		std::vector<std::shared_ptr<innerobj>> mobjlist;
		std::shared_ptr<innerobj> mplayer_obj;

		std::shared_ptr<sqf::sqfnamespace> mmissionnamespace;
		std::shared_ptr<sqf::sqfnamespace> muinamespace;
		std::shared_ptr<sqf::sqfnamespace> mparsingnamespace;
		std::shared_ptr<sqf::sqfnamespace> mprofilenamespace;

		std::map<std::string, sqf::marker> mmarkers;
		std::shared_ptr<sqf::instruction> m_current_instruction;

		std::map<int, size_t> mgroupidcounter;
		std::map<int, std::vector<std::shared_ptr<groupdata>>> mgroups;
		vmstatus m_status;
		bool m_runtime_error;
		bool m_evaluate_halt;

		std::vector<sqf::diagnostics::breakpoint> m_breakpoints;

		/*
		 * Executes the currently configured setting up to the provided instruction count.
		 * Will return true if run was clean, false if it was not.
		 */
		std::vector<std::shared_ptr<dlops>> mlibraries;
		bool m_exit_flag;
		int m_exit_code = 0;
		bool m_allow_suspension;
		bool m_perform_classname_checks;
		bool m_allow_networking;
		int m_last_breakpoint_line_hit;
		sqf::filesystem m_filesystem;
		std::chrono::system_clock::time_point m_created_timestamp;
		std::chrono::system_clock::time_point m_current_time;
		std::shared_ptr<networking::client> m_current_networking_client;
		std::shared_ptr<networking::server> m_current_networking_server;

		std::vector<sqf::parse::macro> m_preprocessor_macros;

		std::vector<std::function<void(virtualmachine*, const char* text, const sqf::parse::astnode&, evaction)>> m_parsing_callbacks;

	private:
		void navigate_sqf(const char* full, std::shared_ptr<sqf::callstack> stack, const sqf::parse::astnode& node, bool& errorflag);
		void handle_networking();
		bool performexecute(size_t exitAfter = ~0);
		void execute_parsing_callbacks(const char* text, const sqf::parse::astnode& node, evaction act)
		{
			if (m_parsing_callbacks.empty())
			{
				return;
			}
			for (auto& it : m_parsing_callbacks)
			{
				it(this, text, node, act);
			}
		}
		void execute_helper_execution_abort();
		bool execute_helper_execution_end();
		bool is_breakpoint_hit(std::shared_ptr<sqf::instruction> instruction);
	public:
		virtualmachine(Logger& logger, unsigned long long maxinst);
		virtualmachine(Logger& logger) : virtualmachine(logger, 0) {};
		~virtualmachine();

		void push_macro(sqf::parse::macro macro) { m_preprocessor_macros.push_back(macro); }
		const std::vector<sqf::parse::macro>& preprocessor_macros() const { return m_preprocessor_macros; }

		std::chrono::system_clock::time_point get_created_timestamp() const { return m_created_timestamp; }
		std::chrono::system_clock::time_point get_current_time() const { return m_current_time; }
		void set_current_time(std::chrono::system_clock::time_point value) { m_current_time = value; }
		
		std::shared_ptr<sqf::instruction> current_instruction() const { return m_current_instruction; }
		std::shared_ptr<innerobj> player_obj() const { return mplayer_obj; }
		void player_obj(std::shared_ptr<innerobj> val) { mplayer_obj = std::move(val); }
		std::shared_ptr<sqf::vmstack> active_vmstack() { return m_active_vmstack; }
		std::shared_ptr<sqf::vmstack> active_vmstack() const { return m_active_vmstack; }
		std::shared_ptr<sqf::vmstack> main_vmstack() { return m_main_vmstack; }
		std::shared_ptr<sqf::vmstack> main_vmstack() const { return m_main_vmstack; }
		const std::vector<std::shared_ptr<innerobj>>& get_objlist() const { return mobjlist; }
		std::shared_ptr<sqf::sqfnamespace> missionnamespace() const { return mmissionnamespace; }
		std::shared_ptr<sqf::sqfnamespace> uinamespace() const { return muinamespace; }
		std::shared_ptr<sqf::sqfnamespace> parsingnamespace() const { return mparsingnamespace; }
		std::shared_ptr<sqf::sqfnamespace> profilenamespace() const { return mprofilenamespace; }

		inline void logmsg(LogMessageBase&& message)
		{
			log(message);
			if (message.getLevel() <= loglevel::error)
			{
				m_runtime_error = true;
			}
		}

		vmstatus status() const { return m_status; }

		void register_callback(std::function<void(virtualmachine*, const char* text, const sqf::parse::astnode&, evaction)> callback)
		{
			m_parsing_callbacks.push_back(callback);
		}

		std::shared_ptr<networking::client> get_networking_client()
		{
			return m_current_networking_client;
		}
		std::shared_ptr<networking::server> get_networking_server()
		{
			return m_current_networking_server;
		}
		void set_networking(std::shared_ptr<networking::client> client)
		{
			if (is_networking_set())
			{
				throw std::runtime_error("Networking already set. Cannot set Client.");
			}
			m_current_networking_client = client;
		}
		void set_networking(std::shared_ptr<networking::server> server)
		{
			if (is_networking_set())
			{
				throw std::runtime_error("Networking already set. Cannot set Server.");
			}
			m_current_networking_server = server;
		}
		bool is_networking_set() { return m_current_networking_client || m_current_networking_server; }
		void release_networking();

		// Disables the networking capabilities of this VM.
		// Note that existing networking is not affected.
		void disable_networking() { m_allow_networking = false; }
		// Checks wether or not networking got disabled
		bool allow_networking() { return m_allow_networking; }
		void set_max_instructions(unsigned long long value) { m_max_instructions = value; }

		void push_back(sqf::diagnostics::breakpoint breakpoint)
		{
			m_breakpoints.push_back(breakpoint);
		}
		std::vector<sqf::diagnostics::breakpoint>::iterator breakpoints_begin()
		{
			return m_breakpoints.begin();
		}
		std::vector<sqf::diagnostics::breakpoint>::iterator breakpoints_end()
		{
			return m_breakpoints.end();
		}
		std::vector<sqf::diagnostics::breakpoint>::iterator breakpoints_erase(std::vector<sqf::diagnostics::breakpoint>::iterator first, std::vector<sqf::diagnostics::breakpoint>::iterator last)
		{
			return m_breakpoints.erase(first, last);
		}

		std::list<std::shared_ptr<scriptdata>>::iterator scripts_begin()
		{
			return m_scripts.begin();
		}
		std::list<std::shared_ptr<scriptdata>>::iterator scripts_end()
		{
			return m_scripts.end();
		}

		execresult execute(execaction action);
		void exit_flag(bool flag) { m_exit_flag = flag; }
		void exit_flag(bool flag, int exitcode) { m_exit_flag = flag; m_exit_code = exitcode; }
		bool exit_flag() const { return m_exit_flag; }
		int exit_code() const { return m_exit_code; }

		bool perform_classname_checks() const { return m_perform_classname_checks; }
		void perform_classname_checks(bool f) { m_perform_classname_checks = f; }

		marker* get_marker(std::string key) { return mmarkers.find(key) == mmarkers.end() ? nullptr : &mmarkers[key]; }
		void set_marker(std::string key, marker m) { mmarkers[key] = m; }
		void remove_marker(std::string key) { mmarkers.erase(key); }
		const std::map<std::string, sqf::marker>& get_markers() const { return mmarkers; }

		// Parses the provided code and prints the resulting
		// SQF-tree out into provided std::stringstream pointer
		void parse_sqf_tree(std::string_view, std::stringstream*);
		// Parses the provided code and creates assembly
		// instructions into a newly created sqf::callstack
		// that gets pushed onto the default sqf::vmstack receivable using
		// sqf::virtualmachine::stack(void)
		//
		// returns true, if parsing passed.
		// returns false, if parsing failed.
		// in case parsing failed, err_hasdata() and err_printbuff()
		// should be checked and/or printed.
		// 
		// In all cases, the corresponding wrn_ and out_ methods
		// Also should be checked in case they contain additional info.
		bool parse_sqf(std::string_view code, std::string filepath = "") { return parse_sqf(active_vmstack(), code, std::shared_ptr<sqf::callstack>(), filepath); }
		// Parses the provided code and creates assembly
		// instructions into provided sqf::callstack
		// that gets pushed onto the default sqf::vmstack receivable using
		// sqf::virtualmachine::stack(void)
		//
		// returns true, if parsing passed.
		// returns false, if parsing failed.
		// in case parsing failed, err_hasdata() and err_printbuff()
		// should be checked and/or printed.
		// 
		// In all cases, the corresponding wrn_ and out_ methods
		// Also should be checked in case they contain additional info.
		bool parse_sqf(std::string_view str, std::shared_ptr<sqf::callstack> cs, std::string filepath = "") { return parse_sqf(active_vmstack(), str, cs, filepath); }
		// Parses the provided code and creates assembly
		// instructions into provided sqf::callstack
		// that gets pushed onto provided sqf::vmstack
		//
		// returns true, if parsing passed.
		// returns false, if parsing failed.
		// in case parsing failed, err_hasdata() and err_printbuff()
		// should be checked and/or printed.
		// 
		// In all cases, the corresponding wrn_ and out_ methods
		// Also should be checked in case they contain additional info.
		bool parse_sqf(std::shared_ptr<sqf::vmstack>, std::string_view, std::shared_ptr<sqf::callstack>, std::string = "");

		std::string preprocess(std::string input, bool& errflag, std::string filename);

		sqf::parse::astnode parse_sqf_cst(std::string_view code, std::string filepath = "") { bool errflag = false; return parse_sqf_cst(code, errflag, filepath); }
        sqf::parse::astnode parse_sqf_cst(std::string_view code, bool& errorflag, std::string filepath = "");
		std::string pretty_print_sqf(std::string_view code);
		bool parse_config(std::string_view code) { return parse_config(code, ""); }
		bool parse_config(std::string_view code, std::string_view file);
		bool parse_config(std::string_view code, std::string_view file, std::shared_ptr<configdata> config);
		sqf::parse::astnode parse_config_cst(std::string_view code, std::string filepath = "") { bool errflag = false; return parse_config_cst(code, errflag, filepath); }
		sqf::parse::astnode parse_config_cst(std::string_view code, bool& errorflag, std::string filepath = "");
		std::vector<std::shared_ptr<dlops>>& libraries() { return mlibraries; }
		bool allow_suspension() const { return m_allow_suspension; }
		void allow_suspension(bool flag) { m_allow_suspension = flag; }


		// DO NOT USE FROM WITHIN COMMANDS!
		// Executing this method will request a temporary halt of the vm, to then execute
		// whatever is passed in view until the end.
		// A deadlock thus will happen, where the command would wait for itself
		// to return to start evaluating the expression!
		//
		// If you need to execute from within a command, pass "request_halt = false"
		value evaluate_expression(std::string_view view, bool& success, bool request_halt = true);

		filesystem& get_filesystem() { return m_filesystem; }

		size_t push_obj(std::shared_ptr<sqf::innerobj> obj);
		void drop_obj(const sqf::innerobj * obj);
		std::shared_ptr<sqf::innerobj> get_obj_netid(size_t netid);
		std::string get_group_id(std::shared_ptr<sqf::sidedata>);
		void push_group(std::shared_ptr<sqf::groupdata>);
		void drop_group(std::shared_ptr<sqf::groupdata>);
		void push_spawn(std::shared_ptr<scriptdata> scrpt) { m_scripts.push_back(scrpt); }
		static std::chrono::system_clock::time_point system_time();
	};
}