#ifndef _NAMESPACE
#define _NAMESPACE 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error namespace requires <memory> header
#endif // !_MEMORY
#if !defined(_UNORDERED_MAP) & !defined(_UNORDERED_MAP_)
#error namespace requires <unordered_map> header
#endif // !_UNORDERED_MAP
#if !defined(_STRING) & !defined(_STRING_)
#error namespace requires <string> header
#endif // !_STRING
#if !defined(_VARSCOPE)
#error namespace requires <string> header
#endif // !_VARSCOPE

namespace sqf
{
	class sqfnamespace : public data, public varscope
	{
	private:
		std::wstring mname;
	public:
		inline sqfnamespace(std::wstring name) { mname = name; }
		inline virtual std::wstring tosqf(void) const { return mname; }
		inline std::wstring get_name(void) { return mname; }
		virtual bool equals(std::shared_ptr<data> d) const { return this == d.get(); }
	};
	typedef std::shared_ptr<sqfnamespace> namespace_s;
	typedef std::weak_ptr<sqfnamespace> namespace_w;
	typedef std::unique_ptr<sqfnamespace> namespace_u;
}

#endif // !_NAMESPACE
