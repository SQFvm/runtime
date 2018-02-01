#include "full.h"

sqf::scalardata::scalardata(void)
{
	mvalue = 0;
}

sqf::scalardata::scalardata(char num)
{
	mvalue = (double)num;
}

sqf::scalardata::scalardata(short num)
{
	mvalue = (double)num;
}

sqf::scalardata::scalardata(int num)
{
	mvalue = (double)num;
}

sqf::scalardata::scalardata(long num)
{
	mvalue = (double)num;
}

sqf::scalardata::scalardata(float num)
{
	mvalue = (double)num;
}

sqf::scalardata::scalardata(double num)
{
	mvalue = (double)num;
}

sqf::scalardata::scalardata(size_t num)
{
	mvalue = (double)num;
}

std::wstring sqf::scalardata::to_string(void) const
{
	return sqf::stringdata::parse_to_sqf(std::to_wstring(mvalue));
}

sqf::scalardata::operator char(void) const
{
	return (char)mvalue;
}

sqf::scalardata::operator short(void) const
{
	return (short)mvalue;
}

sqf::scalardata::operator int(void) const
{
	return (int)mvalue;
}

sqf::scalardata::operator long(void) const
{
	return (long)mvalue;
}

sqf::scalardata::operator float(void) const
{
	return (float)mvalue;
}

sqf::scalardata::operator double(void) const
{
	return (double)mvalue;
}

sqf::scalardata::operator size_t(void) const
{
	return (size_t)mvalue;
}
