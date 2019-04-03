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

		float as_float() const { return *this; }
		double as_double() const { return *this; }
		char as_char() const { return *this; }
		short as_short() const { return *this; }
		int as_int() const { return *this; }
		long as_long() const { return *this; }
		bool as_bool() const { return *this; }
		std::string as_string() const { return *this; }
		std::vector<std::shared_ptr<sqf::value>> as_vector() const { return *this; }
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

		std::string tosqf()
		{
			if (mdata)
			{
				return mdata->tosqf();
			}
			else if (mtype == type::NOTHING)
			{
				return "nil";
			}
			else if (mtype == type::ANY)
			{
				return "any";
			}
			else
			{
				return "";
			}
		}
		void convert(type type)
		{
			if (mtype == type)
				return;
			mdata = sqf::convert(std::move(mdata), type);
		}
	};
}
