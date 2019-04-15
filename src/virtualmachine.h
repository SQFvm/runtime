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

#include "dlops.h"
#include "marker.h"
#include "filesystem.h"
#include "astnode.h"


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
	class debugger;
	class value;
	class sqfnamespace;
	class virtualmachine
	{
	private:
		unsigned long long minstcount;
		unsigned long long mmaxinst;
		std::shared_ptr<sqf::vmstack> m_main_vmstack;
		std::shared_ptr<sqf::vmstack> m_active_vmstack;
		std::list<std::shared_ptr<scriptdata>> mspawns;
		std::basic_ostream<char, std::char_traits<char>>* mout;
		std::basic_ostream<char, std::char_traits<char>>* merr;
		std::basic_ostream<char, std::char_traits<char>>* mwrn;
		std::stringstream mout_buff;
		std::stringstream merr_buff;
		std::stringstream mwrn_buff;
		bool moutflag = false;
		bool merrflag = false;
		bool mwrnflag = false;
		bool mwrnenabled = true;
		bool mhaltflag;
		std::vector<size_t> mfreeobjids;
		std::vector<std::shared_ptr<innerobj>> mobjlist;
		std::shared_ptr<innerobj> mplayer_obj;

		std::shared_ptr<sqf::sqfnamespace> mmissionnamespace;
		std::shared_ptr<sqf::sqfnamespace> muinamespace;
		std::shared_ptr<sqf::sqfnamespace> mparsingnamespace;
		std::shared_ptr<sqf::sqfnamespace> mprofilenamespace;

		std::map<std::string, sqf::marker> mmarkers;

		std::map<int, size_t> mgroupidcounter;
		std::map<int, std::vector<std::shared_ptr<groupdata>>> mgroups;
		void performexecute(size_t exitAfter = ~0);
		std::vector<std::shared_ptr<dlops>> mlibraries;
		debugger* _debugger;
		bool mexitflag;
		int m_exitcode = 0;
		bool mallowsleep;
		bool m_perform_classname_checks;
		sqf::filesystem m_filesystem;
		std::chrono::system_clock::time_point mcreatedtimestamp;
	public:
		virtualmachine() : virtualmachine(0) {};
		virtualmachine(unsigned long long maxinst);

		std::chrono::system_clock::time_point get_created_timestamp() { return mcreatedtimestamp; }

		std::shared_ptr<innerobj> player_obj() { return mplayer_obj; }
		void player_obj(std::shared_ptr<innerobj> val) { mplayer_obj = val; }
		std::shared_ptr<sqf::vmstack> active_vmstack() { return m_active_vmstack; }
		std::shared_ptr<sqf::vmstack> active_vmstack() const { return m_active_vmstack; }
		const std::vector<std::shared_ptr<innerobj>>& get_objlist() { return mobjlist; }
		std::shared_ptr<sqf::sqfnamespace> missionnamespace() { return mmissionnamespace; }
		std::shared_ptr<sqf::sqfnamespace> uinamespace() { return muinamespace; }
		std::shared_ptr<sqf::sqfnamespace> parsingnamespace() { return mparsingnamespace; }
		std::shared_ptr<sqf::sqfnamespace> profilenamespace() { return mprofilenamespace; }



		std::stringstream& out() { moutflag = true; return mout_buff; }
		void out(std::basic_ostream<char, std::char_traits<char>>* strm) { mout = strm; }
		void out_buffprint(bool force = false)
		{
			if (!moutflag && !force)
				return;
			(*mout) << mout_buff.str();
			out_clear();
		}
		void out_clear() { mout_buff.str({}); moutflag = false; }
		bool out_hasdata() { return moutflag; }

		std::stringstream& err() { merrflag = true; return merr_buff; }
		void err(std::basic_ostream<char, std::char_traits<char>>* strm) { merr = strm; }
		void err_buffprint(bool force = false)
		{
			if (!merrflag && !force)
				return;
			(*merr) << merr_buff.str();
			err_clear();
		}
		void err_clear() { merr_buff.str({}); merrflag = false; }
		bool err_hasdata() { return merrflag; }

		std::stringstream& wrn()
		{
			mwrnflag = true;
			return mwrn_buff;
		}
		void wrn(std::basic_ostream<char, std::char_traits<char>>* strm) { mwrn = strm; }
		void wrn_buffprint(bool force = false)
		{
			if (!mwrnflag && !force)
				return;
			if (mwrnenabled)
			{
				(*mwrn) << mwrn_buff.str();
			}
			wrn_clear();
		}
		void wrn_clear() { mwrn_buff.str({}); mwrnflag = false; }
		bool wrn_hasdata() { return mwrnflag; }
		bool wrn_enabled() { return mwrnenabled; }
		void wrn_enabled(bool flag) { mwrnenabled = flag; }



		void execute();
		static std::string dbgsegment(const char* full, size_t off, size_t length);
		void exitflag(bool flag) { mexitflag = flag; }
		void exitflag(bool flag, int exitcode) { mexitflag = flag; m_exitcode = exitcode; }
		bool exitflag() { return mexitflag; }
		int exitcode() { return m_exitcode; }

		bool perform_classname_checks() { return m_perform_classname_checks; }
		void perform_classname_checks(bool f) { m_perform_classname_checks = f; }

		marker* get_marker(std::string key) { return mmarkers.find(key) == mmarkers.end() ? nullptr : &mmarkers[key]; }
		void set_marker(std::string key, marker m) { mmarkers[key] = m; }
		void remove_marker(std::string key) { mmarkers.erase(key); }
		const std::map<std::string, sqf::marker>& get_markers() { return mmarkers; }

		void parse_assembly(std::string);
		// Parses the provided code and prints the resulting
		// SQF-tree out into provided std::stringstream pointer
		void parse_sqf_tree(std::string, std::stringstream*);
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
		bool parse_sqf(std::string code, std::string filepath = "") { return parse_sqf(active_vmstack(), code, std::shared_ptr<sqf::callstack>(), filepath); }
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
		bool parse_sqf(std::string str, std::shared_ptr<sqf::callstack> cs, std::string filepath = "") { return parse_sqf(active_vmstack(), str, cs, filepath); }
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
		bool parse_sqf(std::shared_ptr<sqf::vmstack>, std::string, std::shared_ptr<sqf::callstack>, std::string = "");

		astnode parse_sqf_cst(std::string_view code, std::string filepath = "") { bool errflag = false; return parse_sqf_cst(code, errflag, filepath); }
        astnode parse_sqf_cst(std::string_view code, bool& errorflag, std::string filepath = "");
		void pretty_print_sqf(std::string code);
		void parse_config(std::string, std::shared_ptr<configdata>);
		bool errflag() const { return merrflag; }
		bool wrnflag() const { return mwrnflag; }
		void halt() { mhaltflag = true; }
		std::vector<std::shared_ptr<dlops>>& libraries() { return mlibraries; }
		bool allowsleep() const { return mallowsleep; }
		void allowsleep(bool flag) { mallowsleep = flag; }

		debugger* dbg() { return _debugger; }
		void dbg(debugger* debugger) { _debugger = debugger; }
		filesystem& get_filesystem() { return m_filesystem; }

		size_t push_obj(std::shared_ptr<sqf::innerobj> obj);
		void drop_obj(const sqf::innerobj * obj);
		std::shared_ptr<sqf::innerobj> get_obj_netid(size_t netid);
		std::string get_group_id(std::shared_ptr<sqf::sidedata>);
		void push_group(std::shared_ptr<sqf::groupdata>);
		void drop_group(std::shared_ptr<sqf::groupdata>);
		void push_spawn(std::shared_ptr<scriptdata> scrpt) { mspawns.push_back(scrpt); }
		static std::chrono::system_clock::time_point system_time();
	};
}