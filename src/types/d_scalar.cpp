#include "scalardata.h"
#include <sstream>
#include <iomanip>


std::string sqf::types::d_scalar::to_string_sqf() const
{
	if (decimals == -1)
	{
		auto bufflen = std::snprintf(nullptr, 0, "%g", mvalue) + 1;
		auto buff = new char[bufflen];
		std::snprintf(buff, bufflen, "%g", mvalue);
		auto str = std::string(buff, bufflen - 1);
		delete[] buff;
		return str;
	}
	else
	{
		auto bufflen = std::snprintf(nullptr, 0, "%0.*f", decimals, mvalue) + 1;
		auto buff = new char[bufflen];
		std::snprintf(buff, bufflen, "%0.*f", decimals, mvalue);
		auto str = std::string(buff, bufflen - 1);
		delete[] buff;
		return str;
	}
}

std::string sqf::types::d_scalar::to_string() const
{
	if (decimals == -1)
	{
		auto bufflen = std::snprintf(nullptr, 0, "%g", mvalue) + 1;
		auto buff = new char[bufflen];
		std::snprintf(buff, bufflen, "%g", mvalue);
		auto str = std::string(buff, bufflen - 1);
		delete[] buff;
		return str;
	}
	else
	{
		auto bufflen = std::snprintf(nullptr, 0, "%0.*f", decimals, mvalue) + 1;
		auto buff = new char[bufflen];
		std::snprintf(buff, bufflen, "%0.*f", decimals, mvalue);
		auto str = std::string(buff, bufflen - 1);
		delete[] buff;
		return str;
	}
}
