#pragma once

#include <string>
#include <string_view>

#ifndef CONCAT_
#define CONCAT_(L, R) L ## R
#endif // !CONCAT_
#ifndef CONCAT
#define CONCAT(L, R) CONCAT_(L, R)
#endif // !CONCAT
#ifndef STR_
#define STR_(IN) # IN
#endif // !STR_
#ifndef STR
#define STR(IN) STR_(IN)
#endif // !STR

// Check windows
#if defined(_WIN32) || defined(_WIN64)
#if defined(_WIN64)
#define ENVIRONMENT x64
#define ENVIRONMENT64 ENVIRONMENT
#else
#define ENVIRONMENT x86
#define ENVIRONMENT32 ENVIRONMENT
#endif
#endif

// Check GCC
#if defined(__GNUC__)
#if defined(__x86_64__) || defined(__ppc64__)
#define ENVIRONMENT x64
#define ENVIRONMENT64 ENVIRONMENT
#else
#define ENVIRONMENT x86
#define ENVIRONMENT32 ENVIRONMENT
#endif
#endif

#if !defined(ENVIRONMENT)
#define ENVIRONMENT NA
#endif

#define ENVIRONMENTSTR STR(ENVIRONMENT)




namespace sqf::runtime::util
{
    inline std::string_view ltrim(std::string_view str, std::string_view chars = " \t")
    {
        size_t startpos = str.find_first_not_of(chars);
        return std::string_view::npos == startpos ? str : str.substr(startpos);
    }

    inline std::string_view rtrim(std::string_view str, std::string_view chars = " \t")
    {
        size_t endpos = str.find_last_not_of(chars);
        return str.substr(0, endpos + 1);
    }

    inline std::string_view trim(std::string_view str, std::string_view chars = " \t")
    {
        return ltrim(rtrim(str, chars), chars);
    }

    inline std::string_view strip_quotes(std::string_view str, std::string_view quotes = "\"'")
    {
        if (str.length() < 2)
        {
            return {};
        }
        if (quotes.find(str.front()) == std::string_view::npos)
        {
            return {};
        }
        char quote_type = str.front();
        if (str.back() != quote_type)
        {
            return {};
        }
        return str.substr(1, str.length() - 2);
    }

    static size_t constexpr strlen(const char* str)
    {
        return str[0] ? 1 + strlen(str + 1) : 0;
    }
    static size_t constexpr wcslen(const wchar_t* str)
    {
        return str[0] ? 1 + wcslen(str + 1) : 0;
    }
    static constexpr double pi()
    {
        return 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665;
    }
}

namespace sqf::parser::util
{
    class string_ref
    {
    private:
        std::string& m_ref;
        const char* m_cstr;
    public:
        static constexpr const size_t npos = std::string::npos;
        string_ref(std::string& ref) : m_ref(ref), m_cstr(ref.c_str()) {}
        std::string substr(size_t index = 0, size_t len = npos) { if (index < m_ref.length()) { return m_ref.substr(index, len); } else { return {}; } }
        char operator[](size_t index) const { return m_cstr[index]; }
        size_t size() const { return m_ref.size(); }

        const char* begin() { return m_cstr; }
        const char* end() { return  m_cstr + m_ref.size(); }

        operator const char*() const { return m_cstr; }
        operator std::string() const { return m_ref; }
        operator std::string_view() const { return m_ref; }
    };
    #pragma region is_match
    template<typename = void>
    inline bool is_match(char value) { return false; }
    template<char TArg, char ... TArgs>
    bool is_match(char value)
    {
        switch (value)
        {
        case TArg: return true;
        default: return is_match<TArgs...>(value);
        }
    }
    template<size_t len, char ... TArgs>
    bool is_match_repeated(const char* value)
    {
        size_t i = 0;
        while (is_match<TArgs...>(*value++)) { ++i; }
        return len <= i;
    }

    template<char ... TArgs>
    size_t len_match(const char* str)
    {
        const char* it = str;
        while (is_match<TArgs...>(*it++)) {}
        return it - str - 1;
    }
    #pragma endregion
    #pragma region is_match_inv
    template<typename = void>
    inline bool is_match_inv(char value) { return true; }
    template<char TArg, char ... TArgs>
    bool is_match_inv(char value)
    {
        switch (value)
        {
        case TArg: return false;
        default: return is_match_inv<TArgs...>(value);
        }
    }
    template<char ... TArgs>
    size_t len_match_inv(const char* str)
    {
        const char* it = str;
        while (is_match_inv<TArgs...>(*it++)) {}
        return it - str - 1;
    }
    template<size_t len, char ... TArgs>
    bool is_match_inv_repeated(const char* value)
    {
        size_t i = 0;
        while (is_match_inv<TArgs...>(*value++)) { ++i; }
        return len == i;
    }
    #pragma endregion
    #pragma region is_string_match
    inline bool is_string_match(const char* start, const char* against)
    {
        auto len = ::sqf::runtime::util::strlen(against);
        for (size_t i = 0; i < len; i++)
        {
            if (start[i] != against[i]) { return false; }
        }
        return true;
    }
    #pragma endregion

}