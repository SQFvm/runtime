#pragma once

#include <memory>
namespace sqf
{
	class virtualmachine;
}
static std::shared_ptr<sqf::virtualmachine> py_virtualmachine;
extern "C" {
	void py_init(unsigned long long);
	void py_uninit(void);
	void py_exec(wchar_t*, wchar_t*, unsigned int);
}