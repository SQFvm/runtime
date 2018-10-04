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
		std::shared_ptr<sqf::vmstack> mmainstack;
		std::shared_ptr<sqf::vmstack> mactivestack;
		std::list<std::shared_ptr<scriptdata>> mspawns;
		std::basic_ostream<char, std::char_traits<char>>* mout;
		std::basic_ostream<char, std::char_traits<char>>* merr;
		std::basic_ostream<char, std::char_traits<char>>* mwrn;
		std::stringstream mout_buff;
		std::stringstream merr_buff;
		std::stringstream mwrn_buff;
		bool moutflag;
		bool merrflag;
		bool mwrnflag;
		std::vector<size_t> mfreeobjids;
		std::vector<std::shared_ptr<innerobj>> mobjlist;

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
		bool mallowsleep;
		bool mperformclassnamechecks;
	public:
		const std::vector<std::shared_ptr<innerobj>>& get_objlist() { return mobjlist; }
		std::shared_ptr<sqf::sqfnamespace> missionnamespace() { return mmissionnamespace; }
		std::shared_ptr<sqf::sqfnamespace> uinamespace() { return muinamespace; }
		std::shared_ptr<sqf::sqfnamespace> parsingnamespace() { return mparsingnamespace; }
		std::shared_ptr<sqf::sqfnamespace> profilenamespace() { return mprofilenamespace; }
		std::stringstream& out() { /* on purpose */((virtualmachine*)this)->moutflag = true; return mout_buff; }
		std::stringstream& err() { /* on purpose */((virtualmachine*)this)->merrflag = true; return merr_buff; }
		std::stringstream& wrn() { /* on purpose */((virtualmachine*)this)->mwrnflag = true; return mwrn_buff; }
		void out(std::basic_ostream<char, std::char_traits<char>>* strm) { mout = strm; }
		void out_buffprint() { (*mout) << mout_buff.str(); mout_buff.str(std::string()); }
		void err(std::basic_ostream<char, std::char_traits<char>>* strm) { merr = strm; }
		void err_buffprint() { (*merr) << merr_buff.str(); merr_buff.str(std::string()); }
		void wrn(std::basic_ostream<char, std::char_traits<char>>* strm) { mwrn = strm; }
		void wrn_buffprint() { (*mwrn) << mwrn_buff.str(); mwrn_buff.str(std::string()); }
		virtualmachine() : virtualmachine(0) {};
		virtualmachine(unsigned long long maxinst);
		void execute();
		std::shared_ptr<sqf::vmstack> stack() const { return mactivestack; }
		static std::string dbgsegment(const char* full, size_t off, size_t length);
		void exitflag(bool flag) { mexitflag = flag; }
		bool exitflag() { return mexitflag; }

		bool perform_classname_checks() { return mperformclassnamechecks; }
		void perform_classname_checks(bool f) { mperformclassnamechecks = f; }

		marker* get_marker(std::string key) { return mmarkers.find(key) == mmarkers.end() ? nullptr : &mmarkers[key]; }
		void set_marker(std::string key, marker m) { mmarkers[key] = m; }
		void remove_marker(std::string key) { mmarkers.erase(key); }
		const std::map<std::string, sqf::marker>& get_markers() { return mmarkers; }

		void parse_assembly(std::string);
		void parse_sqf(std::string, std::stringstream*);
		void parse_sqf(std::string code, std::string filepath = "") { parse_sqf(stack(), code, std::shared_ptr<sqf::callstack>(), filepath); }
		void parse_sqf(std::string str, std::shared_ptr<sqf::callstack> cs, std::string filepath = "") { parse_sqf(stack(), str, cs, filepath); }
		void parse_sqf(std::shared_ptr<sqf::vmstack>, std::string, std::shared_ptr<sqf::callstack>, std::string = "");
		void pretty_print_sqf(std::string code);
		void parse_config(std::string, std::shared_ptr<configdata>);
		bool errflag() const { return merrflag; }
		bool wrnflag() const { return mwrnflag; }
		std::vector<std::shared_ptr<dlops>>& libraries() { return mlibraries; }
		bool allowsleep() const { return mallowsleep; }
		void allowsleep(bool flag) { mallowsleep = flag; }

		debugger* dbg() { return _debugger; }
		void dbg(debugger* debugger) { _debugger = debugger; }

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