#ifndef _STRINGDATA
#define _STRINGDATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error stringdata requires <string> header
#endif // !_STRING
#if !defined(_MEMORY_) & !defined(_MEMORY_)
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
		stringdata();
		stringdata(std::wstring);
		virtual std::wstring to_string(void) const;
		operator const wchar_t*() const;
		operator std::wstring() const;

		static std::wstring parse_from_sqf(std::wstring);
		static std::wstring parse_to_sqf(std::wstring);
	};
	typedef std::shared_ptr<stringdata> string_s;
	typedef std::weak_ptr<stringdata> string_w;
	typedef std::unique_ptr<stringdata> string_u;
}
#endif // !_STRINGDATA