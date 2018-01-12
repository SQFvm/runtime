#include "full.h"
sqf::booldata::booldata()
{
	mflag = false;
}
sqf::booldata::booldata(bool flag)
{
	mflag = flag;
}
std::wstring sqf::booldata::to_string(void) const
{
	return std::wstring(mflag ? L"true" : L"false");
}

sqf::booldata::operator bool() const
{
	return mflag;
}
