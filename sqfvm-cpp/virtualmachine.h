#pragma once
#include <typeinfo>
#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <map>


namespace sqf
{
	class innerobj;
	class configdata;
	class callstack;
	class vmstack;
	class objectdata;
	class groupdata;
	class sidedata;
	class virtualmachine
	{
	private:
		unsigned long long minstcount;
		unsigned long long mmaxinst;
		std::shared_ptr<sqf::vmstack> mstack;
		std::wostream* mout;
		std::wostream* merr;
		std::wostream* mwrn;
		bool merrflag;
		bool mwrnflag;
		std::vector<size_t> mfreeobjids;
		std::vector<std::shared_ptr<innerobj>> mobjlist;

		std::map<int, size_t> mgroupidcounter;
		std::map<int, std::vector<std::shared_ptr<groupdata>>> mgroups;
	public:
		std::wostream& out(void) const { return *mout; }
		std::wostream& err(void) const { /* on purpose */((virtualmachine*)this)->merrflag = true; return *merr; }
		std::wostream& wrn(void) const { /* on purpose */((virtualmachine*)this)->mwrnflag = true; return *mwrn; }
		virtualmachine() : virtualmachine(0) {};
		virtualmachine(unsigned long long maxinst);
		void execute(void);
		inline std::shared_ptr<sqf::vmstack> stack(void) const { return mstack; }
		static std::wstring dbgsegment(const wchar_t* full, size_t off, size_t length);

		void parse_assembly(std::wstring);
		inline void parse_sqf(std::wstring code) { parse_sqf(code, std::shared_ptr<sqf::callstack>()); }
		void parse_sqf(std::wstring, std::shared_ptr<sqf::callstack>);
		void parse_config(std::wstring, std::shared_ptr<configdata>);
		bool errflag(void) const { return merrflag; }
		bool wrnflag(void) const { return mwrnflag; }

		size_t push_obj(std::shared_ptr<sqf::innerobj> obj);
		std::shared_ptr<sqf::innerobj> get_obj_netid(size_t netid);
		std::wstring get_group_id(std::shared_ptr<sqf::sidedata>);
		void push_group(std::shared_ptr<sqf::groupdata>);
	};
}