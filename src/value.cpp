#include "value.h"
#include "arraydata.h"
#include "booldata.h"
#include "scalardata.h"
#include "stringdata.h"
#include "codedata.h"


sqf::value::value(std::vector<sqf::value> arr)
{
	mdata = std::make_shared<arraydata>(arr);
}

sqf::value::value(std::string s)
{
	mdata = std::make_shared<stringdata>(s);
}

sqf::value::value(char c)
{
	mdata = std::make_shared<stringdata>(std::string(1, c));
}

sqf::value::value(float num)
{
	mdata = std::make_shared<scalardata>(static_cast<float>(num));
}

sqf::value::value(double num)
{
	mdata = std::make_shared<scalardata>(static_cast<float>(num));
}

sqf::value::value(short num)
{
	mdata = std::make_shared<scalardata>(static_cast<float>(num));
}

sqf::value::value(int num)
{
	mdata = std::make_shared<scalardata>(static_cast<float>(num));
}

sqf::value::value(long num)
{
	mdata = std::make_shared<scalardata>(static_cast<float>(num));
}

sqf::value::value(bool flag)
{
	mdata = std::make_shared<booldata>(flag);
}

sqf::value::value(size_t num)
{
	mdata = std::make_shared<scalardata>(static_cast<float>(num));
}

sqf::value::value(std::shared_ptr<sqf::callstack> cs)
{
	mdata = std::make_shared<codedata>(cs);
}

sqf::value::value()
{
	mdata = std::shared_ptr<sqf::data>();
}

sqf::value::operator float() const
{
	auto data = mdata;
    auto type = dtype();
	if (type != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return *(std::dynamic_pointer_cast<scalardata>(data).get());
}
sqf::value::operator double() const
{
	auto data = mdata;
    auto type = dtype();
	if (type != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return *(std::dynamic_pointer_cast<scalardata>(data).get());
}


sqf::value::operator short() const
{
	auto data = mdata;
    auto type = dtype();
	if (type != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return static_cast<short>(*(std::dynamic_pointer_cast<scalardata>(data).get()));
}

sqf::value::operator int() const
{
	auto data = mdata;
    auto type = dtype();
	if (type != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return static_cast<int>(*(std::dynamic_pointer_cast<scalardata>(data).get()));
}

sqf::value::operator long() const
{
	auto data = mdata;
    auto type = dtype();
	if (type != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return static_cast<long>(*(std::dynamic_pointer_cast<scalardata>(data).get()));
}

sqf::value::operator char() const
{
	auto data = mdata;
    auto type = dtype();
	if (type != SCALAR)
	{
		data = sqf::convert(data, SCALAR);
	}
	return static_cast<char>(*(std::dynamic_pointer_cast<scalardata>(data).get()));
}

sqf::value::operator bool() const
{
	auto data = mdata;
    auto type = dtype();
	if (type != BOOL && type != IF)
	{
		data = sqf::convert(data, SCALAR);
	}
	return static_cast<bool>(*(std::dynamic_pointer_cast<booldata>(data).get()));
}
sqf::value::operator std::string() const
{
	auto data = mdata;
    auto type = dtype();
	if (type != STRING)
	{
		data = sqf::convert(data, STRING);
	}
	return *(std::dynamic_pointer_cast<stringdata>(data).get());
}
sqf::value::operator std::vector<sqf::value>() const
{
	auto data = mdata;
    auto type = dtype();
	if (type != ARRAY && type != CONFIG)
	{
		data = sqf::convert(data, ARRAY);
	}
	return *(std::dynamic_pointer_cast<arraydata>(data).get());
}