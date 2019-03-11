#include "fileio.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

#ifdef LOADFILE_CACHE
#include <unordered_map>

std::unordered_map<std::string, std::string> fileCache;

std::string load_file(const std::string & filename)
{
    auto found = fileCache.find(filename);
    if (found != fileCache.end()) return found->second;
    auto vec = readFile(filename);
    std::string ret(vec.begin() + get_bom_skip(vec), vec.end());
    fileCache[filename] = ret;
    return ret;
}

#else

std::string load_file(const std::string& filename)
{
    auto vec = readFile(filename);
    return std::string(vec.begin() + get_bom_skip(vec), vec.end());
}

#endif

std::vector<char> readFile(const std::string & filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
        std::stringstream sstream;
		sstream << "Could not open file '" << filename << "'.";
		throw std::runtime_error(sstream.str());
	}

	auto fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

int get_bom_skip(const std::vector<char>& buff)
{
	if (buff.size() == 0)
		return 0;
	if (buff[0] == (char)0xEF && buff[1] == (char)0xBB && buff[2] == (char)0xBF)
	{
		//UTF-8
		return 3;
	}
	else if (buff[0] == (char)0xFE && buff[1] == (char)0xFF)
	{
		//UTF-16 (BE)
		return 2;
	}
	else if (buff[0] == (char)0xFE && buff[1] == (char)0xFE)
	{
		//UTF-16 (LE)
		return 2;
	}
	else if (buff[0] == (char)0x00 && buff[1] == (char)0x00 && buff[2] == (char)0xFF && buff[3] == (char)0xFF)
	{
		//UTF-32 (BE)
		return 2;
	}
	else if (buff[0] == (char)0xFF && buff[1] == (char)0xFF && buff[2] == (char)0x00 && buff[3] == (char)0x00)
	{
		//UTF-32 (LE)
		return 2;
	}
	else if (buff[0] == (char)0x2B && buff[1] == (char)0x2F && buff[2] == (char)0x76 &&
		(buff[3] == (char)0x38 || buff[3] == (char)0x39 || buff[3] == (char)0x2B || buff[3] == (char)0x2F))
	{
		//UTF-7
		return 4;
	}
	else if (buff[0] == (char)0xF7 && buff[1] == (char)0x64 && buff[2] == (char)0x4C)
	{
		//UTF-1
		return 3;
	}
	else if (buff[0] == (char)0xDD && buff[1] == (char)0x73 && buff[2] == (char)0x66 && buff[3] == (char)0x73)
	{
		//UTF-EBCDIC
		return 3;
	}
	else if (buff[0] == (char)0x0E && buff[1] == (char)0xFE && buff[2] == (char)0xFF)
	{
		//SCSU
		return 3;
	}
	else if (buff[0] == (char)0xFB && buff[1] == (char)0xEE && buff[2] == (char)0x28)
	{
		//BOCU-1
		if (buff[3] == (char)0xFF)
			return 4;
		return 3;
	}
	else if (buff[0] == (char)0x84 && buff[1] == (char)0x31 && buff[2] == (char)0x95 && buff[3] == (char)0x33)
	{
		//GB 18030
		return 3;
	}
	return 0;
}


bool file_exists(const std::string& filename)
{
	std::ifstream infile(filename);
	return infile.good();
}