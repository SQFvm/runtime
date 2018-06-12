#pragma once
#include <typeinfo>
#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <map>
#include <list>
#include <sstream>

#include "dlops.h"


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
		bool merrflag;
		bool mwrnflag;
		std::vector<size_t> mfreeobjids;
		std::vector<std::shared_ptr<innerobj>> mobjlist;

		std::map<int, size_t> mgroupidcounter;
		std::map<int, std::vector<std::shared_ptr<groupdata>>> mgroups;
		void performexecute(size_t exitAfter = ~0);
		std::vector<std::shared_ptr<dlops>> mlibraries;
		debugger* _debugger;
	public:
		inline std::basic_ostream<char, std::char_traits<char>>& out(void) const { return *mout; }
		inline std::basic_ostream<char, std::char_traits<char>>& err(void) const { /* on purpose */((virtualmachine*)this)->merrflag = true; return *merr; }
		inline std::basic_ostream<char, std::char_traits<char>>& wrn(void) const { /* on purpose */((virtualmachine*)this)->mwrnflag = true; return *mwrn; }
		inline void out(std::basic_ostream<char, std::char_traits<char>>* strm) { mout = strm; }
		inline void err(std::basic_ostream<char, std::char_traits<char>>* strm) { merr = strm; }
		inline void wrn(std::basic_ostream<char, std::char_traits<char>>* strm) { mwrn = strm; }
		virtualmachine() : virtualmachine(0) {};
		virtualmachine(unsigned long long maxinst);
		void execute(void);
		inline std::shared_ptr<sqf::vmstack> stack(void) const { return mactivestack; }
		static std::string dbgsegment(const char* full, size_t off, size_t length);

		void parse_assembly(std::string);
		void parse_sqf(std::string, std::stringstream*);
		inline void parse_sqf(std::string code, std::string filepath = "") { parse_sqf(stack(), code, std::shared_ptr<sqf::callstack>(), filepath); }
		inline void parse_sqf(std::string str, std::shared_ptr<sqf::callstack> cs, std::string filepath = "") { parse_sqf(stack(), str, cs, filepath); }
		void parse_sqf(std::shared_ptr<sqf::vmstack>, std::string, std::shared_ptr<sqf::callstack>, std::string = "");
		void parse_config(std::string, std::shared_ptr<configdata>);
		bool errflag(void) const { return merrflag; }
		bool wrnflag(void) const { return mwrnflag; }
		std::vector<std::shared_ptr<dlops>>& libraries(void) { return mlibraries; }

		debugger* dbg(void) { return _debugger; }
		void dbg(debugger* debugger) { _debugger = debugger; }

		size_t push_obj(std::shared_ptr<sqf::innerobj> obj);
		std::shared_ptr<sqf::innerobj> get_obj_netid(size_t netid);
		std::string get_group_id(std::shared_ptr<sqf::sidedata>);
		void push_group(std::shared_ptr<sqf::groupdata>);
		inline void push_spawn(std::shared_ptr<scriptdata> scrpt) { mspawns.push_back(scrpt); }
	};
}