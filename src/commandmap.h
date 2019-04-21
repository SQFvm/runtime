#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <cmath>
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

        static std::string tolowerstring(std::string_view s)
		{
            std::string ret(s);
			std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
			return ret;
		}

        static std::string& tolowerstring_ref(std::string& s)
        {
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
            return s;
        }

	public:
		void init()
		{
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
			initunimplemented();
		}

		commandmap() {}
		void add(std::shared_ptr<nularcmd> cmd);
		void add(std::shared_ptr<unarycmd> cmd);
		void add(std::shared_ptr<binarycmd> cmd);
		std::shared_ptr<nularcmd> get(std::string str) { return mnularcmd[tolowerstring(str)]; }
		std::shared_ptr<unarycmd> get(std::string str, type rtype);
		std::shared_ptr<binarycmd> get(std::string str, type ltype, type rtype);

		std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> getrange_u(std::string_view str) { return munarycmd[tolowerstring(str)]; }
		std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> getrange_b(std::string_view str) { return mbinarycmd[tolowerstring(str)]; }

		static std::shared_ptr<unarycmd> find(std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> list, type rtype);
		static std::shared_ptr<binarycmd> find(std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> list, type ltype, type rtype);

		bool contains_n(std::string_view name) { return mnularcmd.find(tolowerstring(name)) != mnularcmd.end(); }
		bool contains_u(std::string_view name) { return munarycmd.find(tolowerstring(name)) != munarycmd.end(); }
		bool contains_b(std::string_view name) { return mbinarycmd.find(tolowerstring(name)) != mbinarycmd.end(); }


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
