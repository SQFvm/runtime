#pragma once
#include <map>
#include <string>
#include <vector>
#include <stdexcept>

namespace sqf
{
	class filesystem
	{
	private:
		// Left -> Virtual
		// Right -> Physical
		std::map<std::string, std::string> m_virtualphysicalmap;
		std::vector<std::string> m_physicalboundaries;
		std::vector<std::string> m_virtualpaths;

		static std::string sanitize(std::string input);
		static std::string up(std::string input)
		{
			size_t index = input.rfind('/');
			if (index == std::string::npos)
			{
				return std::string();
			}
			return input.substr(0, index);
		}
		static std::string down(std::string input, std::string navigator);
	public:
		// Attempts to receive a physical path from provided virtual path.
		// Will throw a runtime_error if the requested path is not allowed
		// to be received.
		std::string get_physical_path(std::string virt)
		{
			std::string s;
			if (try_get_physical_path(virt, s))
			{
				return s;
			}
			else
			{
				throw std::runtime_error("File Not Found");
			}
		}
		// Attempts to receive a physical path from provided virtual path.
		// Will return true if the path was received successfully.
		// Returns false (and does not modifies second param) if not.
		bool try_get_physical_path(std::string virt, std::string& out);
		// Adds a physical path to the allowed list.
		void add_allowed_physical(std::string phys);
		// Adds a mapping of a virtual path to a physical one.
		void add_mapping(std::string virt, std::string phys);

	};
}