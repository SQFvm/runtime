#pragma once
#include <iostream>
namespace console
{
    class color
    {
    private:
        constexpr color(const char* view) : data(view) {}
    public:
        const char* data;
        static constexpr color foreground_black() { return color("\u001b[30m"); }
        static constexpr color foreground_red() { return color("\u001b[31m"); }
        static constexpr color foreground_green() { return color("\u001b[32m"); }
        static constexpr color foreground_yellow() { return color("\u001b[33m"); }
        static constexpr color foreground_blue() { return color("\u001b[34m"); }
        static constexpr color foreground_magenta() { return color("\u001b[35m"); }
        static constexpr color foreground_cyan() { return color("\u001b[36m"); }
        static constexpr color foreground_white() { return color("\u001b[37m"); }
        static constexpr color reset() { return color("\u001b[0m"); }
        static constexpr color foreground_bright_black() { return color("\u001b[30;1m"); }
        static constexpr color foreground_bright_red() { return color("\u001b[31;1m"); }
        static constexpr color foreground_bright_green() { return color("\u001b[32;1m"); }
        static constexpr color foreground_bright_yellow() { return color("\u001b[33;1m"); }
        static constexpr color foreground_bright_blue() { return color("\u001b[34;1m"); }
        static constexpr color foreground_bright_magenta() { return color("\u001b[35;1m"); }
        static constexpr color foreground_bright_cyan() { return color("\u001b[36;1m"); }
        static constexpr color foreground_bright_white() { return color("\u001b[37;1m"); }
        static constexpr color background_black() { return color("\u001b[40m"); }
        static constexpr color background_red() { return color("\u001b[41m"); }
        static constexpr color background_green() { return color("\u001b[42m"); }
        static constexpr color background_yellow() { return color("\u001b[43m"); }
        static constexpr color background_blue() { return color("\u001b[44m"); }
        static constexpr color background_magenta() { return color("\u001b[45m"); }
        static constexpr color background_cyan() { return color("\u001b[46m"); }
        static constexpr color background_white() { return color("\u001b[47m"); }
        static constexpr color background_bright_black() { return color("\u001b[40;1m"); }
        static constexpr color background_bright_red() { return color("\u001b[41;1m"); }
        static constexpr color background_bright_green() { return color("\u001b[42;1m"); }
        static constexpr color background_bright_yellow() { return color("\u001b[43;1m"); }
        static constexpr color background_bright_blue() { return color("\u001b[44;1m"); }
        static constexpr color background_bright_magenta() { return color("\u001b[45;1m"); }
        static constexpr color background_bright_cyan() { return color("\u001b[46;1m"); }
        static constexpr color background_bright_white() { return color("\u001b[47;1m"); }
    };
}
inline std::ostream& operator<<(std::ostream& os, const console::color& c)
{
    os << c.data;
    return os;
}