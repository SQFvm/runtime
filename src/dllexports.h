#pragma once

#include <memory>
#include <sstream>
#include "logging.h"
namespace sqf
{
	class virtualmachine;
}
class exportstarget : public Logger
{
	std::stringstream m_sstream;
	// Inherited via Logger
	virtual void log(loglevel level, std::string_view message) override;
public:
	exportstarget() : Logger() {}
	std::string str() { return m_sstream.str(); }
	void clear() { m_sstream.str(""); }
};
#ifdef _WIN32
#define DLLEXPORT_PREFIX __declspec(dllexport)
#else
#define DLLEXPORT_PREFIX __attribute__((visibility("default")))
#endif

static std::shared_ptr<sqf::virtualmachine> sqfvm_virtualmachine;
static exportstarget sqfvm_exportstarget;
extern "C" {
	DLLEXPORT_PREFIX void sqfvm_init(unsigned long long);
	DLLEXPORT_PREFIX void sqfvm_exec(const char*, char*, unsigned int);
	DLLEXPORT_PREFIX void sqfvm_loadconfig(const char* cfg);
	DLLEXPORT_PREFIX void sqfvm_uninit();
}