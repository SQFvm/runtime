#include "filesystem.h"
#include "fileio.h"
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <fstream>

void sqf::filesystem::addPathMappingInternal(std::filesystem::path virt, std::filesystem::path phy) {
    std::vector<std::string> virtElements;

    while (virt.has_parent_path()) {
        virtElements.emplace_back(virt.filename().string());
        virt = virt.parent_path();
    }
    virtElements.emplace_back(virt.filename().string()); //last element

    std::reverse(virtElements.begin(), virtElements.end());


    auto found = m_virtualphysicalmapNew.find(virtElements[0]);
    auto curIter = m_virtualphysicalmapNew.end();
    bool first = true;
    for (auto& it : virtElements) {
        if (first) { //first element
            first = false; //this is ugly. But comparing iterators doesn't work
            curIter = m_virtualphysicalmapNew.find(it);
            if (curIter == m_virtualphysicalmapNew.end())
                curIter = m_virtualphysicalmapNew.insert({ it, pathElement{} }).first;
            continue;
        }
        auto& curEl = curIter->second;
        curIter = curEl.subPaths.find(it);
        if (curIter == curEl.subPaths.end())
            curIter = curEl.subPaths.insert({ it, pathElement{} }).first;
    }

    curIter->second.physicalPath = phy;
}

std::optional<std::filesystem::path> sqf::filesystem::resolvePath(std::filesystem::path virt) {
    std::vector<std::string> virtElements;

    while (virt.has_parent_path()) {
        if (virt.filename().string().empty()) break; //don't resolve starting \\ 
        virtElements.emplace_back(virt.filename().string());
        virt = virt.parent_path();
    }

    std::reverse(virtElements.begin(), virtElements.end());


    std::vector<std::map<std::string, pathElement>::iterator> pathStack; //In case we need to walk back upwards

    auto curIter = m_virtualphysicalmapNew.end();
    bool first = true;
    for (auto& it : virtElements) {
        if (first) { //first element
            first = false; //this is ugly. But comparing iterators doesn't work
            curIter = m_virtualphysicalmapNew.find(it);
            if (curIter == m_virtualphysicalmapNew.end())
                break; //not found
            pathStack.emplace_back(curIter);
            continue;
        }
        auto& curEl = curIter->second;
        curIter = curEl.subPaths.find(it);
        if (curIter == curEl.subPaths.end())
            break; //not found
        pathStack.emplace_back(curIter);
    }

    virtElements.erase(virtElements.begin(), virtElements.begin() + pathStack.size()); //Remove elements we resolved to get a list of leftovers


    //walk up stack until we find a phys path.

    while (!pathStack.back()->second.physicalPath) {
        virtElements.emplace_back(pathStack.back()->first);
        pathStack.pop_back();
        if (pathStack.empty())
            return {}; //whole stack didn't have physical path
    }

    //build full path to file
    auto curPath = *pathStack.back()->second.physicalPath;
    for (auto& it : virtElements)
        curPath = curPath / it;

    return curPath;
}

std::optional<std::string> sqf::filesystem::try_get_physical_path(std::string virt, std::string current)
{
	std::string virtMapping;
	std::string physPath;
    if (virt.front() != '\\') { //It's a local path
        auto parentDirectory = std::filesystem::path(current).parent_path(); //Get parent of current file
        auto wantedFile = parentDirectory / virt;

        if (std::filesystem::exists(wantedFile)) return wantedFile.string();
    } else { //global path
        auto resolved = resolvePath(virt); //#TODO only if starting with \\ 
        if (resolved) {
            return resolved->string();
        }
    }

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
        


		auto res = std::find_if(m_virtualpaths.begin(), m_virtualpaths.end(), [current](std::string it) -> bool
		{
			return current.find(it) != std::string::npos;
		});
		if (res == m_virtualpaths.end())
		{
			auto res2 = std::find_if(m_physicalboundaries.begin(), m_physicalboundaries.end(), [virt, current](std::string it) -> bool
			{
				auto findRes = current.find(it);
				if (!current.empty() && findRes == std::string::npos)
				{
					return false;
				}
				auto partial = current.substr(it.length());
				partial = up(partial);
				partial = navigate(it, partial);
				partial = navigate(partial, virt);
				return file_exists(partial);
			});
			if (res2 == m_physicalboundaries.end())
			{
				return {};
			}
			else
			{
				physPath = *res2;
				auto partial = current.substr(physPath.length());
				partial = up(partial);
				partial = navigate(physPath, partial);
				partial = navigate(partial, virt);
				return partial;
			}
		}
		else
		{
			physPath = m_virtualphysicalmap[*res];
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
	m_virtualpaths.push_back(virt);
	m_virtualphysicalmap[virt] = phys;
}

void sqf::filesystem::add_mapping_auto(std::string phys) {
    const std::filesystem::path ignoreGit(".git");
    const std::filesystem::path ignoreSvn(".svn");

    //recursively search for pboprefix
    for (auto i = std::filesystem::recursive_directory_iterator(phys, std::filesystem::directory_options::follow_directory_symlink);
        i != std::filesystem::recursive_directory_iterator();
        ++i)
    {
        if (i->is_directory() && (i->path().filename() == ignoreGit || i->path().filename() == ignoreSvn))
        {
            i.disable_recursion_pending(); //Don't recurse into that directory
            continue;
        }
        if (!i->is_regular_file()) continue;

        if (i->path().filename() == "$PBOPREFIX$")
        {
            
            std::ifstream prefixFile(i->path());
            std::string prefix;
            std::getline(prefixFile, prefix);
            prefixFile.close();
            addPathMappingInternal(prefix, i->path().parent_path());
           add_mapping(prefix, i->path().parent_path().string());
        }
    }
}


std::string sqf::filesystem::sanitize(std::string input)
{
	std::stringstream sstream;
	size_t i;
	if (!(input.length() > 2 && input[1] == ':'))
	{
		sstream << FSDELIMITER;
	}
	bool wasSlash = true;
	if (input.empty())
	{
		return input;
	}
	for (i = input[0] == '/' || input[0] == '\\' ? 1 : 0; i < input.length() - 1; i++)
	{
		char c = input[i];
		switch (c)
		{
			case '\\':
			case '/':
				if (!wasSlash)
				{
					sstream << FSDELIMITER;
					wasSlash = true;
				}
				break;
			case '.':
				if (wasSlash && (input[i + 1] == '/' || input[i + 1] == '\\'))
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
	sstream << input << FSDELIMITER << navigator;
	return sstream.str();
}

std::string sqf::filesystem::navigate(std::string input, std::string navigator)
{
	size_t index = 0;
	if (navigator.empty())
	{
		return input;
	}
	while ((index = navigator.find(FSDELIMITER, 1)) != std::string::npos)
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