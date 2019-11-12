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

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <cstring>
#else
#include <limits.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <execinfo.h>
#endif

extern "C" {

	std::string get_working_dir()
	{
#if defined(_WIN32) || defined(_WIN64)
		char buffer[MAX_PATH];
		_getcwd(buffer, MAX_PATH);
		return std::string(buffer);
#elif defined(__GNUC__)
		char buffer[PATH_MAX];
		getcwd(buffer, PATH_MAX);
		return std::string(buffer);
#else
#error "NO IMPLEMENTATION AVAILABLE"
#endif
	}
	DLLEXPORT_PREFIX void sqfvm_init(unsigned long long limit)
	{
		sqfvm_virtualmachine = std::make_shared<sqf::virtualmachine>(limit);
		sqfvm_virtualmachine->allow_suspension(false);
#if !defined(FILESYSTEM_DISABLE_DISALLOW)
		sqfvm_virtualmachine->get_filesystem().disallow(true);
#endif
		sqfvm_virtualmachine->disable_networking();
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
			auto val = sqfvm_virtualmachine->active_vmstack()->last_value();
			sqfvm_virtualmachine->err_buffprint(true);
			sqfvm_virtualmachine->wrn_buffprint(true);
			sqfvm_virtualmachine->out_buffprint(true);
			if (val.data() != nullptr)
			{
				sstream << "[WORK]\t<" << sqf::type_str(val.dtype()) << ">\t" << val.as_string() << std::endl;
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
#ifdef _WIN32
		strncpy_s(buffer, str.length() + 1, str.c_str(), bufferlen);
#else
		std::strncpy(buffer, str.c_str(), bufferlen);
#endif
	}

	DLLEXPORT_PREFIX void sqfvm_loadconfig(const char* cfg)
	{
		sqfvm_virtualmachine->parse_config(cfg, sqf::configdata::configFile().data<sqf::configdata>());
	}

	DLLEXPORT_PREFIX void sqfvm_uninit()
	{
		sqf::commandmap::get().uninit();
		sqfvm_virtualmachine = std::shared_ptr<sqf::virtualmachine>();
	}
}