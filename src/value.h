#pragma once
#include <typeinfo>
#include <string>
#include <vector>
#include <array>
#include <memory>

#include "type.h"
#include "data.h"
#include "convert.h"

namespace sqf
{
	class callstack;
	class data;
	class value
	{
	private:
		std::shared_ptr<sqf::data> mdata;
		type mtype;
	public:
		value(std::vector<std::shared_ptr<value>>);
		value(std::string);
		value(char* str) : value(std::string(str)) {}
		value(const char* str) : value(std::string(str)) {}
		value(float);
		value(double);
		value(char);
		value(short);
		value(int);
		value(long);
		value(bool);
		value(size_t);
		value(std::shared_ptr<callstack>);
		value();
		value(std::shared_ptr<sqf::data> d, type t) { mdata = d; mtype = t; }
		value(type t) { mtype = t; }

		operator float() const;
		operator double() const;
		operator char() const;
		operator short() const;
		operator int() const;
		operator long() const;
		operator bool() const;
		operator std::string() const;
		operator std::vector<std::shared_ptr<sqf::value>>() const;
		operator type() const { return mtype; }

		inline float as_float() const { return *this; }
		inline double as_double() const { return *this; }
		inline char as_char() const { return *this; }
		inline short as_short() const { return *this; }
		inline int as_int() const { return *this; }
		inline long as_long() const { return *this; }
		inline bool as_bool() const { return *this; }
		inline std::string as_string() const { return *this; }
		inline std::vector<std::shared_ptr<sqf::value>> as_vector() const { return *this; }
		type dtype() const { return *this; }
		std::shared_ptr<sqf::data> data() const { return mdata; }

		///Tries to convert to T, if it fails it returns nullptr
		template <class T>
		std::shared_ptr<T> data_try_as() const {
			static_assert(std::is_base_of<sqf::data, T>::value, "value::data_try_as<T>() can only convert to sqf::data types");
			return std::dynamic_pointer_cast<T>(mdata);
		}
		template<class T>
		std::shared_ptr<T> data() const {
			static_assert(std::is_base_of<sqf::data, T>::value, "value::data<T>() can only convert to sqf::data types");
			return std::static_pointer_cast<T>(mdata);
		}
		bool equals(std::shared_ptr<sqf::value> v) const { return mtype == v->mtype ? mdata->equals(v->mdata) : false; }

		inline std::string tosqf() { return mdata.get() ? mdata->tosqf() : mtype == type::NOTHING ? std::string("ni") : mtype == type::ANY ? std::string("any") : std::string(); }
		inline void convert(type type)
		{
			if (mtype == type)
				return;
			mdata = sqf::convert(std::move(mdata), type);
		}
	};
}
