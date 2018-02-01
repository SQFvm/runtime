#ifndef _COMMANDMAP
#define _COMMANDMAP 1

namespace sqf
{
	class commandmap
	{
	private:
		std::unordered_map<std::wstring, std::shared_ptr<nularcmd>> mnularcmd;
		std::unordered_map<std::wstring, std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>>> munarycmd;
		std::unordered_map<std::wstring, std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>>> mbinarycmd;
		void initmath(void);
		void initnamespaces(void);
		void initgenericops(void);
		void initdiagops(void);
		void initsqfvmcmds(void);
	public:
		inline void init(void)
		{
			initmath();
			initnamespaces();
			initgenericops();
			initdiagops();
			initsqfvmcmds();
		}





		commandmap() {}
		void add(std::shared_ptr<nularcmd> cmd) { mnularcmd[cmd->name()] = cmd; }
		void add(std::shared_ptr<unarycmd> cmd)
		{
			auto listsptr = munarycmd[cmd->name()];
			if (!listsptr.get())
			{
				listsptr = munarycmd[cmd->name()] = std::make_shared<std::vector<std::shared_ptr<unarycmd>>>();
			}
			listsptr->push_back(cmd);
		}
		void add(std::shared_ptr<binarycmd> cmd)
		{
			auto listsptr = mbinarycmd[cmd->name()];
			if (!listsptr.get())
			{
				listsptr = mbinarycmd[cmd->name()] = std::make_shared<std::vector<std::shared_ptr<binarycmd>>>();
			}
			listsptr->push_back(cmd);
		}
		std::shared_ptr<nularcmd> get(std::wstring str) { return mnularcmd[str]; }
		std::shared_ptr<unarycmd> get(std::wstring str, type rtype);
		std::shared_ptr<binarycmd> get(std::wstring str, type ltype, type rtype);

		std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> getrange_u(std::wstring str) { return munarycmd[str]; }
		std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> getrange_b(std::wstring str) { return mbinarycmd[str]; }

		static std::shared_ptr<unarycmd> find(std::shared_ptr<std::vector<std::shared_ptr<unarycmd>>> list, type rtype) {
			for each (auto it in *list)
			{
				if (it->matches(type::NA, rtype))
				{
					return it;
				}
			}
			return std::shared_ptr<unarycmd>();
		}
		static std::shared_ptr<binarycmd> find(std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> list, type ltype, type rtype) {
			for each (auto it in *list)
			{
				if (it->matches(ltype, rtype))
				{
					return it;
				}
			}
			return std::shared_ptr<binarycmd>();
		}

		bool contains_n(std::wstring name) { return mnularcmd.find(name) != mnularcmd.end(); }
		bool contains_u(std::wstring name) { return munarycmd.find(name) != munarycmd.end(); }
		bool contains_b(std::wstring name) { return mbinarycmd.find(name) != mbinarycmd.end(); }


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

#endif // !_COMMANDMAP
