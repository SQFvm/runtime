#include "full.h"


sqf::value::value(std::vector<sqf::value_s> arr)
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

sqf::value::value(size_t num)
{
	mtype = type::SCALAR;
	mdata = std::make_shared<scalardata>(num);
}

sqf::value::value(std::shared_ptr<sqf::callstack> cs)
{
	mtype = type::CODE;
	mdata = std::make_shared<codedata>(cs);
}

sqf::value::value()
{
	mtype = type::NOTHING;
	mdata = data_s();
}

sqf::value::operator float() const
{
	auto data = mdata;
	if (mtype != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return *(std::dynamic_pointer_cast<scalardata>(data).get());
}
sqf::value::operator double() const
{
	auto data = mdata;
	if (mtype != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return *(std::dynamic_pointer_cast<scalardata>(data).get());
}


sqf::value::operator short() const
{
	auto data = mdata;
	if (mtype != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return *(std::dynamic_pointer_cast<scalardata>(data).get());
}

sqf::value::operator int() const
{
	auto data = mdata;
	if (mtype != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return *(std::dynamic_pointer_cast<scalardata>(data).get());
}

sqf::value::operator long() const
{
	auto data = mdata;
	if (mtype != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return *(std::dynamic_pointer_cast<scalardata>(data).get());
}

sqf::value::operator char() const
{
	auto data = mdata;
	if (mtype != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return *(std::dynamic_pointer_cast<scalardata>(data).get());
}

sqf::value::operator bool() const
{
	auto data = mdata;
	if (mtype != BOOL && mtype != IF)
	{
		data = sqf::convert(data, SCALAR);
	}
	return *(std::dynamic_pointer_cast<booldata>(data).get());
}
sqf::value::operator std::wstring() const
{
	auto data = mdata;
	if (mtype != STRING)
	{
		data = sqf::convert(data, STRING);
	}
	return *(std::dynamic_pointer_cast<stringdata>(data).get());
}
sqf::value::operator std::vector<std::shared_ptr<sqf::value>>() const
{
	auto data = mdata;
	if (mtype != ARRAY)
	{
		data = sqf::convert(data, ARRAY);
	}
	return *(std::dynamic_pointer_cast<arraydata>(data).get());
}