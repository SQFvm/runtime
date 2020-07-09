#pragma once
#include "../runtime/fileio.h"
#include <unordered_map>
#include <filesystem>
#include <string>

namespace sqf::filesystem
{
	class fileio_default : public sqf::runtime::fileio
	{
	private:
		struct pathElement
		{
			std::unordered_map<std::string, pathElement> subPaths;
			std::optional<std::filesystem::path> physicalPath;
		};
		// Left -> Virtual
		// Right -> Physical
		std::unordered_map<std::string, pathElement> m_virtualphysicalmap;
		std::vector<std::string> m_physicalboundaries;
		std::vector<std::string> m_virtualpaths;
		void add_path_mapping_internal(std::filesystem::path virt, std::filesystem::path phy);
	public:
		std::optional<sqf::runtime::fileio::pathinfo> resolve_virtual(std::string_view virtual_) const;
		void add_allowed_physical(std::string_view physical) { m_physicalboundaries.push_back(std::string(physical)); }

#pragma region sqf::runtime::fileio
		virtual std::optional<sqf::runtime::fileio::pathinfo> get_info(std::string_view view, sqf::runtime::fileio::pathinfo current) const override;
		virtual void add_mapping(std::string_view physical, std::string_view virtual_) override;
		virtual std::string read_file(sqf::runtime::fileio::pathinfo info) const override;
#pragma endregion

	};
}