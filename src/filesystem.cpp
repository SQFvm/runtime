#include "filesystem.h"
#include "fileio.h"
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <fstream>

void sqf::filesystem::addPathMappingInternal(std::filesystem::path virt, std::filesystem::path phy) {
    std::vector<std::string> virtElements;

    for (auto& el : virt) { //Split path into elements
        virtElements.emplace_back(el.string());
    }

    auto found = m_virtualphysicalmap.find(virtElements[0]);
    auto curIter = m_virtualphysicalmap.end();
    bool first = true;
    for (auto& it : virtElements) {
        if (first) { //first element
            first = false; //this is ugly. But comparing iterators doesn't work
            curIter = m_virtualphysicalmap.find(it);
            if (curIter == m_virtualphysicalmap.end())
                curIter = m_virtualphysicalmap.insert({ it, pathElement{} }).first;
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

    for (auto& el : virt) { //Split path into elements
        virtElements.emplace_back(el.string());
    }

    if (virtElements.front() == "\\") //We already know it's a global path. We don't want starting backslash
        virtElements.erase(virtElements.begin());

    std::vector<std::map<std::string, pathElement>::iterator> pathStack; //In case we need to walk back upwards

    auto curIter = m_virtualphysicalmap.end();
    bool first = true;
    for (auto& it : virtElements) {
        if (first) { //first element needs special handling as it comes directly from the map
            first = false; //this is ugly. But comparing iterators doesn't work
            curIter = m_virtualphysicalmap.find(it);
            if (curIter == m_virtualphysicalmap.end())
                return {}; //if we didn't find the starting element, we won't find any of the next elements either
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
    if (virt.front() != '\\') { //It's a local path
        auto parentDirectory = std::filesystem::path(current).parent_path(); //Get parent of current file
        auto wantedFile = (parentDirectory / virt).lexically_normal();

        if (virt.find("..") != std::string::npos) {//need to check against physical boundary
            //#TODO implement this as a tree lookup
            auto found = std::find_if(m_physicalboundaries.begin(), m_physicalboundaries.end(), [search = wantedFile.string()](std::string_view it) -> bool {
                return search.find(it) != std::string::npos;
            });

            if (found == m_physicalboundaries.end())
                return {}; //boundary violation
        }

        if (std::filesystem::exists(wantedFile)) return wantedFile.string();
        return {}; //file doesn't exist
    } else { //global path
        auto resolved = resolvePath(virt);
        if (resolved) {
            if (std::filesystem::exists(*resolved)) return resolved->string();
            return {}; //file doesn't exist
        }
        return {}; //can't resolve file
    }
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
    addPathMappingInternal(virt, phys);
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