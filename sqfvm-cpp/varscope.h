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
		std::map<std::wstring, std::shared_ptr<value>> mvarmap;
		std::wstring mscopename;
		std::wstring tolowerstring(std::wstring s)
		{
			std::transform(s.begin(), s.end(), s.begin(), ::tolower);
			return s;
		}
	public:
		varscope() { mscopename = L""; }
		varscope(std::wstring name) { mscopename = name; }
		inline void setvar(std::wstring key, std::shared_ptr<value> value) { mvarmap[tolowerstring(key)] = value; }
		std::shared_ptr<value> getvar(std::wstring key);
		inline bool containsvar(std::wstring key) { auto it = mvarmap.find(tolowerstring(key)); return it != mvarmap.end(); }
		inline void setscopename(std::wstring newname) { mscopename = newname; }
		inline std::wstring getscopename(void) { return mscopename; }
		inline const std::map<std::wstring, std::shared_ptr<value>>& varmap(void) const { return mvarmap; }
	};
}