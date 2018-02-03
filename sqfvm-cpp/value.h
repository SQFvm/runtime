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
#if !defined(_MEMORY) & !defined(_MEMORY_)
#error value requires <memory> header
#endif // !_MEMORY_

#if !defined(_SQFTYPE)
#error value requires "sqftype.h" header
#endif // !_SQFTYPE
#if !defined(_DATA)
#error value requires "data.h" header
#endif // !_SQFTYPE

namespace sqf
{
	class callstack;
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
		value(size_t);
		value(std::shared_ptr<callstack>);
		value();
		value(data_s d, type t) { mdata = d; mtype = t; }

		operator float() const;
		operator double() const;
		operator char() const;
		operator short() const;
		operator int() const;
		operator long() const;
		operator bool() const;
		operator std::wstring() const;
		operator std::vector<std::shared_ptr<sqf::value>>() const;
		operator type() const { return mtype; }

		float as_float(void) const { return *this; }
		double as_double(void) const { return *this; }
		char as_char(void) const { return *this; }
		short as_short(void) const { return *this; }
		int as_int(void) const { return *this; }
		long as_long(void) const { return *this; }
		bool as_bool(void) const { return *this; }
		std::wstring as_string(void) const { return *this; }
		std::vector<std::shared_ptr<sqf::value>> as_vector(void) const { return *this; }
		type as_type(void) const { return *this; }
		data_s as_data(void) const { return mdata; }
		bool equals(std::shared_ptr<sqf::value> v) const { return mtype == v->mtype ? mdata->equals(v->mdata) : false; }

		std::wstring name(void) const;
		inline std::wstring to_string(void) { return mdata.get() ? mdata->to_string() : std::wstring(L"nil"); }
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