#ifndef _VARSCOPE
#define _VARSCOPE 1


#if !defined(_MEMORY) & !defined(_MEMORY_)
#error varscope requires <memory> header
#endif // !_MEMORY
#if !defined(_UNORDERED_MAP) & !defined(_UNORDERED_MAP_)
#error varscope requires <unordered_map> header
#endif // !_UNORDERED_MAP
#if !defined(_STRING) & !defined(_STRING_)
#error varscope requires <string> header
#endif // !_STRING

namespace sqf
{
	class value;
	class varscope
	{
	private:
		std::unordered_map<std::wstring, std::shared_ptr<value>> mvarmap;
		std::wstring mscopename;
	public:
		varscope() { mscopename = L""; }
		varscope(std::wstring name) { mscopename = name; }
		inline void setvar(std::wstring key, std::shared_ptr<value> value) { mvarmap[key] = value; }
		inline std::shared_ptr<value> getvar(std::wstring key) { auto it = mvarmap.find(key); return it == mvarmap.end() ? std::make_shared<value>() : it->second; }
		inline bool containsvar(std::wstring key) { auto it = mvarmap.find(key); return it != mvarmap.end(); }
		inline void setscopename(std::wstring newname) { mscopename = newname; }
		inline std::wstring getscopename(void) { return mscopename; }
	};

	typedef std::shared_ptr<varscope> varscope_s;
	typedef std::weak_ptr<varscope> varscope_w;
	typedef std::unique_ptr<varscope> varscope_u;
}

#endif // !_VARSCOPE
