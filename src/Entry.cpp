#include "Entry.h"
#include "netserver.h"
#include "virtualmachine.h"
#include "commandmap.h"
#include "value.h"
#include "vmstack.h"
#include "configdata.h"
#include "fileio.h"
#include "parsepreprocessor.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <tclap/CmdLine.h>
#include <algorithm>

#include "dllexports.h"
#include "debugger.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <limits.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#endif



int console_width()
{
#if _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left;
	return columns;
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_row;
#endif
}


std::string get_executable_path()
{
#if defined(_WIN32) || defined(_WIN64)
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
#elif defined(__GNUC__)
	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	return std::string(result, (count > 0) ? count : 0);
#else
#error "NO IMPLEMENTATION AVAILABLE"
#endif
}

bool isInLoadFileCliMode = false;

int main(int argc, char** argv)
{
	auto executable_path = sqf::filesystem::sanitize(get_executable_path());
	TCLAP::CmdLine cmd("Emulates the ArmA-Series SQF environment.", ' ', VERSION_FULL "\n");
	TCLAP::MultiArg<std::string> loadSqfFileArg("f", "sqf-file", "Loads provided sqf-file from the hdd into the sqf-vm. Supports relative directory using './path' and absolut pathing.", false, "PATH");
	cmd.add(loadSqfFileArg);

	TCLAP::MultiArg<std::string> loadConfigFileArg("F", "config-file", "Loads provided config-file from the hdd into the sqf-vm. Supports relative directory using './path' and absolut pathing.", false, "PATH");
	cmd.add(loadConfigFileArg);

	TCLAP::MultiArg<std::string> loadSqfRawArg("r", "sqf-code", "Loads provided sqf-code directly into the sqf-vm. (executed after files). Input is not getting preprocessed!", false, "CODE");
	cmd.add(loadSqfRawArg);

	TCLAP::MultiArg<std::string> loadConfigRawArg("R", "config-code", "Loads provided config-code directly into the sqf-vm. (executed after files). Input is not getting preprocessed!", false, "CODE");
	cmd.add(loadConfigRawArg);

	TCLAP::SwitchArg noPromptArg("a", "no-prompt", "Disables the prompt which expects you to type in sqf-code.", false);
	cmd.add(noPromptArg);

	TCLAP::ValueArg<int> debuggerArg("d", "debugger", "Causes the sqf-vm to start a network server that allows to attach a single debugger to it.", false, 0, "PORT");
	cmd.add(debuggerArg);

	TCLAP::ValueArg<int> maxInstructionsArg("m", "max-instructions", "Sets the maximum ammount of instructions to execute before a hard exit may occur. Setting this to 0 will disable the limit.", false, 0, "NUMBER");
	cmd.add(maxInstructionsArg);

	TCLAP::SwitchArg noPrintArg("n", "no-print", "Prevents the value stack to be printed out at the very end.", false);
	cmd.add(noPrintArg);

	TCLAP::SwitchArg noExecutePrintArg("N", "no-execute-print", "Prevents the `Execute` and two horizontal lines to be printed.", false);
	cmd.add(noExecutePrintArg);

	TCLAP::SwitchArg disableClassnameCheckArg("c", "enable-classname-check", "Enables the config checking for eg. createVehicle.", false);
	cmd.add(disableClassnameCheckArg);

	TCLAP::MultiArg<std::string> prettyPrintArg("", "pretty-print", "Loads provided file from disk and pretty-prints it onto console.", false, "PATH");
	cmd.add(prettyPrintArg);

	TCLAP::SwitchArg noLoadExecDirArg("", "no-load-execdir", "Prevents automatically adding the workspace to the path of allowed locations.", false);
	cmd.add(noLoadExecDirArg);

	TCLAP::MultiArg<std::string> loadArg("l", "load", "Adds provided path to the allowed locations list. Supports relative directory using './path' and absolut pathing.", false, "PATH");
	cmd.add(loadArg);

	TCLAP::MultiArg<std::string> preprocessFileArg("E", "preprocess-file", "Runs the preprocessor on provided file and prints it to stdout. Supports relative directory using './path' and absolut pathing.", false, "PATH");
	cmd.add(preprocessFileArg);

	TCLAP::MultiArg<std::string> virtualArg("v", "virtual", "Creates a mapping for a virtual and a physical path. Mapping is separated by a '|', with the left side being the physical, and the right argument the virtual path. Supports relative directory using './path' and absolut pathing for physical path.", false, "PATH|VIRTUAL");
	cmd.add(virtualArg);

	TCLAP::ValueArg<std::string> cliFileArg("", "cli-file", "Allows to provide a file from which to load arguments from. If passed, all other arguments will be ignored! Supports relative directory using './path' and absolut pathing. Each argument needs to be separated by line-feed.", false, "", "PATH");
	cmd.add(cliFileArg);

	cmd.parse(argc, argv);

	// ALWAYS needs to be parsed first!
	if (!cliFileArg.getValue().empty())
	{
		if (isInLoadFileCliMode)
		{
			std::cout << "'" << cliFileArg.getName() << "' is not allowed inside of the actual file!." << std::endl;
			return -1;
		}
		isInLoadFileCliMode = true;
		auto f = cliFileArg.getValue();
		auto sanitized = sqf::filesystem::sanitize(f);
		if (sanitized.empty())
		{
			std::cout << "'" << f << "' is no valid file path after sanitize." << std::endl;
			return -1;
		}
		if (f.length() > 2 && f[0] == '.' && (f[1] == '/' || f[1] == '\\'))
		{
			sanitized = sqf::filesystem::navigate(executable_path, sanitized);
		}
		auto str = load_file(sanitized);
		std::vector<char*> args;
		args.push_back(argv[0]); // ToDo: Catch those moments when argv[0] is not set by the OS
		size_t index = 0, last_index = 0;

		try
		{
			bool dobreak = false;
			while (!dobreak)
			{
				if ((index = str.find('\n', last_index)) == std::string::npos)
				{
					index = str.length();
					dobreak = true;
				}
				auto line = str.substr(last_index, index - last_index);
				line.erase(std::find_if(line.rbegin(), line.rend(), [](char c) -> bool {
					return c != '\r';
				}).base(), line.end());
				last_index = index + 1;
				if ((index = line.find(' ')) != std::string::npos)
				{
					auto arg = line.substr(index + 1);
					line = line.substr(0, index);
					auto args_cstr = new char[arg.length() + 1];
					std::strcpy(args_cstr, arg.c_str());
					auto line_cstr = new char[line.length() + 1];
					std::strcpy(line_cstr, line.c_str());
					args.push_back(line_cstr);
					args.push_back(args_cstr);
				}
				else
				{
					auto line_cstr = new char[line.length() + 1];
					std::strcpy(line_cstr, line.c_str());
					args.push_back(line_cstr);
				}
			}

			main((int)args.size(), args.data());

			if (args.size() > 1)
			{
				for (auto it = args.begin() + 1; it != args.end(); it++)
				{
					delete[] * it;
				}
			}
			return 0;
		}
		catch (std::runtime_error err)
		{
			std::cout << err.what() << std::endl;
			return -1;
		}
	}

	std::vector<std::string> sqfFiles = loadSqfFileArg.getValue();
	std::vector<std::string> configFiles = loadConfigFileArg.getValue();
	std::vector<std::string> sqfRaw = loadSqfRawArg.getValue();
	std::vector<std::string> configRaw = loadConfigRawArg.getValue();
	bool noPrompt = noPromptArg.getValue();
	int maxinstructions = maxInstructionsArg.getValue();
	bool noPrint = noPrintArg.getValue();
	bool noExecutePrint = noExecutePrintArg.getValue();
	bool disableClassnameCheck = disableClassnameCheckArg.getValue();
	bool noLoadExecDir = noLoadExecDirArg.getValue();

	bool debugger_port = debuggerArg.getValue();


	sqf::virtualmachine vm;
	sqf::commandmap::get().init();
	bool errflag = false;
	netserver* srv = nullptr;
	sqf::debugger* dbg = nullptr;

	vm.perform_classname_checks(disableClassnameCheck);

	// Prepare Virtual-File-System
	if (!noLoadExecDir)
	{
		vm.get_filesystem().add_allowed_physical(executable_path);
	}
	for (auto& f : loadArg.getValue())
	{
		auto sanitized = sqf::filesystem::sanitize(f);
		if (sanitized.empty())
		{
			continue;
		}
		if (f.length() > 2 && f[0] == '.' && (f[1] == '/' || f[1] == '\\'))
		{
			sanitized = sqf::filesystem::navigate(executable_path, sanitized);
		}
		vm.get_filesystem().add_allowed_physical(sanitized);
	}
	for (auto& f : virtualArg.getValue())
	{
		auto split_index = f.find('|');
		if (split_index == -1)
		{
			errflag = true;
			std::cout << "Failed find splitter '|' for mapping '" << f << "'." << std::endl;
			continue;
		}
		auto phys = f.substr(0, split_index);
		auto virt = f.substr(split_index + 1);
		auto physSanitized = sqf::filesystem::sanitize(f.substr(0, split_index));
		auto virtSanitized = sqf::filesystem::sanitize(f.substr(split_index + 1));
		if (physSanitized.empty() || virtSanitized.empty())
		{
			continue;
		}
		if (f.length() > 2 && f[0] == '.' && (f[1] == '/' || f[1] == '\\'))
		{
			physSanitized = sqf::filesystem::navigate(executable_path, physSanitized);
		}
		vm.get_filesystem().add_mapping(virtSanitized, physSanitized);
	}


	// Execute all possible Pretty-Print requests
	for (auto& f : prettyPrintArg.getValue())
	{
		try
		{
			auto str = load_file(f);
			vm.pretty_print_sqf(str);
			vm.out_buffprint();
		}
		catch (const std::runtime_error& ex)
		{
			errflag = true;
			std::cout << "Failed to load file " << ex.what() << std::endl;
		}
	}

	// Preprocess the files
	for (auto& f : preprocessFileArg.getValue())
	{
		auto sanitized = sqf::filesystem::sanitize(f);
		try
		{
			if (sanitized.empty())
			{
				continue;
			}
			if (f.length() > 2 && f[0] == '.' && (f[1] == '/' || f[1] == '\\'))
			{
				sanitized = sqf::filesystem::navigate(executable_path, sanitized);
			}
			auto str = load_file(sanitized);
			bool err = false;
			auto ppedStr = sqf::parse::preprocessor::parse(&vm, str, err, sanitized);
			if (err)
			{
				vm.err_buffprint();
				vm.err_clear();
			}
			else
			{
				vm.out() << ppedStr;
				vm.out_buffprint();
				vm.out_clear();
			}
		}
		catch (const std::runtime_error& ex)
		{
			errflag = true;
			std::cout << "Failed to load file " << ex.what() << std::endl;
		}
	}

	//Load all sqf-files provided via arg.
	for (auto& f : sqfFiles)
	{
		auto sanitized = sqf::filesystem::sanitize(f);
		try
		{
			if (sanitized.empty())
			{
				continue;
			}
			if (f.length() > 2 && f[0] == '.' && (f[1] == '/' || f[1] == '\\'))
			{
				sanitized = sqf::filesystem::navigate(executable_path, sanitized);
			}
			auto str = load_file(sanitized);
			bool err = false;
			auto ppedStr = sqf::parse::preprocessor::parse(&vm, str, err, sanitized);
			if (err)
			{
				vm.err_buffprint();
				vm.err_clear();
			}
			else
			{
				vm.parse_sqf(ppedStr, f);
			}
		}
		catch (const std::runtime_error& ex)
		{
			errflag = true;
			std::cout << "Failed to load file '" << sanitized << "': " << ex.what() << std::endl;
		}
	}

	//Load & merge all config-files provided via arg.
	for (auto& f : configFiles)
	{
		auto sanitized = sqf::filesystem::sanitize(f);
		try
		{
			if (sanitized.empty())
			{
				continue;
			}
			if (f.length() > 2 && f[0] == '.' && (f[1] == '/' || f[1] == '\\'))
			{
				sanitized = sqf::filesystem::navigate(executable_path, sanitized);
			}
			auto str = load_file(sanitized);
			bool err = false;
			auto ppedStr = sqf::parse::preprocessor::parse(&vm, str, err, sanitized);
			if (err)
			{
				vm.err_buffprint();
				vm.err_clear();
			}
			else
			{
				vm.parse_config(ppedStr, sqf::configdata::configFile()->data<sqf::configdata>());
			}
		}
		catch (const std::runtime_error& ex)
		{
			errflag = true;
			std::cout << "Failed to load file '" << sanitized << "': " << ex.what() << std::endl;
		}
	}
	if (errflag)
	{
		if (!noPrompt)
		{
			std::string line;
			std::cout << std::endl << "Press [ENTER] to continue...";
			std::getline(std::cin, line);
		}
		return -1;
	}
	//Load all sqf-code provided via arg.
	for (auto& raw : sqfRaw)
	{
		vm.parse_sqf(raw);
	}

	//Load & merge all config-code provided via arg.
	for (auto& raw : configRaw)
	{
		vm.parse_config(raw, sqf::configdata::configFile()->data<sqf::configdata>());
	}

	if (debugger_port > 0)
	{
		networking_init();
		dbg = new sqf::debugger((srv = new netserver(debugger_port)));
		vm.dbg(dbg);
		std::cout << "Waiting for client to connect..." << std::endl;
		try
		{
			srv->wait_accept();
			std::cout << "Client connected!" << std::endl;
		}
		catch (const std::runtime_error& err)
		{
			std::cout << err.what() << std::endl;
		}
	}
	vm.execute();
	do
	{
		if (!noPrompt)
		{
			//Prompt user to type in code.
			int i = 0;
			printf("Please enter your SQF code.\n"
				"To get info about a command, use the `help__` operator.\n"
				"For a list of all implemented commands, use the `cmds__` operator.\n"
				"For a list of all SQF-VM internal commands, use the `vm__` operator.\n"
				"To run the code, Press [ENTER] twice.\n"
				"To exit, use the `exit__` command.\n"
				"If you enjoy this tool, consider donating: https://paypal.me/X39\n");
			std::string line;
			std::stringstream sstream;
			do
			{
				printf("%d:\t", i++);
				std::getline(std::cin, line);
				sstream << line << std::endl;
			} while (!line.empty());

			std::cout << std::endl;

			auto input = sstream.str();
			bool err = false;
			auto inputAfterPP = sqf::parse::preprocessor::parse(&vm, input, err, sqf::filesystem::navigate(executable_path, "__commandlinefeed.sqf"));
			if (err)
			{
				vm.err_buffprint();
			}
			else
			{
				vm.parse_sqf(inputAfterPP, sqf::filesystem::navigate(executable_path, "__commandlinefeed.sqf"));
			}
		}


		if (noExecutePrint)
		{
			vm.execute();
		}
		else
		{
			std::cout << "Executing..." << std::endl;
			std::cout << std::string(console_width(), '-') << std::endl;
			vm.execute();
			std::cout << std::string(console_width(), '-') << std::endl;
		}
		if (!noPrint)
		{
			std::shared_ptr<sqf::value> val;
			bool success;
			do {
				val = vm.stack()->popval(success);
				if (success)
				{
					std::cout << "[WORK]\t<" << sqf::type_str(val->dtype()) << ">\t" << val->as_string() << std::endl;
				}
			} while (success);
			std::wcout << std::endl;
		}

	} while (!noPrompt && !vm.exitflag());

	sqf::commandmap::get().uninit();

	if (debugger_port > 0)
	{
		networking_cleanup();
		if (dbg)
		{
			delete dbg;
			dbg = nullptr;
		}
		if (srv)
		{
			delete srv;
			srv = nullptr;
		}
	}

	sqf::commandmap::get().uninit();
	return 0;
}
