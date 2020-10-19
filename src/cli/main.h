#pragma once
#include <string>

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


#define VERSION_MAJOR 2
#define VERSION_MINOR 0
#define VERSION_REVISION 0
#define VERSION STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_REVISION)
#define VERSION_FULL VERSION " - " __DATE__ " " __TIME__
#define VERSION_MAJORMINOR CONCAT(VERSION_MAJOR, VERSION_MINOR)



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


void strcpy_safe(char* const dest, size_t len, const char* const src);
char* const copy_str(const std::string& str);

int console_width();
int main_actual(int argc, char** argv);
int main(int argc, char** argv);
