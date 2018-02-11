#ifndef _SQFVM
#define _SQFVM 1

#if !defined(_TYPEINFO) & !defined(_TYPEINFO_)
#error virtualmachine requires <typeinfo> header
#endif // !_TYPEINFO
#if !defined(_STRING) & !defined(_STRING_)
#error virtualmachine requires <string> header
#endif // !_STRING
#if !defined(_VECTOR) & !defined(_VECTOR_)
#error virtualmachine requires <vector> header
#endif // !_VECTOR
#if !defined(_MEMORY) & !defined(_MEMORY_)
#error virtualmachine requires <memory> header
#endif // !_MEMORY
#if !defined(_OSTREAM) & !defined(_OSTREAM_)
#error virtualmachine requires <ostream> header
#endif // !_OSTREAM

#ifndef _VALUE
#error virtualmachine requires "value.h" header
#endif // !_VALUE
#ifndef _INSTRUCTION
#error virtualmachine requires "instruction.h" header
#endif // !_INSTRUCTION
#ifndef _VMSTACK
#error virtualmachine requires "vmstack.h" header
#endif // !_VMSTACK

namespace sqf
{
	class virtualmachine
	{
	private:
		unsigned long long minstcount;
		unsigned long long mmaxinst;
		vmstack_s mstack;
		std::wostream* mout;
		std::wostream* merr;
		std::wostream* mwrn;
		bool merrflag;
		bool mwrnflag;
	public:
		std::wostream& out(void) const { return *mout; }
		std::wostream& err(void) const { /* on purpose */((virtualmachine*)this)->merrflag = true; return *merr; }
		std::wostream& wrn(void) const { /* on purpose */((virtualmachine*)this)->mwrnflag = true; return *mwrn; }
		virtualmachine() : virtualmachine(0) {};
		virtualmachine(unsigned long long maxinst);
		void execute(void);
		inline vmstack_s stack(void) const { return mstack; }
		static std::wstring dbgsegment(const wchar_t* full, size_t off, size_t length);

		void parse_assembly(std::wstring);
		inline void parse_sqf(std::wstring code) { parse_sqf(code, callstack_s()); }
		void parse_sqf(std::wstring, callstack_s) const;
		void parse_config(std::wstring, std::shared_ptr<configdata>) const;
		bool errflag(void) const { return merrflag; }
		bool wrnflag(void) const { return mwrnflag; }
	};
	typedef std::shared_ptr<virtualmachine> virtualmachine_s;
	typedef std::weak_ptr<virtualmachine> virtualmachine_w;
	typedef std::unique_ptr<virtualmachine> virtualmachine_u;
}
#endif // !_SQFVM