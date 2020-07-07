#pragma once
#include <string>
#include <string_view>
using namespace std::string_view_literals;
#include <unordered_map>
#include <string>
#include <algorithm>
#include <unordered_map>



namespace sqf::runtime
{
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
		class conversion;

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

		std::string_view to_string() const
		{
			auto& map = namemap();
			return { map.at(m_value) };
		}

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
}