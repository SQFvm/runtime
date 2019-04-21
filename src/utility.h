#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <algorithm>

struct string_view_hash
{
    std::size_t operator()(std::string_view v)const
    {
        return std::hash<std::string_view>()(v);
    }
    std::size_t operator()(const std::string& s)const
    {
        return std::hash<std::string_view>()(s);
    }
};


//fnv1a hash
#if defined(_WIN64)
static_assert(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
const size_t fnv_offset_basis = 14695981039346656037ULL;
const size_t fnv_prime = 1099511628211ULL;
#else /* defined(_WIN64) */
static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
const size_t fnv_offset_basis = 2166136261U;
const size_t fnv_prime = 16777619U;
#endif /* defined(_WIN64) */

struct string_view_hash_ci
{
    std::size_t operator()(std::string_view v)const
    {
        size_t val = fnv_offset_basis;
        for (char x : v) {
            val ^= static_cast<size_t>(::tolower(x));
            val *= fnv_prime;
        }
        return val;
    }
    std::size_t operator()(std::string s)const
    {
        size_t val = fnv_offset_basis;
        for (char x : s) {
            val ^= static_cast<size_t>(::tolower(x));
            val *= fnv_prime;
        }
        return val;
    }
};

struct string_view_equal_to
{
    std::size_t operator()(const std::string& s1, const std::string& s2)const
    {
        return s1 == s2;
    }
    std::size_t operator()(const std::string& s1, const std::string_view& v2)const
    {
        return s1 == v2;
    }
    std::size_t operator()(const std::string_view & v1, const std::string & s2)const
    {
        return v1 == s2;
    }
};

struct string_view_equal_to_ci
{
    std::size_t operator()(const std::string& s1, const std::string& s2)const
    {
        return s1.length() == s2.length() &&
            std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), [](char l, char r) {
                    return ::tolower(l) == ::tolower(r);
        });
    }
    std::size_t operator()(const std::string& s1, const std::string_view& v2)const
    {
        return s1.length() == v2.length() &&
            std::equal(s1.begin(), s1.end(), v2.begin(), v2.end(), [](char l, char r) {
            return ::tolower(l) == ::tolower(r);
        });
    }
    std::size_t operator()(const std::string_view& v1, const std::string& s2)const
    {
        return v1.length() == s2.length() &&
            std::equal(v1.begin(), v1.end(), s2.begin(), s2.end(), [](char l, char r) {
            return ::tolower(l) == ::tolower(r);
        });
    }
};

template<typename Q>
using unordered_string_map=std::unordered_map<
    std::string, Q,
    string_view_hash,
    string_view_equal_to>;

template<typename Q>
using unordered_string_map_ci=std::unordered_map<
    std::string, Q,
    string_view_hash_ci,
    string_view_equal_to_ci>;



static std::string string_tolower(std::string_view s)
{
    std::string ret(s);
    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;
}

static std::string& string_tolower_ref(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}