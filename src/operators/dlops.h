#pragma once
#include <string>
#include <sstream>


class dlops
{
private:
    void* handle;
    std::string mpath;
public:
    //Prevent copy
    dlops(const dlops&) = delete;
    void operator=(dlops const &x) = delete;


    dlops(std::string path);
    ~dlops()
    {
        close();
    }
    std::string path() { return mpath; }

    void* resolve(std::string name);
    bool try_resolve(std::string name, void** outptr);
    void close();
};
