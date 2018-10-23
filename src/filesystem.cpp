#include "filesystem.h"
#include <sstream>
#include <algorithm>


std::optional<std::string> sqf::filesystem::try_get_physical_path(std::string virt)
{
	std::string virtMapping;
	virt = sanitize(virt);
	for (const auto& vpath : m_virtualpaths)
	{
		if (virt.find(vpath) == 0)
		{
			virtMapping = vpath;
			virt = virt.substr(vpath.length());
		}
	}
	if (virtMapping.empty())
	{
		// Path not found.
		return {};
	}
	std::string physPath = m_virtualphysicalmap[virtMapping];
	size_t index;
	while ((index = virt.find('/')) != std::string::npos)
	{	
		std::string tmp = virt.substr(0, index);
		virt = virt.substr(index, 0);
		if (tmp == "..")
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
		return {};
	}
	return physPath;
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
	bool wasSlash = false;
	for (size_t i = input[0] == '/' || input[0] == '\\' ? 1 : 0; i < input.length - 1; i++)
	{
		char c = input[i];
		switch (c)
		{
			case '\\':
			case '/':
				if (!wasSlash)
				{
					sstream << '/';
					wasSlash = true;
				}
				break;
			case '.':
				wasSlash = false;
				if (input[i + 1] == '/' || input[i + 1] == '\\')
				{
					i += 2;
					break;
				}
			default:
				wasSlash = false;
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
