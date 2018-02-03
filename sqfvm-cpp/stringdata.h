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
		std::wstring mvalue_parsed;
	public:
		stringdata(void);
		stringdata(std::wstring);
		virtual std::wstring to_string(void) const;
		operator const wchar_t*(void) const;
		operator std::wstring(void) const;

		static std::wstring parse_from_sqf(std::wstring);
		static std::wstring parse_to_sqf(std::wstring);
		inline size_t length(void) { return mvalue_parsed.size(); }
		virtual bool equals(std::shared_ptr<data> d) const { wstr_cmpi(mvalue.c_str(), 0, std::dynamic_pointer_cast<stringdata>(d)->mvalue.c_str(), 0); }
	};
	typedef std::shared_ptr<stringdata> string_s;
	typedef std::weak_ptr<stringdata> string_w;
	typedef std::unique_ptr<stringdata> string_u;
}
#endif // !_STRINGDATA