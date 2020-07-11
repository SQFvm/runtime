#pragma once
#include <string>
#include <string_view>
#include <optional>

namespace sqf
{
	namespace runtime
	{
		class fileio
		{
		public:
			struct pathinfo
			{
				/// <summary>
				/// The full physical path as found on the OSs filesystem.
				/// </summary>
				std::string physical;

				/// <summary>
				/// Additional informations. Can be used if the filesystem
				/// implementation requires additional info
				/// (eg. physical points to a pbo file and additional to the
				///  file inside the PBO)
				/// </summary>
				std::string additional;

				/// <summary>
				/// The full virtual path of the file.
				/// </summary>
				std::string virtual_;
				pathinfo(std::string_view p, std::string_view v) : pathinfo(p, {}, v) {}
				pathinfo(std::string_view p, std::string_view a, std::string_view v) : pathinfo(std::string(p), std::string(a), std::string(v)) {}
				pathinfo(std::string p, std::string v) : pathinfo(p, {}, v) {}
				pathinfo(std::string p, std::string a, std::string v) : physical(p), additional(a), virtual_(v) {}
			};
		protected:
			/// <summary>
			/// Convenience method to read a file from disk.
			/// </summary>
			/// <param name="physical_path">The physical path of the file</param>
			/// <returns>The contents of the file. Optional will be empty if file does not exist or could not be opened for any other reason.</returns>
			static std::optional<std::string> read_file(std::string_view physical_path);
			/// <summary>
			/// Convenience method to read a file from disk.
			/// </summary>
			/// <param name="physical_path">The physical path of the file</param>
			/// <returns>The contents of the file. Optional will be empty if file does not exist or could not be opened for any other reason.</returns>
			static std::optional<std::string> read_file(std::string physical_path) { return read_file(std::string_view(physical_path)); }
		public:
			virtual ~fileio() = 0;
			/// <summary>
			/// Method to receive path informations of a new path.
			/// </summary>
			/// <param name="view">The new path requested.</param>
			/// <param name="current">Current pathinfo or empty '{}'.</param>
			/// <returns>Optional that is filled when the path resolution was successful</returns>
			virtual std::optional<sqf::runtime::fileio::pathinfo> get_info(std::string_view view, sqf::runtime::fileio::pathinfo current) const = 0;

			/// <summary>
			/// Maps a physical path onto a virtual one.
			/// </summary>
			/// <param name="physical">Physical path to map.</param>
			/// <param name="virtual_">Virtual path to map onto the physical one.</param>
			virtual void add_mapping(std::string_view physical, std::string_view virtual_) = 0;

			/// <summary>
			/// Reads the contents of the file into a string.
			/// </summary>
			/// <param name="info">The pathinfo leading to the file. Can be aquired using `get`.</param>
			/// <returns>The contents of the file.</returns>
			virtual std::string read_file(sqf::runtime::fileio::pathinfo info) const = 0;

			/// <summary>
			/// Recursively scans directory for $PBOPREFIX$ files and adds mappings for them.
			/// </summary>
			/// <param name="physical">Physical path to search inside.</param>
			void add_mapping_auto(std::string_view physical);
		};
	}
	namespace fileio
	{
		class disabled : public sqf::runtime::fileio
		{
		public:
			virtual ~disabled() override {}
			// Inherited via fileio
			virtual std::optional<sqf::runtime::fileio::pathinfo> get_info(std::string_view view, sqf::runtime::fileio::pathinfo current) const override { return {}; }
			virtual void add_mapping(std::string_view physical, std::string_view virtual_) override { }
			virtual std::string read_file(sqf::runtime::fileio::pathinfo info) const override { return {}; }
		};
		class passthrough : public sqf::runtime::fileio
		{
		public:
			virtual ~passthrough() override {}
			virtual std::optional<sqf::runtime::fileio::pathinfo> get_info(std::string_view view, sqf::runtime::fileio::pathinfo current) const override { return { { view, ""} }; }
			virtual void add_mapping(std::string_view physical, std::string_view virtual_) override { }
			virtual std::string read_file(sqf::runtime::fileio::pathinfo info) const override { auto opt = fileio::read_file(info.physical); return opt.has_value() ? opt.value() : std::string{}; }
		};
	}
}