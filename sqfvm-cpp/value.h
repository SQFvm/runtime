#ifndef _VALUE
#define _VALUE 1

#if !defined(_TYPEINFO) & !defined(_TYPEINFO_)
#error value requires <typeinfo> header
#endif // !_TYPEINFO
#if !defined(_STRING) & !defined(_STRING_)
#error value requires <string> header
#endif // !_STRING
#if !defined(_VECTOR) & !defined(_VECTOR_)
#error value requires <vector> header
#endif // !_VECTOR
#if !defined(_ARRAY) & !defined(_ARRAY_)
#error value requires <vector> header
#endif // !_ARRAY
#if !defined(_MEMORY_) & !defined(_MEMORY_)
#error value requires <memory> header
#endif // !_STRING

#if !defined(_SQFTYPE)
#error value requires "sqftype.h" header
#endif // !_SQFTYPE
#if !defined(_DATA)
#error value requires "data.h" header
#endif // !_SQFTYPE

namespace sqf
{
	class value
	{
	private:
		data_s mdata;
		type mtype;
	public:
		value(std::vector<std::shared_ptr<value>>);
		value(std::wstring);
		value(float);
		value(double);
		value(wchar_t);
		value(short);
		value(int);
		value(long);
		value(bool);
		value();

		explicit operator float() const;
		explicit operator double() const;
		explicit operator char() const;
		explicit operator short() const;
		explicit operator int() const;
		explicit operator long() const;
		explicit operator bool() const;
		explicit operator std::wstring() const;
		explicit operator std::vector<std::shared_ptr<value>>() const;
		std::wstring name(void) const;
		inline std::wstring to_string(void) { return mdata.get() ? mdata->to_string() : std::wstring(L"any"); }
		type get_valuetype(void) const;
		inline void convert(type type)
		{
			if (mtype == type)
				return;
			mdata = sqf::convert(std::move(mdata), type);
		}
	};
	typedef std::shared_ptr<value> value_s;
	typedef std::weak_ptr<value> value_w;
	typedef std::unique_ptr<value> value_u;

}
#endif // !_VALUE