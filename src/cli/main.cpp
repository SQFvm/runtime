#include "main.h"
#include "cli.hpp"
#include "../runtime/git_sha1.h"

#if defined(SQF_SQC_SUPPORT)
#include "../sqc/sqc_parser.h"
#endif

#include "interactive_helper.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <tclap/CmdLine.h>
#include <algorithm>
#include <string_view>

#include <csignal>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <cstring>
#else
#include <limits.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <execinfo.h>
#endif

//#define MEM_LEAK_TEST


#if defined(_WIN32) && defined(_DEBUG) && defined(MEM_LEAK_TEST)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define CRTDBG(CONTENT) CONTENT
#else
#define CRTDBG(CONTENT)
#endif

#ifdef _WIN32
#define RELPATHHINT "Supports absolute and relative pathing using '.\\path\\to\\file' or 'C:\\path\\to\\file'."
#else
#define RELPATHHINT "Supports absolute and relative pathing using './path/to/file' or '/path/to/file'."
#endif

void strcpy_safe(char* const dest, size_t len, const char* const src)
{
#ifdef WIN32
    strcpy_s(dest, len, src);
#else
    std::strcpy(dest, src);
#endif
}

char* const copy_str(const std::string& str)
{
    auto dest = new char[str.length() + 1];
    strcpy_safe(dest, str.length() + 1, str.c_str());
    return dest;
}

#ifdef WIN32
// ToDo: Implement StackTrace on error
#else
void handle_SIGSEGV(int val)
{
    void *arr[20];
    size_t size;
    size = backtrace(arr, 20);
    fprintf(stderr, "Error: signal %d:\n", val);
    backtrace_symbols_fd(arr, size, STDERR_FILENO);
    exit(-1);
}
#endif



int console_width()
{
#if _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        return 20;
    }
    columns = csbi.srWindow.Right - csbi.srWindow.Left;
    return columns;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
#endif
}

int main(int argc, char** argv)
{
    CRTDBG(_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF));
    CRTDBG(_CrtSetBreakAlloc(2294));
    auto res = main_actual(argc, argv);
    return res;
}
int main_actual(int argc, char** argv)
{
#ifdef WIN32
    // ToDo: Implement StackTrace on error
#else
    struct sigaction action_SIGSEGV;

    memset(&action_SIGSEGV, 0, sizeof(struct sigaction));
    action_SIGSEGV.sa_handler = handle_SIGSEGV;
    sigaction(SIGSEGV, &action_SIGSEGV, NULL);
#endif

#ifdef DF__CLI_PRINT_INPUT_ARGS
    std::cout << "\x1B[95m[CLI-INARG-PRINT]\033[0m" << "Got arguments:" << std::endl;
    for (int i = 0; i < argc; i++)
    {
        std::cout << "\x1B[95m[CLI-INARG-PRINT]\033[0m" << "    " << std::setw(3) << i << ": `" << argv[i] << "`" << std::endl;
    }
#endif // DF__CLI_PRINT_INPUT_ARGS

    cli command_line_interface;
    return command_line_interface.run(argc, const_cast<const char**>(argv));
}
