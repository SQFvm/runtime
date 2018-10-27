#include "filesystem.h"
#include "fileio.h"
#include <sstream>
#include <algorithm>


std::optional<std::string> sqf::filesystem::try_get_physical_path(std::string virt)
{
	std::string virtMapping;
	std::string physPath;
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
		auto res = std::find_if(m_physicalboundaries.begin(), m_physicalboundaries.end(), [virt](std::string it) -> bool
		{
			it = navigate(it, virt);
			return file_exists(it);
		});
		if (res == m_physicalboundaries.end())
		{
			return {};
		}
		else
		{
			physPath = *res;
		}
	}
	else
	{
		physPath = m_virtualphysicalmap[virtMapping];
	}
	physPath = navigate(physPath, virt);
	if (std::find_if(m_physicalboundaries.begin(), m_physicalboundaries.end(), [physPath](std::string it) -> bool { return physPath.find(it) == 0; }) == m_physicalboundaries.end())
	{
		return {};
	}
	return physPath;
}

void sqf::filesystem::add_allowed_physical(std::string phys)
{
	phys = sanitize(phys);
	m_physicalboundaries.push_back(phys);
}

void sqf::filesystem::add_mapping(std::string virt, std::string phys)
{
	virt = sanitize(virt);
	phys = sanitize(phys);
	m_physicalboundaries.push_back(phys);
	m_virtualphysicalmap[virt] = phys;
}



std::string sqf::filesystem::sanitize(std::string input)
{
	std::stringstream sstream;
	size_t i;
	if (!(input.length() > 2 && input[1] == ':'))
	{
		sstream << "/";
	}
	bool wasSlash = true;
	for (i = input[0] == '/' || input[0] == '\\' ? 1 : 0; i < input.length() - 1; i++)
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
				if (wasSlash && input[i + 1] == '/' || input[i + 1] == '\\')
				{
					i++;
					break;
				}
				wasSlash = false;
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
	std::stringstream sstream;
	sstream << input << "/" << navigator;
	return sstream.str();
}

std::string sqf::filesystem::navigate(std::string input, std::string navigator)
{
	size_t index = 0;
	while ((index = navigator.find('/', 1)) != std::string::npos)
	{
		std::string tmp = navigator.substr(1, index - 1);
		if (tmp.empty())
		{
			continue;
		}
		navigator = navigator.substr(index);
		if (tmp == "..")
		{
			input = up(input);
		}
		else
		{
			input = down(input, tmp);
		}
	}
	input = down(input, navigator.substr(1));

	return input;
}