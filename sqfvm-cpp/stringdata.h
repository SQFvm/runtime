#ifndef _STRINGDATA
#define _STRINGDATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error stringdata requires <string> header
#endif // !_STRING
#if !defined(_MEMORY) & !defined(_MEMORY_)
#error stringdata requires <memory> header
#endif // !_STRING
#if !defined(_DATA)
#error stringdata requires "data.h" header
#endif // !_DATA

namespace sqf
{
	class stringdata : public data
	{
	private:
		std::wstring mvalue;
	public:
		stringdata(void) : mvalue(std::wstring()) {}
		stringdata(std::wstring s) : mvalue(s[0] != L'"' && s[0] != L'\'' ? s : parse_from_sqf(s)) {}
		stringdata(std::wstring s, bool parse) : mvalue(parse ? parse_from_sqf(s) : s) {}
		virtual std::wstring tosqf(void) const { return parse_to_sqf(mvalue); }
		operator const wchar_t*(void) const { return mvalue.c_str(); }
		operator std::wstring(void) const { return mvalue; }

		static std::wstring parse_from_sqf(std::wstring);
		static std::wstring parse_to_sqf(std::wstring);
		inline size_t length(void) { return mvalue.size(); }
		virtual bool equals(std::shared_ptr<data> d) const { return 0 == wstr_cmpi(mvalue.c_str(), 0, std::dynamic_pointer_cast<stringdata>(d)->mvalue.c_str(), 0); }
	};
	typedef std::shared_ptr<stringdata> string_s;
	typedef std::weak_ptr<stringdata> string_w;
	typedef std::unique_ptr<stringdata> string_u;
}
#endif // !_STRINGDATA