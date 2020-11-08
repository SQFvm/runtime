#include "dlops.h"
#include <string>
#include <sstream>

#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#include <dlfcn.h>
#define DLOPS_LIB_OPEN(P) dlopen(P, RTLD_LAZY)
#define DLOPS_LIB_CLOSE(H) dlclose(H)
#define DLOPS_LIB_SYM(H, N) dlsym(H, N)
#define DLOPS_LIB_GETLASTERROR() dlerror()
#define DLOPS_LIB_HANDLE void*
#elif defined(_WIN32)
#include <Windows.h>
#define DLOPS_LIB_OPEN(P) LoadLibraryA(P)
#define DLOPS_LIB_CLOSE(H) FreeLibrary(H)
#define DLOPS_LIB_SYM(H, N) GetProcAddress(H, N)
#define DLOPS_LIB_GETLASTERROR() std::to_string(GetLastError())
#define DLOPS_LIB_HANDLE HMODULE
#else
#define DLOPS_LIB_OPEN(P)
#define DLOPS_LIB_CLOSE(H)
#define DLOPS_LIB_SYM(H, N)
#define DLOPS_LIB_GETLASTERROR()
#define DLOPS_LIB_HANDLE
#error UNSUPPORTED PLATFORM
#endif

dlops::dlops(std::string path)
{
    handle = DLOPS_LIB_OPEN(path.c_str());
    if (nullptr == handle)
    {
        std::stringstream sstream;
        sstream << "Loading Library failed with error code '" << DLOPS_LIB_GETLASTERROR() << "'";
        throw std::runtime_error(sstream.str());
    }
    mpath = path;
}

void* dlops::resolve(std::string name)
{
    if (nullptr == handle)
        return nullptr;
    auto res = DLOPS_LIB_SYM(static_cast<DLOPS_LIB_HANDLE>(handle), name.c_str());
    if (nullptr == res)
    {
        std::stringstream sstream;
        sstream << "Receiving symbol failed with error '" << DLOPS_LIB_GETLASTERROR() << "'";
        throw std::runtime_error(sstream.str());
    }
    else
    {
        return res;
    }
};

bool dlops::try_resolve(std::string name, void** outptr)
{
    if (nullptr == handle)
        return false;
    *outptr = DLOPS_LIB_SYM(static_cast<DLOPS_LIB_HANDLE>(handle), name.c_str());
    return *outptr;
}

void dlops::close()
{
    if (nullptr == handle)
        return;
#if defined(__linux__)
    if (DLOPS_LIB_CLOSE(handle))
#elif defined(_WIN32)
    if (!DLOPS_LIB_CLOSE(static_cast<DLOPS_LIB_HANDLE>(handle)))
#else
    if (DLOPS_LIB_CLOSE(handle))
#endif
    {
        std::stringstream sstream;
        sstream << "Closing library failed with error code '" << DLOPS_LIB_GETLASTERROR() << "'";
        throw std::runtime_error(sstream.str());
    }
}



#undef DLOPS_LIB_OPEN
#undef DLOPS_LIB_CLOSE
#undef DLOPS_LIB_SYM
#undef DLOPS_LIB_GETLASTERROR
#undef DLOPS_LIB_HANDLE
