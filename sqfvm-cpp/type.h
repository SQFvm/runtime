#pragma once
#include <string>



namespace sqf
{
	enum type
	{
		NA = -1,
		NOTHING,
		ANY,
		SCALAR,
		BOOL,
		ARRAY,
		STRING,
		NAMESPACE,
		NaN,
		IF,
		WHILE,
		FOR,
		SWITCH,
		EXCEPTION,
		WITH,
		CODE,
		OBJECT,
		VECTOR,
		TRANS,
		ORIENT,
		SIDE,
		GROUP,
		TEXT,
		SCRIPT,
		TARGET,
		JCLASS,
		CONFIG,
		DISPLAY,
		CONTROL,
		NetObject,
		SUBGROUP,
		TEAM_MEMBER,
		TASK,
		DIARY_RECORD,
		LOCATION
	};
	std::string type_str(type t);
}