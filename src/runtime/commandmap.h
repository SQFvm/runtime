#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "utility.h"
#include <vector>
#include "type.h"

namespace sqf
{
	class cmd;
	class nularcmd;
	class unarycmd;
	class binarycmd;
	class commandmap
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<nularcmd>> mnularcmd;
		std::unordered_map<std::string, std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>>> munarycmd;
		std::unordered_map<std::string, std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>>> mbinarycmd;
#ifndef NO_COMMANDS
		void initmathcmds();
		void initnamespacecmds();
		void initgenericcmds();
		void initdiagcmdss();
		void initsqfvmcmds();
		void initlogiccmds();
		void initstringcmds();
		void initunimplemented();
		void initconfigcmds();
		void initgroupcmds();
		void initobjectcmds();
		void initmarkercmds();
		void initosspecificcmds();
#endif // !NO_COMMANDS
	public:
		void init()
		{
#ifndef NO_COMMANDS
			initmathcmds();
			initnamespacecmds();
			initgenericcmds();
			initdiagcmdss();
			initsqfvmcmds();
			initlogiccmds();
			initstringcmds();
			initconfigcmds();
			initgroupcmds();
			initobjectcmds();
			initmarkercmds();
			initosspecificcmds();
			initunimplemented();
#endif // !NO_COMMANDS
		}

		commandmap() {}
		void add(std::shared_ptr<nularcmd> cmd);
		void add(std::shared_ptr<unarycmd> cmd);
		void add(std::shared_ptr<binarycmd> cmd);

		bool remove(std::string str);
		bool remove(std::string str, sqf::type rtype);
		bool remove(sqf::type ltype, std::string str, sqf::type rtype);

		std::shared_ptr<nularcmd> get(std::string_view str) { return mnularcmd[string_tolower(str)]; }
		std::shared_ptr<unarycmd> get(std::string_view str, type rtype);
		std::shared_ptr<binarycmd> get(std::string_view str, type ltype, type rtype);

		std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> getrange_u(std::string_view str) { return munarycmd[string_tolower(str)]; }
		std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> getrange_b(std::string_view str) { return mbinarycmd[string_tolower(str)]; }

		static std::shared_ptr<unarycmd> find(std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> list, type rtype);
		static std::shared_ptr<binarycmd> find(std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> list, type ltype, type rtype);

		bool contains_n(std::string_view name) { return mnularcmd.find(string_tolower(name)) != mnularcmd.end(); }
		bool contains_u(std::string_view name) { return munarycmd.find(string_tolower(name)) != munarycmd.end(); }
		bool contains_b(std::string_view name) { return mbinarycmd.find(string_tolower(name)) != mbinarycmd.end(); }


		const auto& all_n() const { return mnularcmd; }
		const auto& all_u() const { return munarycmd; }
		const auto& all_b() const { return mbinarycmd; }

		static sqf::commandmap& get();

		void uninit()
		{
			get().mnularcmd.clear();
			get().munarycmd.clear();
			get().mbinarycmd.clear();
		}
	};
}
