#ifndef _SQFTYPE
#define _SQFTYPE 1

#if !defined(_STRING) & !defined(_STRING_)
#error type requires <string> header
#endif // !_STRING



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
	std::wstring type_str(type t);
}
#endif // !_SQFTYPE