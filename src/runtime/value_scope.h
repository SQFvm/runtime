#pragma once
#include "value.h"

#include <unordered_map>
#include <string>
#include <string_view>
#include <optional>
#include <cctype>
#include <algorithm>

#ifdef DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG
#include <iostream>
#endif // DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG

namespace sqf::runtime
{
    class value_scope
    {
    private:
        std::string m_scope_name;
        std::unordered_map<std::string, sqf::runtime::value> m_map;
    public:
        value_scope() = default;

        sqf::runtime::value operator[](const std::string& index) const { return at(index); }
        sqf::runtime::value& operator[](const std::string& index) { return at(index); }

        bool contains(std::string variable_name) const
        {
            std::transform(variable_name.begin(), variable_name.end(), variable_name.begin(), [](char& c) { return (char)std::tolower((int)c); });
            auto res = m_map.find(variable_name) != m_map.end();
#ifdef DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG
            std::cout << "\x1B[33m[VALUE-SCOPE-DBG]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "    " << "contains(\"" << variable_name << "\") const := " << res << std::endl;
#endif // DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG
            return res;
        }
        sqf::runtime::value at(std::string variable_name) const
        {
            std::transform(variable_name.begin(), variable_name.end(), variable_name.begin(), [](char& c) { return (char)std::tolower((int)c); });
            auto res = m_map.find(variable_name);
#ifdef DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG
            std::cout << "\x1B[33m[VALUE-SCOPE-DBG]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "    " << "at(\"" << variable_name << "\") const := { " << (res == m_map.end() ? "" : res->second.to_string_sqf()) << " }" << std::endl;
#endif // DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG
            return res == m_map.end() ? value() : res->second;
        }
        sqf::runtime::value& at(std::string variable_name)
        {
            std::transform(variable_name.begin(), variable_name.end(), variable_name.begin(), [](char& c) { return (char)std::tolower((int)c); });
#ifdef DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG
            auto res = m_map.find(variable_name);
            std::cout << "\x1B[33m[VALUE-SCOPE-DBG]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "    " << "at(\"" << variable_name << "\") := { " << (res == m_map.end() ? "" : res->second.to_string_sqf()) << " }" << std::endl;
#endif // DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG
            return m_map[variable_name];
        }
        std::string_view scope_name() const { return m_scope_name; }
        void scope_name(std::string value) { m_scope_name = value; }
        void clear_value_scope() { m_map.clear(); }
        std::optional<sqf::runtime::value> try_get(std::string variable_name) { if (contains(variable_name)) { return at(variable_name); } return {}; }

        std::unordered_map<std::string, sqf::runtime::value>::iterator begin() { return m_map.begin(); }
        std::unordered_map<std::string, sqf::runtime::value>::iterator end() { return m_map.end(); }
    };
}