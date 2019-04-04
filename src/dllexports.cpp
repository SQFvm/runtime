#include "dllexports.h"
#include "virtualmachine.h"
#include "commandmap.h"
#include "value.h"
#include "vmstack.h"
#include "configdata.h"
#include "parsepreprocessor.h"
#include "Entry.h"
#include <iostream>
#include <sstream>
#include <cstring>

extern "C" {
	DLLEXPORT_PREFIX void sqfvm_init(unsigned long long limit)
	{
		sqfvm_virtualmachine = std::make_shared<sqf::virtualmachine>(limit);
		sqfvm_virtualmachine->allowsleep(false);
#if !defined(FILESYSTEM_DISABLE_DISALLOW)
		sqfvm_virtualmachine->get_filesystem().disallow(true);
#endif
		sqf::commandmap::get().init();
	}
	DLLEXPORT_PREFIX void sqfvm_exec(const char* code, char* buffer, unsigned int bufferlen)
	{
		std::stringstream sstream;
		sqfvm_virtualmachine->out(&sstream);
		sqfvm_virtualmachine->err(&sstream);
		sqfvm_virtualmachine->wrn(&sstream);

		bool err;
		auto executable_path = get_working_dir();
		auto inputAfterPP = sqf::parse::preprocessor::parse(sqfvm_virtualmachine.get(), code, err, "__libraryfeed.sqf");
		if (!err)
		{
			sqfvm_virtualmachine->parse_sqf(inputAfterPP, "__libraryfeed.sqf");
			sqfvm_virtualmachine->execute();
			auto val = sqfvm_virtualmachine->stack()->last_value();
			sqfvm_virtualmachine->err_buffprint(true);
			sqfvm_virtualmachine->wrn_buffprint(true);
			sqfvm_virtualmachine->out_buffprint(true);
			if (val != nullptr)
			{
				sstream << "[WORK]\t<" << sqf::type_str(val->dtype()) << ">\t" << val->as_string() << std::endl;
			}
			else
			{
				sstream << "[WORK]\t<" << "EMPTY" << ">\t" << std::endl;
			}
		}
		else
		{
			sqfvm_virtualmachine->err_buffprint(true);
			sqfvm_virtualmachine->wrn_buffprint(true);
			sqfvm_virtualmachine->out_buffprint(true);
		}
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