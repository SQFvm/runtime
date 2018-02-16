#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <algorithm>
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
		std::unordered_map<std::wstring, std::shared_ptr<nularcmd>> mnularcmd;
		std::unordered_map<std::wstring, std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>>> munarycmd;
		std::unordered_map<std::wstring, std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>>> mbinarycmd;
		void initmathcmds(void);
		void initnamespacecmds(void);
		void initgenericcmds(void);
		void initdiagcmdss(void);
		void initsqfvmcmds(void);
		void initlogiccmds(void);
		void initstringcmds(void);
		void initunimplemented(void);
		void initconfigcmds(void);
		void initgroupcmds(void);

		std::wstring tolowerstring(std::wstring s)
		{
			std::transform(s.begin(), s.end(), s.begin(), ::tolower);
			return s;
		}

	public:
		inline void init(void)
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
#ifndef _DEBUG
			initunimplemented();
#endif // !_DEBUG
		}

		commandmap() {}
		void add(std::shared_ptr<nularcmd> cmd);
		void add(std::shared_ptr<unarycmd> cmd);
		void add(std::shared_ptr<binarycmd> cmd);
		std::shared_ptr<nularcmd> get(std::wstring str) { return mnularcmd[tolowerstring(str)]; }
		std::shared_ptr<unarycmd> get(std::wstring str, type rtype);
		std::shared_ptr<binarycmd> get(std::wstring str, type ltype, type rtype);

		std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> getrange_u(std::wstring str) { return munarycmd[tolowerstring(str)]; }
		std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> getrange_b(std::wstring str) { return mbinarycmd[tolowerstring(str)]; }

		static std::shared_ptr<unarycmd> find(std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> list, type rtype);
		static std::shared_ptr<binarycmd> find(std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> list, type ltype, type rtype);

		bool contains_n(std::wstring name) { return mnularcmd.find(tolowerstring(name)) != mnularcmd.end(); }
		bool contains_u(std::wstring name) { return munarycmd.find(tolowerstring(name)) != munarycmd.end(); }
		bool contains_b(std::wstring name) { return mbinarycmd.find(tolowerstring(name)) != mbinarycmd.end(); }


		const std::unordered_map<std::wstring, std::shared_ptr<nularcmd>>& all_n(void) { return mnularcmd; }
		const std::unordered_map<std::wstring, std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>>>&  all_u(void) { return munarycmd; }
		const std::unordered_map<std::wstring, std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>>>&  all_b(void) { return mbinarycmd; }

		static sqf::commandmap& get(void);

		inline void uninit(void)
		{
			get().mnularcmd.clear();
			get().munarycmd.clear();
			get().mbinarycmd.clear();
		}
	};
}
