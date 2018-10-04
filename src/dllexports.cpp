#include "dllexports.h"
#include "virtualmachine.h"
#include "commandmap.h"
#include "value.h"
#include "vmstack.h"
#include "configdata.h"
#include <iostream>
#include <sstream>
#include <cstring>

extern "C" {
	DLLEXPORT_PREFIX void sqfvm_init(unsigned long long limit)
	{
		sqfvm_virtualmachine = std::make_shared<sqf::virtualmachine>(limit);
		sqfvm_virtualmachine->allowsleep(false);
		sqf::commandmap::get().init();
	}
	DLLEXPORT_PREFIX void sqfvm_exec(const char* code, char* buffer, unsigned int bufferlen)
	{
		std::stringstream sstream;
		sqfvm_virtualmachine->out(&sstream);
		sqfvm_virtualmachine->err(&sstream);
		sqfvm_virtualmachine->wrn(&sstream);
		sqfvm_virtualmachine->parse_sqf(code);
		sqfvm_virtualmachine->execute();
		std::shared_ptr<sqf::value> val;
		bool success;
		do {
			val = sqfvm_virtualmachine->stack()->popval(success);
			if (success)
			{
				sstream << "[WORK]\t<" << sqf::type_str(val->dtype()) << ">\t" << val->as_string() << std::endl;
			}
		} while (success);
		auto str = sstream.str();

		memset(buffer, 0, sizeof(char) * bufferlen);
		std::strncpy(buffer, str.c_str(), bufferlen);
	}

	DLLEXPORT_PREFIX void sqfvm_loadconfig(const char* cfg)
	{
		sqfvm_virtualmachine->parse_config(cfg, sqf::configdata::configFile()->data<sqf::configdata>());
	}

	DLLEXPORT_PREFIX void sqfvm_uninit()
	{
		sqf::commandmap::get().uninit();
		sqfvm_virtualmachine = std::shared_ptr<sqf::virtualmachine>();
	}
}