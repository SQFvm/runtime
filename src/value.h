#pragma once
#include <typeinfo>
#include <string>
#include <vector>
#include <array>
#include <memory>

#include "type.h"

namespace sqf
{
	class callstack;
	class data;
	class value
	{
	public:
        using cref = const value&;
	private:
		std::shared_ptr<sqf::data> mdata;
	public:
		value(std::vector<value>);
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
	    value(std::shared_ptr<sqf::data> d) { mdata = std::move(d); }
		//#TODO add a is_nil() function instead of comparing dtype
		explicit operator float() const;
		explicit operator double() const;
		explicit operator char() const;
		explicit operator short() const;
		explicit operator int() const;
		explicit operator long() const;
		//explicit operator bool() const;
		explicit operator std::string() const;
		explicit operator std::vector<sqf::value>() const;
        explicit operator type() const { return dtype(); };

		float as_float() const { return static_cast<float>(*this); }
		double as_double() const { return static_cast<double>(*this); }
		char as_char() const { return static_cast<char>(*this); }
		short as_short() const { return static_cast<short>(*this); }
		int as_int() const { return static_cast<int>(*this); }
		long as_long() const { return static_cast<long>(*this); }
		bool as_bool() const;
		std::string as_string() const { return static_cast<std::string>(*this); }
		std::vector<sqf::value> as_vector() const { return static_cast<std::vector<sqf::value>>(*this); }
        type dtype() const;
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

        bool equals(value::cref v) const;
        bool equals_exact(value::cref v) const;

        std::string tosqf() const;

        void convert(type type);
	};
}
