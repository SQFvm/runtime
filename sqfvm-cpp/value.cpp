#include "full.h"

using namespace sqf;

template<size_t SIZE> sqf::value::value(std::array<value_s, SIZE> arr)
{
	mtype = type::ARRAY;
	mdata = std::make_shared<arraydata>(arr);
}
sqf::value::value(std::vector<value_s> arr)
{
	mtype = type::ARRAY;
	mdata = std::make_shared<arraydata>(arr);
}

sqf::value::value(std::wstring s)
{
	mtype = type::STRING;
	mdata = std::make_shared<stringdata>(s);
}

sqf::value::value(wchar_t c)
{
	mtype = type::STRING;
	mdata = std::make_shared<stringdata>(std::wstring(1, c));
}

sqf::value::value(float num)
{
	mtype = type::SCALAR;
	mdata = std::make_shared<scalardata>(num);
}


sqf::value::value(double num)
{
	mtype = type::SCALAR;
	mdata = std::make_shared<scalardata>(num);
}

sqf::value::value(short num)
{
	mtype = type::SCALAR;
	mdata = std::make_shared<scalardata>(num);
}

sqf::value::value(int num)
{
	mtype = type::SCALAR;
	mdata = std::make_shared<scalardata>(num);
}

sqf::value::value(long num)
{
	mtype = type::SCALAR;
	mdata = std::make_shared<scalardata>(num);
}

sqf::value::value(bool flag)
{
	mtype = type::BOOL;
	mdata = std::make_shared<booldata>(flag);
}

sqf::value::value()
{
	mtype = type::NOTHING;
	mdata = data_s();
}

sqf::value::operator float() const
{
}
sqf::value::operator double() const
{
}

sqf::value::operator char() const
{
}

sqf::value::operator short() const
{
}

sqf::value::operator int() const
{
}

sqf::value::operator long() const
{
}

sqf::value::operator bool() const
{
}


std::wstring sqf::value::name(void) const
{
	return std::wstring();
}

type sqf::value::get_valuetype(void) const
{
	return mtype;
}
