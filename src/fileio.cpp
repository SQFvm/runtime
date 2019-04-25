#include "fileio.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>


#ifdef LOADFILE_CACHE
#include <unordered_map>
#include <mutex>

std::unordered_map<std::string, std::string> fileCache;
std::mutex cacheMutex;

std::string load_file(const std::string & filename)
{
    std::unique_lock<std::mutex> lock(cacheMutex);
    auto found = fileCache.find(filename);
    if (found != fileCache.end()) return found->second;
    lock.unlock();
    auto vec = readFile(filename);
    std::string ret(vec.begin() + get_bom_skip(vec), vec.end());
    lock.lock();
    fileCache[filename] = ret;
    lock.unlock();
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
	if (buff.empty())
		return 0;
	// We are comparing against unsigned
	auto ubuff = reinterpret_cast<const unsigned char*>(buff.data());
	if (ubuff[0] == 0xEF && ubuff[1] == 0xBB && ubuff[2] == 0xBF)
	{
		//UTF-8
		return 3;
	}
	else if (ubuff[0] == 0xFE && ubuff[1] == 0xFF)
	{
		//UTF-16 (BE)
		return 2;
	}
	else if (ubuff[0] == 0xFE && ubuff[1] == 0xFE)
	{
		//UTF-16 (LE)
		return 2;
	}
	else if (ubuff[0] == 0x00 && ubuff[1] == 0x00 && ubuff[2] == 0xFF && ubuff[3] == 0xFF)
	{
		//UTF-32 (BE)
		return 2;
	}
	else if (ubuff[0] == 0xFF && ubuff[1] == 0xFF && ubuff[2] == 0x00 && ubuff[3] == 0x00)
	{
		//UTF-32 (LE)
		return 2;
	}
	else if (ubuff[0] == 0x2B && ubuff[1] == 0x2F && ubuff[2] == 0x76 &&
		(ubuff[3] == 0x38 || ubuff[3] == 0x39 || ubuff[3] == 0x2B || ubuff[3] == 0x2F))
	{
		//UTF-7
		return 4;
	}
	else if (ubuff[0] == 0xF7 && ubuff[1] == 0x64 && ubuff[2] == 0x4C)
	{
		//UTF-1
		return 3;
	}
	else if (ubuff[0] == 0xDD && ubuff[1] == 0x73 && ubuff[2] == 0x66 && ubuff[3] == 0x73)
	{
		//UTF-EBCDIC
		return 3;
	}
	else if (ubuff[0] == 0x0E && ubuff[1] == 0xFE && ubuff[2] == 0xFF)
	{
		//SCSU
		return 3;
	}
	else if (ubuff[0] == 0xFB && ubuff[1] == 0xEE && ubuff[2] == 0x28)
	{
		//BOCU-1
		if (ubuff[3] == 0xFF)
			return 4;
		return 3;
	}
	else if (ubuff[0] == 0x84 && ubuff[1] == 0x31 && ubuff[2] == 0x95 && ubuff[3] == 0x33)
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