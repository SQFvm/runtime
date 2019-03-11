#pragma once
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <optional>
#include <filesystem>

#if defined(_WIN32) || defined(_WIN64)
#define FSDELIMITER '\\'
#define FSDELIMITER_STR "\\"
#else
#define FSDELIMITER '/'
#define FSDELIMITER_STR "/"
#endif

namespace sqf
{
	class filesystem
	{
	private:

        struct pathElement {
            std::map<std::string, pathElement> subPaths;
            std::optional<std::filesystem::path> physicalPath;
        };
	public:
        void addPathMappingInternal(std::filesystem::path virt, std::filesystem::path phy);
        std::optional<std::filesystem::path> resolvePath(std::filesystem::path virt);


		// Left -> Virtual
		// Right -> Physical
        std::map<std::string, pathElement> m_virtualphysicalmapNew;
        std::map<std::string, std::string> m_virtualphysicalmap;
		std::vector<std::string> m_physicalboundaries;
		std::vector<std::string> m_virtualpaths;
	public:
		// Attempts to receive a physical path from provided virtual path.
		// Will throw a runtime_error if the requested path is not allowed
		// to be received.
		std::string get_physical_path(std::string virt, std::string current = "")
		{
			auto val = try_get_physical_path(virt, current);
			if (val.has_value())
			{
				return val.value();
			}
			else
			{
				throw std::runtime_error("File Not Found");
			}
		}
		// Attempts to receive a physical path from provided virtual path.
		// Will return true if the path was received successfully.
		// Returns false (and does not modifies second param) if not.
		std::optional<std::string> try_get_physical_path(std::string virt, std::string current = "");
		// Adds a physical path to the allowed list.
		void add_allowed_physical(std::string phys);
		// Adds a mapping of a virtual path to a physical one.
		void add_mapping(std::string virt, std::string phys);
        // Recursively scans directory for $PBOPREFIX$ files and adds mappings for them.
        void add_mapping_auto(std::string phys);

		static std::string sanitize(std::string input);
		static std::string up(std::string input)
		{
			size_t index = input.rfind(FSDELIMITER);
			if (index == std::string::npos)
			{
				return std::string();
			}
			return input.substr(0, index);
		}
		static std::string down(std::string input, std::string navigator);
		static std::string navigate(std::string input, std::string navigator);
	};
}