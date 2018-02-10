#ifndef _INSTRUCTION
#define _INSTRUCTION 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instruction requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instruction requires <stack> header
#endif // !_STACK

namespace sqf
{
	class vmstack;
	class virtualmachine;
	class instruction
	{
	private:
		size_t mline;
		size_t mcol;
		std::wstring mfile;
		std::wstring msegment;
	public:
		enum insttype
		{
			push,
			assignto,
			assigntolocal,
			callnular,
			callunary,
			callbinary,
			endstatement,
			makearray,
			getvariable
		};
		virtual void execute(const virtualmachine*) const = 0;
		void setdbginf(size_t line, size_t col, std::wstring file, std::wstring segment);
		std::wstring dbginf(std::wstring tag) const;
		virtual insttype thistype(void) const = 0;
	};
	typedef std::shared_ptr<instruction> instruction_s;
	typedef std::weak_ptr<instruction> instruction_w;
	typedef std::unique_ptr<instruction> instruction_u;
}

#endif // !_INSTRUCTION
