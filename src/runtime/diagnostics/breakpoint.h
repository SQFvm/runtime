#pragma once
#include <string>
#include <string_view>

namespace sqf::runtime::diagnostics
{
    class breakpoint
    {
        std::size_t m_line;
        std::string m_file;
        std::string m_condition;
        bool m_enabled;
    public:
        breakpoint(std::size_t line, std::string_view file) : m_line(line), m_file(file), m_condition(), m_enabled(true) {}
        //breakpoint(std::size_t line, std::string_view file, std::string_view condition) : m_line(line), m_file(file), m_condition(condition), m_enabled(true) {}

        void disable() { m_enabled = false; }
        void enable() { m_enabled = true; }
        bool is_enabled() const { return m_enabled; }

        std::size_t line() const { return m_line; }
        void line(std::size_t value) { m_line = value; }

        std::string_view file() const { return m_file; }
        void file(std::string value) { m_file = value; }

        // std::string condition() const { return m_condition; }
        // void condition(std::string value) { m_condition = value; }
    };
}