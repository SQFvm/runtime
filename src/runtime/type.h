#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>



namespace sqf
{

    namespace runtime
    {
        /*
        template<typename family>
        struct type_family
        {
            template<typename type>
            uint32_t get_identifier() -> member<type>;
    
            private:
            static inline uint32_t counter = 0u;
            template<typename type>
            static inline uint32_t member= counter++;
        }
        */
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

            template<class T>
            bool is() const
            {
                static_assert(std::is_base_of<type, T>::value,
                    "sqf::runtime::type::is<T>() expects T to be a derivative of sqf::runtime::type.");
                return m_value == (short)(T());
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
    }
    namespace types
    {

        struct t_nothing : public sqf::runtime::type::extend<t_nothing> { t_nothing() : extend() {} static const std::string name() { return "NOTHING"; } };
        struct t_any : public sqf::runtime::type::extend<t_any> { t_any() : extend() {} static const std::string name() { return "ANY"; } };
        struct t_nan : public sqf::runtime::type::extend<t_nan> { t_nan() : extend() {} static const std::string name() { return "NaN"; } };
        struct t_vector : public sqf::runtime::type::extend<t_vector> { t_vector() : extend() {} static const std::string name() { return "VECTOR"; } };
        struct t_trans : public sqf::runtime::type::extend<t_trans> { t_trans() : extend() {} static const std::string name() { return "TRANS"; } };
        struct t_orient : public sqf::runtime::type::extend<t_orient> { t_orient() : extend() {} static const std::string name() { return "ORIENT"; } };
        struct t_task : public sqf::runtime::type::extend<t_task> { t_task() : extend() {} static const std::string name() { return "TASK"; } };
        struct t_target : public sqf::runtime::type::extend<t_target> { t_target() : extend() {} static const std::string name() { return "TARGET"; } };
        struct t_jclass : public sqf::runtime::type::extend<t_jclass> { t_jclass() : extend() {} static const std::string name() { return "JCLASS"; } };
        struct t_location : public sqf::runtime::type::extend<t_location> { t_location() : extend() {} static const std::string name() { return "LOCATION"; } };
        struct t_display : public sqf::runtime::type::extend<t_display> { t_display() : extend() {} static const std::string name() { return "DISPLAY"; } };
        struct t_control : public sqf::runtime::type::extend<t_control> { t_control() : extend() {} static const std::string name() { return "CONTROL"; } };
        struct t_netobject : public sqf::runtime::type::extend<t_netobject> { t_netobject() : extend() {} static const std::string name() { return "NetObject"; } };
    }
}

template<>
struct std::hash<sqf::runtime::type>
{
    std::size_t operator()(sqf::runtime::type const& t) const noexcept
    {
        std::size_t h1 = std::hash<short>{}(t);
        return h1;
    }
};