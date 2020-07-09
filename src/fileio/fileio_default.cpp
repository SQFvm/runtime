#include "fileio_default.h"

using namespace std::literals::string_literals;

void sqf::filesystem::fileio_default::add_path_mapping_internal(std::filesystem::path virt, std::filesystem::path phy)
{
    std::vector<std::string> virtElements;

    for (auto& el : virt) { //Split path into elements
        if (el.string().empty())
        {
            continue;
        }
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

std::optional<sqf::runtime::fileio::pathinfo> sqf::filesystem::fileio_default::resolve_virtual(std::string_view virtual_) const
{
    std::filesystem::path virt(virtual_);
    std::vector<std::string> virtElements;

    for (auto& el : virt)
    { // Split path into elements
        virtElements.emplace_back(el.string());
    }

    // We already know it's a global path. We don't want starting backslash
    if (virtElements.front() == "\\" || virtElements.front() == "/")
    {
        virtElements.erase(virtElements.begin());
    }

    // In case we need to walk back upwards
    std::vector<std::unordered_map<std::string, pathElement>::iterator> pathStack;

    auto curIter = m_virtualphysicalmap.end();
    bool first = true;
    for (auto& it : virtElements)
    {
        if (first)
        { // first element needs special handling as it comes directly from the map
            first = false; // this is ugly. But comparing iterators doesn't work
            curIter = m_virtualphysicalmap.find(it);
            if (curIter == m_virtualphysicalmap.end())
            { // if we didn't find the starting element, we won't find any of the next elements either
                return {};
            }
            pathStack.emplace_back(curIter);
            continue;
        }
        auto& curEl = curIter->second;
        curIter = curEl.subPaths.find(it);
        if (curIter == curEl.subPaths.end())
        { // not found
            break;
        }
        pathStack.emplace_back(curIter);
    }

    // Remove elements we resolved to get a list of leftovers
    virtElements.erase(virtElements.begin(), virtElements.begin() + pathStack.size());


    // walk up stack until we find a phys path.
    while (!pathStack.back()->second.physicalPath) {
        virtElements.emplace_back(pathStack.back()->first);
        pathStack.pop_back();
        if (pathStack.empty())
        { // whole stack didn't have physical path
            return {};
        }
    }

    //build full path to file
    auto mapping = pathStack.back();
    auto curPath = *mapping->second.physicalPath;
    for (auto& it : virtElements)
    {
        curPath = curPath / it;
    }
    sqf::runtime::fileio::pathinfo info(curPath.string(), mapping->first);
    return info;
}

std::optional<sqf::runtime::fileio::pathinfo> sqf::filesystem::fileio_default::get_info(std::string_view virt, sqf::runtime::fileio::pathinfo current) const
{
    if (virt.empty())
    {
        return {};
    }
    std::string virtMapping;
    if (virt.front() != '\\' && virt.front() != '/') { // It's a local path
    local_path:
        auto parentDirectory = std::filesystem::path(current.physical).parent_path(); // Get parent of current file
        auto wantedFile = (parentDirectory / virt).lexically_normal();

        if (virt.find("..") != std::string::npos)
        { // need to check against physical boundary
            // #TODO implement this as a tree lookup
            auto found = std::find_if(m_physicalboundaries.begin(), m_physicalboundaries.end(),
                [search = wantedFile.string()](std::string_view it) -> bool
            {
                return search.find(it) != std::string::npos;
            });

            if (found == m_physicalboundaries.end())
            { // boundary violation
                return {};
            }
        }

        if (std::filesystem::exists(wantedFile))
        {
            auto absolute = std::filesystem::absolute(wantedFile);
            sqf::runtime::fileio::pathinfo info(absolute.string(), current.virtual_.empty() ? ""s : (current.virtual_ + wantedFile.string()));
            return info;
        }

        // file doesn't exist
        return {};
    }
    else 
   { // global path
        auto resolved = resolve_virtual(virt);
        if (resolved)
        {
            if (resolved.has_value())
            {
                auto info = *resolved;
                auto absolute = std::filesystem::absolute(info.physical);
                info.physical = absolute.string();
                return info;
            }
            return {}; // file doesn't exist
        }
        else
        {
            goto local_path; // Handle normally
        }
    }
}

void sqf::filesystem::fileio_default::add_mapping(std::string_view physical, std::string_view virtual_)
{
    if (!virtual_.empty() && (virtual_.front() == '/' || virtual_.front() == '\\'))
    {
        virtual_ = virtual_.substr(1);
    }
    if (!virtual_.empty() && (virtual_.back() == '/' || virtual_.back() == '\\'))
    {
        virtual_ = virtual_.substr(0, virtual_.length() - 1);
    }
    std::string s_physical(physical);
    std::string s_virtual(virtual_);
    m_physicalboundaries.push_back(s_physical);
    m_virtualpaths.push_back(s_virtual);
    add_path_mapping_internal(s_virtual, s_physical);
}

std::string sqf::filesystem::fileio_default::read_file(sqf::runtime::fileio::pathinfo info) const
{
    auto res = sqf::runtime::fileio::read_file(info.physical);
    return *res;
}
