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
		virtual std::wstring to_string(void) const;
		void add(instruction_s inst) { minsts.push_back(inst); }
		void loadinto(vmstack_s stack)
		{
			auto cs = std::make_shared<callstack>();
			for each (auto it in minsts)
			{
				cs->pushinst(it);
			}
			stack->pushcallstack(cs);
		}
	};
	typedef std::shared_ptr<codedata> code_s;
	typedef std::weak_ptr<codedata> code_w;
	typedef std::unique_ptr<codedata> code_u;
}
#endif // !_CODEDATA