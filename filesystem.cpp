#include "filesystem.h"
#include <sstream>
#include <algorithm>


bool sqf::filesystem::try_get_physical_path(std::string virt, std::string & out)
{
	std::string virtMapping;
	virt = sanitize(virt);
	for (auto& vpath : m_virtualpaths)
	{
		if (virt.find(vpath) == 0)
		{
			virtMapping = vpath;
			virt = virt.substr(vpath.length());
		}
	}
	if (virtMapping.length == 0)
	{
		// Path not found.
		return false;
	}
	std::string physPath = m_virtualphysicalmap[virtMapping];
	size_t index = virt.find('/');
	while (index != std::string::npos)
	{
		std::string tmp = virt.substr(0, index);
		virt = virt.substr(index, 0);
		if (tmp.compare("..") == 0)
		{
			physPath = up(physPath);
		}
		else
		{
			physPath = down(physPath, tmp);
		}
	}
	if (std::find_if(m_physicalboundaries.begin(), m_physicalboundaries.end(), [physPath](std::string it) -> bool { return physPath.find(it) == 0; }) == m_physicalboundaries.end())
	{
		return false;
	}
	out = physPath;
	return true;
}

void sqf::filesystem::add_allowed_physical(std::string phys)
{
	phys = sanitize(phys);
}

void sqf::filesystem::add_mapping(std::string virt, std::string phys)
{
	virt = sanitize(virt);
	phys = sanitize(phys);
}



std::string sqf::filesystem::sanitize(std::string input)
{
	std::stringstream sstream;
	size_t i;
	sstream << '//';
	for (size_t i = input[0] == '/' || input[0] == '\\' ? 1 : 0; i < input.length - 1; i++)
	{
		char c = input[i];
		switch (c)
		{
			case '\\':
				sstream << '/';
				break;
			default:
				sstream << c;
				break;
		}
	}
	if (input[i] != '/' && input[i] != '\\')
	{
		sstream << input[i];
	}
	return sstream.str();
}

std::string sqf::filesystem::down(std::string input, std::string navigator)
{
	std::stringstream sstream(input.length + 1 + navigator.length);
	sstream << input;
	sstream << '/';
	sstream << navigator;
	return sstream.str();
}
