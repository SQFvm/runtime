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

// FILESYSTEM_DISABLE_DISALLOW may be used to fully disable
// the disallow functionality.
// Note that the disallow functionallity is a workaround for a bug



namespace sqf
{
	class filesystem
	{
	private:

        struct pathElement {
            std::map<std::string, pathElement> subPaths;
            std::optional<std::filesystem::path> physicalPath;
        };
#if !defined(FILESYSTEM_DISABLE_DISALLOW)
		// ToDo: Fix physical path getting returned when navigating out of
		// allowed range. Occured on Discord-Bot with libsqfvm on debian-linux.
		bool mdisallow = false;
#endif
	public:
        void addPathMappingInternal(std::filesystem::path virt, std::filesystem::path phy);
        std::optional<std::filesystem::path> resolvePath(std::filesystem::path virt);


		// Left -> Virtual
		// Right -> Physical
        std::map<std::string, pathElement> m_virtualphysicalmap;
		std::vector<std::string> m_physicalboundaries;
		std::vector<std::string> m_virtualpaths;
	public:
		// Attempts to receive a physical path from provided virtual path.
		// Will throw a runtime_error if the requested path is not allowed
		// to be received.
		std::string get_physical_path(std::string virt, std::string current = "")
		{
#if !defined(FILESYSTEM_DISABLE_DISALLOW)
			if (mdisallow)
			{
				throw std::runtime_error("Not Allowed");
			}
#endif
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


#if !defined(FILESYSTEM_DISABLE_DISALLOW)
		/// Allows to change wether or not the filesystem can be used.
		void disallow(bool flag) { mdisallow = flag; }
#endif

		static std::string sanitize(std::string input) { return input; }
	};
}