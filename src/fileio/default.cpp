#include "default.h"
#include "../runtime/util.h"

#include <algorithm>
#include <filesystem>
#include <sstream>
#include <fstream>

#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
#include <iostream>
#endif // DF__SQF_FILEIO__TRACE_REESOLVE

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

std::optional<sqf::runtime::fileio::pathinfo> sqf::fileio::default::get_info_virtual(std::string_view viewVirtual, sqf::runtime::fileio::pathinfo current) const
{
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
    std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
        "        " <<
        "        " <<
        "    " << "\x1B[36mget_info_virtual\033[0m(\x1B[90m" << viewVirtual << "\033[0m, \x1B[90m{" << current.physical << ", " << current.virtual_ << "}\033[0m) requested" << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE

    // Create & Cleanse stuff
    auto virt = std::string(viewVirtual);
    std::replace(virt.begin(), virt.end(), '\\', '/');
    virt = std::string(sqf::runtime::util::trim(virt));
    std::string virtFull = virt;

    // Abort conditions
    if (virt.empty())
    {
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
        std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
            "        " <<
            "        " <<
            "    " << "\x1B[36mget_info_virtual\033[0m" <<
            "    " << "provided virtual path is empty. \x1B[31mReturning FileNotFound\033[0m." << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
        return {};
    }

    // Prepare local tree-node list
    std::vector<std::shared_ptr<path_element>> nodes;
    nodes.push_back(m_virtual_file_root);

#if WIN32
    if (viewVirtual[0] != '/' && !(viewVirtual.length() >= 2 && viewVirtual[1] == ':'))
#else
    if (viewVirtual[0] != '/')
#endif
    { // Navigate current virtual path if relative

        // Iterate over the whole existing virtual path
        if (!current.virtual_.empty())
        {
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
            std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mget_info_virtual\033[0m" <<
                "    " << "Navigating Relative-Path provided:" << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE

            virtFull = current.virtual_ + "/" + virt;

            std::istringstream stream_virt(current.virtual_);
            for (auto it = std::istream_iterator<StringDelimiter<'/'>>{ stream_virt }; it != std::istream_iterator<StringDelimiter<'/'>>{}; ++it)
            {
                if (it->empty())
                { /* skip empty */
                    continue;
                }
                if (nodes.back()->next.find(*it) != nodes.back()->next.end())
                {
                    nodes.push_back(nodes.back()->next.at(*it));
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
                    std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
                        "        " <<
                        "        " <<
                        "    " << "\x1B[36mget_info_virtual\033[0m" <<
                        "    " << "    " << "Navigated '" << *it << "'." << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
                }
                else
                { /* Dead-End. File Not Found. */
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
                    std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
                        "        " <<
                        "        " <<
                        "    " << "\x1B[36mget_info_virtual\033[0m" <<
                        "    " << "    " << "Reached dead end with '" << *it << "'. \x1B[31mReturning FileNotFound\033[0m." << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
                    return {};
                }
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
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
                std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
                    "        " <<
                    "        " <<
                    "    " << "\x1B[36mget_info_virtual\033[0m" <<
                    "    " << "    " << "Moved dir up." << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
            }
            else
            {
                if (nodes.back()->next.find(*it) == nodes.back()->next.end())
                { /* Dead-End.  */
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
                    std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
                        "        " <<
                        "        " <<
                        "    " << "\x1B[36mget_info_virtual\033[0m" <<
                        "    " << "    " << "Reached dead end while exploring path on '" << *it << "'." << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
                    break;
                }
                else
                {
                    nodes.push_back(nodes.back()->next.at(*it));
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
                    std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
                        "        " <<
                        "        " <<
                        "    " << "\x1B[36mget_info_virtual\033[0m" <<
                        "    " << "    " << "Navigated '" << *it << "'." << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
                }
            }
        }

        
        if (nodes.empty())
        { /* Invalid path from our perspective. Return File-Not-Found. */

#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
            std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mget_info_virtual\033[0m" <<
                "    " << "    " << "No navigation nodes found. \x1B[31mReturning FileNotFound\033[0m." << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
            return {};
        }

        // Set virtual to remaining and ensure no further dir-up occur
        virt.clear();
        for (; it != std::istream_iterator<StringDelimiter<'/'>>{}; ++it)
        {
            if (*it == ".."s) { /* skip dir-up */ continue; }
            virt.append("/");
            virt.append(*it);
        }
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
        std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
            "        " <<
            "        " <<
            "    " << "\x1B[36mget_info_virtual\033[0m" <<
            "    " << "    " << "Built remainer '" << virt << "'." << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
        return {};
    }

    // Check every physical path in current tree_element if the file exists
    for (auto& phys : nodes.back()->physical)
    {
        std::filesystem::path p(phys);
        p /= virt;
        if (file_exists(p))
        {
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
            std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mget_info_virtual\033[0m" <<
                "    " << "    " << "Match!. \x1B[32mReturning " << p.string() << "\033[0m." << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
            return { { p.string(), virtFull } };
        }
    }

#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
    std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
        "        " <<
        "        " <<
        "    " << "\x1B[36mget_info_virtual\033[0m" <<
        "    " << "    " << "No matching file found. \x1B[31mReturning FileNotFound\033[0m." << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
    // As we reached this, file-not-found
    return {};
}
std::optional<sqf::runtime::fileio::pathinfo> sqf::fileio::default::get_info_physical(std::string_view viewVirtual, sqf::runtime::fileio::pathinfo current) const
{
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
    std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
        "        " <<
        "        " <<
        "    " << "\x1B[36mget_info_physical\033[0m" <<
        "    " << "    " << "Attempting to find physical path for '" << viewVirtual << "'" << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
    for (auto& it : m_path_elements)
    {
        for (auto& phys : it->physical)
        {
            auto str = phys.string();
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
            std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mget_info_physical\033[0m" <<
                "    " << "    " << "Comparing against '" << str << "'" << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
            if (viewVirtual.size() >= str.size() && viewVirtual.substr(0, str.size()) == str)
            {
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
                std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
                    "        " <<
                    "        " <<
                    "    " << "\x1B[36mget_info_physical\033[0m" <<
                    "    " << "    " << "Match! Passing to get_info_virtual" << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
                auto res = get_info_virtual(it->virtual_full + std::string(viewVirtual.substr(str.size())), current);
                if (res.has_value())
                {
                    return res;
                }
            }
        }
    }
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
    std::shared_ptr<path_element> tree = m_virtual_file_root;
    std::vector<std::string> path_elements;
    for (auto it = std::istream_iterator<StringDelimiter<'/'>>{ stream_virt }; it != std::istream_iterator<StringDelimiter<'/'>>{}; ++it)
    {
        if (it->empty()) { /* skip empty */ continue; }
        path_elements.push_back(*it);
        auto res = tree->next.find(*it);
        if (res == tree->next.end())
        {
            std::stringstream sstream;
            for (auto& el : path_elements)
            {
                sstream << "/" << el;
            }
            tree = tree->next[*it] = std::make_shared<path_element>();
            tree->virtual_full = sstream.str();
            m_path_elements.push_back(tree);
        }
        else
        {
            tree = res->second;
        }
    }

    // Add physical path to final tree node
    tree->physical.push_back(phys);
}

std::string sqf::fileio::default::read_file(sqf::runtime::fileio::pathinfo info) const
{
    auto res = sqf::runtime::fileio::read_file_from_disk(info.physical);
    return *res;
}
