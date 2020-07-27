#include "default.h"
#include "../runtime/util.h"

#include <algorithm>
#include <filesystem>
#include <sstream>
#include <fstream>

using namespace std::literals::string_literals;

template<char delimiter>
class StringDelimiter : public std::string
{ };
template<char delimiter>
std::istream & operator>>(std::istream & is, StringDelimiter<delimiter>& output)
{
    std::getline(is, output, delimiter);
    return is;
}

inline bool file_exists(std::filesystem::path p)
{
    std::ifstream infile(p.string());
    return infile.good();
}

std::optional<sqf::runtime::fileio::pathinfo> sqf::fileio::default::get_info(std::string_view viewVirtual, sqf::runtime::fileio::pathinfo current) const
{
    // Create & Cleanse stuff
    auto virt = std::string(viewVirtual);
    std::replace(virt.begin(), virt.end(), '\\', '/');
    virt = std::string(sqf::runtime::util::trim(virt));
    std::string virtFull = virt;

    // Abort conditions
    if (virt.empty()) { return {}; }

    // Prepare local tree-node list
    std::vector<const path_element*> nodes;
    nodes.push_back(&m_virtual_file_root);

    if (viewVirtual[0] != '/')
    { // Navigate current virtual path if relative

        // Iterate over the whole existing virtual path
        if (!current.virtual_.empty())
        {
            virtFull = current.virtual_ + "/" + virt;

            std::istringstream stream_virt(current.virtual_);
            for (auto it = std::istream_iterator<StringDelimiter<'/'>>{ stream_virt }; it != std::istream_iterator<StringDelimiter<'/'>>{}; ++it)
            {
                if (it->empty()) { /* skip empty */ continue; }
                if (nodes.back()->next.find(*it) == nodes.back()->next.end()) { /* Dead-End. File Not Found. */ return {}; }
                nodes.push_back(&(nodes.back()->next.at(*it)));
            }
        }
    }

    // Explore further until we hit dead-end
    {
        std::istringstream stream_virt(virt);
        auto it = std::istream_iterator<StringDelimiter<'/'>>{ stream_virt };
        for (; it != std::istream_iterator<StringDelimiter<'/'>>{}; ++it)
        {
            if (it->empty()) { /* skip empty */ continue; }
            if (*it == ".."s && !nodes.empty())
            {
                // Move dir-up
                nodes.pop_back();
            }
            else
            {
                if (nodes.back()->next.find(*it) == nodes.back()->next.end()) { /* Dead-End. */ break; }
                nodes.push_back(&(nodes.back()->next.at(*it)));
            }
        }

        // Invalid path from our perspective. Return File-Not-Found.
        if (nodes.empty()) { return {}; }

        // Set virtual to remaining and ensure no further dir-up occur
        virt.clear();
        for (; it != std::istream_iterator<StringDelimiter<'/'>>{}; ++it)
        {
            if (*it == ".."s) { /* skip dir-up */ continue; }
            virt.append(*it);
            virt.append("/");
        }
    }

    // Check every physical path in current tree_element if the file exists
    for (auto& phys : nodes.back()->physical)
    {
        std::filesystem::path p(phys);
        p /= virt;
        if (file_exists(p))
        {
            return { { p.string(), virtFull } };
        }
    }

    // As we reached this, file-not-found
    return {};
}

void sqf::fileio::default::add_mapping(std::string_view viewPhysical, std::string_view viewVirtual)
{
    // Create & Cleanse stuff
    auto phys = std::string(viewPhysical);
    std::replace(phys.begin(), phys.end(), '\\', '/');
    auto path_phys = std::filesystem::path(phys);

    auto virt = std::string(viewVirtual);
    std::replace(virt.begin(), virt.end(), '\\', '/');

    // Iterate over the whole virtual path and add missing elements to the file_tree
    std::istringstream stream_virt(virt);
    path_element* tree = &m_virtual_file_root;
    for (auto it = std::istream_iterator<StringDelimiter<'/'>>{ stream_virt }; it != std::istream_iterator<StringDelimiter<'/'>>{}; ++it)
    {
        if (it->empty()) { /* skip empty */ continue; }
        tree = &(tree->next[*it]);
    }

    // Add physical path to final tree node
    tree->physical.push_back(phys);
}

std::string sqf::fileio::default::read_file(sqf::runtime::fileio::pathinfo info) const
{
    auto res = sqf::runtime::fileio::read_file_from_disk(info.physical);
    return *res;
}
