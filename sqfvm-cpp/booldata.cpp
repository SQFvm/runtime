#include "full.h"
sqf::booldata::booldata()
{
	mflag = false;
}
sqf::booldata::booldata(bool flag)
{
	mflag = flag;
}
std::string sqf::booldata::to_string(void) const
{
	return std::string(mflag ? "true" : "false");
}

sqf::booldata::operator bool() const
{
	return mflag;
}
