#include "type.h"

std::string sqf::type_str(type t)
{
	switch (t)
	{
	case sqf::NA: return "NA";
	case sqf::NOTHING: return "NOTHING";
	case sqf::ANY: return "ANY";
	case sqf::SCALAR: return "SCALAR";
	case sqf::BOOL: return "BOOL";
	case sqf::ARRAY: return "ARRAY";
	case sqf::STRING: return "STRING";
	case sqf::NAMESPACE: return "NAMESPACE";
	case sqf::NaN: return "NaN";
	case sqf::IF: return "IF";
	case sqf::WHILE: return "WHILE";
	case sqf::FOR: return "FOR";
	case sqf::SWITCH: return "SWITCH";
	case sqf::EXCEPTION: return "EXCEPTION";
	case sqf::WITH: return "WITH";
	case sqf::CODE: return "CODE";
	case sqf::OBJECT: return "OBJECT";
	case sqf::VECTOR: return "VECTOR";
	case sqf::TRANS: return "TRANS";
	case sqf::ORIENT: return "ORIENT";
	case sqf::SIDE: return "SIDE";
	case sqf::GROUP: return "GROUP";
	case sqf::TEXT: return "TEXT";
	case sqf::SCRIPT: return "SCRIPT";
	case sqf::TARGET: return "TARGET";
	case sqf::JCLASS: return "JCLASS";
	case sqf::CONFIG: return "CONFIG";
	case sqf::DISPLAY: return "DISPLAY";
	case sqf::CONTROL: return "CONTROL";
	case sqf::NetObject: return "NetObject";
	case sqf::SUBGROUP: return "SUBGROUP";
	case sqf::TEAM_MEMBER: return "TEAM_MEMBER";
	case sqf::TASK: return "TASK";
	case sqf::DIARY_RECORD: return "DIARY_RECORD";
	case sqf::LOCATION: return "LOCATION";
	default: return std::string();
	}
}