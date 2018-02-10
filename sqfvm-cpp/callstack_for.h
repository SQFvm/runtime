#ifndef _CALLSTACK_FOR
#define _CALLSTACK_FOR 1


#if !defined(_MEMORY) & !defined(_MEMORY_)
#error callstack_for requires <memory> header
#endif // !_MEMORY
#if !defined(_QUEUE) & !defined(_QUEUE_)
#error callstack_for requires <queue> header
#endif // !_QUEUE
#if !defined(_STRING) & !defined(_STRING_)
#error callstack_for requires <string> header
#endif // !_STRING
#if !defined(_INSTRUCTION)
#error callstack_for requires "instruction.h" header
#endif // !_INSTRUCTION
#if !defined(_NAMESPACE)
#error callstack_for requires "namespace.h" header
#endif // !_NAMESPACE
#if !defined(_CALLSTACK)
#error callstack_for requires "callstack.h" header
#endif // !_CALLSTACK

namespace sqf
{
	class callstack_for : public callstack
	{
	private:
		std::shared_ptr<fordata> mfordata;
		std::shared_ptr<codedata> mexec;
	public:
		callstack_for(std::shared_ptr<fordata> fordata, std::shared_ptr<codedata> exec);
		virtual instruction_s popinst(const sqf::virtualmachine* vm);
	};
}

#endif // !_CALLSTACK_FOR
