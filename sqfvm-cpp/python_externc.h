#pragma once

#include <memory>

extern "C" {
	static std::shared_ptr<sqf::virtualmachine> py_virtualmachine;
	void py_init(unsigned long long);
	void py_uninit(void);
	void py_exec(wchar_t*, wchar_t*, unsigned int);
}