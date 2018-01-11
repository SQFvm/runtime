#include "full.h"

sqf::scalardata::scalardata()
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

std::string sqf::scalardata::to_string(void) const
{
	return std::to_string(mvalue);
}

sqf::scalardata::operator char() const
{
	return (char)mvalue;
}

sqf::scalardata::operator short() const
{
	return (short)mvalue;
}

sqf::scalardata::operator int() const
{
	return (int)mvalue;
}

sqf::scalardata::operator long() const
{
	return (long)mvalue;
}

sqf::scalardata::operator float() const
{
	return (float)mvalue;
}

sqf::scalardata::operator double() const
{
	return (double)mvalue;
}
