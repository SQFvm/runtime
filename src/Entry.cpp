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
#include <tclap/CmdLine.h>

#include "dllexports.h"
#include "debugger.h"
#ifdef _WIN32
#include <windows.h>
#else
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

int main(int argc, char** argv)
{

	TCLAP::CmdLine cmd("Emulates the ArmA-Series SQF environment.", ' ', VERSION_FULL "\n");
	TCLAP::MultiArg<std::string> loadSqfFileArg("f", "sqf-file", "Loads provided sqf-file from the hdd into the sqf-vm.", false, "PATH");
	cmd.add(loadSqfFileArg);

	TCLAP::MultiArg<std::string> loadConfigFileArg("F", "config-file", "Loads provided config-file from the hdd into the sqf-vm.", false, "PATH");
	cmd.add(loadConfigFileArg);

	TCLAP::MultiArg<std::string> loadSqfRawArg("r", "sqf-code", "Loads provided sqf-code directly into the sqf-vm. (executed after files)", false, "CODE");
	cmd.add(loadSqfRawArg);

	TCLAP::MultiArg<std::string> loadConfigRawArg("R", "config-code", "Loads provided config-code directly into the sqf-vm. (executed after files)", false, "CODE");
	cmd.add(loadConfigRawArg);

	TCLAP::SwitchArg noPromptArg("a", "no-prompt", "Disables the prompt which expects you to type in sqf-code.", false);
	cmd.add(noPromptArg);

	TCLAP::SwitchArg useDebuggingServer("s", "start-server", "Causes the sqf-vm to start a network server allowing for automated control.", false);
	cmd.add(useDebuggingServer);

	TCLAP::ValueArg<int> maxInstructionsArg("m", "max-instructions", "Sets the maximum ammount of instructions to execute before a hard exit may occur. Setting this to 0 will disable the limit.", false, 0, "NUMBER");
	cmd.add(maxInstructionsArg);

	TCLAP::ValueArg<int> serverPortArg("p", "server-port", "Sets the port of the server. Defaults to 9090.", false, 9090, "NUMBER");
	cmd.add(serverPortArg);

	TCLAP::SwitchArg noPrintArg("n", "no-print", "Prevents the value stack to be printed out at the very end.", false);
	cmd.add(noPrintArg);

	TCLAP::SwitchArg noExecutePrintArg("N", "no-execute-print", "Prevents the `Execute` and two horizontal lines to be printed.", false);
	cmd.add(noExecutePrintArg);

	TCLAP::SwitchArg disableClassnameCheckArg("c", "enable-classname-check", "Enables the config checking for eg. createVehicle.", false);
	cmd.add(disableClassnameCheckArg);

	TCLAP::MultiArg<std::string> prettyPrintArg("", "pretty-print", "Loads provided file from disk and pretty-prints it onto console.", false, "PATH");
	cmd.add(prettyPrintArg);

	TCLAP::MultiArg<std::string> fileSystemPathArg("", "filesystem-path", "Adds provided base-path to the list of allowed locations one can be inside of. Should be absolute paths!", false, "PATH");
	cmd.add(fileSystemPathArg);

	TCLAP::SwitchArg noAutoaddFilesystemArg("", "no-autoadd-filesystem", "Prevents automatically adding the workspace to the path of allowed locations.", false);
	cmd.add(noAutoaddFilesystemArg);

	cmd.parse(argc, argv);

	std::vector<std::string> sqfFiles = loadSqfFileArg.getValue();
	std::vector<std::string> configFiles = loadConfigFileArg.getValue();
	std::vector<std::string> sqfRaw = loadSqfRawArg.getValue();
	std::vector<std::string> configRaw = loadConfigRawArg.getValue();
	bool noPrompt = noPromptArg.getValue();
	bool startServer = useDebuggingServer.getValue();
	int maxinstructions = maxInstructionsArg.getValue();
	int serverPort = serverPortArg.getValue();
	bool noPrint = noPrintArg.getValue();
	bool noExecutePrint = noExecutePrintArg.getValue();
	bool disableClassnameCheck = disableClassnameCheckArg.getValue();
	bool noAutoaddFilesystem = noAutoaddFilesystemArg.getValue();


	sqf::virtualmachine vm;
	sqf::commandmap::get().init();
	bool errflag = false;
	netserver* srv = nullptr;
	sqf::debugger* dbg = nullptr;

	vm.perform_classname_checks(disableClassnameCheck);

	if (!noAutoaddFilesystem)
	{
		vm.get_filesystem().add_allowed_physical(get_executable_path());
	}

	for (auto& f : fileSystemPathArg.getValue())
	{
		vm.get_filesystem().add_allowed_physical(f);
	}

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

	//Load all sqf-files provided via arg.
	for (auto& f : sqfFiles)
	{
		try
		{
			auto str = load_file(f);
			vm.parse_sqf(str, f);
		}
		catch (const std::runtime_error& ex)
		{
			errflag = true;
			std::cout << "Failed to load file " << ex.what() << std::endl;
		}
	}

	//Load & merge all config-files provided via arg.
	for (auto& f : configFiles)
	{
		try
		{
			auto str = load_file(f);
			vm.parse_config(str, sqf::configdata::configFile()->data<sqf::configdata>());
		}
		catch (const std::runtime_error& ex)
		{
			errflag = true;
			std::cout << "Failed to load file " << ex.what() << std::endl;
		}
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

	if (startServer)
	{
		networking_init();
		dbg = new sqf::debugger((srv = new netserver(serverPort)));
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
			auto inputAfterPP = sqf::parse::preprocessor::parse(&vm, input, err, "__commandlinefeed.sqf");
			if (err)
			{
				vm.err_buffprint();
			}
			else
			{
				vm.parse_sqf(inputAfterPP, "__commandlinefeed.sqf");
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

	if (startServer)
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
}