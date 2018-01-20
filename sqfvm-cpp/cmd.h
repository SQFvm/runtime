#ifndef _CMD
#define _CMD 1
#include <functional>

namespace sqf
{
	typedef value_s(*nullarcb)(const virtualmachine*);
	typedef value_s(*unarycb)(const virtualmachine*, value_s);
	typedef value_s(*binarycb)(const virtualmachine*, value_s, value_s);
	class cmd
	{
	private:
		type mltype;
		type mrtype;
		std::wstring mdesc;
		std::wstring mname;
		std::wstring mname;
		short mprecedence;

	public:
		cmd(short precedence, std::wstring name, type ltype, type rtype, std::wstring description) { mprecedence = precedence; mname = name; mltype = ltype; mrtype = rtype; mdesc = description; }
		virtual value_s execute(const virtualmachine*, value_s left, value_s right) const = 0;
		bool matches(type ltype, type rtype) { return mltype == ltype && mrtype == rtype; }
		bool matches(std::wstring name, type ltype, type rtype) { return mltype == ltype && mrtype == rtype && wstr_cmpi(mname.c_str(), -1, name.c_str(), -1); }
		std::wstring desc(void) { return mdesc; }
		std::wstring name(void) { return mname; }
	};


	class nullarcmd : public cmd
	{
	private:
		nullarcb mfnc;
	public:
		nullarcmd(short precedence, std::wstring name, std::wstring description, nullarcb fnc) : cmd(precedence, name, type::NA, type::NA, description) { mfnc = fnc; }
		virtual value_s execute(const virtualmachine* vm, value_s left, value_s right) const { return mfnc(vm); }
	};
	class unarycmd : public cmd
	{
	private:
		unarycb mfnc;
	public:
		unarycmd(short precedence, std::wstring name, type rtype, std::wstring description, unarycb fnc) : cmd(precedence, name, type::NA, rtype, description) { mfnc = fnc; }
		virtual value_s execute(const virtualmachine* vm, value_s left, value_s right) const { return mfnc(vm, right); }
	};
	class binarycmd : public cmd
	{
	private:
		binarycb mfnc;
	public:
		binarycmd(short precedence, std::wstring name, type ltype, type rtype, std::wstring description, binarycb fnc) : cmd(precedence, name, ltype, rtype, description) { mfnc = fnc; }
		virtual value_s execute(const virtualmachine* vm, value_s left, value_s right) const { return mfnc(vm, left, right); }
	};

	static std::shared_ptr<nullarcmd> nullar(short precedence, std::wstring name, std::wstring description, nullarcb fnc) { return std::make_shared<nullarcmd>(name, description, fnc); }
	static std::shared_ptr<unarycmd> unary(short precedence, std::wstring name, type rtype, std::wstring description, unarycb fnc) { return std::make_shared<unarycmd>(name, rtype, description, fnc); }
	static std::shared_ptr<binarycmd> binary(short precedence, std::wstring name, type ltype, type rtype, std::wstring description, binarycb fnc) { return std::make_shared<binarycmd>(name, ltype, rtype, description, fnc); }

	class commandmap
	{
	private:
		std::unordered_map<std::wstring, std::shared_ptr<nullarcmd>> mnullarcmd;
		std::unordered_map<std::wstring, std::vector<std::shared_ptr<unarycmd>>> munarycmd;
		std::unordered_map<std::wstring, std::vector<std::shared_ptr<binarycmd>>> mbinarycmd;
	public:
		commandmap() {}
		void add(std::shared_ptr<nullarcmd> cmd) { mnullarcmd[cmd->name()] = cmd; }
		void add(std::shared_ptr<unarycmd> cmd) { auto list = munarycmd[cmd->name()]; list.push_back(cmd); }
		void add(std::shared_ptr<binarycmd> cmd) { auto list = mbinarycmd[cmd->name()]; list.push_back(cmd); }
		std::shared_ptr<nullarcmd> get(std::wstring str) { return mnullarcmd[str]; }
		std::shared_ptr<unarycmd> get(std::wstring str, type rtype)
		{
			auto list = munarycmd[str];
			for each (auto it in list)
			{
				if (it->matches(type::NA, rtype))
				{
					return it;
				}
			}
			return std::shared_ptr<unarycmd>();
		}
		std::shared_ptr<binarycmd> get(std::wstring str, type ltype, type rtype)
		{
			auto list = mbinarycmd[str];
			for each (auto it in list)
			{
				if (it->matches(ltype, rtype))
				{
					return it;
				}
			}
			return std::shared_ptr<binarycmd>();
		}

		std::vector<std::shared_ptr<unarycmd>> getrange_u(std::wstring str) { return munarycmd[str]; }
		std::vector<std::shared_ptr<binarycmd>> getrange_b(std::wstring str) { return mbinarycmd[str]; }

		static std::shared_ptr<unarycmd> find(std::vector<std::shared_ptr<unarycmd>> list, type rtype) {
			for each (auto it in list)
			{
				if (it->matches(type::NA, rtype))
				{
					return it;
				}
			}
			return std::shared_ptr<unarycmd>();
		}
		static std::shared_ptr<binarycmd> find(std::vector<std::shared_ptr<binarycmd>> list, type ltype, type rtype) {
			for each (auto it in list)
			{
				if (it->matches(ltype, rtype))
				{
					return it;
				}
			}
			return std::shared_ptr<binarycmd>();
		}

		bool contains_n(std::wstring name) { return mnullarcmd.find(name) != mnullarcmd.end(); }
		bool contains_u(std::wstring name) { return munarycmd.find(name) != munarycmd.end(); }
		bool contains_b(std::wstring name) { return mbinarycmd.find(name) != mbinarycmd.end(); }

		static sqf::commandmap& get(void);

		void init(void);
		inline void uninit(void)
		{
			get().mnullarcmd.clear();
			get().munarycmd.clear();
			get().mbinarycmd.clear();
		}
	};
	typedef std::shared_ptr<cmd> cmd_s;
	typedef std::weak_ptr<cmd> cmd_w;
	typedef std::unique_ptr<cmd> cmd_u;
}

#endif // !_CMD
