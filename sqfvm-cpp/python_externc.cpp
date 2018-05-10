#include "python_externc.h"
#include "virtualmachine.h"
#include "commandmap.h"
#include "value.h"
#include "vmstack.h"
#include "configdata.h"
#include <iostream>
#include <sstream>

extern "C" {
	DLLEXPORT_PREFIX void py_init(unsigned long long limit)
	{
		py_virtualmachine = std::make_shared<sqf::virtualmachine>(limit);
		sqf::commandmap::get().init();
	}
	DLLEXPORT_PREFIX void py_exec(char* code, char* buffer, unsigned int bufferlen)
	{
		auto sstream = std::stringstream();
		py_virtualmachine->out(&sstream);
		py_virtualmachine->err(&sstream);
		py_virtualmachine->wrn(&sstream);
		py_virtualmachine->parse_sqf(code);
		py_virtualmachine->execute();
		std::shared_ptr<sqf::value> val;
		bool success;
		do {
			val = py_virtualmachine->stack()->popval(success);
			if (success)
			{
				py_virtualmachine->out() << "[WORK]\t<" << sqf::type_str(val->dtype()) << ">\t" << val->as_string() << std::endl;
			}
		} while (success);
		auto str = sstream.str();

		std::strncpy(buffer, str.c_str(), bufferlen);
	}

	DLLEXPORT_PREFIX void py_loadconfig(char* cfg)
	{
		py_virtualmachine->parse_config(cfg, sqf::configdata::configFile()->data<sqf::configdata>());
	}

	DLLEXPORT_PREFIX void py_uninit(void)
	{
		sqf::commandmap::get().uninit();
		py_virtualmachine = std::shared_ptr<sqf::virtualmachine>();
	}
}