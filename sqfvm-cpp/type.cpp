#include <string>
#include "type.h"

std::wstring sqf::type_str(type t)
{
	switch (t)
	{
	case sqf::NA: return L"NA";
	case sqf::NOTHING: return L"NOTHING";
	case sqf::ANY: return L"ANY";
	case sqf::SCALAR: return L"SCALAR";
	case sqf::BOOL: return L"BOOL";
	case sqf::ARRAY: return L"ARRAY";
	case sqf::STRING: return L"STRING";
	case sqf::NAMESPACE: return L"NAMESPACE";
	case sqf::NaN: return L"NaN";
	case sqf::IF: return L"IF";
	case sqf::WHILE: return L"WHILE";
	case sqf::FOR: return L"FOR";
	case sqf::SWITCH: return L"SWITCH";
	case sqf::EXCEPTION: return L"EXCEPTION";
	case sqf::WITH: return L"WITH";
	case sqf::CODE: return L"CODE";
	case sqf::OBJECT: return L"OBJECT";
	case sqf::VECTOR: return L"VECTOR";
	case sqf::TRANS: return L"TRANS";
	case sqf::ORIENT: return L"ORIENT";
	case sqf::SIDE: return L"SIDE";
	case sqf::GROUP: return L"GROUP";
	case sqf::TEXT: return L"TEXT";
	case sqf::SCRIPT: return L"SCRIPT";
	case sqf::TARGET: return L"TARGET";
	case sqf::JCLASS: return L"JCLASS";
	case sqf::CONFIG: return L"CONFIG";
	case sqf::DISPLAY: return L"DISPLAY";
	case sqf::CONTROL: return L"CONTROL";
	case sqf::NetObject: return L"NetObject";
	case sqf::SUBGROUP: return L"SUBGROUP";
	case sqf::TEAM_MEMBER: return L"TEAM_MEMBER";
	case sqf::TASK: return L"TASK";
	case sqf::DIARY_RECORD: return L"DIARY_RECORD";
	case sqf::LOCATION: return L"LOCATION";
	default: return std::wstring();
	}
}