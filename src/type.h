#pragma once
#include <string>
#include <string_view>
using namespace std::string_view_literals;
#include <unordered_map>
#include <string>
#include <algorithm>



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
	inline sqf::type parsetype(std::string_view str)
	{
		constexpr auto toupper = [](std::string_view val) -> std::string
		{
			std::string ret(val);
			std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
			return ret;
		};


		static std::unordered_map<std::string, sqf::type> typeMap{ //#TODO make a string to type func in type.h
			{toupper("NOTHING"sv), sqf::type::NOTHING},
			{toupper("ANY"sv), sqf::type::ANY},
			{toupper("SCALAR"sv), sqf::type::SCALAR},
			{toupper("BOOL"sv), sqf::type::BOOL},
			{toupper("ARRAY"sv), sqf::type::ARRAY},
			{toupper("STRING"sv), sqf::type::STRING},
			{toupper("NAMESPACE"sv), sqf::type::NAMESPACE},
			{toupper("NaN"sv), sqf::type::NaN},
			{toupper("IF"sv), sqf::type::IF},
			{toupper("WHILE"sv), sqf::type::WHILE},
			{toupper("FOR"sv), sqf::type::FOR},
			{toupper("SWITCH"sv), sqf::type::SWITCH},
			{toupper("EXCEPTION"sv), sqf::type::EXCEPTION},
			{toupper("WITH"sv), sqf::type::WITH},
			{toupper("CODE"sv), sqf::type::CODE},
			{toupper("OBJECT"sv), sqf::type::OBJECT},
			{toupper("VECTOR"sv), sqf::type::VECTOR},
			{toupper("TRANS"sv), sqf::type::TRANS}, //#TODO remove, invalid type
			{toupper("ORIENT"sv), sqf::type::ORIENT}, //#TODO remove, invalid type
			{toupper("SIDE"sv), sqf::type::SIDE},
			{toupper("GROUP"sv), sqf::type::GROUP},
			{toupper("TEXT"sv), sqf::type::TEXT},
			{toupper("SCRIPT"sv), sqf::type::SCRIPT},
			{toupper("TARGET"sv), sqf::type::TARGET},
			{toupper("JCLASS"sv), sqf::type::JCLASS},
			{toupper("CONFIG"sv), sqf::type::CONFIG},
			{toupper("DISPLAY"sv), sqf::type::DISPLAY},
			{toupper("CONTROL"sv), sqf::type::CONTROL},
			{toupper("NetObject"sv), sqf::type::NetObject},
			{toupper("SUBGROUP"sv), sqf::type::SUBGROUP},
			{toupper("TEAM_MEMBER"sv), sqf::type::TEAM_MEMBER},
			{toupper("TASK"sv), sqf::type::TASK},
			{toupper("DIARY_RECORD"sv), sqf::type::DIARY_RECORD},
			{toupper("LOCATION"sv), sqf::type::LOCATION}
		};

		auto found = typeMap.find(toupper(str));
		if (found == typeMap.end())
			return sqf::type::NA;
		return found->second;
	}
}