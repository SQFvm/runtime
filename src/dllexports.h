#pragma once

#include <memory>
namespace sqf
{
	class virtualmachine;
}
#ifdef _WIN32
#define DLLEXPORT_PREFIX __declspec(dllexport)
#else
#define DLLEXPORT_PREFIX __attribute__((visibility("default")))
#endif

static std::shared_ptr<sqf::virtualmachine> sqfvm_virtualmachine;
extern "C" {
	DLLEXPORT_PREFIX void sqfvm_init(unsigned long long);
	DLLEXPORT_PREFIX void sqfvm_exec(const char*, char*, unsigned int);
	DLLEXPORT_PREFIX void sqfvm_loadconfig(const char* cfg);
	DLLEXPORT_PREFIX void sqfvm_uninit();
}