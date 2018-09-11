#pragma once
#ifndef CONCAT_
#define CONCAT_(L, R) L ## R
#endif // !CONCAT_
#ifndef CONCAT
#define CONCAT(L, R) CONCAT_(L, R)
#endif // !CONCAT


#define VERSION_MAJOR 1
#define VERSION_MINOR 1
#define VERSION_REVISION 0
#define VERSION "INDEV"
#define VERSION_FULL VERSION " - " __DATE__ " " __TIME__
#define VERSION_MAJORMINOR CONCAT(VERSION_MAJOR, VERSION_MINOR)

int console_width(void);
int main(int argc, char** argv);