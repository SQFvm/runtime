#ifndef _CMD
#define _CMD 1

namespace sqf
{
	class cmd
	{
	private:
		type mltype;
		type mrtype;
		std::wstring mdesc;
		std::wstring mname;

	public:
		cmd(std::wstring name, type ltype, type rtype, std::wstring description) { mname = name; mltype = ltype; mrtype = rtype; mdesc = description; }
		virtual value_s execute(const virtualmachine*, vmstack_s, value_s left, value_s right) const = 0;
		bool matches(type ltype, type rtype) { return mltype == ltype && mrtype == rtype; }
	};


	class nullar : public cmd
	{
	protected:
		virtual value_s exec(const virtualmachine*, vmstack_s) const = 0;
	public:
		nullar(std::wstring name, std::wstring description) : cmd(name, type::NA, type::NA, description) {}
		virtual value_s execute(const virtualmachine* vm, vmstack_s s, value_s left, value_s right) const { return exec(vm, s); }
	};
	class unary : public cmd
	{
	protected:
		virtual value_s exec(const virtualmachine*, vmstack_s, value_s right) const = 0;
	public:
		unary(std::wstring name, type rtype, std::wstring description) : cmd(name, type::NA, rtype, description) {}
		virtual value_s execute(const virtualmachine* vm, vmstack_s s, value_s left, value_s right) const { return exec(vm, s, right); }
	};
	class binary : public cmd
	{
	protected:
		virtual value_s exec(const virtualmachine*, vmstack_s, value_s left, value_s right) const = 0;
	public:
		binary(std::wstring name, type ltype, type rtype, std::wstring description) : cmd(name, ltype, rtype, description) {}
		virtual value_s execute(const virtualmachine* vm, vmstack_s s, value_s left, value_s right) const { return exec(vm, s, left, right); }
	};


	typedef std::shared_ptr<cmd> cmd_s;
	typedef std::weak_ptr<cmd> cmd_w;
	typedef std::unique_ptr<cmd> cmd_u;
}

#endif // !_CMD
