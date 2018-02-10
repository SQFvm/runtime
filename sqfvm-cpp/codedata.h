#ifndef _CODEDATA
#define _CODEDATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error codedata requires <string> header
#endif // !_STRING
#if !defined(_MEMORY) & !defined(_MEMORY_)
#error codedata requires <memory> header
#endif // !_STRING
#if !defined(_VECTOR) & !defined(_VECTOR_)
#error codedata requires <vector> header
#endif // !_VECTOR
#if !defined(_DATA)
#error codedata requires "data.h" header
#endif // !_DATA
#if !defined(_INSTRUCTION)
#error codedata requires "instruction.h" header
#endif // !_DATA

namespace sqf
{
	class codedata : public data
	{
	private:
		std::vector<instruction_s> minsts;
	public:
		codedata(callstack_s);
		codedata(std::vector<instruction_s> v) : minsts(v) {}
		virtual std::wstring tosqf(void) const;
		void add(instruction_s inst) { minsts.push_back(inst); }
		inline void loadinto(vmstack_s stack) { auto cs = std::make_shared<callstack>(); loadinto(stack, cs); stack->pushcallstack(cs); }
		void loadinto(vmstack_s, callstack_s);
		virtual bool equals(std::shared_ptr<data> d) const { return minsts == std::dynamic_pointer_cast<codedata>(d)->minsts; }
	};
	typedef std::shared_ptr<codedata> code_s;
	typedef std::weak_ptr<codedata> code_w;
	typedef std::unique_ptr<codedata> code_u;
}
#endif // !_CODEDATA