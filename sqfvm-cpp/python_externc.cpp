#include "python_externc.h"
#include "virtualmachine.h"
#include "commandmap.h"
#include "value.h"
#include "vmstack.h"
#include <iostream>
#include <sstream>

extern "C" {
	sqf::virtualmachine py_virtualmachine;
	void py_init(unsigned long long limit)
	{
		py_virtualmachine = std::make_shared<sqf::virtualmachine>(limit);
		sqf::commandmap::get().init();
	}
	void py_exec(wchar_t* code, wchar_t* buffer, unsigned int bufferlen)
	{
		auto sstream = std::wstringstream();
		py_virtualmachine->out(sstream);
		py_virtualmachine->err(sstream);
		py_virtualmachine->wrn(sstream);
		py_virtualmachine->parse_sqf(code);
		py_virtualmachine->execute();
		std::shared_ptr<sqf::value> val;
		bool success;
		do {
			val = py_virtualmachine->stack()->popval(success);
			if (success)
			{
				py_virtualmachine->out() << L"[WORK]\t<" << sqf::type_str(val->dtype()) << L">\t" << val->as_string() << std::endl;
			}
		} while (success);
		auto str = sstream.str();

		std::wcsncpy(buffer, str.c_str(), bufferlen);
	}

	void py_uninit()
	{
		sqf::commandmap::get().uninit();
		py_virtualmachine = std::shared_ptr<sqf::virtualmachine>();
	}
}