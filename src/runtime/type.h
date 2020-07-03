#pragma once
#include <string>
#include <string_view>
using namespace std::string_view_literals;
#include <unordered_map>
#include <string>
#include <algorithm>
#include <unordered_map>


class type;
class type
{
protected:
	unsigned short m_value;
	static inline unsigned short s_type_value = 0;
	static std::unordered_map<std::string, type>& typemap_nc()
	{
		static std::unordered_map<std::string, type> map = std::unordered_map<std::string, type>();
		return map;
	}
	static std::unordered_map<unsigned short, std::string>& namemap_nc()
	{
		static std::unordered_map<unsigned short, std::string> map = std::unordered_map<unsigned short, std::string>();
		return map;
	}
public:
	template<typename T>
	class extend;

	operator unsigned short() const { return m_value; }
	operator unsigned int() const { return (unsigned int)m_value; }
	operator unsigned long() const { return (unsigned long)m_value; }
	operator short() const { return (short)m_value; }
	operator int() const { return (int)m_value; }
	operator long() const { return (long)m_value; }
	explicit operator float() const { return (float)m_value; }
	explicit operator double() const { return (double)m_value; }

	bool operator==(const type& b) const { return m_value == b.m_value; }
	bool operator!=(const type& b) const { return m_value != b.m_value; }
	bool operator<(const type& b) const { return m_value < b.m_value; }
	bool operator>(const type& b) const { return m_value > b.m_value; }
	bool operator<=(const type& b) const { return m_value <= b.m_value; }
	bool operator>=(const type& b) const { return m_value >= b.m_value; }

	static const std::unordered_map<std::string, type>& typemap()
	{
		auto& map = typemap_nc();
		return map;
	}
	static const std::unordered_map<unsigned short, std::string>& namemap()
	{
		auto& map = namemap_nc();
		return map;
	}
};
template<typename T>
class type::extend : public type
{
private:
	static inline unsigned short s_local_type_value = 0;
public:
	extend() : type()
	{
		if (s_local_type_value == 0)
		{
			s_local_type_value = ++s_type_value;
			m_value = s_local_type_value;
			typemap_nc()[T::name()] = *this;
			namemap_nc()[*this] = T::name();
		}
		else
		{
			m_value = s_local_type_value;
		}
	}
};


struct t_nothing : public type::extend<t_nothing> { t_nothing() : extend() {} static const std::string name() { return "NOTHING"; } };
struct t_any : public type::extend<t_any> { t_any() : extend() {} static const std::string name() { return "ANY"; } };
struct t_scalar : public type::extend<t_scalar> { t_scalar() : extend() {} static const std::string name() { return "SCALAR"; } };
struct t_boolean : public type::extend<t_boolean> { t_boolean() : extend() {} static const std::string name() { return "BOOL"; } };
struct t_array : public type::extend<t_array> { t_array() : extend() {} static const std::string name() { return "ARRAY"; } };
struct t_string : public type::extend<t_string> { t_string() : extend() {} static const std::string name() { return "STRING"; } };
struct t_namespace : public type::extend<t_namespace> { t_namespace() : extend() {} static const std::string name() { return "NAMESPACE"; } };
struct t_nan : public type::extend<t_nan> { t_nan() : extend() {} static const std::string name() { return "NaN"; } };
struct t_if : public type::extend<t_if> { t_if() : extend() {} static const std::string name() { return "IF"; } };
struct t_while : public type::extend<t_while> { t_while() : extend() {} static const std::string name() { return "WHILE"; } };
struct t_for : public type::extend<t_for> { t_for() : extend() {} static const std::string name() { return "FOR"; } };
struct t_switch : public type::extend<t_switch> { t_switch() : extend() {} static const std::string name() { return "SWITCH"; } };
struct t_exception : public type::extend<t_exception> { t_exception() : extend() {} static const std::string name() { return "EXCEPTION"; } };
struct t_with : public type::extend<t_with> { t_with() : extend() {} static const std::string name() { return "WITH"; } };
struct t_code : public type::extend<t_code> { t_code() : extend() {} static const std::string name() { return "CODE"; } };
struct t_object : public type::extend<t_object> { t_object() : extend() {} static const std::string name() { return "OBJECT"; } };
struct t_vector : public type::extend<t_vector> { t_vector() : extend() {} static const std::string name() { return "VECTOR"; } };
struct t_trans : public type::extend<t_trans> { t_trans() : extend() {} static const std::string name() { return "TRANS"; } };
struct t_orient : public type::extend<t_orient> { t_orient() : extend() {} static const std::string name() { return "ORIENT"; } };
struct t_side : public type::extend<t_side> { t_side() : extend() {} static const std::string name() { return "SIDE"; } };
struct t_group : public type::extend<t_group> { t_group() : extend() {} static const std::string name() { return "GROUP"; } };
struct t_text : public type::extend<t_text> { t_text() : extend() {} static const std::string name() { return "TEXT"; } };
struct t_script : public type::extend<t_script> { t_script() : extend() {} static const std::string name() { return "SCRIPT"; } };
struct t_target : public type::extend<t_target> { t_target() : extend() {} static const std::string name() { return "TARGET"; } };
struct t_jclass : public type::extend<t_jclass> { t_jclass() : extend() {} static const std::string name() { return "JCLASS"; } };
struct t_config : public type::extend<t_config> { t_config() : extend() {} static const std::string name() { return "CONFIG"; } };
struct t_display : public type::extend<t_display> { t_display() : extend() {} static const std::string name() { return "DISPLAY"; } };
struct t_control : public type::extend<t_control> { t_control() : extend() {} static const std::string name() { return "CONTROL"; } };
struct t_netobject : public type::extend<t_netobject> { t_netobject() : extend() {} static const std::string name() { return "NetObject"; } };



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