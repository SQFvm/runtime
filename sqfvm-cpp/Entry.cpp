#include "virtualmachine.h"
#include "commandmap.h"
#include "value.h"
#include "vmstack.h"
#include "configdata.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

#include "python_externc.h"

static std::string load_file(const std::string& filename);
static std::vector<char> readFileW(const std::string& filename);
static std::vector<char> readFile(const std::string& filename);

static std::string load_file(const std::string& filename)
{
	auto vec = readFileW(filename);
	return std::string(vec.begin(), vec.end());
}
static std::vector<char> readFileW(const std::string& filename)
{
	auto vec = readFile(filename);
	auto vecW = std::vector<char>(vec.size());
	for (size_t i = 0; i < vec.size(); i++)
	{
		vecW[i] = vec[i];
	}
	return vecW;
}
static std::vector<char> readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		return std::vector<char>();
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

int main(int argc, char** argv)
{
	//py_init(100000);
	//char buff[2000];
	//py_exec("diag_log 12", buff, 2000);
	//py_uninit();

	auto vm = sqf::virtualmachine();
	//std::string line;
	//std::wcin >> line;
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

	std::wcout << std::endl;

	/*
	sqf::commandmap::get().init();

	vm.out() << std::endl << "ASSEMBLY" << std::endl;
	//vm.parse_assembly("push SCALAR 1; push SCALAR 1; callBinary +; push SCALAR 2; callBinary -;");
	//vm.parse_assembly("push SCALAR 1; push STRING 'test'; push STRING 'test''escape'; push SCALAR 1; push SCALAR 2; push SCALAR 3; callnular ntest; makeArray 4; makeArray 4;");
	//vm.parse_assembly("push SCALAR 12; assignToLocal _test;");
	vm.execute();
	
	vm.out() << std::endl << "SQF" << std::endl;
	//vm.parse_sqf("1 + 1 - 2; 0; {1 + 2 * 3 - 1 * 4}; 6; utest 1; ntest; [1, 'test', 'test''escape', [1, 2, 3, ntest]]; private _test = 12; _test2 = 13; _test; globalvar;");
	//vm.parse_sqf("1 + 1 - 2;");
	//vm.parse_sqf("[1, 'test', 'test''escape', [1, 2, 3, ntest]]");
	//vm.parse_sqf("private _test = 12;");
	vm.execute();

	sqf::commandmap::get().uninit();
	*/

	sqf::commandmap::get().init();
	//auto cd = std::make_shared<sqf::configdata>();

	vm.parse_config(load_file("C:\\Users\\marco.silipo\\Downloads\\AiO.1.76.142872\\AiO.1.76.142872.cpp"), sqf::configdata::configFile()->data<sqf::configdata>());
	vm.parse_sqf(sstream.str());
	vm.execute();
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
	system("pause");
}

/*
    0       4         0       4    0
[1, 2, 3] select ([0, 1, 2] select 2);

                  [0, 1, 2]        2
[1, 2, 3]        (          select  )
          select                     ;
                       [0, 1]
                 count          1
[1, 2, 3]                     +
          select                     ;

- (...) wie konstanten wert behandeln
- (...) rekursiv abarbeiten
- array mit index führen

*/