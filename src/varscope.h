#pragma once
#include <map>
#include <string>
#include <memory>
#include <algorithm>

namespace sqf
{
	class value;
	class varscope
	{
	private:
		std::map<std::string, std::shared_ptr<value>> mvarmap;
		std::string mscopename;
		std::string tolowerstring(std::string s)
		{
			std::transform(s.begin(), s.end(), s.begin(), ::tolower);
			return s;
		}
	public:
		varscope() { mscopename = ""; }
		varscope(std::string name) { mscopename = name; }
		void setvar(std::string key, std::shared_ptr<value> value) { mvarmap[tolowerstring(key)] = value; }
		std::shared_ptr<value> getvar_empty(std::string key)
		{
			auto it = mvarmap.find(tolowerstring(key));
			return it == mvarmap.end() ? std::shared_ptr<value>() : it->second;
		}
		std::shared_ptr<value> getvar(std::string key);
		bool containsvar(std::string key) { auto it = mvarmap.find(tolowerstring(key)); return it != mvarmap.end(); }
		void setscopename(std::string newname) { mscopename = newname; }
		std::string getscopename() { return mscopename; }
		const std::map<std::string, std::shared_ptr<value>>& varmap() const { return mvarmap; }
	};
}