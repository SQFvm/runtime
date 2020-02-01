#pragma once
#include <vector>
#include <string>
#include <string_view>
namespace sqf
{
	class virtualmachine;
	namespace parse
	{
		namespace preprocessor
		{
			class finfo;
			class macro {
			public:
				std::string name;
				std::string content;
				std::vector<std::string> args;
				std::string filepath;
				size_t line;
				size_t column;
				bool hasargs;
				// Special method pointer that may be filled
				// to give this macro a special behavior rather
				// then a content.
				// Gets only applied if pointer is != nullptr
				// m -> the original macro
				// local_fileinfo -> the location, where the macro is called locally (lowest level)
				// original_fileinfo -> the most upper file in the macro chain
				// params -> the passed parameters
				std::string(*callback)(
					const macro& m,
					const finfo* local_fileinfo,
					const finfo* original_fileinfo,
					const std::vector<std::string>& params,
					sqf::virtualmachine* vm);

				macro(std::string_view name, std::string_view filepath) : macro()
				{
					this->name = name;
				}
				macro(std::string_view name, std::string_view content, std::string_view filepath) : macro()
				{
					this->content = content;
				}
				macro() : name(), content(), args(), filepath(), line(0), column(0), hasargs(false), callback(nullptr) {}
			};
		}
	}
}