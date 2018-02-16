#include "convert.h"
#include "stringdata.h"
#include "scalardata.h"
#include "booldata.h"
#include <cwctype>


using namespace sqf;

std::shared_ptr<sqf::data> sqf::convert(std::shared_ptr<sqf::data> val, type type)
{
	std::wstring stringifiedval = val.get() ? val->tosqf() : L"nil";
	auto stringval = sqf::stringdata::parse_from_sqf(stringifiedval);
	switch (type)
	{
	case sqf::SCALAR:
		return std::make_shared<scalardata>(wcstod(stringval.c_str(), 0));
	case sqf::BOOL:
		if (wstr_cmpi(stringval.c_str(), stringval.length(), L"true", 4) == 0)
		{
			return std::make_shared<booldata>(true);
		}
		else if (wstr_cmpi(stringval.c_str(), stringval.length(), L"false", 5) == 0)
		{
			return std::make_shared<booldata>(false);
		}
		else
		{
			return std::make_shared<booldata>(wcstod(stringval.c_str(), 0) > 0);
		}
	case sqf::STRING:
		return std::make_shared<stringdata>(stringifiedval);
	case sqf::EXCEPTION:
		return val;
	case sqf::ARRAY:
	case sqf::TEXT:
	case sqf::NA:
	case sqf::NOTHING:
	case sqf::ANY:
	case sqf::NAMESPACE:
	case sqf::NaN:
	case sqf::VECTOR:
	case sqf::CODE:
	case sqf::OBJECT:
	case sqf::WITH:
	case sqf::SWITCH:
	case sqf::IF:
	case sqf::WHILE:
	case sqf::FOR:
	case sqf::TRANS:
	case sqf::ORIENT:
	case sqf::SIDE:
	case sqf::GROUP:
	case sqf::SCRIPT:
	case sqf::TARGET:
	case sqf::JCLASS:
	case sqf::CONFIG:
	case sqf::DISPLAY:
	case sqf::CONTROL:
	case sqf::NetObject:
	case sqf::SUBGROUP:
	case sqf::TEAM_MEMBER:
	case sqf::TASK:
	case sqf::DIARY_RECORD:
	case sqf::LOCATION:
	default:
		return std::shared_ptr<sqf::data>();
	}
}
