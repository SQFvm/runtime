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
	public:
		virtual void execute(const virtualmachine*, std::shared_ptr<vmstack>) const = 0;
	};
	typedef std::shared_ptr<instruction> instruction_s;
	typedef std::weak_ptr<instruction> instruction_w;
	typedef std::unique_ptr<instruction> instruction_u;
}

#endif // !_INSTRUCTION
