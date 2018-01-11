#ifndef _SQFTYPE
#define _SQFTYPE 1

namespace sqf
{
	enum type
	{
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
}
#endif // !_SQFTYPE