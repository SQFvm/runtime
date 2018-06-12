#include "netserver.h"
#include "virtualmachine.h"
#include "commandmap.h"
#include "value.h"
#include "vmstack.h"
#include "configdata.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <tclap\CmdLine.h>

#include "python_externc.h"


static std::string load_file(const std::string& filename);
static std::vector<char> readFileW(const std::string& filename);
static std::vector<char> readFile(const std::string& filename);

static std::string load_file(const std::string& filename)
{
	auto vec = readFile(filename);
	return std::string(vec.begin(), vec.end());
}
static std::vector<char> readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		std::stringstream sstream("Could not open file '");
		sstream << filename << "'.";
		throw std::runtime_error(sstream.str());
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

void StartServer()
{

}

int main(int argc, char** argv)
{
	TCLAP::CmdLine cmd("Emulates the ArmA-Series SQF environment.", ' ', "1.0");
	TCLAP::MultiArg<std::string> loadSqfFileArg("f", "sqf-file", "Loads provided sqf-file from the hdd into the sqf-vm.", false, "PATH");
	cmd.add(loadSqfFileArg);

	TCLAP::MultiArg<std::string> loadConfigFileArg("F", "config-file", "Loads provided config-file from the hdd into the sqf-vm.", false, "PATH");
	cmd.add(loadConfigFileArg);

	TCLAP::SwitchArg noPromptArg("a", "no-prompt", "Disables the prompt which expects you to type in sqf-code.", false);
	cmd.add(noPromptArg);

	TCLAP::SwitchArg useDebuggingServer("s", "start-server", "Causes the sqf-vm to start a network server allowing for automated control. Implicitly will cause --no-prompt to be set.", false);
	cmd.add(useDebuggingServer);

	TCLAP::ValueArg<int> maxInstructionsArg("m", "max-instructions", "Sets the maximum ammount of instructions to execute before a hard exit may occur. Setting this to 0 will disable the limit.", false, 0, "NUMBER");
	cmd.add(maxInstructionsArg);

	TCLAP::SwitchArg noPrintArg("n", "no-print", "Prevents the value stack to be printed out at the very end.", false);
	cmd.add(noPrintArg);

	cmd.parse(argc, argv);

	std::vector<std::string> sqfFiles = loadSqfFileArg.getValue();
	std::vector<std::string> configFiles = loadConfigFileArg.getValue();
	bool noPrompt = noPromptArg.getValue() || useDebuggingServer.getValue();
	bool startServer = useDebuggingServer.getValue();
	int maxinstructions = maxInstructionsArg.getValue();
	bool noPrint = noPrintArg.getValue();

	auto vm = sqf::virtualmachine();
	sqf::commandmap::get().init();
	bool errflag = false;

	//Load all sqf-files provided via arg.
	for (auto& f : sqfFiles)
	{
		try
		{
			auto str = load_file(f);
			vm.parse_sqf(str, f);
		}
		catch (std::runtime_error ex)
		{
			errflag = true;
			std::cout << ex.what() << std::endl;
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
		catch (std::runtime_error ex)
		{
			errflag = true;
			std::cout << ex.what() << std::endl;
		}
	}

	if (!noPrompt)
	{
		//Prompt user to type in code.
		int i = 0;
		printf("Please enter your SQF code.\nTo get the capabilities, use the `help__` instruction.\nTo run the code, Press <ENTER> twice.\n");
		std::string line;
		std::stringstream sstream;
		do
		{
			printf("%d:\t", i++);
			std::getline(std::cin, line);
			sstream << line << std::endl;
		} while (line.size() != 0);

		std::cout << std::endl;
		vm.parse_sqf(sstream.str());
	}

	if (startServer)
	{
		networking_init();
	}

	vm.execute();

	if (!noPrint)
	{
		std::shared_ptr<sqf::value> val;
		bool success;
		do {
			val = vm.stack()->popval(success);
			if (success)
			{
				vm.out() << "[WORK]\t<" << sqf::type_str(val->dtype()) << ">\t" << val->as_string() << std::endl;
			}
		} while (success);
		sqf::commandmap::get().uninit();
		std::wcout << std::endl;
	}

	if (startServer)
	{
		networking_cleanup();
	}

	sqf::commandmap::get().uninit();

	if (!noPrompt)
	{
		std::cout << std::endl << std::endl << "Press [ENTER] to continue.";
		std::string line;
		std::getline(std::cin, line);
	}
}