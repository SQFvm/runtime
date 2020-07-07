#include "value.h"
#include "arraydata.h"
#include "booldata.h"
#include "scalardata.h"
#include "stringdata.h"
#include "codedata.h"
#include "data.h"
#include "convert.h"

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

bool sqf::value::equals(value::cref v) const {
    return (mdata && v.mdata && mdata->dtype() == v.mdata->dtype() && ((mdata.get() == v.mdata.get()) || mdata->equals(v.mdata)));
}
bool sqf::value::equals_exact(value::cref v) const {
    return (mdata && v.mdata && mdata->dtype() == v.mdata->dtype() && ((mdata.get() == v.mdata.get()) || mdata->equals_exact(v.mdata)));
}

std::string sqf::value::tosqf() const {
    if (mdata) {
        return mdata->tosqf();
    } else if (!mdata || mdata->dtype() == type::NOTHING) {
        return "nil";
    } else if (mdata->dtype() == type::ANY) {
        return "any";
    } else {
        return "";
    }
}

void sqf::value::convert(type type) {
    if (mdata->dtype() == type)
        return;
    mdata = sqf::convert(std::move(mdata), type);
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

bool sqf::value::as_bool() const
{
	auto data = mdata;
    auto type = dtype();
	if (type != BOOL && type != IF)
	{
		data = sqf::convert(data, BOOL);
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

sqf::type sqf::value::dtype() const {
    if (!mdata) return type::NOTHING;
    return mdata->dtype();
}
