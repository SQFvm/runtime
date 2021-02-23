#pragma once
#include "../runtime/fileio.h"
#include "../runtime/logging.h"
#include "../rvutils/pbofile.hpp"
#include <unordered_map>
#include <filesystem>
#include <string>
#include <vector>
#include <memory>

namespace sqf::fileio
{
    class impl_default : public sqf::runtime::fileio, public CanLog
    {
    private:
        struct path_element
        {
            std::unordered_map<std::string, std::shared_ptr<path_element>> next;
            std::vector<std::filesystem::path> physical;
            std::string virtual_full;
        };
        std::shared_ptr<path_element> m_virtual_file_root;
        using file_tree_iterator = std::unordered_map<std::string, std::shared_ptr<path_element>>::iterator;

        std::vector<std::shared_ptr<path_element>> m_path_elements;
        std::unordered_map<std::string, rvutils::pbo::pbofile> m_pbos;


        void get_directories_recursive(std::vector<std::string>& paths, const std::shared_ptr<path_element>& el) const
        {
            for (auto& path : el->physical)
            {
                paths.push_back(path.string());
            }
            for (auto& next : el->next)
            {
                get_directories_recursive(paths, next.second);
            }
        }
        /// <summary>
        /// Attempts to interpret the view provided as virtual path.
        /// </summary>
        /// <param name="view">The path to lookup as string_view.</param>
        /// <param name="current">The current pathinfo as available</param>
        /// <returns>empty optional on filenotfound or the pathinfo to the actual file.</returns>
        std::optional<sqf::runtime::fileio::pathinfo> get_info_virtual(std::string_view view, sqf::runtime::fileio::pathinfo current) const;
        /// <summary>
        /// Attempts to interpret the view provided as physical path.
        /// </summary>
        /// <param name="view">The path to lookup as string_view.</param>
        /// <param name="current">The current pathinfo as available</param>
        /// <returns>empty optional on filenotfound or the pathinfo to the actual file.</returns>
        std::optional<sqf::runtime::fileio::pathinfo> get_info_physical(std::string_view view, sqf::runtime::fileio::pathinfo current) const;
    public:
        impl_default(Logger& logger) : CanLog(logger),
            m_virtual_file_root(std::make_shared<path_element>()),
            m_path_elements()
        {
            m_virtual_file_root->virtual_full = "/";
            m_path_elements.push_back(m_virtual_file_root);
        }
        void add_pbo_mapping(rvutils::pbo::pbofile& pbo);
        void add_pbo_mapping(std::filesystem::path p);
        virtual std::optional<sqf::runtime::fileio::pathinfo> get_info(std::string_view view, sqf::runtime::fileio::pathinfo current) const override
        {
            auto res =  get_info_virtual(view, current);
            if (!res.has_value())
            {
                return get_info_physical(view, current);
            }
            return res;
        }
        virtual void add_mapping(std::string_view viewPhysical, std::string_view viewVirtual) override;
        virtual std::string read_file(sqf::runtime::fileio::pathinfo info) const override;
        virtual std::vector<std::string> get_directories() const override
        {
            std::vector<std::string> paths;
            get_directories_recursive(paths, m_virtual_file_root);
            return paths;
        }

    };
}
