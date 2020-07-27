#pragma once
#include "../runtime/fileio.h"
#include <unordered_map>
#include <filesystem>
#include <string>
#include <vector>

namespace sqf::fileio
{
	class default : public sqf::runtime::fileio
	{
	private:
		struct path_element
		{
			std::unordered_map<std::string, path_element> next;
			std::vector<std::filesystem::path> physical;
		};
		path_element m_virtual_file_root;
		using file_tree_iterator = std::unordered_map<std::string, path_element>::iterator;
		void get_directories_recursive(std::vector<std::string>& paths, const path_element& el) const
		{
			for (auto& path : el.physical)
			{
				paths.push_back(path.string());
			}
			for (auto& next : el.next)
			{
				get_directories_recursive(paths, next.second);
			}
		}
	public:
#pragma region sqf::runtime::fileio
		virtual std::optional<sqf::runtime::fileio::pathinfo> get_info(std::string_view view, sqf::runtime::fileio::pathinfo current) const override;
		virtual void add_mapping(std::string_view viewPhysical, std::string_view viewVirtual) override;
		virtual std::string read_file(sqf::runtime::fileio::pathinfo info) const override;
		virtual std::vector<std::string> get_directories() const override
		{
			std::vector<std::string> paths;
			get_directories_recursive(paths, m_virtual_file_root);
			return paths;
		}
#pragma endregion

	};
}